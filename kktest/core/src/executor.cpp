#include "kktest/core/src/executor.hpp"

#include "common/utils/src/time.hpp"
#include "kktest/core/src/errors.hpp"

using namespace kktest::utils;
using namespace std;

namespace kktest {

Executor::Executor(OnTestFinished onTestFinished):
        onTestFinished(move(onTestFinished)) {}

double Executor::GetTimeTickLengthMs() {
    static double timeTickLengthMs = -1.0;
    if (timeTickLengthMs == -1.0) {
        // TODO(darius98): Don't hard-code this, estimate it based on how much a
        //  series of computations takes.
        timeTickLengthMs = 1000.0;
    }
    return timeTickLengthMs;
}

bool Executor::isActive() const {
    return state != INACTIVE;
}

string Executor::stateAsString() const {
    switch (state) {
        case INACTIVE: return "inactive";
        case INSIDE_TEST: return "test";
        case INSIDE_SET_UP: return "setUp";
        case INSIDE_TEAR_DOWN: return "tearDown";
    }
}

void Executor::finalize() {}

void Executor::execute(Test test) {
    vector<ExecutedTest::Info> executions;
    executions.reserve(test.getNumAttempts());
    for (size_t i = 0; i < test.getNumAttempts(); ++ i) {
        executions.push_back(run(test));
    }
    onTestFinished(ExecutedTest(move(test), move(executions)));
}

ExecutedTest::Info Executor::run(const Test& test) {
    state = INSIDE_SET_UP;
    ExecutedTest::Info info;
    RealTimeTimer t;
    vector<GroupPtr> testGroupStack = test.getGroupStack();
    vector<GroupPtr>::iterator it;
    // Execute setUp()-s, in the order of the group stack.
    for (it = testGroupStack.begin(); it != testGroupStack.end(); ++ it) {
        runJob([it] { (*it)->setUp(); },
               &info,
               "setUp of group \"" + (*it)->getDescription() + "\"");
        if (!info.passed) {
            // If a setUp() fails, do not execute the rest.
            break;
        }
    }
    state = INSIDE_TEST;
    if (info.passed) {
        // Only run the test if all setUp()-s passed without exception.
        runJob(bind(&Test::run, &test), &info, "test");
        -- it;
    }
    state = INSIDE_TEAR_DOWN;
    // Execute tearDown()-s in reverse order, from where setUp()-s stopped.
    for (; it + 1 != testGroupStack.begin(); -- it) {
        runJob([it] { (*it)->tearDown(); },
               &info,
               "tearDown of group \"" + (*it)->getDescription() + "\"");
    }
    double elapsedMs = 1.0 * t.elapsed().totalNs() / Duration::kMilliToNano;
    info.timeTicks = elapsedMs / GetTimeTickLengthMs();
    state = INACTIVE;
    return info;
}

void Executor::runJob(const Executable& job,
                      ExecutedTest::Info* execution,
                      const string& where) {
    try {
        job();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& failure) {
        execution->fail(failure.what());
    } catch(const exception& e) {
        execution->fail("Uncaught exception in " + where + ": " + e.what());
    } catch(...) {
        execution->fail("Uncaught non-exception type in " + where + "\".");
    }
}

}
