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
// The code goes through the list of all articles we used to create BoW tree
// and create a root node to hold all articles that did not get saved in the generated
// tree (article content cannot be found/article content is now <100B/article shares
// too few (we used 3 for cutoff) high tf-idf word with any other articles)
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
            parsed.push_back(tmp);
            tmp = "";
        }
        else{
            if(s[i]!= ' '){
                tmp+=s[i];
            }
        }
    }
    parsed.push_back(tmp);
}

int count_levels(string& s){
    int lv = 0;
    for(int i=0; i<s.length();i++){
        if(s[i]=='_'){
            lv++;
        }
    }
    return lv;
}







int main(int argc, const char * argv[]) {
    if(argc < 4){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Address_for_used_article | Address_for_decoded_tree | Output_address"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string used_art_addr = argv[1];
    string decoded_tree_addr = argv[2];
    string output_address = argv[3];
    output_address+="/9_BoW_tree.txt";

    
    ofstream outfile;
    outfile.open(output_address);
    
    //initiate set for storing encountered articles
    unordered_set<string> stored_article;
    vector<string> unstored;
    
    //local variables
    vector<string> parsed;
    string line = "";

    ifstream infile;
    infile.open(decoded_tree_addr);


    //get all articles in the generated tree
    if(!infile){
        cerr<<"Can't open "<<decoded_tree_addr<<endl;
        exit(1);
    }

    while(!infile.eof()){
        getline(infile,line);
        if (!line.empty()){
            
            parsed.clear();
            parse(line, parsed);

            for (int i=1;i<parsed.size();i++){
                if(stored_article.count(parsed[i])!=0){
                cerr<<"generated tree contains duplicates [article "<<parsed[i]<<"]"<<endl;
                exit(0);
                }
                stored_article.insert(parsed[i]);
            }
        }
    }
    infile.close();

    cout<<"There are "<<stored_article.size()<<" articles in the generated tree"<<endl;
    
    


    //get articles that did not get stored in the tree
    infile.open(used_art_addr);
    //get all articles in the generated tree
    if(!infile){
        cerr<<"Can't open "<<used_art_addr<<endl;
        exit(1);
    }

    while(!infile.eof()){
        getline(infile,line);
        if (!line.empty()){
            
            parsed.clear();
            parse(line, parsed);

            if (parsed.size()!=1){
                cerr<<"parsed.size()!=1 for each line in used_article.txt"<<endl;
                exit(0);
            }

            //check if it's an article that didn't get stored
            if (stored_article.count(parsed[0])==0){
                unstored.push_back(parsed[0]);
            }
        }
    }
    infile.close();

    cout<<"There are "<<unstored.size()<<" articles that did not get stored in the tree"<<endl;

    //create a node to hold all unstored articles
    outfile<<"_0"<<endl;

    infile.open(decoded_tree_addr);
    
    if(!infile){
        cerr<<"Can't open "<<decoded_tree_addr<<endl;
        exit(1);
    }
    
    int line_ctr = 0;
    int num_lv = INT_MIN;
    while(!infile.eof()){
        getline(infile,line);
        if (!line.empty()){
            parsed.clear();
            parse(line, parsed);
            string new_node_code = "";
            if (line_ctr == 0){
                if (parsed[0]!="_0" || parsed.size()!=1){
                    cerr<<"The first node in the generated tree is not empty"<<endl;
                    cout<<line<<endl;
                    exit(0);
                }
                line_ctr++;
                new_node_code = "_0_0";
                outfile<<new_node_code;
                for (int i=0; i<unstored.size();i++){
                    outfile<<", "<<unstored[i];
                }
                outfile<<endl;
            }
            else{
                new_node_code = "_0"+parsed[0];
                outfile<<new_node_code;
                for (int i=1;i<parsed.size();i++){
                    outfile<<", "<<parsed[i];
                }
                outfile<<endl;
            }
            num_lv = max(num_lv, count_levels(new_node_code));
        }
    }
    
    
    infile.close();
    
    
    cout<<"There are in total "<<unstored.size()+stored_article.size()<<" articles now in the tree"<<endl;

    cout<<"There are "<<num_lv<<" levels in the tree"<<endl;
    
    cout<<"Completed"<<endl;

    return 0;
}

