#include "box_executor.hpp"

using namespace std;


namespace kktest {

BoxExecutor::BoxExecutor(size_t _maxNumContainers): maxNumContainers(_maxNumContainers) {}

void BoxExecutor::execute(Test* test, Executable func) {
    ensureFreeContainers(1);
    openContainers.insert(new TestContainer(
        test,
        test->getConfig().timeTicksLimit * getTimeTickLengthMs() + 100.0,
        [this, func, test]() {
            run(test, func);
        },
        [this, test](double ticks, bool passed, string failureMessage) {
            setTestExecuted(test, ticks, passed, failureMessage);
        }
    ));
}

void BoxExecutor::finalize() {
    ensureFreeContainers(maxNumContainers);
}

void BoxExecutor::ensureFreeContainers(size_t numContainers) {
    while (openContainers.size() > maxNumContainers - numContainers) {
        for (auto it = openContainers.begin(); it != openContainers.end(); ) {
            if ((*it)->isTestFinished()) {
                delete (*it);
                it = openContainers.erase(it);
            } else {
                ++ it;
            }
        }
    }
}

}
