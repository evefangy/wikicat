//
//  main.cpp
//  8_sort_art_weight
//
//  Created by Eve on 10/17/17.
//  Copyright © 2017 Eve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
using namespace std;

int c;
bool change_a1 = false;
bool change_a2 = false;
unordered_map<string, int> a_index;

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

void addToMap(string& s, map<int, vector<pair<int,int> >,greater<int> >& m, vector<string>& parsed){
    parsed.clear();
    parse(s,parsed);
    if(parsed.size()<3) {
        cerr<<"incomplete edge list: "<<s<<endl;
        return;
    }
    vector<pair<int,int> > tmp;
    int w = stoi(parsed[2]);
    //    map<int, vector<pair<string,string> > >::iterator it;
    map<int, vector<pair<int,int> > >::iterator it;
    it = m.find(w);
    if(it == m.end()){
        tmp.clear();
        
        if(a_index.count(parsed[0]) == 0){
            c+=1;
            a_index[parsed[0]] = c;
            change_a1 = true;
//            cout<<parsed[0]<<endl;
           
        }
        if(a_index.count(parsed[1]) == 0){
            c+=1;
            a_index[parsed[1]] = c;
            change_a2 = true;
//            cout<<parsed[1]<<endl;
        }
        
        tmp.push_back(make_pair(a_index[parsed[0]], a_index[parsed[1]]));
//        cout<<"here"<<endl;
        m[w] = tmp;
    }
    else{
        if(a_index.count(parsed[0]) == 0){
            c+=1;
            a_index[parsed[0]] = c;
            change_a1 = true;
//            cout<<parsed[0]<<endl;
            
        }
        if(a_index.count(parsed[1]) == 0){
            c+=1;
            a_index[parsed[1]] = c;
            change_a2 = true;
//            cout<<parsed[1]<<endl;
        }
        it->second.push_back(make_pair(a_index[parsed[0]], a_index[parsed[1]]));
//        cout<<"here2"<<endl;
    }
}
int main(int argc, const char * argv[]) {
    if(argc < 3){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Address_for_art_weight_pair_file Output_address"<<endl;
        cout<<"(e.g ./art_weight_pair_cate_0_to_100000.txt ./)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    string output_address = argv[2];
    string sorted_addr = output_address+"/sorted_weight.txt";
    string num_edge_addr = output_address+"/num_edge.txt";
    
    map<int, vector<pair<int,int> >, greater<int> > sorted;

//    cout<<a_index.size()<<endl;
    map<int, vector<pair<int,int> > >::iterator it;
    // input article_weight pair
    ifstream infile;
    infile.open(argv[1]);

    // sorted graph
    ofstream weight_graph;
    weight_graph.open(sorted_addr);
    
    // number of edges with certain weight
    ofstream num_edge;
    num_edge.open (num_edge_addr);
    
    // ************ parse art_weight_pair.txt ************* //
    string line = "";
    vector<string> parsed;
    c = -1;
    
    if(!infile){
        cerr<<"Can't open the file"<<endl;
        exit(1);
    }
    while(!infile.eof()){
        getline(infile,line);
        
        if (!line.empty()){
            addToMap(line, sorted, parsed);
//            if(change_a1){
//                change_a1 = false;
//                artname<<a_index[parsed[0]]<<", "<<parsed[0]<<endl;
//            }
//            if(change_a2){
//                change_a2 = false;
//                artname<<a_index[parsed[1]]<<", "<<parsed[1]<<endl;
//            }
        }
    }
    infile.close();
    
    num_edge<<"Weight | Number of Article Pairs "<<endl;
    for(it=sorted.begin();it!=sorted.end();it++){
        for(int i=0;i<it->second.size();i++){
            weight_graph<<it->first<<", "<<it->second[i].first<<", "<<it->second[i].second<<endl;
        }
        num_edge<<it->first<<"        "<<it->second.size()<<endl;
    }
    num_edge.close();
    weight_graph.close();
    
    cout<<"Completed"<<endl;
    
    return 0;
}

