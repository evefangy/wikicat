//
//  main.cpp
//  7_get_article_weight_pair_new
//
//  Created by Eve on 10/24/17.
//  Copyright © 2017 Eve. All rights reserved.
//

//
//  main.cpp
//  7_get_article_weight_pair
//
//  Created by Eve on 10/17/17.
//  Copyright © 2017 Eve. All rights reserved.
//


/*********************************************************************************/
// The code goes through artname created and get articles that don't share any
// edges -> create an edge to link all those articles with a weight that is smaller
// than current minimum weight
/*********************************************************************************/

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
    if(argc < 5){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Address_for_artname | Address_for_edge_list | Output_address | Number_of_articles"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string artname_add = argv[1];
    string edge_addr = argv[2];
    string output_address = argv[3];
    output_address+="/new_edge_list.txt";
    string tmp = argv[4];
    int num_art = stoi(tmp);

    
    ofstream outfile;
    outfile.open(output_address);
    
    //initiate map for storing article_name and its index (for look up)
    unordered_set<string> stored_article;
    
    //local variables
    vector<string> parsed;
    string line = "";

    ifstream infile;
    infile.open(artname_add);

    if(!infile){
        cerr<<"Can't open infile"<<endl;
        exit(1);
    }

    while(!infile.eof()){
        getline(infile,line);
        if (!line.empty()){
            
            parsed.clear();
            parse(line, parsed);
            if(stored_article.count(parsed[0])!=0){
                cerr<<"existed"<<endl;
                exit(0);
            }
            stored_article.insert(parsed[0]);
            
        }
    }
    infile.close();

    cout<<"Finish storing artname"<<endl;
    cout<<"There are "<<stored_article.size()<<" articles with at least one edge"<<endl;
    
    vector<string> unstored;
    string curr_art="";
    for(int i=0;i<num_art;i++){
        curr_art = to_string(i);
        if(stored_article.count(curr_art)==0){
            unstored.push_back(curr_art);
        }
    }
    
    cout<<"There are "<<unstored.size()<<" articles with no edge"<<endl;
    
    infile.open(edge_addr);
    
    if(!infile){
        cerr<<"Can't open infile"<<endl;
        exit(1);
    }
    
    
    int counter = 0;
    while(!infile.eof()){
        getline(infile,line);
        if (!line.empty()){
            outfile<<line<<endl;
            counter++;
        }
    }
    
    for(int i=0; i<unstored.size();i++){
        if(i==0){
            continue;
        }
        outfile<<unstored[0]<<", "<<unstored[i]<<", -1"<<endl;
        counter++;
    }
    
    
    infile.close();
    
    
    
    cout<<"Completed"<<endl;
    
    cout<<"-------------------------------"<<endl;

    cout<<"There are "<<counter<<" weight pairs in total"<<endl;

    return 0;
}

