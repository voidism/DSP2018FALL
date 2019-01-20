#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include "Vocab.h"
#include "Ngram.h"

using namespace std;

map<string, vector<string> > ZYmap;
Vocab voc;
Ngram *lm;

void buildZYmap(char* filename){
    fstream mapfile(filename);
    string line;
    while(getline(mapfile, line)){
            stringstream in_line(line);
            if(!in_line.good()) continue;
            string key;
            string value;
            in_line >> key;
            ZYmap.insert(pair<string, vector<string>>(key, vector<string>()));
            while(in_line.good()){
                in_line >> value;
                ZYmap[key].push_back(value);
            }
    }
}

void print_map(){
    for(std::map<string, vector<string> >::iterator iter = ZYmap.begin(); iter != ZYmap.end(); ++iter)
    {
        cout << iter->first << ": ";
        for(auto x:iter->second){
            cout << x << " ";
        }
        cout << endl;
    }
}

void get_lm(char* filename){
    {
        File lmFile( filename, "r" );
        lm->read(lmFile);
        lmFile.close();
    }
    // cout << "load LM successfully!" << endl;
}

// Get P(W2 | W1) -- bigram
double getBigramProb(string w1, string w2)
{
    VocabIndex wid1 = voc.getIndex(w1.c_str());
    VocabIndex wid2 = voc.getIndex(w2.c_str());

    if(wid1 == Vocab_None)  //OOV
        wid1 = voc.getIndex(Vocab_Unknown);
    if(wid2 == Vocab_None)  //OOV
        wid2 = voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid1, Vocab_None };
    return lm->wordProb( wid2, context);
}

// Get P(W3 | W1, W2) -- trigram
double getTrigramProb(string w1, string w2, string w3) 
{
    VocabIndex wid1 = voc.getIndex(w1.c_str());
    VocabIndex wid2 = voc.getIndex(w2.c_str());
    VocabIndex wid3 = voc.getIndex(w3.c_str());

    if(wid1 == Vocab_None)  //OOV
        wid1 = voc.getIndex(Vocab_Unknown);
    if(wid2 == Vocab_None)  //OOV
        wid2 = voc.getIndex(Vocab_Unknown);
    if(wid3 == Vocab_None)  //OOV
        wid3 = voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid2, wid1, Vocab_None };
    return lm->wordProb( wid3, context );
}

double get_best_path(vector<string>& last_words, vector<double>& last_probs, string this_word){
    double max_prob = -1000;
    for (int i = 0; i < last_words.size();i++){
        double new_prob = last_probs[i] + getBigramProb(last_words[i], this_word);
        if(new_prob>max_prob) max_prob = new_prob;
    }
    return max_prob;
}

int get_arg_last(vector<string>& last_words, vector<double>& last_probs, string& this_word){
    double max_prob = -1000;
    int max_idx = 0;
    for (int i = 0; i < last_words.size();i++){
        double new_prob = last_probs[i] + getBigramProb(last_words[i], this_word);
        if(new_prob>max_prob){
            max_prob = new_prob;
            max_idx = i;
        }
    }
    return max_idx;
}

int argmax(vector<double>& probs){
    double max_prob = -1000;
    int max_idx = 0;
    for (int i = 0; i < probs.size();i++){
        if(probs[i]>=max_prob){
            max_prob=probs[i];
            max_idx = i;
        }
    }
    return max_idx;
}

string viterbi(string org_sent){
    // read in sentence to vector
    stringstream line(org_sent);
    vector<string> sent;
    vector<vector<string>> search_space;
    vector<vector<double>> delta;
    string value;
    
    while(line >> value){
        sent.push_back(value);
        delta.push_back(vector<double>());
        search_space.push_back(ZYmap[value]);
    }
    // run first time step of viterbi
    for (int i = 0; i < search_space[0].size(); i++){
        delta[0].push_back(getBigramProb("<s>", search_space[0][i]));
    }
    // run the consequent viterbi
    for (int i = 1; i < search_space.size(); i++){
        for (int j = 0; j < search_space[i].size(); j++){
            delta[i].push_back(get_best_path(search_space[i-1], delta[i-1], search_space[i][j]));
        } 
    }
    // consider final gram!
    delta.push_back(vector<double>());
    search_space.push_back(vector<string>());
    search_space[search_space.size()-1].push_back("</s>");
    delta[delta.size()-1].push_back(
        get_best_path(search_space[search_space.size()-2],
            delta[delta.size()-2], "</s>"));
    // find max prob tail
    int q[delta.size()];
    int T = delta.size() - 1;
    q[T] = 0;
    sent.push_back("</s>");
    // backtracking
    for(int i = T; i>0; i--){
        q[i - 1] = get_arg_last(search_space[i - 1], delta[i - 1], search_space[i][q[i]]);
        sent[i - 1] = search_space[i - 1][q[i - 1]];
    }

    string result = "<s>";
    for(auto x:sent){
        result += ' ' + x;
    }
    return result;
}

void testing(char* filename){
    fstream myfile(filename);
    string line;
    while(getline(myfile, line)){
        string newline = viterbi(line);
        cout << newline << endl;
    }
}

bool check_args(char *argv[]){
    if (strcmp(argv[1],"-text")) cerr << "Error: the 1st argument must be -text" << endl;
    else if (strcmp(argv[3],"-map")) cerr << "Error: the 2nd argument must be -map" << endl;
    else if (strcmp(argv[5],"-lm")) cerr << "Error: the 3rd argument must be -lm" << endl;
    else if (strcmp(argv[7],"-order")) cerr << "Error: the 4th argument must be -order" << endl;
    else return true;
    return false;
}

int main(int argc, char *argv[]){
    if(argc < 8){
        cout << "Usage: ./mydisambig -text seg_1.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > out_1.txt" << endl;
        cerr << "Too few argument number!" << endl;
        return 0;
    }
    if(!check_args(argv)) return 0;
    int ngram_order = 2;//atoi(argv[8]);
    Ngram temp(voc, ngram_order);
    lm = &temp;
    buildZYmap(argv[4]);
    get_lm(argv[6]);
    testing(argv[2]);
    return 0;
}
