/*
 * commands.h
 *
 * Author: 311547087, Itamar Laredo
 */
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include "HybridAnomalyDetector.h"
#include <sys/socket.h>

using namespace std;

/*
 * input/output class. could be standardIO or socketIO
 */
class DefaultIO {
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

};

/*
 * CLI Data connect between all the commands.
 * the Cli data saves commands data and each command gets
 * the Cli data as argument and can use it.
 */
class CLIData {
public:
    TimeSeries* ts;
    HybridAnomalyDetector *hy = new HybridAnomalyDetector();
    vector<pair<string, TimeSeries>> files;
};

/*
 * Commands class.
 */

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

/*
 * Upload command class. user use it to upload files.
 */
class UploadCommand: public Command {
public:
    UploadCommand(DefaultIO* dio, CLIData* clid) : Command(dio, clid) {
        this->description = "1.upload a time series csv file\n";
    }

    string getDescription() override {
        return this->description;
    }

    void execute() override {
        // if the file exists, remove it
        std::ifstream infile("anomalyTrain.csv");
        if (infile.good())
            remove("anomalyTrain.csv");

        std::ifstream infile2("anomalyTest.csv");
        if (infile2.good())
            remove("anomalyTest.csv");

        const char *fileName = "anomalyTrain.csv";
        // repeat the process twice: for train and test files.
        for (int i = 0; i < 2; i++) {
            if (i == 0)
                dio->write("Please upload your local train CSV file.\n");
            else
                dio->write("Please upload your local test CSV file.\n");
            // upload line by line to new file
            while (true) {
                string line = dio->read();
                if (line == "done") {
                    dio->write("Upload complete.\n");
                    break;
                } else {
                    clid->ts->add_new_line(fileName, line);
                }
            }
            // save the data at cli data as timeseries object
            TimeSeries times(fileName);
            clid->ts = &times;
            clid->files.emplace_back(fileName, *clid->ts);
            fileName = "anomalyTest.csv";
        }
    }
};

/*
 * Threshold command class, user gets the current threshold and
 * can modify it with values between 0 to 1.
 */
class ThresholdCommand: public Command {
public:
    ThresholdCommand(DefaultIO* dio, CLIData* clid) : Command(dio, clid) {
        this->description = "2.algorithm settings\n";
    }

    string getDescription() override {
        return this->description;
    }

    void execute() override {
        // get current threshold
        float threshold = clid->hy->get_threshold();

        dio->write("The current correlation threshold is ");
        dio->write(threshold);
        dio->write("\nType a new threshold\n");
        // user may change the threshold.
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

/*
 * Anomaly Detection command class. use the hybrid detector
 * to report the anomalies.
 */
class AnomalyDetectionCommand: public Command {
public:
    AnomalyDetectionCommand(DefaultIO* dio, CLIData* clid) : Command(dio, clid) {
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

/*
 * Report command class.
 * display to the user the hybrid detector results.
 */
class ReportCommand: public Command {
public:
    ReportCommand(DefaultIO* dio, CLIData* clid) : Command(dio, clid) {
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

/*
 * Analyze results and compare the hybrid detector data with
 * the user alarms data.
 */
class AnalyzeResultsCommand: public Command {
public:
    AnalyzeResultsCommand(DefaultIO* dio, CLIData* clid) : Command(dio, clid) {
        this->description = "5.upload anomalies and analyze results\n";
    }

    string getDescription() override {
        return this->description;
    }

    void execute() override {

        dio->write("Please upload your local anomalies file.\n");
        vector<AnomalyReport> vAr = clid->hy->get_vAr();
        vector<AnomalyReport> cdr;  // Common description report
        vector<pair<long,long>> sq; // Sequence of anomalies, begin,end

        // sq gets anomalies with same description and sequence of time.
        for (int i = 0; i < vAr.size(); i++) {

            int j = i + 1;
            cdr.push_back(vAr[i]);

            while (vAr[i].description == vAr[j].description) {
                cdr.push_back(vAr[j]);
                j++;
            }

            long s_begin; // begin of time sequence
            long s_end;   // end of time sequence

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
        long f,s;                     // first second
        string first, second;
        int pos;

        // gets from the user anomaly file
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

        float P = u_sq.size();  // Positive
        int sum = 0;            // sum the time user had alarms
        for (auto & i : u_sq) {
            sum += (i.second - i.first) + 1;
        }

        int n = clid->files[1].second.file_size() - 1; // total lines in data file
        float N = n - sum;           // Negative
        sort(sq.begin(), sq.end());  // sorting sq vector
        float TP = 0, FP;            // True positive, False positive results.

        // Checks whether there was an overlap between the reports
        // received from the user and the actual anomalies
        for (auto & user : u_sq) {
            for (auto & machine : sq) {
                if (machine.first <= user.first) {
                    if (machine.second >= user.second)
                        TP++;
                    else if ((machine.second - machine.first) + (user.second - user.first)
                    >= (user.second - machine.first))
                        TP++;
                } else {
                    if (user.second >= machine.second)
                        TP++;
                    else if ((user.second - user.first) + (machine.second - machine.first)
                    >= (machine.second - user.first))
                        TP++;
                }
            }
        }
        FP = sq.size() - TP;  // False positive is the size of sq minus the true positive

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
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write((fpr));
        dio->write("\n");
    }
};

/*
 * Exit command class. finish the connection between
 * user to server.
 */
class ExitCommand: public Command {
public:
    ExitCommand(DefaultIO* dio, CLIData* clid) : Command(dio, clid) {
        this->description = "6.exit\n";
    }

    string getDescription() override {
        return this->description;
    }

    void execute() override {
    }
};

#endif /* COMMANDS_H_ */
