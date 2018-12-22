//
//  main.cpp
//  generate_random
//
//  Created by Eve on 3/9/18.
//  Copyright © 2018 Eve. All rights reserved.
//


/****************************************************************/
// The code is written for randomly choose an article of a given
// structure, where the structure (tree enco) and number of
// articles in each node is the same.
//
// [Input format]
// <Tree.txt>
// tree_enc_, art_id1, art_id2, ...
//
// <Artname.txt>
// index, article id
//
//
// [Output format]
// (random tree has articles in their real article ids)
// Tree encoding, random article 1, random article 2 ...
//
/***************************************************************/

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
        cout<<"Tree_Address(merged group_art and tree_enco) | Output_Address"<<endl;
        cout<<"(e.g ./tree.txt ./Output)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string tree_enco_addr = argv[1];
    string output_addr = argv[2];
    output_addr+="/random_tree_structure.txt";
    
    srand ( unsigned ( std::time(0) ) );
    
    // get all articles in the tree
    vector<string> all_articles;
    unordered_set<string> visited;
    
    string line = "";
    vector<string> parsed;
    ifstream tree_file;
    tree_file.open(tree_enco_addr);
    while(!tree_file.eof()){
        getline(tree_file, line);
        if(line.size()!=0){
            parsed.clear();
            parse(parsed, line);
            for (int i=1; i<parsed.size();i++){
                if (parsed[i].size()!=0 && parsed[i]!=" "){
                    if (visited.count(parsed[i])!=0){
                        cerr<<"Duplicated article ["<<parsed[i]<<"] found in the tree"<<endl;
                        exit(0);
                    }
                    visited.insert(parsed[i]);
                    all_articles.push_back(parsed[i]);
                }
            }
            
        }
    }
    cout<<"There are "<<visited.size()<<" unique articles in the tree"<<endl;
    tree_file.close();
    
    
    // randomly permute the index
    vector<int> index_to_insert;
    for (int i=0; i<visited.size(); i++) {
        index_to_insert.push_back(i);
    }
    random_shuffle ( index_to_insert.begin(), index_to_insert.end() );




    tree_file.open(tree_enco_addr);
    int num_art_inserted = 0;
    int num_art_for_curr_node = 0;
    
    ofstream outfile;
    outfile.open(output_addr);

    int index = 0;
    
    while(!tree_file.eof()){
        getline(tree_file, line);
        if(line.size()!=0){
            parsed.clear();
            parse(parsed, line);
            //calculate number of articles in each node
            num_art_for_curr_node = parsed.size()-1;
            outfile<<parsed[0];
            //randomly pick and store n articles in the node
            for(int i=0;i<num_art_for_curr_node;i++){
                index = index_to_insert[num_art_inserted++];
                outfile<<", "<<all_articles[index];
            }
            outfile<<endl;
        }
    }
    
    tree_file.close();
    outfile.close();

    cout<<"Randomly permuted "<<num_art_inserted<<" articles"<<endl;

    if (num_art_inserted!=visited.size()){
        cerr<<endl<<"num_art_inserted!=visited.size()"<<endl;

    }
    
    
    
    return 0;
}
