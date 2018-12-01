// TODO(darius98): This should be auto-generated from a list of extensions.
#include <kktest_impl/main.hpp>

// @<AUTO-GENERATE>[IMPORTS]
#include <kktest_ext/feedback_impl/ext.hpp>

using kktest::ArgumentsApi;
using kktest::Extension;
using kktest::InternalArgs;
using kktest::registerInternalFlags;
using std::vector;

int main(int argc, char** argv) {
    vector<Extension*> extensions;
    ArgumentsApi* argumentsApi = ArgumentsApi::create("KKTest test binary.");
    InternalArgs internalArgs = registerInternalFlags(argumentsApi);

// @<AUTO-GENERATE>[EXTENSIONS_DEF]
kktest::feedback::FeedbackExtension feedbackExtension;
feedbackExtension.registerCommandLineArguments(argumentsApi);
extensions.push_back(&feedbackExtension);

    argumentsApi->interpret(argc, argv);
    int ret = kktest::main(extensions, internalArgs);
    delete argumentsApi;
    return ret;
}
