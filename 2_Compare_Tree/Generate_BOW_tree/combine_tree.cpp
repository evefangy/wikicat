//
//  main.cpp
//  combine_tree_enc_n_group_art
//
//  Created by Eve on 2/3/18.
//  Copyright © 2018 Eve. All rights reserved.
//

// The code reads in 2-3 files
// combine tree_enc and group_art
// if group_art is represented by encoded ids, match the id with the actual article id


#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

void parse(vector<string>& parsed, string& line){
    string tmp = "";
    for(int i=0;i<line.size();i++){
        if(line[i]==' '){
            parsed.push_back(tmp);
            tmp = "";
        }
        else if(line[i]!=',' && line[i]!=':'){
            tmp+=line[i];
        }
    }
    parsed.push_back(tmp);
}

void store_name(string& name_file, unordered_map<int, int>& code_name){
    ifstream artname;
    artname.open(name_file);
    string line = "";
    vector<string> parsed;
    while(!artname.eof()){
        getline(artname, line);
        if(line.size()!=0){
            parsed.clear();
            parse(parsed, line);
            code_name[stoi(parsed[0])] = stoi(parsed[1]);
        }
    }
    artname.close();
}

int main(int argc, const char * argv[]) {
    if(argc < 4){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Tree_Address Name_encoding_file_address Output_Address "<<endl;
        cout<<"(e.g ./tree.txt ./art_name_filtered.txt ./ )"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    

    string tree_addr = argv[1];
    string artname_addr = argv[2];
    string output_addr = argv[3];
    output_addr+="/tree.txt";
    

    //get tree enc
    ifstream tree;
    tree.open(tree_addr);
    
    //decode file
    ofstream outfile;
    outfile.open(output_addr);
    
    
    string line = "";
    vector<string> parsed;
    unordered_map<int, int> code_name;
    store_name(artname_addr, code_name);
    
    
    
    while(!tree.eof()){
        getline(tree, line);
        parsed.clear();
        if(line.size()!=0){
            parse(parsed, line);
            outfile<<parsed[0];
            if(parsed.size()>1){
                //translate encoded id into actual page id
                
                for(int i=1;i<parsed.size();i++){
                    parsed[i] = to_string(code_name[stoi(parsed[i])]);
                }
                
                
                for(int i=1;i<parsed.size();i++){
                    outfile<<", "<<parsed[i];
                }
            }
            outfile<<endl;
        }
        

    }
    
    tree.close();
    
    outfile.close();
    
    
    return 0;
}
