//
//  main.cpp
//  convert_name_to_id
//
//  Created by Eve on 2/6/18.
//  Copyright © 2018 Eve. All rights reserved.
//

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

void parse(vector<string>& parsed, string& line){
    string tmp = "";
    for(int i=0;i<line.size();i++){
        if(line[i]==' '){
            parsed.push_back(tmp);
            tmp = "";
        }
        else{
            if(line[i]!=':' && line[i]!=','){
                tmp+=line[i];
                
            }
        }
    }
    parsed.push_back(tmp);
}

int main(int argc, const char * argv[]) {
    if(argc < 4){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"valid_article_file | revision_tree_file | Output_Address"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string artname_file_addr = argv[1];
    string tree_addr = argv[2];
    string output_addr = argv[3];
    output_addr+="/7_decoded_tree.txt";
    
   
    
    ifstream artname_file, tree;
    artname_file.open(artname_file_addr);
    tree.open(tree_addr);
    
    ofstream outfile;
    outfile.open(output_addr);
    
    if(!outfile){
        cerr<<"can't open output file"<<endl;
        exit(0);
    }
    
    //read in id file
    map<string, string> id_name;
    string line;
    string id, name;
    vector<string> parsed;
    int c=0;
    while(!artname_file.eof()){
        line = "";
        getline(artname_file, line);
        if(line.size()>0){
            parsed.clear();
            parse(parsed, line);
            if(parsed.size()!=1){
                // cerr<<"parsed.size()!=1"<<endl;
                // exit(0);
                if (parsed[0]!=to_string(c)){
                    cerr<<"parsed[0]!=to_string(c)"<<endl;
                }
                id_name[parsed[0]] = parsed[1];
            }
            else{
                id_name[to_string(c)] = parsed[0];
            }
            
            c++;
        }
    }
    cout<<"Stored "<<c<<" articles"<<endl;
    
    
    artname_file.close();

    c = 0;
    while(!tree.eof()){
        line = "";
        getline(tree, line);
        if(line.size()!=0){
            parsed.clear();
            parse(parsed, line);
            
            outfile<<parsed[0];
            for(int i=1;i<parsed.size();i++){
                if(parsed[i]!=" " && parsed[i].size()!=0){
                    if (id_name.count(parsed[i])==0){
                        cerr<<"Encoded article "<<parsed[i]<<" can't be decoded"<<endl;
                        exit(0);
                    }
                    outfile<<", "<<id_name[parsed[i]];
                    c++;
                }
                
            }
            outfile<<endl;
        }
        
    }
    
    tree.close();
    outfile.close();

    cout<<"Decoded "<<c<<" articles"<<endl;
    cout<<"Number of decoded articles indicate the number of articles in the tree"<<endl;
    
    return 0;
}

