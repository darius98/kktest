#ifndef KKTEST_CORE_SRC_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_EXECUTOR_HPP_

#include <kktest_impl/types.hpp>
#include "group.hpp"
#include "test.hpp"

namespace kktest {

class Executor {
 private:
    enum State { INACTIVE, ACTIVE };
    typedef const std::function<void(const String&)>& SetFailureType;

    static double computeTimeTickLengthFromHardware();

 public:
    Executor();

    virtual ~Executor();

    void checkIsInactive(const String& methodName) const;

    double getTimeTickLengthMs() const;

    virtual void execute(Test* test, Executable func);

    virtual void finalize();

    void onTestFinished(const std::function<void(Test*)>& _onTestFinishedCallback);

 protected:
    void run(Test* test, Executable func);

    void setTestExecuted(Test* test,
                         double executionTimeTicks,
                         bool passed,
                         const String& failureMessage);

 private:
    void runSetUpsRecursively(Group* group, SetFailureType setFailure);

    void runTearDownsRecursively(Group* group, SetFailureType setFailure);

    void runTest(Executable func, SetFailureType setFailure);

    State state = State::INACTIVE;
    double timeTickLengthMs;
    std::function<void(Test*)> onTestFinishedCallback;
};

}  // namespace kktest

#endif  // KKTEST_CORE_SRC_EXECUTOR_HPP_
