#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <string>
#include <map>
using namespace std;

class TimeSeries {
private:
    const char *m_fileName;
    map<std::string, vector<float>> data_structure;
    vector<std::string> features;
public:

    TimeSeries(const char *CSVfileName) {
        m_fileName = CSVfileName;
        data_structure = set_data_structure(CSVfileName);
    }

    void add_new_line(const char *fileName, char *newLine);
    map<std::string, vector<float>> get_data_structure();
    map<std::string, vector<float>> set_data_structure(const char *fileName);
    vector<string> get_features();
};
#endif /* TIMESERIES_H_ */
