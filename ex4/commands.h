

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};

// you may add here helper classes


// you may edit this class
class Command {
protected:
	DefaultIO* dio;
	string description;
public:
	//Command(DefaultIO* dio, string description):dio(dio), description(description){}
	Command(DefaultIO* dio):dio(dio){}
	virtual void execute()=0;
	virtual ~Command(){}
	virtual string getDescription()=0;
};


// implement here your command classes
//class UploadCommand: public Command {
//public://
//    UploadCommand(DefaultIO *dio, Clidata *clidata) : Command(dio, clidata)
//};

class Option1: public Command {
public:
    Option1(DefaultIO *dio) : Command(dio) {
        this->description = "1.upload a time series csv file\n";
    }

    string getDescription() {
        return this->description;
    }

    void execute() override {

        const char *fileName = "anomalyTrain.csv";
        TimeSeries *ts = new TimeSeries(fileName);
        for (int i = 0; i < 2; i++) {
            dio->write("Please upload your local train CSV file.\n");
            while (true) {
                string line = dio->read();
                if (line == "done") {
                    dio->write("Upload complete.\n");
                    break;
                } else {
                    ts->add_new_line(fileName, line);
                }
            }
            fileName = "anomalyTest.csv";
        }
    }
};

class Option2: public Command {
public:
    Option2(DefaultIO *dio) : Command(dio) {
        this->description = "2.algorithm settings\n";
    }

    string getDescription() {
        return this->description;
    }

    void execute() override {
        cout << "IM IN OPTION 2###";
    }
};

class Option3: public Command {
public:
    Option3(DefaultIO *dio) : Command(dio) {
        this->description = "3.detect anomalies\n";
    }

    string getDescription() {
        return this->description;
    }

    void execute() override {

    }
};

class Option4: public Command {
public:
    Option4(DefaultIO *dio) : Command(dio) {
        this->description = "4.display results\n";
    }

    string getDescription() {
        return this->description;
    }

    void execute() override {

    }
};

class Option5: public Command {
public:
    Option5(DefaultIO *dio) : Command(dio) {
        this->description = "5.upload anomalies and analyze results\n";
    }

    string getDescription() {
        return this->description;
    }

    void execute() override {

    }
};

class Option6: public Command {
public:
    Option6(DefaultIO *dio) : Command(dio) {
        this->description = "6.exit\n";
    }

    string getDescription() {
        return this->description;
    }

    void execute() override {

    }
};


#endif /* COMMANDS_H_ */
