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
    for (auto col : train) {
        if (col == train.end() - 1)
            break;
    }
}

int main(int argc, char *argv[]) {
    ifstream train, weights;
    string line;

    if (argc < 2) {
        cerr << "No arguments were provided" << endl;
        exit(EXIT_FAILURE);
    }

    train.open(string(argv[1]) + "/train.csv");
    weights.open(string(argv[1]) + "/weights.csv");

    vector<vector<float>> train_data(21);
    read_train(train_data, train);

    // cout << train_data[0][4] << endl;
    auto a = minmax_element(begin(train_data[2]),
        end(train_data[2]));
    cout << *(a.first) << " " << *(a.second) << endl;
    

    exit(EXIT_SUCCESS);
}