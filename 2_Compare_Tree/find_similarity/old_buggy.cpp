 //
//  main.cpp
//  find_similarity
//
//  Created by Eve on 2/4/18.
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
#include <algorithm>
#include <unordered_set>
using namespace std;

set<string> visited; //global variable: used for repreventing adding repeated articles to the vector

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

void parse_position(vector<string>& parsed, string& line){
    string tmp = "";
    bool start = false;
    for(int i=0;i<line.length();i++){
        if(line[i]=='_'){
            if(!start){
                start = true;
            }
            else{
                
                parsed.push_back(tmp);
                tmp = "";
            }
        }
        else {
            tmp+=line[i];
        }
    }
    parsed.push_back(tmp);
    
//    cout<<"parsed: ";
//    for(int i=0;i<parsed.size();i++){
//        cout<<"_"<<parsed[i];
//    }
//    cout<<endl;
    
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
        
        //store key: tree_encoding, values: articles at the node
        vector<string> articles;
        articles.clear();
        t[parsed[0]] = articles;
//        cout<<parsed[0]<<": ";
        if(parsed.size()>1){
            for(int i=1;i<parsed.size();i++){
                t[parsed[0]].push_back(parsed[i]);
//                cout<<parsed[i]<<" ";
            }
        }
//        cout<<endl;
    }
    tree_file.close();
}

//get the articles in the node and articles in the childrens node, except for child node "except"
void getNodeContent(string& loc, string& except, unordered_map<string, vector<string> >& t, vector<string>& content){
//    vector<string> content;
    content.clear();
    queue<string> node_to_visit;
    node_to_visit.push(loc);
    //BFS
    while(!node_to_visit.empty()){
        string curr = node_to_visit.front();
//        cout<<"Node "<<curr<<": ";
        
        
        //store articles in the current node (get rid of repetitions)
        if(t.count(curr) == 0) {
            cerr<<"Cannot locate node "<<curr<<" in the tree"<<endl;
            exit(0);
        }
        unordered_map<string, vector<string> >::iterator it = t.find(curr);
//        cout<<"articles inside loc "<<loc<<": ";
        for(int i=0; i<(it->second).size();i++){
//            cout<<it->second[i]<<" ";
            if(visited.find(it->second[i])==visited.end()){
                content.push_back(it->second[i]);
                visited.insert(it->second[i]);
//                cout<<it->second[i]<<" ";
            }
        }
//        cout<<endl;
        //get children nodes (except for "except)
        int c=0;
        string tmp = curr+"_"+to_string(c);
        while(t.count(tmp) == 1){
            if(tmp != except){
                node_to_visit.push(tmp);
            }
            c++;
            tmp = curr+"_"+to_string(c);
        }
        //remove the node
        node_to_visit.pop();
    }
    
//    cout<<"Node "<<loc<<": ";
//    for(int i=0;i<content.size();i++){
//        cout<<content[i]<<" ";
//    }
//    cout<<endl;
//
//    return content;
}

void get_path(string& loc, unordered_map<string, vector<string> >& t, vector<vector<string> >& tree_nodes){
    //corner case: the article is contained in the root
    if(loc == "_0"){
        cout<<"Detected an article in the root"<<endl;
        return;
    }
    
    //populate first entry: node that contains sampled article
    string tmp = "-1";
    string current_node = loc;
    vector<string> content;
    content.clear();
    getNodeContent(loc, tmp, t, content);
    tree_nodes.push_back(content);
    
    
    //get its ancestors (except for the root)
    vector<string> pos;
    pos.clear();
    parse_position(pos, loc);

    int c = pos.size()-1;
//    cout<<"c = "<<c<<endl;
    
    while(c!=1){
      
        tmp = current_node;
        current_node = "";
        for(int i=0;i<c;i++){
            current_node = current_node+ "_" +pos[i];
        }
//        cout<<"ancestor: "<<current_node<<endl;
//        cout<<"except: "<<tmp<<endl;
        content.clear();
        getNodeContent(current_node, tmp, t, content);
        tree_nodes.push_back(content);
        
        c--;
    }
    
    
//    cout<<"Tree Nodes: +++++++++++++++++++++"<<endl;
//    for(int i=0;i<tree_nodes.size();i++){
//        for(int j=0;j<tree_nodes.size();j++){
//            cout<<tree_nodes[i][j]<<" ";
//        }
//        cout<<endl;
//    }
//    cout<<endl<<endl;
//
//
}

