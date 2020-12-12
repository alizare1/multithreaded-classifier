#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>
#include <fstream>
#include <sstream> 
#include <iterator>
#include <algorithm>

using namespace std;


void read_train(vector<vector<float>> &train, ifstream &file) {
    
    string line, tmp;
    int i;
    float value;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        i = 0;
        while (getline(ss, tmp, ',')) {
            value = stof(tmp);
            train[i].push_back(value);
            i++;
        }
    }
    
}

void normalize(vector<vector<float>> &train) {
    float min, max;
    for (auto &col : train) {
        if (&col == &train.back())
            continue;
        
        auto minmax = minmax_element(begin(col), end(col));
        if (*minmax.first == 0 && *minmax.second == 1)
            continue;
        
        min = *minmax.first;
        max = *minmax.second;
        
        for (auto &num : col) {
            num = (num - min) / (max - min);
        }
        
    }
}

int main(int argc, char *argv[]) {
    ifstream train, weights;

    if (argc < 2) {
        cerr << "No arguments were provided" << endl;
        exit(EXIT_FAILURE);
    }

    train.open(string(argv[1]) + "/train.csv");
    weights.open(string(argv[1]) + "/weights.csv");

    vector<vector<float>> train_data(21);
    read_train(train_data, train);
    
    normalize(train_data);

    exit(EXIT_SUCCESS);
}