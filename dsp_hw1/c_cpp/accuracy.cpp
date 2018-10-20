#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;


int main(int argc, char const *argv[]){
    if(argc!=4){
        cout << "argc: " << argc << endl;
        cout << "argument num false!" << endl;
        cout << "Example: ./accuracy result1.txt ../testing_answer.txt acc1.txt" << endl;
        return 0;
    }

    ifstream res(argv[1], ios::in);
    ifstream ans(argv[2], ios::in);
    vector<string> ans_list, res_list;
    string line;
    while(getline(ans, line)){
        ans_list.push_back(line);
    }
    ans.close();
    while(getline(res, line)){
        res_list.push_back(line);
    }
    res.close();
    if(res_list.size()!=ans_list.size()){
        cout << "line numbers in file " << argv[1] << " and " << argv[2] << "are not the same. Abort!" << endl;
        return 0;
    }
    int correct = 0;
    for(int i=0; i<res_list.size();i++){
        if(res_list[i].substr(0, res_list[i].find(' '))==ans_list[i]){
            correct += 1;
        }
        // else{
        //     cout << res_list[i].substr(0, res_list[i].find(' ')) << "<+>" << ans_list[i] << endl;
        // }
    }
    cout << double(correct)/double(res_list.size()) << endl;
    ofstream output(argv[3], ios::out);
	output << double(correct)/double(res_list.size()) << endl;
	output.close();
    return 0;
}
// s.substr(0, s.find(delimiter))