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
}

int main(int argc, const char * argv[]) {
    if(argc < 4){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Tree_Encode_Address | Group_Art_Address | Output_Address"<<endl;
        cout<<"(e.g ./tree_enc.txt ./group_art.txt ./)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    

    string tree_enc_addr = argv[1];
    string group_art_addr = argv[2];
    string output_addr = argv[3];
    output_addr+="/7_combined_tree(encoded).txt";
    
    //store name encoding
    string name_file;
    
    //get tree enc
    ifstream tree_enc;
    tree_enc.open(tree_enc_addr);
    //get group art
    ifstream group_art;
    group_art.open(group_art_addr);
    //combined file
    ofstream outfile;
    outfile.open(output_addr);
    
    
    string enc = "";
    string arts = "";
    string curr_node = "";
    vector<string> parsed;
    while(!tree_enc.eof()){
        enc = "";
        arts = "";
        getline(tree_enc, enc);
        getline(group_art, arts);
        if (enc.size()==0 && arts.size()==0) continue;
        parsed.clear();
        parse(parsed, enc);
        // error check: no group id for the current node
        if (parsed.size()!=2){
            cerr<<"No group id for the current node"<<endl;
            cout<<enc<<endl;
            exit(0);
        }
        outfile<<parsed[0];
        curr_node = parsed[1];
        parsed.clear();
        parse(parsed, arts);

        //error check: group id for tree encoding doesn't match with group id for the articles
        if (parsed[0]!=curr_node){
            cerr<<"Group id for tree encoding doesn't match with group id for the articles"<<endl;
            cout<<"curr_node [in tree_enco] = "<<curr_node<<endl;
            cout<<"curr_node [in articles] = "<<parsed[0]<<endl;
            exit(0); 
        }
        if(parsed.size()>1){
            for(int i=1;i<parsed.size();i++){
                if (parsed[i].size()!=0 && parsed[i]!=" "){
                    outfile<<", "<<parsed[i];
                }
            }
        }
        outfile<<endl;

    }
    
    tree_enc.close();
    group_art.close();
    outfile.close();
    
    
    return 0;
}
