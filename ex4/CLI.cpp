#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;

    Command *opt1 = new Option1(dio);
    Command *opt2 = new Option2(dio);
    Command *opt3 = new Option3(dio);
    Command *opt4 = new Option4(dio);
    Command *opt5 = new Option5(dio);
    Command *opt6 = new Option6(dio);

    this->menu.insert({"1", opt1});
    this->menu.insert({"2", opt2});
    this->menu.insert({"3", opt3});
    this->menu.insert({"4", opt4});
    this->menu.insert({"5", opt5});
    this->menu.insert({"6", opt6});

}

void CLI::start(){

    dio->write("Welcome to the Anomaly Detection Server.\n");
    dio->write("Please choose an option:\n");

    for (auto & itr : menu) {
        dio->write(itr.second->getDescription());
    }

    string input = dio->read();

    menu[input]->execute();
}


CLI::~CLI() {
}