void similarity_score(vector<string>& a, vector<string>& b, vector<int>& curr_score){
    //new score = (new number of intersection)/(new number of union) = (prev inter + new inter)/(prev union + (new node size - new inter)
    int intersect_num;
    int added_num;
//    vector<int> ans;
    
    vector<string>::iterator it;
    vector<string> intersect(b.size());
    sort (a.begin(), a.end());
    sort (b.begin(), b.end());
//    cout<<"here3.1"<<endl;
    it=set_intersection (a.begin(), a.end(), b.begin(), b.end(), intersect.begin());
//    cout<<"here3.2"<<endl;
    intersect.resize(it-intersect.begin());
//    cout<<"here3.3"<<endl;
    
    intersect_num = intersect.size();
    if(curr_score[1]==0){
        added_num = b.size()+a.size()-intersect_num;
    }
    else{
        added_num = b.size()-intersect_num;
    }
    
//    cout<<"here3.4"<<endl;
    
    curr_score[0] += intersect_num;
    curr_score[1] += added_num;
    
 
    
}

void output_similarity_matrix (vector<vector<string> >& path_m, vector<vector<string> >& path_n, ofstream& outfile){
    //corner case: the node is in the root
    if(path_m.size() == 0 || path_n.size() == 0){
        outfile<<1<<endl<<endl;
        return;
    }
    
    //create a similarity matrix
    vector<vector<float> >sim;
    
    int m = path_m.size();
    int n = path_n.size();
    vector<float> tmp;
    for(int i=0;i<m;i++){
        tmp.clear();
        for(int j=0;j<n;j++){
            tmp.push_back(-1.00);
        }
        sim.push_back(tmp);
    }
//    cout<<"here"<<endl;
    
    //calculate similarities between two nodes if applicable (to ensure 1-1 match)
    int index_m = 0;
    int index_n = 0;
    float score = -1.0;
    
    vector<int> curr_score; //curr_score[0] = num inter; curr_score[1] = num_union
    curr_score.push_back(0);
    curr_score.push_back(0);

//    cout<<"m = "<<m<<", n = "<<n<<endl;
    
    for(int i=m-1;i>=0;i--){
        for(int j=n-1-index_m; j>=m-1-index_m; j--){
//            cout<<"i = "<<i<<", j = "<<j<<endl;
//            cout<<"m_index = "<<index_m<<", n_index = "<<index_n<<endl;
            similarity_score(path_m[index_m], path_n[index_n], curr_score);
            score = curr_score[0]*1.0/curr_score[1];
            
            sim[i][j] = score;
            
            index_n ++;
        }
        curr_score[0] = 0;
        curr_score[1] = 0;
        index_m ++;
        index_n = index_m;
    }
  
    
    
    
    
    
//    cout<<"here"<<endl;
    
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            outfile<<sim[i][j]<<" ";
        }
        outfile<<endl;
    }
    outfile<<endl;
    
}

