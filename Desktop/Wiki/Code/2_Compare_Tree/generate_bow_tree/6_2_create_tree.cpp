//////////////////////////////////////////////////////////////////////
// The code takes in group assignments calculated for each article  //
// every iteration and construct a tree structure based on the      //
// result.                                                          //
//                                                                  //
//  Input: Group Assignment File                                    //
//  Example: {[-1, -1, 2, 2], [2, 2, 2, 2]}                         //
//                                                                  //
//  Output: Tree Encoding                                           //
//  Example: {[0: ], [0_0: 0, 1], [0_0_0: 2,3]}                     //
//  --------------------------------------------------------------- //
//  Note:                                                           //
//  1. For input file, each row indicates the result after an       //
//      iteration of union-find. Each index corresponds to the      //
//      article                                                     //
//////////////////////////////////////////////////////////////////////



#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <set>
#include <map>

int processed = 0;
int num_art = 0;
bool all_art = false;

using namespace std;


bool comp_map(const string& l, const string& r)
{
    int left_lv = 0;
    int right_lv = 0;
    
    vector<string> left_node;
    vector<string> right_node;
    
    string l_node = "";
    string r_node = "";
    
    for(int i=0;i<l.size();i++){
        if(l[i] == '_'){
            left_node.push_back(l_node);
            left_lv++;
            l_node = "";
        }
        else{
            l_node+=l[i];
        }
    }
    
    for(int i=0;i<r.size();i++){
        if(r[i] == '_'){
            right_node.push_back(r_node);
            right_lv++;
            r_node = "";
        }
        else{
            r_node+=r[i];
        }
    }
    if(left_lv<right_lv){
        return true;
    }
    else if(left_lv>right_lv){
        return false;
    }
    else{
        for(int i=0;i<left_node.size();i++){
            if(left_node[i]<right_node[i]){
                return true;
            }
            else if (left_node[i]>right_node[i]){
                return false;
            }
        }
    }
    return true;
}



//map<string, vector<int>, comp_map>::iterator search_map(map<string, vector<int>, comp_map>& enc, string s){
//    map<string, vector<int>, comp_map>::iterator it;
//    for(it = enc.begin();it!=enc.end();it++){
//        if(it->first == s){
//            return it;
//        }
//    }
//    return it;
//}
//
//void insert_map(map<string, vector<int>, comp_map>& enc, string s, int num){
//    map<string, vector<int>, comp_map>::iterator it = search_map(enc, s);
//    if(it==enc.end()){
//        vector<int> tmp;
//        tmp.push_back(num);
//        enc[s] = tmp;
//    }
//    else{
//        it->second.push_back(num);
//    }
//}




void parse(string& s, vector<int>& parsed){
    string tmp = "";
    for(int i=0; i<s.length();i++){
        if(s[i]==','){
            if (tmp.size()!=0 && tmp!=" "){
                parsed.push_back(stoi(tmp));
            }
            
            tmp = "";
        }
        else{
            if(s[i]!= ' '){
                tmp+=s[i];
            }
        }
    }
    if (tmp.size()!=0 && tmp!=" "){
        parsed.push_back(stoi(tmp));
    }
}






void get_tree_enco(const vector<vector<int> >& group, int ite, int child_counter, string prev_enco, 
    vector<int>& indices, map<string, vector<int> >& tree_enco){
    map<int, int> visited; //id, child number
    int c = 0;
    int ite_index = group[0].size()-1-ite;
    
    int next_ite = ite+1;
    string curr_enco = prev_enco+"_"+to_string(child_counter);
    vector<vector<int> > new_indices;
    new_indices.clear();
    vector<int> tmp;
    int index = 0;
    
    vector<int> arts;
    arts.clear();
    tree_enco[curr_enco] = arts;
   
    
    map<string, vector<int> >::iterator it = tree_enco.find(curr_enco);
    if(it==tree_enco.end()){
        cerr<<"cannot locate "<<curr_enco<<" from the map"<<endl;
        exit(0);
    }
    
    for(int i=0;i<indices.size();i++){
        index = indices[i];
        //base case
        if(group[index][ite_index] == -1){
            if(it==tree_enco.end()){
                cerr<<"cannot locate "<<curr_enco<<" from the map"<<endl;
                exit(0);
            }
            it->second.push_back(index);
            
            processed++;
        }
        else{
            //last level
            if(ite == group[0].size()){
                it->second.push_back(index);
                
                processed++;
                            }
            else{
                //if it's a new child
                if(visited.count(group[index][ite_index])==0){
                    visited[group[index][ite_index]] = c;
                    tmp.clear();
                    tmp.push_back(index);
                    new_indices.push_back(tmp);
                    c++;
                }
                //get old children's counter
                else{
                    int child_num = visited[group[index][ite_index]];
                    new_indices[child_num].push_back(index);
                }
            }
            
        }
        
    }
    
    for(int i=0;i<new_indices.size();i++){
        get_tree_enco(group, next_ite, i, curr_enco, new_indices[i], tree_enco);
        
    }
    
}

