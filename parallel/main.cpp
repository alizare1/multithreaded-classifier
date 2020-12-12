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
#include <pthread.h>

using namespace std;


#define THREAD_NUMBER 4

string address;
vector<vector<vector<float>>> train_data(THREAD_NUMBER, vector<vector<float>>(21));
vector<vector<float>> weights_data(21);
vector<vector<pair<float, float>>> all_minmax(THREAD_NUMBER, vector<pair<float, float>>(20));
vector<pair<float, float>> final_minmax(20);
bool initial_minmax = true;

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

void* thread_read(void* tid) {
    long thread_id = (long) tid;
    ifstream train;
    
    train.open(address + "/train_" + to_string(thread_id) + ".csv");
    read_csv(train_data[thread_id], train);

    for (uint i = 0; i < train_data[thread_id].size() - 1; i++) {
        auto col_minmax = minmax_element(begin(train_data[thread_id][i]), end(train_data[thread_id][i]));
        all_minmax[thread_id][i].first = *col_minmax.first;
        all_minmax[thread_id][i].second = *col_minmax.second;
    }

    pthread_exit(tid);
}

float get_accuracy(const vector<float> &answer, const vector<int> &predict) {
    int count = 0;
    for (uint i = 0; i < answer.size(); i++)
        if (answer[i] == predict[i])
            count++;

    return (float) count / (float) answer.size();
}

void update_minmax(long tid) {
    for (uint i = 0; i < final_minmax.size(); i++) {
        if (initial_minmax) {
            final_minmax[i].first = all_minmax[tid][i].first;
            final_minmax[i].second = all_minmax[tid][i].second;
        } else {
            final_minmax[i].first = min(all_minmax[tid][i].first, final_minmax[i].first);
            final_minmax[i].second = max(all_minmax[tid][i].second, final_minmax[i].second);
        }
    }

    initial_minmax = false;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        cerr << "No arguments were provided" << endl;
        exit(EXIT_FAILURE);
    }

    address = string(argv[1]);
    pthread_t thread[THREAD_NUMBER];

    for (long tid = 0; tid < THREAD_NUMBER; tid++) {
		pthread_create(&thread[tid], NULL, thread_read, (void*)tid); 
	}

	for(long tid = 0; tid < THREAD_NUMBER; tid++)
	{
		pthread_join(thread[tid], NULL);
        update_minmax(tid);
	}
    

    // ifstream  weights;

    // weights.open(string(argv[1]) + "/weights.csv");

    // vector<int> results;
    // read_csv(weights_data, weights);
    // normalize(train_data);
    // classify(train_data, weights_data, results);
    
    // printf("%.2f\n", get_accuracy(train_data[20], results) * 100);



    exit(EXIT_SUCCESS);
}