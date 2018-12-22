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
    parsed.clear();
    string tmp = "";
    for(int i=0;i<line.size();i++){
        if(line[i]==','){
            if (tmp.size()!=0 && tmp!=" "){
                parsed.push_back(tmp);
            }
            
            tmp = "";
        }
        else if(line[i]!=' '){
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
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
    
    while ((dirp = readdir(dp)) != NULL) {
        if (string(dirp->d_name)!= "." && string(dirp->d_name) != ".." && string(dirp->d_name) != ".DS_Store"){
            files.push_back(string(dirp->d_name));
        }
    }
    closedir(dp);
    return 0;
}


int main(int argc, const char * argv[]) {
    if(argc < 5){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Note that all trees should have same articles and ids. "<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Tree_Address | Article_id_addr | Output_Address | Number_of_Groups_to_Sample"<<endl;
        cout<<"(e.g ./all_trees ./artid.txt ./ 50 )"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string tree_folder_addr = argv[1];
    string artid_addr = argv[2];
    string output_addr = argv[3];
    string num_groups_sample_str = argv[4];
    int num_groups_sample = stoi(num_groups_sample_str);
    
    vector<string> tree_files;
    getdir(tree_folder_addr, tree_files);
    
    
    string line = "";
    vector<string> parsed;
    
    //get all trees
    vector< vector<vector<string> > > trees;
    ifstream tree_file;
 
    
    for (int i=0;i<tree_files.size();i++){
        string tree_file_addr = tree_folder_addr+"/"+tree_files[i];
        cout<<"Reading in tree <"<<tree_files[i]<<">"<<endl;
        
        tree_file.open(tree_file_addr);
        vector<vector<string> > tree1;
        tree1.clear();
        while (!tree_file.eof()){
            line = "";
            parsed.clear();
            getline(tree_file, line);
            parse(parsed, line);
            if (parsed.size()!=0){
                tree1.push_back(parsed);
            }
            
        }
        trees.push_back(tree1);
        tree_file.close();
    }
    
    cout<<"Reading in used articles..."<<endl;
   //get all articles to be sampled
    unordered_set<string> unique_art_set;
    vector<string> unique_art_vec;
    ifstream artid;

    artid.open(artid_addr);
    if(!artid){
        cerr<<"Can't open file "<<artid_addr<<endl;
        exit(0);
    }
    
    while(!artid.eof()){
        line = "";
        parsed.clear();
        getline(artid, line);
        if (line.size()==0) continue;
        parse(parsed, line);
        
        string article = parsed[0];
        if(unique_art_set.count(article)==0){
            unique_art_set.insert(article);
            unique_art_vec.push_back(article);
        }
        else{
            cerr<<"Found duplicate article <"<<article<<">"<<endl;
            exit(0);
        }
        
    }
    artid.close();
    long num_unique_article = unique_art_vec.size();
    cout<<"There are in total "<<num_unique_article<<" unique articles in the tree"<<endl;
    

    long num_article_per_group = 25000;

    if (num_unique_article<num_article_per_group){
        num_article_per_group = num_unique_article*.75;
    }
    cout<<"We are sampling "<<num_article_per_group<<" articles for each group"<<endl;
    
    
    ofstream used_articles;
    ofstream filtered_tree;

    //sample 50 groups of articles
    for (int i=0;i<num_groups_sample;i++){

        //create a folder to hold all trees
        string folder_path = output_addr+"/"+to_string(i);
        if (mkdir(folder_path.c_str(), 0777)!=0 && errno != EEXIST){
            cerr<<"Failed to create folder "<<folder_path<<endl;
            exit(0);
        }
        //create a text file to record used articles for the tree
        string used_articles_addr = folder_path+"/used_articles.txt";
        used_articles.open(used_articles_addr);

        unordered_set<string> sampled_group;
        srand((int)time(NULL));
            // randomly permute the index
        vector<int> index_to_insert;
        for (int i=0; i<num_unique_article; i++) {
            index_to_insert.push_back(i);
        }
        random_shuffle ( index_to_insert.begin(), index_to_insert.end() );


        for (int q=0;q<num_article_per_group;q++){
            string sample_article = unique_art_vec[index_to_insert[q]];
            sampled_group.insert(sample_article);
            used_articles<<sample_article<<endl;
        }

        used_articles.close();
        
        if (sampled_group.size()!=num_article_per_group){
            cout<<"++++++++++++++++++++++++++"<<endl;
            cerr<<"num_sampled = "<<num_article_per_group<<endl;
            cerr<<"sampled_group.size() = "<<sampled_group.size()<<endl;
            cout<<"++++++++++++++++++++++++++"<<endl;
        }
        
        //filter out other articles from the trees
        for (int j=0;j<trees.size();j++){
            
            int ctr = 0;
            int notsampled = 0;
            //for each tree
            string tree_name = output_addr+"/"+to_string(i)+"/"+tree_files[j];
            filtered_tree.open(tree_name);

            for (int k=0;k<trees[j].size();k++){
                // cout<<filtered_tree<<trees[j][k][0]<<endl;
                filtered_tree<<trees[j][k][0];
                for (int q=1;q<trees[j][k].size();q++){
                    // cout<<filtered_tree<<trees[j][k][q]<<endl;
                    if (sampled_group.count(trees[j][k][q])==1){
                        filtered_tree<<", "<<trees[j][k][q];
                        ctr++;
                    }
                    else{
                        if (trees[j][k][q].length()!=0){
                            notsampled++;
                        }
                        
                    }
                }
                filtered_tree<<endl;
            }
            filtered_tree.close();
cout<<"here5"<<endl;
            
            if (ctr!=num_article_per_group){
                cerr<<"Didn't sample the correct amount of articles for "<<tree_files[j]<<endl;
                cerr<<"num_sampled = "<<num_article_per_group<<endl;
                cerr<<"ctr = "<<ctr<<endl;
                cerr<<"notsampled = "<<notsampled<<endl;
                cout<<"-----------------------------------------------"<<endl;
//                cout<<"[It's fine if wikitree has more articles because it has duplicates]"<<endl;
//                exit(0);
            }
cout<<"here6"<<endl;
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


