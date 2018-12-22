//
//  main.cpp
//  1_Get_50_Random_Trees
//
//  Created by Eve on 9/19/18.
//  Copyright © 2018 Eve. All rights reserved.
//

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
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
using namespace std;
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



int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL ) {
        cout << "Error (" << errno << ") opening " << dir << endl;
        return errno;
    }
    
    while ((dirp = readdir(dp)) != NULL) {
        if (string(dirp->d_name)!= "." && string(dirp->d_name) != ".." && string(dirp->d_name) != ".DS_Store"){
            string tree_name = string(dirp->d_name);
            if (tree_name!="wiki_tree.txt"){
                files.push_back(tree_name);
            }
        }
    }
    closedir(dp);
    return 0;
}

void generate_random_tree(string& output_addr, string& tree_enco_addr){
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
}


int main(int argc, const char * argv[]) {
    if(argc < 4){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Note that all trees should have same articles and ids. "<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Tree_Address | Output_Address | Number_of_Groups_to_Sample"<<endl;
        cout<<"(e.g ./all_trees ./ 50 )"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string tree_folder_addr = argv[1];
    string output_addr = argv[2];
    string num_groups_sample_str = argv[3];
    int num_groups_sample = stoi(num_groups_sample_str);
    
    vector<string> tree_files;
    getdir(tree_folder_addr, tree_files);
    
    
    //sample 50 groups of articles
    for (int i=0;i<num_groups_sample;i++){

        //create a folder to hold all trees
        string folder_path = output_addr+"/"+to_string(i);
        if (mkdir(folder_path.c_str(), 0777)!=0 && errno != EEXIST){
            cerr<<"Failed to create folder "<<folder_path<<endl;
            exit(0);
        }
        
        
        //filter out other articles from the trees
        for (int j=0;j<tree_files.size();j++){
            //for each tree
            string random_tree_addr = output_addr+"/"+to_string(i)+"/random_"+tree_files[j];
            string original_tree_addr = tree_folder_addr+"/"+tree_files[j];
            generate_random_tree(random_tree_addr, original_tree_addr);
        }
        
        
        
    }

    
    
    
    
    
//    //load in and filter out articles in the bigger tree (wiki tree)
//    ofstream outfile;
//    outfile.open(output_addr);
//
//    ifstream out_tree_file;
//    tree_file.open(tree_1_addr);
//    if(!tree_file){
//        cerr<<"Can't open file"<<endl;
//        exit(0);
//    }
//    line = "";
//    parsed.clear();
//
//    unordered_set<string> unique_art_wiki;
//    while(!tree_file.eof()){
//        line = "";
//        parsed.clear();
//        getline(tree_file, line);
//        parse(parsed, line);
//        outfile<<parsed[0];
//        if(parsed.size()>1){
//            for(int i=1;i<parsed.size();i++){
//                //if the article is also in the smaller tree
//                if(unique_art.count(parsed[i])==1){
//                    outfile<<", "<<parsed[i];
//                    unique_art_wiki.insert(parsed[i]);
//                }
//                //                else{
//                //                    cout<<parsed[i]<<endl;
//                //                }
//            }
//        }
//        outfile<<endl;
//    }
//    tree_file.close();
//    outfile.close();
//
//    cout<<"After filter out articles that are not in the small tree"<<endl;
//    cout<<"There are in total "<<unique_art_wiki.size()<<" unique articles in the bigger tree"<<endl;
//
    
    
    return 0;
}