//void calculate_similarity(vector<string>& t1_loc, vector<string> t2_loc, unordered_map<string, vector<string> >& t1, unordered_map<string, vector<string> >& t2, ofstream& outfile, ofstream& path){
void calculate_similarity(vector<string>& t1_loc, vector<string> t2_loc, unordered_map<string, vector<string> >& t1, unordered_map<string, vector<string> >& t2, ofstream& outfile){
    
    // each entry of the vector indicates the articles in node from the node first contains sampled article to the one belows the root (without repetitions)-->since in our tree we only assign each article to one category
    vector<vector<string> > s_tree_nodes;
    vector<vector<string> > b_tree_nodes;
    
    visited.clear();
    get_path(t1_loc[0], t1, s_tree_nodes);
    

//    path<<endl<<"+++++++++++++ Revision Tree Path +++++++++++++"<<endl<<endl;
//    path<<"Getting path for "<<t1_loc[0]<<endl;
//    for(int i=0;i<s_tree_nodes.size();i++){
//        for(int j=0;j<s_tree_nodes[i].size();j++){
//            path<<s_tree_nodes[i][j]<<" ";
//        }
//        path<<endl;
//    }
//
//
//    path<<endl<<"+++++++++++++ Wiki Tree Path +++++++++++++"<<endl<<endl;
    
    for (int i=0;i<t2_loc.size();i++){
        
//        path<<"Getting path for "<<t2_loc[i]<<endl;
        b_tree_nodes.clear();
        visited.clear();
        get_path(t2_loc[i], t2, b_tree_nodes);
        
//        for(int i=0;i<b_tree_nodes.size();i++){
//            for(int j=0;j<b_tree_nodes[i].size();j++){
//                path<<b_tree_nodes[i][j]<<" ";
//            }
//            path<<endl;
//        }
//        if (i!=t2_loc.size()-1){
//            path<<endl<<"--------------------------------------"<<endl<<endl;
//        }
        
        
        //calculate similarity between two path
        //find depth of two paths
        int m = s_tree_nodes.size();
        int n = b_tree_nodes.size();
        //ensure we are matching the smaller depth to bigger depth path
        if(m<n){
            output_similarity_matrix(s_tree_nodes, b_tree_nodes, outfile);
        }
        else{
            output_similarity_matrix(b_tree_nodes, s_tree_nodes, outfile);
        }
        
        
    }
    
    
    
//    path<<endl<<"*****************************";
//    path<<"****************************";
//    path<<endl<<endl;
}

int main(int argc, const char * argv[]) {
    if(argc < 6){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Tree_1_Address | Tree_2_Adress | Selected_Article_Address | Output_Address | Start_index"<<endl;
        cout<<"(e.g ./tree_1.txt ./tree_2.txt ./sampled_arts.txt ./ 0)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string tree_1_addr = argv[1];
    string tree_2_addr = argv[2];
    string sampled_file = argv[3];
    string output_addr = argv[4];
    output_addr+="/reported_similartity.txt";
    
    string path_addr = argv[4];
    path_addr+="/path.txt";
    
    string s_in = argv[5];
    int start_index = stoi(s_in);
    if(start_index>1){
        output_addr = argv[4];
        output_addr = output_addr+ "/reported_similartity_"+s_in+".txt";
    }
    
    //create output file
    ofstream outfile;
    outfile.open(output_addr);
//    ofstream pathfile;
//    pathfile.open(path_addr);
    
    //store tree structure in a map with format of [tree_enc, <art1, art2...>]
    unordered_map<string, vector<string> > t1;
    unordered_map<string, vector<string> > t2;
    store_tree(tree_1_addr, t1);
    store_tree(tree_2_addr, t2);
    cout<<"Finish storing trees"<<endl;
    
    //get sampled articles
    ifstream sampled_art;
    sampled_art.open(sampled_file);
    vector<string> t1_loc;
    vector<string> t2_loc;
    string line = "";
    int counter = 0;
    int tmp_counter = 0;
    while(!sampled_art.eof()){
        line = "";
        getline(sampled_art, line);
        //new article
        if(counter == 0){
            tmp_counter ++;
            cout<<tmp_counter<<endl;
//            pathfile<<"SAMPLED ARTICLE: "<<line<<endl;
            if(tmp_counter>=start_index) {
                outfile<<"-----------"<<line<<"-----------"<<endl;
            }
        }
        if(counter == 1){
            t1_loc.clear();
            parse(t1_loc, line);
        }
        else if(counter == 2){
            t2_loc.clear();
            parse(t2_loc, line);
        }
        //once finish storing all possible locations of the articles in two trees, get the path
        else if(counter == 3){
            counter = -1;
            
            if(tmp_counter>=start_index) {
                //ensure the smaller tree is the first parameter
                if(t1_loc.size()==1){
                    
                    calculate_similarity(t1_loc, t2_loc, t1, t2, outfile);
                }
                else{
                    
                    calculate_similarity(t2_loc, t1_loc, t2, t1, outfile);
                }
            }
        }
        
        counter++;
    }
    // process the last group
    //ensure the smaller tree is the first parameter
    if(t1_loc.size()==1){
        calculate_similarity(t1_loc, t2_loc, t1, t2, outfile);
    }
    else{
        calculate_similarity(t2_loc, t1_loc, t2, t1, outfile);
    }
    
    sampled_art.close();
    outfile.close();
//    pathfile.close();
    
    return 0;
}
