// The code is for filtering out articles in <arts_w_major_editors> that do not exist in the wiki tree
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
using namespace std;

void parse(string& s, vector<string>& parsed){
    string tmp = "";
    for(int i=0; i<s.length();i++){
        if(s[i]==','){
            parsed.push_back(tmp);
            tmp = "";
        }
        
        
        else{
            if(s[i]!= ' '){
                tmp+=s[i];
            }
        }
    }
     parsed.push_back(tmp);
}

//index art
void store_id(unordered_set<string>& artname, string& line){
    vector<string> parsed;
    parse(line, parsed);

    if(artname.count(parsed[0])!=0){
        cerr<<"existed"<<endl;
        exit(0);
    }
    artname.insert(parsed[0]);
//    cout<<parsed[0]<<endl;
    
}

int main(int argc, const char * argv[]) {
    ifstream artname_txt;
    artname_txt.open("/Users/eve/Desktop/Wiki/Life/WikiTree/unique_art_id.txt");
    unordered_set<string> artname;
    string line = "";
    
    while(!artname_txt.eof()){
        getline(artname_txt, line);
        if(line.size()!=0){
            store_id(artname, line);
        }
    }
    
    ifstream input;
    input.open("/Volumes/easystore/Wiki/All_Materials_for_Paper/Experiment/3_Art_w_Major_Editors/unique_combined.txt");
    line = "";
    int counter = 0;
    ofstream nonexisted_arts, art_w_major_ed;
    art_w_major_ed.open("/Volumes/easystore/Wiki/All_Materials_for_Paper/Experiment/3_Art_w_Major_Editors/unique_combined(filtered).txt");
    nonexisted_arts.open("/Volumes/easystore/Wiki/All_Materials_for_Paper/Experiment/3_Art_w_Major_Editors/nonexisted.txt");
    
    vector<string> par;
    while(!input.eof()){
        getline(input, line);
        if(line.size()!=0){
            par.clear();
            parse(line,par);
            if(artname.count(par[0])==0){
                counter++;
                nonexisted_arts<<par[0]<<endl;
                continue;
            }
            art_w_major_ed<<line<<endl;
        }
    }
    
    cout<<"There are "<<counter<<" parsed articles that do not exists in wikitree"<<endl;
    
    return 0;
}
