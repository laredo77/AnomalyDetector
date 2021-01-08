/*
 * timeseries.cpp
 *
 * Author: 311547087, Itamar Laredo
 */
#include "timeseries.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>

// Adding new line to the data
void TimeSeries::add_new_line(const char *fileName, string newLine) {

    fstream fout;
    fout.open(fileName, ios::app);
    fout << newLine;
    fout.close();
}
// Setting data structure as a map.
// The keys hold the features and the elements is vector of float numbers
map<std::string, vector<float>> TimeSeries::set_data_structure(const char *fileName) {

    // File pointer
    fstream fin;
    std::map<std::string, vector<float> > data_s;
    // Open an existing file
    fin.open(fileName, ios::in);
    std::string line, line2;
    // Adding features as key and initialize vector as value
    for (int i = 0; i < 1; i++) {
        std::getline(fin, line);
        std::stringstream ss(line);

        while (getline(ss, line2, ',')) {
            features.push_back(line2);
            std::vector<float> v;
            data_s.insert({line2, v});
        }
    }
    // Adding values to each vector
    while (std::getline(fin, line)) {
        int i = 0;
        std::stringstream ss(line);
        while (getline(ss, line2, ',')) {
            float line2_float = std::stof(line2);
            data_s[features[i]].push_back(line2_float);
            i++;
        }
    }
    fin.close();
    return data_s;
}
// Getter to the data structure
map<std::string, vector<float>> TimeSeries::get_data_structure() {
    return this->data_structure;
}
// Getter to the features
vector<string> TimeSeries::get_features() {
    return this->features;
}