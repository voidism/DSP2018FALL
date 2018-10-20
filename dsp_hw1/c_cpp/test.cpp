#include "hmm.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

#define N mymodel->state_num
#define K mymodel->observ_num
#define A mymodel->transition
#define B mymodel->observation
#define PI mymodel->initial
#define O(t) (line[t]-'A')

using namespace std;

double output_prob(HMM* mymodel, string line){
    int T = line.size();
    double delta[T][N];
    // initialize the first column of delta
    for(int i=0; i<N; i++){
        delta[0][i] = PI[i]*B[line[0] - 'A'][i];
    }
    // forward recursion
    for(int t=1; t<T; t++){
        for(int j=0; j<N; j++){
            double max_val = 0.0;
            int max_idx = 0;
            for(int i=0; i<N; i++){
                double cur_val = delta[t-1][i]*A[i][j];
                if(cur_val>max_val){
                    max_val = cur_val;
                    max_idx = i;
                }
            }
            delta[t][j] = max_val*B[line[t] - 'A'][j];
        }
    }
    // termination (argmax delta[T])
    double ret = 0.0;
    for(int i=0; i<N; i++){
        double cur_prob = delta[T-1][i];
        if(cur_prob>ret) ret=cur_prob;
    }
    return ret;
}

int main(int argc, char const *argv[]) {

if(argc!=4){
    cout << "argc: " << argc << endl;
    cout << "argument num false!" << endl;
    cout << "Example: ./test modellist.txt testing_data1.txt result1.txt" << endl;
    return 0;
}

HMM models[5];
load_models(argv[1], models, 5);
ifstream test_seq(argv[2], ios::in);
ofstream output(argv[3], ios::out);
vector<string> seq;

string line;
while(getline(test_seq, line)){
    seq.push_back(line);
}
test_seq.close();
for(int seq_idx=0;seq_idx<seq.size();seq_idx++){
    line = seq[seq_idx];
    double max_prob = 0.0;
    int max_idx = 0;
    for(int m=0;m<5;m++){
        double this_prob = output_prob(&models[m], line);
        if(this_prob>max_prob){
            max_prob = this_prob;
            max_idx = m;
        }
    }
    output << models[max_idx].model_name << " " << setprecision(6) << scientific << max_prob << endl;
}
    output.close();
    return 0;
}