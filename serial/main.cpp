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
#include <limits>

using namespace std;


void read_csv(vector<vector<float>> &vec, ifstream &file) {
    
    string line, tmp;
    int i;
    float value;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        i = 0;
        while (getline(ss, tmp, ',')) {
            value = stof(tmp);
            vec[i].push_back(value);
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

void classify(
    const vector<vector<float>> &train,
    const vector<vector<float>> &weights, 
    vector<int> &results
    ) {

    for (uint i = 0; i < train[0].size(); i++) {
        float scores[4] = {
            weights[20][0], 
            weights[20][1], 
            weights[20][2], 
            weights[20][3]
        };
        for (uint j = 0; j < train.size() - 1; j++) {
            scores[0] += train[j][i] * weights[j][0];
            scores[1] += train[j][i] * weights[j][1];
            scores[2] += train[j][i] * weights[j][2];
            scores[3] += train[j][i] * weights[j][3];
        }

        results.push_back(max_element(scores, scores + 4) - scores);
    }
}

float get_accuracy(const vector<float> &answer, const vector<int> &predict) {
    int count = 0;
    for (uint i = 0; i < answer.size(); i++)
        if (answer[i] == predict[i])
            count++;

    return (float) count / (float) answer.size();
}

int main(int argc, char *argv[]) {
    ifstream train, weights;

    if (argc < 2) {
        cerr << "No arguments were provided" << endl;
        exit(EXIT_FAILURE);
    }

    train.open(string(argv[1]) + "/train.csv");
    weights.open(string(argv[1]) + "/weights.csv");

    vector<vector<float>> train_data(21), weights_data(21);
    vector<int> results;
    read_csv(train_data, train);
    read_csv(weights_data, weights);
    normalize(train_data);
    classify(train_data, weights_data, results);
    
    printf("%.2f\n", get_accuracy(train_data[20], results) * 100);



    exit(EXIT_SUCCESS);
}