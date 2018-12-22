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
    if(argc < 3){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<" Artname_Adress | Output_Address "<<endl;
        cout<<"(e.g ./artname.txt ./Output )"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    

    string artname_addr = argv[1];
    string output_addr = argv[2];
    output_addr+="/sorted_random_edge_list.txt";
    
    
    // get all articles in the tree
    vector<string> all_articles;
    set<int> visited;
    
    string line = "";
    vector<string> parsed;
    ifstream artname;
    artname.open(artname_addr);
    
    string id_name = argv[2];
    id_name+="/artname.txt";
    ofstream id_file;
    id_file.open(id_name);
    int counter=0;
    
    while(!artname.eof()){
        getline(artname, line);
        if(line.size()!=0){
            parsed.clear();
            parse(parsed, line);
//            if(parsed.size()!=2){
//                cerr<<"parsed.size()!=1"<<endl;
//                exit (0);
//            }
//            all_articles.push_back(parsed[1]); //store real article ids
            all_articles.push_back(parsed[0]);
            id_file<<counter<<", "<<parsed[0]<<endl;;
            counter++;
        }
    }
    artname.close();
   
    ofstream outfile;
    outfile.open(output_addr);
    int num_art = all_articles.size();
    map<int, vector<int>, std::greater<int> > edge_list;
    
    int a1=-1;
    int a2=0;
    int weight=0;
    int num_edges = 0;
    int pairs = 0;
    while(visited.size()!=num_art){
        a1 ++;
        
        
        
        while(visited.count(a2)!=0){
            a2 = rand()%num_art;
        }
        
        if(a1==a2) continue;
        weight = rand()%300+1;
        
        visited.insert(a1);
        visited.insert(a2);
        
        num_edges++;
        
        if(edge_list.count(weight)==0){
            vector<int> tmp;
            tmp.push_back(a1);
            tmp.push_back(a2);
            edge_list[weight] = tmp;
        }
        else{
            edge_list[weight].push_back(a1);
            edge_list[weight].push_back(a2);
        }
        
    }
    cout<<"randomly generated "<<num_edges<<" edges for "<<num_art<<" articles"<<endl;
    
    for(auto i=edge_list.begin();i!=edge_list.end();i++){
        for(int j=0;j<i->second.size();j+=2){
            outfile<<i->first<<", "<<i->second[j]<<", "<<i->second[j+1]<<endl;
        }
        
    }

    outfile.close();
    
    
    
    return 0;
}

