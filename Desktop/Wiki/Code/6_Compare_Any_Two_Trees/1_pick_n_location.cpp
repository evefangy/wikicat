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
using namespace std;

//ofstream artname;
//bool first = false;

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

void store_tree(string& addr, unordered_map<string, vector<string> >& t){
    ifstream tree_file;
    tree_file.open(addr);
    if(!tree_file){
        cerr<<"Can't open file"<<endl;
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
int main(int argc, const char * argv[]) {
    if(argc < 5){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Rev_tree_Address Wiki_tree_Adress Output_Address Number_of_Articles"<<endl;
        cout<<"(e.g ./rev_tree.txt ./wiki_tree.txt ./ 3)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string tree_1_addr = argv[1];
    string tree_2_addr = argv[2];
    string output_addr = argv[3];
    output_addr+="/sampled_arts.txt";
    int num_art = stoi(argv[4]);
    

    
    //store tree structure in a map with format of [article_id, <tree_enc1, tree_enc2,...>]
    unordered_map<string, vector<string> > t1;
    unordered_map<string, vector<string> > t2;
    store_tree(tree_1_addr, t1);
    cout<<"Finish storing tree 1"<<endl;
    store_tree(tree_2_addr, t2);
    cout<<"Finish storing tree 2"<<endl;

    
    

    
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
        
        // skip empty ids
        if(selected_article.size()==0){
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
    
    return 0;
    
}
