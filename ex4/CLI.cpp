/*
 * CLI.cpp
 *
 * Author: Itamar Laredo
 */
#include "CLI.h"

/*
 * CLI constructor. initialize menu options, dio and clidata.
 */
CLI::CLI(DefaultIO* dio) {

    this->dio = dio;
    this->clid = new CLIData();

    Command *opt1 = new UploadCommand(dio, clid);
    Command *opt2 = new ThresholdCommand(dio, clid);
    Command *opt3 = new AnomalyDetectionCommand(dio, clid);
    Command *opt4 = new ReportCommand(dio, clid);
    Command *opt5 = new AnalyzeResultsCommand(dio, clid);
    Command *opt6 = new ExitCommand(dio, clid);

    this->menu.insert({"1", opt1});
    this->menu.insert({"2", opt2});
    this->menu.insert({"3", opt3});
    this->menu.insert({"4", opt4});
    this->menu.insert({"5", opt5});
    this->menu.insert({"6", opt6});

}

/*
 * run the menu for the user.
 */
void CLI::start() {

    dio->write("Welcome to the Anomaly Detection Server.\n");
    dio->write("Please choose an option:\n");

    for (auto &itr : menu) {
        dio->write(itr.second->getDescription());
    }

    string input = dio->read();
    menu[input]->execute();

    if (input == "6") {
        return;
    } else {
        this->start();
    }
}

CLI::~CLI() {
}

