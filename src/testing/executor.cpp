#include <iostream>

#include "executor.hpp"

using namespace std;

namespace kktest {

bool Executor::AscendingByTestIndex::operator()(Test* const& a,
                                                Test* const& b) {
    return a->getIndex() < b->getIndex();
}

Executor::Executor(int _testIndexToRun, bool _verbose):
    testIndexToRun(_testIndexToRun), verbose(_verbose) {}

void Executor::executeTest(const vector<Group*>& groups,
                           Test* test,
                           Executable func) {
    if (testIndexToRun == 0 || testIndexToRun == test->getIndex()) {
        execute(groups, test, func);
    }
}

bool Executor::isSingleTestExecutor() const {
    return testIndexToRun != 0;
}

void Executor::enqueueTestForLogging(Test* test) {
    if (!verbose) {
        return;
    }
    if (isSingleTestExecutor()) {
        if (test->isFailed()) {
            cout << test->getFailureMessage();
        }
        return;
    }
    loggingQueue.insert(test);
    while (!loggingQueue.empty() &&
                (*loggingQueue.begin())->getIndex() == testsLogged + 1) {
        Test* test = *loggingQueue.begin();
        cout << test->getFullDescription()
             << ": "
             << (test->isFailed() ? "FAILED" : "PASSED")
             << "\n";
        if (test->isFailed()) {
            cout << "\t" << test->getFailureMessage() << "\n";
        }
        testsLogged += 1;
        loggingQueue.erase(loggingQueue.begin());
    }
}

}
