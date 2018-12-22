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

void parse(vector<string>& parsed, string& line){
    string tmp = "";
    for(int i=0;i<line.size();i++){
        if(line[i]==' '){
            parsed.push_back(tmp);
            tmp = "";
        }
        else if(line[i]!=','){
            tmp+=line[i];
        }
    }
    parsed.push_back(tmp);
}

int main(int argc, const char * argv[]) {
    if(argc < 5){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Tree_Address(merged group_art and tree_enco) | Used_Article_addr | Output_Address | Num_Nodes"<<endl;
        cout<<"(e.g ./tree.txt ./used_art.txt ./Output 27)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string tree_enco_addr = argv[1];
    string artname_addr = argv[2];
    string output_addr = argv[3];
    output_addr+="/random_tree_structure_new.txt";
    
    string t_n = argv[4];
    int tot_nodes = stoi(t_n);
    
    // get all articles in the tree
    vector<string> all_articles;
    set<int> visited;
    
    string line = "";
    vector<string> parsed;
    ifstream artname;
    artname.open(artname_addr);
    while(!artname.eof()){
        getline(artname, line);
        if(line.size()!=0){
            parsed.clear();
            parse(parsed, line);
            if(parsed.size()!=1){
                cerr<<"parsed.size()!=1"<<endl;
                exit (0);
            }
            all_articles.push_back(parsed[0]); //store real article ids
        }
    }
    
    artname.close();
    
    //get revision tree structure
    ifstream tree_enco;
    tree_enco.open(tree_enco_addr);
    
    int num_art_per_node = 0;
    int total_art_num = all_articles.size();
    int num_art_rem = total_art_num;
    int avg_num_per_node = total_art_num/tot_nodes*2;
    if(avg_num_per_node==0) avg_num_per_node = 1;
    int index = 0;
    
    ofstream outfile;
    outfile.open(output_addr);
    int c=0;
    while(!tree_enco.eof()){
        getline(tree_enco, line);
        c++;
        if(line.size()!=0){
            parsed.clear();
            parse(parsed, line);
            //calculate number of articles in each node
            num_art_per_node = rand()%(avg_num_per_node);
            num_art_rem -= num_art_per_node;
            outfile<<parsed[0];
            
            if(c == tot_nodes){
                num_art_per_node = num_art_rem;
                num_art_rem = 0;
            }
            
            //randomly pick and store n articles in the node
            for(int i=0;i<num_art_per_node;i++){
                index = rand()%total_art_num;
                while(visited.find(index)!=visited.end()){
                    index = rand()%total_art_num;
                }
                visited.insert(index);
                outfile<<", "<<all_articles[index];
            }
            outfile<<endl;
            
        }
    }
    
    if(num_art_rem!=0){
        cerr<<"Didn't store all articles"<<endl;
        exit(0);
    }
    
    tree_enco.close();
    outfile.close();
    
    
    
    return 0;
}

