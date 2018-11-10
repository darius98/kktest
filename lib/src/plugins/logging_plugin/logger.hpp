#ifndef KKTEST_PLUGINS_LOGGING_PLUGIN_LOGGER_H_
#define KKTEST_PLUGINS_LOGGING_PLUGIN_LOGGER_H_

#include <ostream>
#include <set>

#include <core/test.hpp>


namespace kktest {

class Logger {
public:
    explicit Logger(std::ostream& _stream);

    void enqueueTestForLogging(Test* test);

    void logFinalInformation(int passedTests, int failedTests, int failedOptionalTests);

private:
    bool isInTerminal() const;

    void modifyOutput(const int& code);

    void logTest(Test* test);

    struct AscendingByTestIndex {
        bool operator()(Test* a, Test* b) const;
    };

    std::ostream& stream;

    int testsLogged = 0;
    std::set<Test*, AscendingByTestIndex> testsQueue;
};

}

#endif
