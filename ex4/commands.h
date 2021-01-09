

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>

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
    TimeSeries* ts;
    HybridAnomalyDetector *hy = new HybridAnomalyDetector();
    vector<pair<string, TimeSeries>> files;
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
            TimeSeries times(fileName);
            clid->ts = &times;
            clid->files.emplace_back(fileName, *clid->ts);
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
        vector<pair<long,long>> sq; // Sequence of anomalies, begin,end

        for (int i = 0; i < vAr.size(); i++) {

            int j = i + 1;
            cdr.push_back(vAr[i]);

            while (vAr[i].description == vAr[j].description) {
                cdr.push_back(vAr[j]);
                j++;
            }

            long s_begin; // begining of time sequence
            long s_end; // end of time sequence

            for (int k = 0; k < cdr.size(); k++) {
                s_begin = cdr[k].timeStep;

                while (k < cdr.size() - 1) {
                    if (cdr[k].timeStep + 1 == cdr[k + 1].timeStep) {
                        k++;
                        continue;
                    }
                    break;
                }
                if (cdr.size() > 1) {
                    if (cdr[k].timeStep == cdr[k - 1].timeStep + 1) {
                        s_end = cdr[k].timeStep;
                        sq.emplace_back(s_begin, s_end);
                    } else {
                        s_end = cdr[k].timeStep;
                        sq.emplace_back(s_begin, s_end);
                    }
                } else {
                    sq.emplace_back(s_begin, s_begin);
                }
            }
            cdr.clear();
            i = j - 1;
        }

        vector<pair<long,long>> u_sq; // user sequence of anomalies
        long f,s;                   // first second
        string first, second;
        int pos;

        while (true) {
            string line = dio->read();
            if (line == "done") {
                dio->write("Upload complete.\n");
                break;
            } else {
                pos = line.find_first_of(',');
                first = line.substr(0, pos);
                second = line.substr(pos + 1);
                f = std::stol(first);
                s = std::stol(second);
                u_sq.emplace_back(f,s);
            }
        }

        float P = u_sq.size();
        int sum = 0;
        for (auto & i : u_sq) {
            sum += (i.second - i.first);
        }

//        ptrdiff_t pos2 = find(clid->files.begin()->first,
//                              clid->files.end()->first, old_name_)
//                                      - clid->files.begin()->first;
        int n = clid->files[0].second.file_size() - 1;  // files[0] not currect!!!!!
        float N = n - sum;
        sort(sq.begin(), sq.end());
        float TP = 0, FP = 0;

        for (auto & i : u_sq) {
            for (auto & j : sq) {
                if (i.second >= j.second) {
                    if (i.first >= j.first) {
                        if ((i.second - i.first) + (j.second - j.first) >=
                            i.second - j.first) {
                            TP++;
                        } else {
                            FP++;
                        }
                    } else {
                        TP++;
                    }
                } else {
                    if (i.first >= j.first) {
                        TP++;
                    } else {
                        if ((i.second - i.first) + (j.second - j.first) >=
                         j.second - i.first) {
                            TP++;
                        } else {
                            FP++;
                        }
                    }
                }
            }
        }

        float tpr = (TP / P); // True Positive Rate
        tpr = tpr * 1000;
        tpr = floor(tpr);
        tpr = tpr / 1000;

        float fpr = (FP / N); // False Positive Rate
        fpr = fpr * 1000;
        fpr = floor(fpr);
        fpr = fpr / 1000;

        dio->write("True Positive Rate: ");
        dio->write((tpr));
        dio->write("\nFalse Positive Rate: ");
        dio->write((fpr));
        dio->write("\n");
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
