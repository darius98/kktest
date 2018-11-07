#include <fstream>

#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include "report_plugin.hpp"

using namespace std;

AddArgument(string, argumentReportFileName)
    .Name("report")
    .Short("r")
    .ArgumentType("FILE ")
    .Description("Generate a JSON test report at the end of running the tests")
    .DefaultValue("")
    .ImplicitValue("./report.json");

namespace kktest {

bool ReportPlugin::isEnabled() const {
    return !argumentReportFileName.empty();
}

void ReportPlugin::install() {
    TestingDriver::addBeforeDestroyHook([]() {
        ofstream report(argumentReportFileName);
        report << TestingDriver::toJSON().stringify(0);
        report.close();
    });
}

}