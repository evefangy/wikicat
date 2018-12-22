#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <queue>
#include <set>
#include <unordered_set>
using namespace std;
#include <time.h>       /* time */
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

void parse(vector<string>& parsed, string& line){
    string tmp = "";
    for(int i=0;i<line.size();i++){
        if(line[i]==' '){
            if (tmp.size()!=0 && tmp!=" "){
                parsed.push_back(tmp);
            }
            
            tmp = "";
        }
        else if(line[i]!=','){
            tmp+=line[i];
        }
    }
    if (tmp.size()!=0 && tmp!=" "){
        parsed.push_back(tmp);
    }
}

int main(int argc, const char * argv[]) {
    if(argc < 3){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Number_of_articles | Output_Address"<<endl;
        cout<<"(e.g 33749 ./Output)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string num_art_str = argv[1];
    int num_art = stoi(num_art_str);
    string output_addr = argv[2];
    output_addr+="/sorted_completely_random_tree_weight.txt";
    
    srand ( unsigned ( std::time(0) ) );
    
    
    
    // randomly permute the index
    vector<int> index_to_insert;
    for (int i=0; i<num_art; i++) {
        index_to_insert.push_back(i);
    }
    random_shuffle ( index_to_insert.begin(), index_to_insert.end() );

    map<int, vector<vector<int> >, greater<int> > sorted_weight;
    
    

    // grab every 2 artciles and randomly assign a weight to them 
    for (int i=0;i<index_to_insert.size();i++){
        if (i+1==index_to_insert.size()) continue;
        int a1 = index_to_insert[i];
        int a2 = index_to_insert[i+1];

        // randomly assign a weight between 1 and 30
        int weight = rand()%100+1;
        vector<int> article_index_pairs;
        article_index_pairs.push_back(a1);
        article_index_pairs.push_back(a2);

        sorted_weight[weight].push_back(article_index_pairs);
    }
    

    ofstream outfile;
    outfile.open(output_addr);
    for (auto it=sorted_weight.begin(); it!=sorted_weight.end();it++){
        // for every pair
        for (auto i=0;i<it->second.size();i++){
            outfile<<it->first;
            for (auto j=0;j<it->second[i].size();j++){
                outfile<<", "<<it->second[i][j];
            }
            outfile<<endl;
        }
    }

    outfile.close();
    
    
    
    return 0;
}
