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
        if(line[i]==',' || line[i]==':' ){
            if (tmp.size()!=0 && tmp!=" "){
                parsed.push_back(tmp);
            }
           
            tmp = "";
        }
        else{
            if(line[i]!=' '){
                tmp+=line[i];
                
            }
        }
    }
    if (tmp.size()!=0 && tmp!=" "){
        parsed.push_back(tmp);
    }
}

int main(int argc, const char * argv[]) {
    if(argc < 4){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"artname_file | revision_tree_file | Output_Address | [optional: used article (0/1)]"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string artname_file_addr = argv[1];
    string tree_addr = argv[2];
    string output_addr = argv[3];
    output_addr+="/7_decoded_tree.txt";
    
    string used_art_addr = argv[3];
    used_art_addr += "/used_article.txt";
    
    ifstream artname_file, tree;
    artname_file.open(artname_file_addr);
    tree.open(tree_addr);
    
    ofstream outfile;
    outfile.open(output_addr);

    ofstream used_art;
    if (argc == 5){
        used_art.open(used_art_addr);
    }
    
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
            if(parsed.size()!=2){
                cerr<<"parsed.size()!=2"<<endl;
                exit(0);
            }
            id_name[parsed[0]] = parsed[1];
            c++;
        }
    }
    cout<<"Stored "<<c<<" articles"<<endl;
    
    
    artname_file.close();

    c = 0;
    int num_used_art = 0;
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

                    if (argc == 5){
                        used_art<<id_name[parsed[i]]<<endl;
                        num_used_art++;
                    }

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
    cout<<"There are "<<num_used_art<<" articles used in the tree"<<endl;
    return 0;
}