void get_tree_enco_start(const vector<vector<int> >& group, map<string, vector<int> >& tree_enco){
    // initiate root
    string curr_enco = "_0";
    vector<int> arts;
    tree_enco[curr_enco] = arts;

    int last_ite_index = group[0].size()-1;
    int next_ite = 1;
    

    map<int, int> visited; //id, child number
    int child_counter = 0;

    vector<vector<int> > new_indices;
    vector<int> tmp;

    //store and locate the group in the map
    map<string, vector<int> >::iterator it = tree_enco.find(curr_enco);
    if(it==tree_enco.end()){
        cerr<<"cannot locate "<<curr_enco<<" from the map"<<endl;
        exit(0);
    }
    
    for(int i=0;i<group.size();i++){

        //if the current article didn't get merged into the tree [NEED TO BE IMPLEMENTED: updated 10/28/2018]
        if(group[i][last_ite_index] == -1){
        }
        else{
            //if it's a new child
            if(visited.count(group[i][last_ite_index])==0){
                visited[group[i][last_ite_index]] = child_counter;
                tmp.clear();
                tmp.push_back(i);
                new_indices.push_back(tmp);
                child_counter++;
            }
            //get old children's counter
            else{
                int child_num = visited[group[i][last_ite_index]];
                new_indices[child_num].push_back(i);
            }
        }
        
    }
    
    for(int i=0;i<new_indices.size();i++){
        get_tree_enco(group, next_ite, i, curr_enco, new_indices[i], tree_enco);
        
    }
    
}




int main(int argc, const char * argv[]){
    /*------------------------Get all inputs---------------------------*/
    if(argc < 3){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;

        cout<<"Group_Iteration_File | Output_address | [Optional: output used articles (0/1)]"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string group_add = argv[1];
    string output_add = argv[2];
    output_add+="/tree_enco.txt";
    string used_article_addr = argv[2];
    used_article_addr = used_article_addr+"/used_article(encoded).txt";


    ofstream used_article;

    if (argc == 4){
        used_article.open(used_article_addr);
    }
    
    
    /*------------------------ Create I/O File -------------------------*/
    ifstream group_file;
    group_file.open(group_add); //read in group assignment for each iteration
    ofstream outfile;
    outfile.open(output_add);   //output file
    
    if(!group_file){
        cerr<<"Can't open "<<group_add<<endl;
        exit(0);
    }
    
    if(!outfile){
        cerr<<"Can't create "<<output_add<<endl;
        exit(0);
    }
    
    
    /*------------------------- Read in Group Assigments ---------------------------*/
    // group_ite file: 
    // [{a1_group_ite1, a2_group_ite1, a3_group_ite1...}
    //     {a1_group_ite2, a2_group_ite2, a3_group_ite2...}]

    // group array:
    // [{a1_ite1, a1_ite2, a1_ite3...}
    //      {a2_ite1, a2_ite2, a2_ite3...}]

    string line = "";
    vector<int> parsed;

    vector<vector<int> > group;
    vector<int> tmp;

    int c=0;
    while(!group_file.eof()){
        getline(group_file, line);
        if(line.size()!=0){
            parsed.clear();
            parse(line, parsed);
            if(parsed.size()==0){
                cerr<<"parsed.size()==0"<<endl;
                exit(0);
            }

            if(c==0){
                num_art = parsed.size();
                for(int i=0;i<parsed.size();i++){
                    tmp.clear();
                    tmp.push_back(parsed[i]);
                    group.push_back(tmp);
                }
            }
            else{
                for(int i=0;i<parsed.size();i++){
                    group[i].push_back(parsed[i]);
                }
            }
            c++;
        }
    }

    group_file.close();
    
    /*---------------------------- Extract Groups and Articles in Each Group --------------------------------*/
    map<string, vector<int> > enc;

    get_tree_enco_start(group, enc);
    //output result
    for(map<string, vector<int> >::iterator it = enc.begin();it!=enc.end();it++){
        outfile<<it->first<<", ";
        for(int i=0;i<it->second.size();i++){
            outfile<<it->second[i];
            if (argc==4){
                used_article<<it->second[i]<<endl;
            }
            if(i!=it->second.size()-1){
                outfile<<", ";
            }
        }
        outfile<<endl;
    }

    cout<<endl<<"There are "<<processed<<" out of "<<num_art<<" articles got merged into the tree (with an edge weight >= threshold)"<<endl;
    //c+1 to include the root
    cout<<"There are "<<c+1<<" levels in the tree"<<endl;
    
    
    return 0;
}
