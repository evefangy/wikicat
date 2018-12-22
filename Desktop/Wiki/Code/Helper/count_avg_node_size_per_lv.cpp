// The code is for calculating the node size per lv
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
using namespace std;

int parse(string& s, vector<string>& parsed){
    int lv = 0;
    string tmp = "";
    for(int i=0; i<s.length();i++){
        if(s[i]==','){
            parsed.push_back(tmp);
            tmp = "";
        }
        else{
            if(s[i] == '_'){
                lv++;
            }
            
            if(s[i]!= ' '){
                tmp+=s[i];
            }
        }
    }
     parsed.push_back(tmp);
    return lv;
}



int main(int argc, const char * argv[]) {
    
    cout<<"MAKE SURE THE INPUT TREE IS FILTERED TREE"<<endl;
    ifstream input;
    input.open("/Volumes/easystore/Wiki/EMNLP/6_Edge_list/filtered_wiki_tree.txt");
    string line = "";
    int counter = 0;
    ofstream outfile;
    outfile.open("/Volumes/easystore/Wiki/EMNLP/6_Edge_list/avg_node_size.txt");
    vector<string> parsed;
    int lv = 1;
    int curr_lv = 0;
    int num_node = 0;
    
    unordered_set<string> arts;
    
    while(!input.eof()){
        getline(input, line);
        if(line.size()!=0){
            parsed.clear();
            curr_lv = parse(line,parsed);
            if(curr_lv!=lv){
                cout<<lv<<" "<<arts.size()<<" "<<num_node<<" "<<1.0*arts.size()/num_node<<endl;
                outfile<<lv<<" "<<1.0*arts.size()/num_node<<" "<<arts.size()<<endl;
                arts.clear();
                num_node = 0;
                lv = curr_lv;
            }
            num_node++;
            for(int i=0;i<parsed.size();i++){
                if (i==0) continue;
                arts.insert(parsed[i]);
            }
            
        }
    }
    //last one
    outfile<<lv<<" "<<arts.size()/num_node<<" "<<arts.size()<<endl;
    arts.clear();
    num_node = 0;
    lv = curr_lv;
    
    
    
    
    return 0;
}
