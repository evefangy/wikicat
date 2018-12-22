
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

//ofstream artname;
//bool first = false;

void parse(vector<string>& parsed, string& line){
    string tmp = "";
    for(int i=0;i<line.size();i++){
        if(line[i]==' '){
            if (tmp!=" "&&tmp.size()!=0){
                parsed.push_back(tmp);
            }
            
            tmp = "";
        }
        else if(line[i]!=','){
            tmp+=line[i];
        }
    }
    if (tmp!=" "&&tmp.size()!=0){
        parsed.push_back(tmp);
    }
}

int calculate_lv(string& s){
    int lv = 0;
    for (int i=0;i<s.size();i++){
        if (s[i]=='_'){
            lv++;
        }
    }
    return lv;
}

void store_tree(string& addr, unordered_map<string, vector<string> >& t){
    ifstream tree_file;
    tree_file.open(addr);
    if(!tree_file){
        cerr<<"Can't open file"<<endl;
        exit(0);
    }
    string line = "";
    vector<string> parsed;
    while(!tree_file.eof()){
        line = "";
        parsed.clear();
        getline(tree_file, line);
        parse(parsed, line);
        if(parsed.size()>1){
            for(int i=1;i<parsed.size();i++){
                
                
                if(t.count(parsed[i])==1){
                    t[parsed[i]].push_back(parsed[0]);
                    
                }
                else{
//                    if(first){
//                        artname<<parsed[i]<<", "<<parsed[i]<<endl;
//                    }
                    vector<string> location;
                    location.clear();
                    location.push_back(parsed[0]);
                    t[parsed[i]] = location;
                }
            }
        }
    }
    tree_file.close();
}
int main(int argc, const char * argv[]) {
    if(argc < 2){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Rev_tree_Address"<<endl;
        cout<<"(e.g ./rev_tree.txt )"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string tree_1_addr = argv[1];
    
    map<int, int> lv_counter;

    ifstream infile;
    infile.open(tree_1_addr);
    string line="";
    vector<string> parsed;
    int total_num_art = 0;
    while(!infile.eof()){
        getline(infile, line);
        if (line!="" && line.size()!=0){
            parsed.clear();
            parse(parsed, line);

            string tree_enco = parsed[0];
            int lv = calculate_lv(tree_enco);
            lv_counter[lv]=lv_counter[lv]+parsed.size()-1;
            total_num_art = total_num_art+parsed.size()-1;
        }
    }

    int num_art = 0;
    for (auto it=lv_counter.begin();it!=lv_counter.end();it++){
        cout<<it->first<<" "<<it->second<<" "<<(it->second/total_num_art)*100<<"%"<<endl;
        num_art+=it->second;
    }

    cout<<"There are in total "<<num_art<<" articles"<<endl;
  
    return 0;
    
}
