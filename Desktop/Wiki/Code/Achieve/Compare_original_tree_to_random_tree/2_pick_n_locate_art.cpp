//
//  main.cpp
//  pick_n_locate_art
//
//  Created by Eve on 2/3/18.
//  Copyright © 2018 Eve. All rights reserved.
//

/****************************************************************/
// The code is written for randomly choose an article and
// locate the article in both trees (e.g. wiki and rev tree).
//
// [Input format]
// tree_enc_, art_id1, art_id2, ...
//
//
//
// [Output format]
//
// Chosen article_id 1
// <tree encoding for the node(s) that contains the article
// in tree 1>
// <tree encoding for the node(s) that contains the article
// in tree 2>
//
// Chosen article_id 2
// <tree encoding for the node(s) that contains the article
// in tree 1>
// <tree encoding for the node(s) that contains the article
// in tree 2>
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

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

using namespace std;

//ofstream artname;
//bool first = false;

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

void store_tree(string& addr, unordered_map<string, vector<string> >& t){
    ifstream tree_file;
    tree_file.open(addr);
    if(!tree_file){
        cerr<<"Can't open file ["<<addr<<"]"<<endl;
        exit(0);
    }
    string line = "";
    vector<string> parsed;
    while(!tree_file.eof()){
        line = "";
        parsed.clear();
        getline(tree_file, line);
        parse(parsed, line);
        if(parsed.size()>1){
            for(int i=1;i<parsed.size();i++){
                
                
                if(t.count(parsed[i])==1){
                    t[parsed[i]].push_back(parsed[0]);
                    
                }
                else{
//                    if(first){
//                        artname<<parsed[i]<<", "<<parsed[i]<<endl;
//                    }
                    vector<string> location;
                    location.clear();
                    location.push_back(parsed[0]);
                    t[parsed[i]] = location;
                }
            }
        }
    }
    tree_file.close();
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
        if (dirp->d_type == DT_DIR && string(dirp->d_name)!= "." && string(dirp->d_name) != ".." && string(dirp->d_name) != ".DS_Store"){
            files.push_back(string(dirp->d_name));
        }
    }
    closedir(dp);
    return 0;
}


void getArticleLocations(string& tree_1_addr, string& tree_2_addr, string& output_addr, int& num_art){
    
    
    //store tree structure in a map with format of [article_id, <tree_enc1, tree_enc2,...>]
        unordered_map<string, vector<string> > t1;
        unordered_map<string, vector<string> > t2;
        store_tree(tree_1_addr, t1);
        store_tree(tree_2_addr, t2);

        ofstream outfile;
        outfile.open(output_addr);
    
        int tot_art = t1.size();
        int tot_art2 = t2.size();
    
        if(tot_art!=tot_art2){
    
            cerr<<endl<<"Two trees have different number of unique articles"<<endl<<endl;
            cerr<<"Tree 1 size: "<<tot_art<<endl;
            cerr<<"Tree 2 size: "<<tot_art2<<endl;
    //        exit(0);
        }
    
        if(num_art>tot_art){
            cerr<<endl<<"Required number of articles to be sampled is bigger than number of existing articles in the tree. Maximum number of articles ("<<tot_art<<") are returned."<<endl<<endl;
            num_art = tot_art;
        }
    
        unordered_set<int> used_index;
        string selected_article = "";
        unordered_map<string, vector<string> >::iterator random_it;
        unordered_map<string, vector<string> >::iterator it_tree2;
    
        for(int i=0;i<num_art;i++){
    //        cout<<i<<endl;
            //randomly generate unique indice to pick articles
            int index = rand()%tot_art;
            while(used_index.count(index)==1){
                index = rand()%tot_art;
            }
            used_index.insert(index);
    
    
            random_it = next(begin(t1), index);
    
            selected_article = random_it->first;
    
            if(t2.count(selected_article)==0){
                cout<<"can't find article "<<selected_article<<" in tree 2"<<endl;
                i--;
                continue;
            }
    
            outfile<<selected_article<<endl;
            //print out article's position(s) in the tree
            for(int j=0;j<random_it->second.size();j++){
                outfile<<random_it->second[j];
                if(j!=random_it->second.size()-1){
                    outfile<<", ";
                }
            }
    
            outfile<<endl;
    
    
            //print out article's position(s) in the second tree
            it_tree2 = t2.find(selected_article);
    
    
    //        cout<<(it_tree2->second).size()<<endl;
            for(int j=0;j<(it_tree2->second).size();j++){
                outfile<<(it_tree2->second)[j];
                if(j!=(it_tree2->second).size()-1){
                    outfile<<", ";
                }
            }
    
            outfile<<endl<<endl;
        }
        outfile.close();
}

int getTreeNames (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL ) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
    
    while ((dirp = readdir(dp)) != NULL) {
        if (string(dirp->d_name)!= "." && string(dirp->d_name) != ".." && string(dirp->d_name) != ".DS_Store"){
            string tree_name = string(dirp->d_name);
            string tree = "";
            int found = tree_name.find(".txt");
            if (found==std::string::npos){
                cerr<<"cannot locate the name of the tree"<<endl;
                exit(0);
            }
            else{
                tree = tree_name.substr(0, found);
            }

            if (tree!="wiki_tree"){
                files.push_back(tree);
            }
        }
    }
    closedir(dp);
    return 0;
}

int main(int argc, const char * argv[]) {
    
    
    if(argc < 4){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Number_of_Articles_to_Sample | Sampled_Arts_Folder | Tree_Folder"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }

    int num_art = stoi(argv[1]);
    string folder_path = argv[2];
    vector<string> subFolder;
    getdir(folder_path, subFolder);
    string tree_folder_addr = argv[3];

    vector<string> tree_names;
    getTreeNames(tree_folder_addr, tree_names);
    
    for (int i=0;i<subFolder.size();i++){
        string curr_folder_path = folder_path+"/"+subFolder[i];
        for (int j=0;j<tree_names.size();j++){
            //create a folder to hold each comparison result
            string comp_folder_path = curr_folder_path+"/"+tree_names[j]+" vs wiki";
            if (mkdir(comp_folder_path.c_str(), 0777)!=0 && errno != EEXIST){
                cerr<<"Failed to create folder "<<folder_path<<endl;
                exit(0);
            }
            string customized_tree_addr = curr_folder_path+"/random_"+tree_names[j]+".txt";

            string wiki_tree_addr = tree_folder_addr+"/"+tree_names[j]+".txt";
            
            string output_addr = comp_folder_path+"/sampled_arts.txt";
            getArticleLocations(customized_tree_addr, wiki_tree_addr, output_addr, num_art);
            
        }
        
    }

    
    return 0;
    
}
