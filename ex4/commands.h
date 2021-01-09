

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

class CLIData {
public:
    TimeSeries *ts;
    HybridAnomalyDetector *hy = new HybridAnomalyDetector();
};


// you may edit this class
class Command {
protected:
	DefaultIO* dio;
	CLIData* clid;
    string description;

public:
	Command(DefaultIO* dio, CLIData* clid):dio(dio), clid(clid){}
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
    Option1(DefaultIO* dio, CLIData* clid) : Command(dio, clid) {
        this->description = "1.upload a time series csv file\n";
    }

    string getDescription() override {
        return this->description;
    }

    void execute() override {

        std::ifstream infile("anomalyTrain.csv");
        if (infile.good())
            remove("anomalyTrain.csv");

        std::ifstream infile2("anomalyTest.csv");
        if (infile2.good())
            remove("anomalyTest.csv");

        const char *fileName = "anomalyTrain.csv";
        clid->ts = new TimeSeries(fileName);

        for (int i = 0; i < 2; i++) {
            dio->write("Please upload your local train CSV file.\n");
            while (true) {
                string line = dio->read();
                if (line == "done") {
                    dio->write("Upload complete.\n");
                    break;
                } else {
                    clid->ts->add_new_line(fileName, line);
                }
            }
            fileName = "anomalyTest.csv";
        }
    }
};

class Option2: public Command {
public:
    Option2(DefaultIO* dio, CLIData* clid) : Command(dio, clid) {
        this->description = "2.algorithm settings\n";
    }

    string getDescription() override {
        return this->description;
    }

    void execute() override {

        //clid->hy = new HybridAnomalyDetector();
        float threshold = clid->hy->get_threshold();

        dio->write("The current correlation threshold is ");
        dio->write(threshold);
        dio->write("\nType a new threshold\n");

        while (true) {
            string line = dio->read();
            threshold = std::stof(line);
            if (threshold >= 0 && threshold <= 1) {
                clid->hy->set_threshold(threshold);
                break;
            } else {
                dio->write("please choose a value between 0 and 1.\n");
            }
        }
    }
};

class Option3: public Command {
public:
    Option3(DefaultIO* dio, CLIData* clid) : Command(dio, clid) {
        this->description = "3.detect anomalies\n";
    }

    string getDescription() override {
        return this->description;
    }

    void execute() override {

        TimeSeries ts1("anomalyTrain.csv");
        clid->hy->learnNormal(ts1);
        TimeSeries ts2("anomalyTest.csv");
        clid->hy->detect(ts2);

        dio->write("anomaly detection complete.\n");
    }
};

class Option4: public Command {
public:
    Option4(DefaultIO* dio, CLIData* clid) : Command(dio, clid) {
        this->description = "4.display results\n";
    }

    string getDescription() override {
        return this->description;
    }

    void execute() override {

        for (auto & i : clid->hy->get_vAr()) {
            dio->write(i.timeStep);
            dio->write("\t");
            dio->write(i.description);
            dio->write("\n");
        }
        dio->write("Done.\n");
    }
};

class Option5: public Command {
public:
    Option5(DefaultIO* dio, CLIData* clid) : Command(dio, clid) {
        this->description = "5.upload anomalies and analyze results\n";
    }

    string getDescription() override {
        return this->description;
    }

    void execute() override {

        vector<AnomalyReport> vAr = clid->hy->get_vAr();
        vector<AnomalyReport> cdr;  // Common description report
        vector<vector<AnomalyReport>> reports;
        vector<pair<long,long>> sq; // Sequence of anomalies, start,finish


        for (int i = 0; i < vAr.size(); i++) {

            int j = i + 1;
            cdr.push_back(vAr[i]);

            while (vAr[i].description == vAr[j].description) {
                cdr.push_back(vAr[j]);
                j++;
            }

            long s_begin; // begining of time sequence
            long s_end; // end of time sequence

            for (int d = 0; d < cdr.size(); d++) {
                s_begin = cdr[d].timeStep;

                while (d < cdr.size() - 1) {
                    if (cdr[d].timeStep + 1 == cdr[d + 1].timeStep)
                        d++;
                    break;
                }
                s_end = cdr[d].timeStep;
                sq.emplace_back(s_begin, s_end);
            }

            reports.push_back(cdr);

            cdr.clear();
            i = j - 1;
        }
    }
};

class Option6: public Command {
public:
    Option6(DefaultIO* dio, CLIData* clid) : Command(dio, clid) {
        this->description = "6.exit\n";
    }

    string getDescription() override {
        return this->description;
    }

    void execute() override {

    }
};


#endif /* COMMANDS_H_ */
