#include <iostream>

#include <kktest_impl/main.hpp>
#include <kktest_impl/signature.hpp>
#include "driver.hpp"
#include "test_case_registry.hpp"

using std::cout;
using std::exception;
using std::invalid_argument;
using std::vector;

namespace kktest {

InternalArgs registerInternalFlags(ArgumentsApi* argumentsApi) {
    InternalArgs args;
    args.versionFlag = argumentsApi->addFlag("version", "Display program version", "v");
    args.getSignatureFlag = argumentsApi->addFlag(
            "get-signature",
            "Display the KKTest 32-byte signature in hexadecimal format"
    );
    args.boxedFlag = argumentsApi->addFlag(
        "boxed",
        "Run each test in an isolated process (boxed)",
        "b"
    );
    args.maxParallelTestsArgument = argumentsApi->addArgument(
        "max-parallel-tests",
        "Maximum number of tests to execute in parallel (processes to spawn) when running boxed",
        "",
        "1"
    );
    return args;
}

int main(const vector<Extension*>& extensions, InternalArgs args) {
    if (args.versionFlag->get()) {
        cout << "KKTest generated test-case.\n";
        cout << "KKTest version: 1.0.0\n";
        return 0;
    }
    if (args.getSignatureFlag->get()) {
        for (int i = 0; i < kkTestSigSize; ++ i) {
            cout << "0123456789ABCDEF"[kkTestSignature[i] >> 4]
                 << "0123456789ABCDEF"[kkTestSignature[i] & 15];
        }
        cout << "\n";
        return 0;
    }

    ExtensionApiImpl apiImpl;
    for (Extension* extension: extensions) {
        extension->init(&apiImpl);
    }

    int maxParallelTests = 1;
    try {
        maxParallelTests = stoi(args.maxParallelTestsArgument->get());
        if (maxParallelTests < 1) {
            maxParallelTests = 1;
        }
    } catch(const invalid_argument& exc) {
        if (args.boxedFlag->get()) {
            cout << "Invalid value for argument max-parallel-tests: "
                 << "\"" << args.maxParallelTestsArgument->get() << "\"\n";
            return 1;
        }
    }

    TestingDriver* driver = TestingDriver::init(apiImpl.getHooks(),
                                                args.boxedFlag->get(),
                                                maxParallelTests);
    int ret = 1;
    try {
        for (TestCaseRegistry::TestCase testCase: TestCaseRegistry::all()) {
            driver->beforeTestCase();
            testCase();
            driver->afterTestCase();
        }
        ret = driver->destroy();
    } catch(const ConfigurationError& error) {
        driver->forceDestroy(error);
    } catch(const ExpectationFailed& error) {
        driver->forceDestroy(
            ConfigurationError(String("Expectation failed in global scope: ") + error.what())
        );
    } catch(const exception& error) {
        driver->forceDestroy(
            ConfigurationError(String("Exception thrown in global scope: ") + error.what())
        );
    } catch(...) {
        driver->forceDestroy(
            ConfigurationError("Non-exception object thrown in global scope.")
        );
    }

    for (Extension* extension: extensions) {
        extension->destroy();
    }
    return ret;
}

}
