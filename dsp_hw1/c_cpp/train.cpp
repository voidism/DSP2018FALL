#include "hmm.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#define N mymodel.state_num
#define K mymodel.observ_num
#define A mymodel.transition
#define B mymodel.observation
#define PI mymodel.initial

using namespace std;

int main(int argc, char const *argv[]) {

if(argc!=5){
    cout << "argc: " << argc << endl;
    cout << "argument num false!" << endl;
    cout << "Example: ./train 5 model_init.txt ../seq_model_01.txt model_01.txt" << endl;
    return 0;
}

int iter_num = atoi(argv[1]);
string init_filename = argv[2];
string seq_filename = argv[3];
string model_name = argv[4];

HMM mymodel;
loadHMM(&mymodel, init_filename.c_str());
dumpHMM(stderr,&mymodel);

// read observation
ifstream seq_file;
seq_file.open(seq_filename.c_str(), ios::in);

vector<string> seq;

string line;
while(getline(seq_file, line)){
    seq.push_back(line);
}
// define new parameters
seq_file.close();
double alpha[MAX_SEQ][MAX_STATE];
double beta[MAX_SEQ][MAX_STATE];
double gamma[MAX_SEQ][MAX_STATE];
double epsilon[MAX_SEQ][MAX_STATE][MAX_STATE];

// initialize model
cout << "start training!" << endl;

for(int iter=0; iter<iter_num; iter++){
    cout << seq.size() << endl;
    double gamma_obs[MAX_OBSERV][MAX_STATE];
    double gamma_accum[MAX_STATE];
    double gamma_one[MAX_STATE];
    double epsilon_accum[MAX_STATE][MAX_STATE];
    for(int t=0;t<seq[0].size();t++){
        for(int i=0;i<N;i++){
            gamma_accum[i] = 0.0;
            gamma_one[i] = 0.0;
            for(int j=0;j<N;j++){
                epsilon_accum[i][j] = 0.0;
            }
        }
    }

    for(int seq_idx=0; seq_idx<seq.size(); seq_idx++){
        line = seq[seq_idx];
        // init the first column of alpha
        for(int i=0; i<N; i++){
            alpha[0][i] = PI[i]*B[line[0]-'A'][i];
        }
        // forward algorithm
        for(int t=1;t<line.size();t++){
            int o_t = line[t]-'A';
            for(int j=0; j<N; j++){
                double alpha_t_j = 0.0;
                for(int i=0; i<N; i++){
                    alpha_t_j += alpha[t-1][i]*A[i][j];
                }
                alpha_t_j*=B[o_t][j];
                alpha[t][j] = alpha_t_j;
            }
        }
        // init the last column of beta
        for(int i=0; i<N; i++){
            beta[line.size()-1][i] = 1.0;
        }
        // backward algorithm
        for(int t=line.size()-2;t>=0;t--){
            int o_tplus1 = line[t+1]-'A';
            for(int i=0; i<N; i++){
                double beta_t_i = 0.0;
                for(int j=0; j<N; j++){
                    beta_t_i += beta[t+1][j]*A[i][j]*B[o_tplus1][j];
                }
                beta[t][i] = beta_t_i;
            }
        }
        // sum up gamma
        for(int t=0;t<line.size();t++){
            double divider = 0;
            for(int i=0;i<N;i++){
                divider += alpha[t][i]*beta[t][i];
            }
            for(int i=0;i<N;i++){
                gamma[t][i] = alpha[t][i]*beta[t][i]/divider;
                gamma_obs[line[t]-'A'][i] += gamma[t][i];
            }
        }
        // sum up epsilon
        for(int t=0;t<line.size()-1;t++){
            double divider = 0.0;
            int o_t = line[t]-'A';
            for(int i=0;i<N;i++){
                for(int j=0;j<N;j++){
                    divider += alpha[t][i]*A[i][j]*B[o_t][j]*beta[t+1][j];
                }
            }
            for(int i=0;i<N;i++){
                for(int j=0;j<N;j++){
                    epsilon[t][i][j] = alpha[t][i]*A[i][j]*B[o_t][j]*beta[t+1][j]/divider;
                }
            }
        }
        // accumulate gamma, epsilon
        for(int t=0;t<line.size();t++){
            for(int i=0;i<N;i++){
                if(t==0) gamma_one[i] += gamma[t][i];
                gamma_accum[i] += gamma[t][i];
                if(t<line.size()-1) continue;
                for(int j=0;j<N;j++){
                    epsilon_accum[i][j] += epsilon[t][i][j];
                }
            }
        }
    }
    // update params
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            double e_sum, g_sum = 0.0;
            g_sum = gamma_accum[i];
            e_sum = epsilon_accum[i][j];
            A[i][j] = e_sum/g_sum;
        }
        PI[i] = gamma_one[i]/seq.size();
    }
    // update "B"
    for(int i=0;i<N;i++){
        for(int k=0;k<K;k++){
            double g_sum = 0.0;
            // for(int t=0;t<line.size();t++){
            g_sum = gamma_accum[i];
            // }
            B[k][i] = gamma_obs[k][i]/g_sum;
        }
    }
}

cout << "alpha:" << endl;
for(int i=0; i<seq[0].size();i++){
    for(int j = 0; j<N;j++){
        cout << alpha[i][j] << " - ";
    }
    cout << "\n";
}
cout << "beta:" << endl;
for(int i=0; i<seq[0].size();i++){
    for(int j = 0; j<N;j++){
        cout << beta[i][j] << " - ";
    }
    cout << "\n";
}

// dump HMM model
    FILE* out = fopen(model_name.c_str(), "w");
    dumpHMM(out, &mymodel);
    dumpHMM(stderr,&mymodel);
    fclose(out);
    return 0;
}