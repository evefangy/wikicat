//
//  main.cpp
//  get_editor_with_art
//
//  Created by Eve on 9/19/17.
//  Copyright © 2017 Eve. All rights reserved.
//

/////////////////////////////////////////////////////////////////////////
//  Code rearrage the parsed revision history                          //
//  (following article, major editor 1, major editor 2,... format)     //
//  to the following format                                            //
//  editor 1, majorly edited article 1, majorly edited article 2, ...  //
/////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_set>
using namespace std;

void parse(string& s, vector<string>& parsed){
    string tmp = "";
    for(int i=0; i<s.length();i++){
        if(s[i]==','){
            if(s[i+1]==' '){
                parsed.push_back(tmp);
                tmp = "";
            }
            else{
                tmp+=s[i];
            }
        }
        else{
            if(s[i]!= ' '){
                tmp+=s[i];
            }
        }
    }
    parsed.push_back(tmp);
}

int main(int argc, const char * argv[]) {
    
    string output_address = "/Users/eve/Desktop/Wiki/Code/1_Generate_Tree/7_create_edge_list/7_get_article_weight_pair_map/output.txt";
    string input_addr = "/Volumes/Eve/Wiki/All_Materials_for_Paper/Experiment_for_paper/major_editor_w_art_cate_0_over_1.4M.txt";
    
    ofstream outfile;
    outfile.open (output_address);
    
    
    fstream infile;
    infile.open(input_addr);
    string line = "";
    vector<string> parsed;
    
    while(!infile.eof()){
        getline(infile,line);
        if (!line.empty()){
            parsed.clear();
            parse(line, parsed);
            cout<<parsed[0]<<" "<<parsed.size()-1<<endl;
            
        }
        
    }
    infile.close();
    
    
    
    outfile.close();
    
    
    
    
    return 1;
}
