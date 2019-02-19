#include "core/src/box_executor.hpp"

#include "common/interproc/src/pipe.hpp"
#include "common/utils/src/process_time.hpp"
#include "core/src/errors.hpp"

using namespace kktest::interproc;
using namespace kktest::utils;
using namespace std;
using namespace std::placeholders;

namespace {

enum MessageStatus: uint8_t {
    SUCCESS = 0,
    CONFIGURATION_ERROR = 1
};

}

namespace kktest {

BoxedTest::BoxedTest(Test&& _test, WorkerSubprocess* _process):
        test(move(_test)), process(_process) {}

BoxedTest::BoxedTest(BoxedTest&& other) noexcept:
        test(move(other.test)), process(move(other.process)) {}

BoxedTest& BoxedTest::operator=(BoxedTest&& other) noexcept {
    if (this != &other) {
        test = move(other.test);
        process = move(other.process);
    }
    return *this;
}

BoxExecutor::BoxExecutor(OnTestFinished onTestFinished, size_t _numBoxes):
        Executor(move(onTestFinished)), numBoxes(_numBoxes) {}

void BoxExecutor::execute(Test&& test, Executable func) {
    ensureEmptyBoxes(1);
    double timeLimit = test.getTimeTicksLimit() * getTimeTickLengthMs() + 100.0;
    GroupPtr group = test.getGroup();
    auto process = new WorkerSubprocess(
            timeLimit, bind(&BoxExecutor::runBoxed, this, group, func, _1));
    activeBoxes.emplace_back(move(test), process);
}

void BoxExecutor::runBoxed(GroupPtr group, Executable func, PipeWriter* pipe) {
    try {
        ExecutedTest::Info info = run(group, func);
        pipe->sendMessage(SUCCESS, info.timeTicks, info.passed, info.failure);
    } catch(const ConfigurationError& error) {
        pipe->sendMessage(CONFIGURATION_ERROR, string(error.what()));
    }
}

void BoxExecutor::finalize() {
    ensureEmptyBoxes(numBoxes);
}

void BoxExecutor::ensureEmptyBoxes(size_t requiredEmpty) {
    size_t maxActive = numBoxes - requiredEmpty;
    while (activeBoxes.size() > maxActive) {
        bool progress = false;
        for (auto it = activeBoxes.begin(); it != activeBoxes.end(); ) {
            if (tryCloseBox(it)) {
                it = activeBoxes.erase(it);
                progress = true;
            } else {
                ++ it;
            }
        }
        if (!progress) {
            sleepForDuration(Duration::fromMs(5));
        }
    }
}

bool BoxExecutor::tryCloseBox(vector<BoxedTest>::iterator boxedTest) {
    bool passed = false;
    Message message;
    string error;
    auto process = boxedTest->process.get();
    switch (process->getFinishStatus()) {
        case WorkerSubprocess::NO_EXIT: {
            return false;
        }
        case WorkerSubprocess::ZERO_EXIT: {
            message = process->getNextMessage(32);
            if (message.isInvalid()) {
                passed = false;
                error = "Unexpected 0-code exit.";
            } else {
                passed = true;
            }
            break;
        }
        case WorkerSubprocess::NON_ZERO_EXIT: {
            error = "Test exited with code "
                    + to_string(process->getReturnCode()) + ".";
            break;
        }
        case WorkerSubprocess::SIGNAL_EXIT: {
            error = "Test killed by signal " + to_string(process->getSignal());
            break;
        }
        case WorkerSubprocess::TIMEOUT: {
            error = "Test execution timed out.";
            break;
        }
    }
    if (!passed) {
        onTestFinished(ExecutedTest(move(boxedTest->test), move(error)));
        return true;
    }
    if (message.read<MessageStatus>() == CONFIGURATION_ERROR) {
        // Read the error message and throw it.
        throw ConfigurationError(message.read<string>());
    }
    ExecutedTest::Info info;
    message >> info.timeTicks >> info.passed >> info.failure;
    onTestFinished(ExecutedTest(move(boxedTest->test), move(info)));
    return true;
}

}
