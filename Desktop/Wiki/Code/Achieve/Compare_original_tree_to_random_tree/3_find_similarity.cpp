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
#include <unordered_set>
#include <algorithm>
#include <unordered_set>
#include <math.h>
#include <thread> 
using namespace std;


#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

unordered_set<string> visited; //global variable: used for repreventing adding repeated articles to the vector

void parse(vector<string>& parsed, string& line){
    parsed.clear();
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
    parsed.clear();
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
    
}

// Store the tree in a map in the form of [Key: tree_encoding, Value: <article1, article2...>]
void store_tree(string& addr, unordered_map<string, vector<string> >& t){
    ifstream tree_file;
    tree_file.open(addr);
    if(!tree_file){
        cerr<<"Can't open file "<<addr<<endl;
        exit(0);
    }
    string line = "";
    vector<string> parsed;
    vector<string> articles;

    while(!tree_file.eof()){
        line = "";
        parsed.clear();
        getline(tree_file, line);
        parse(parsed, line);
        
        // store tree in a map t with key=tree_encoding, values=articles at the node
        articles.clear();
        t[parsed[0]] = articles;
        
        // populate articles belong to the node to the corresponding tree_encoding
        if(parsed.size()>1){
            for(int i=1;i<parsed.size();i++){
                t[parsed[0]].push_back(parsed[i]);
            }
        }
    }
    tree_file.close();
}

//get the articles in the node and articles in the children, except for child node "encountered"
void get_node_content(string& loc, string& encountered, unordered_map<string, vector<string> >& t, vector<string>& content){
    content.clear();
    queue<string> node_to_visit;
    node_to_visit.push(loc);
    //BFS
    while(!node_to_visit.empty()){
        string curr = node_to_visit.front();

        //store articles in the current node (get rid of repetitions)
        if(t.count(curr) == 0) {
            cerr<<"Cannot locate node "<<curr<<" in the tree"<<endl;
            exit(0);
        }
        unordered_map<string, vector<string> >::iterator it = t.find(curr);
        for(int i=0; i<(it->second).size();i++){
            if(visited.count(it->second[i])==0){
                content.push_back(it->second[i]);
                visited.insert(it->second[i]);
            }
        }
        //get the immediate children nodes of the current node (except for "encountered" node)
        int c=0;
        string curr_child = curr+"_"+to_string(c);
        while(t.count(curr_child) == 1){
            if(curr_child != encountered){
                node_to_visit.push(curr_child);
            }
            c++;
            curr_child = curr+"_"+to_string(c);
        }
        //remove the node
        node_to_visit.pop();
    }
}

void get_path(string& loc, unordered_map<string, vector<string> >& t, vector<vector<string> >& tree_nodes){
    // corner case: the article is contained in the root
    if(loc == "_0"){
        cerr<<"Detected an article in the root"<<endl;
        exit(0);
    }
    
    // populate first entry: node that contains sampled article
    string encountered_node = "-1";
    string current_node = loc;
    // placeholder for retrieving articles in a node
    vector<string> content;
    content.clear();

    get_node_content(current_node, encountered_node, t, content);
    tree_nodes.push_back(content);
    
    
    //get its ancestors (except for the root)
    
    vector<string> pos;
    pos.clear();
    parse_position(pos, loc);

    int c = pos.size()-1;
    
    while(c!=1){
      
        encountered_node = current_node;
        current_node = "";
        for(int i=0;i<c;i++){
            current_node = current_node+ "_" +pos[i];
        }

        content.clear();
        get_node_content(current_node, encountered_node, t, content);
        tree_nodes.push_back(content);
        
        c--;
    }
    
}


int calculate_node_similarity_score(vector<vector<string> >& p_a, vector<vector<string> >& p_b, vector<float>& curr_score, int a_index, int b_index){
    unordered_set<string> visited;

    // error check for matrix
    unordered_set<string> visited_b;

    int num_insersection = 0;
    int num_union = 0;
    // populate every article in path a 
    for (int i=0;i<=a_index;i++){
        for (int j=0;j<p_a[i].size();j++){
            // skip empty nodes
            if (p_a[i][j].size()==0) continue;
            // new article
            if (visited.count(p_a[i][j])==0){
                visited.insert(p_a[i][j]);
            }
            else{
                cerr<<"There are duplicates in the path for p_a"<<endl;
                return -1;
            }
        }
    }

    // populate every article in path b 
    for (int i=0;i<=b_index;i++){
        for (int j=0;j<p_b[i].size();j++){
            // skip empty nodes
            if (p_b[i][j].size()==0) continue;
            // new article
            if (visited.count(p_b[i][j])==0){
                visited.insert(p_b[i][j]);
                visited_b.insert(p_b[i][j]);
            }
            else{
                if (visited_b.count(p_b[i][j])==1){
                    cerr<<"There are duplicates in the path for b"<<endl;
                    return -1;
                }
                num_insersection++;
            }
        }
    }
    num_union = visited.size();

    curr_score[0] = num_insersection;
    curr_score[1] = num_union;

    return 1;
}

void output_similarity_matrix (vector<vector<string> >& path_m, vector<vector<string> >& path_n, ofstream& outfile){
    //corner case: the target article is in the wikipedia tree's root, 1-1 match results in 100% match
    if(path_m.size() == 0 || path_n.size() == 0){
        outfile<<1<<endl<<endl;
        return;
    }
    
    //create a similarity matrix
    vector<vector<float> >sim;
    int m = path_m.size()+2; // add two nodes: root node and node that contains only the article 
    int n = path_n.size()+2;

    // for (int i=0;i<path_m.size();i++){
    //     outfile<<"[";
    //     for (int j=0;j<path_m[i].size();j++){
    //         outfile<<path_m[i][j]<<" ";
    //     }
    //     outfile<<"]";
    // }
    // outfile<<endl;

    // for (int i=0;i<path_n.size();i++){
    //     outfile<<"[";
    //     for (int j=0;j<path_n[i].size();j++){
    //         outfile<<path_n[i][j]<<" ";
    //     }
    //     outfile<<"]";
    // }
    // outfile<<endl;


    vector<float> tmp;
    for(int j=0;j<n;j++){
        tmp.push_back(-1.0);
    }
    for(int i=0;i<m;i++){
        sim.push_back(tmp);
    }

    //calculate similarities between two nodes if applicable (to ensure 1-1 match)
    // int index_m = 0;
    // int index_n = 0;
    float score = -1.0;
    int status;
    
    //curr_score[0] = num intersection; curr_score[1] = num_union
    vector<float> curr_score; 
    curr_score.push_back(0);
    curr_score.push_back(0);


    // set scores for root and article node to be 1 
    sim[0][0] = 1;
    sim[m-1][n-1] = 1;

    // calculate the score for the rest of the matrix
    for (int i=1; i<m-1;i++){
        for (int j=1; j<n-1;j++){
            // skip invalid matching: top node(s) for smaller path have no matching node
            if (i-1>j-1) continue;
            // skip invalid matching: bottom node(s) for smaller path have no matching node
            if (m-i>n-j) continue;
            
            // calculate similarity score
            status = calculate_node_similarity_score(path_m, path_n, curr_score, path_m.size()-i, path_n.size()-j);
            if (status == -1){
                outfile<<"Fail"<<endl;
                cout<<"Failed to calculate the similarity score"<<endl;
                exit(0);
            }

            score = curr_score[0]*1.0/curr_score[1]; //num same articles/union of articles

            if (isnan(score)){
                cerr<<"Produce NAN"<<endl;
                cerr<<"intersection = "<<curr_score[0]<<", union = "<<curr_score[1];
                exit(0);
            }
            sim[i][j] = score;
            
            curr_score[0] = 0;
            curr_score[1] = 0;
        }
    }

    
    for(int i=0;i<sim.size();i++){
        for(int j=0;j<sim[i].size();j++){
            outfile<<sim[i][j]<<" ";
        }
        outfile<<endl;
    }
    outfile<<endl;
}


void calculate_path_similarity(vector<string>& t1_loc, vector<string> t2_loc, unordered_map<string, vector<string> >& t1, 
    unordered_map<string, vector<string> >& t2, ofstream& outfile){
    // record article ids in each node 
    /* 
        Note: 
        1. Root is not included since it's empty
        2. The articles in each node = the recorded articles in the node + all articles in the children of the node
        3. Repetitive artcles are removed
        4. similarity_score[0][0] = score for matching the nodes that contain the target article
        5. similarity_score[m-1][n-1] = score for matching the nodes right below the root for two paths
    */

    vector<vector<string> > p1_nodes;
    vector<vector<string> > p2_nodes;
    
    visited.clear();
    get_path(t1_loc[0], t1, p1_nodes);
    
    for (int i=0;i<t2_loc.size();i++){
        p2_nodes.clear();
        visited.clear();

        get_path(t2_loc[i], t2, p2_nodes);
        
        //calculate similarity between two path
        //find depth of two paths
        int m = p1_nodes.size();
        int n = p2_nodes.size();
        //ensure we are matching the smaller depth to bigger depth path
        if(m<n){
            output_similarity_matrix(p1_nodes, p2_nodes, outfile);
        }
        else{
            output_similarity_matrix(p2_nodes, p1_nodes, outfile);
        }
    }
    
}


int calculate_and_output_similarity_score(string& tree_1_addr, string& tree_2_addr, string& sampled_art_addr,  string& output_addr, int start_index){
    /* ---------------------------------- Store input trees ---------------------------------------*/
    //store tree structure in a map with format of [tree_enc, <art1, art2...>]
    unordered_map<string, vector<string> > t1;
    unordered_map<string, vector<string> > t2;
    store_tree(tree_1_addr, t1);
    store_tree(tree_2_addr, t2);
    // cout<<"Finish storing trees"<<endl;


    ofstream outfile;
    outfile.open(output_addr);
    if (!outfile){
        cerr<<"Can't open "<<output_addr<<endl;
        return -1;
    }
    
    /* ------------------------------- Extract paths for sampled articles -----------------------------------*/
    ifstream sampled_art;
    sampled_art.open(sampled_art_addr);
    if (!sampled_art){
        cerr<<"Cannot open "<<sampled_art_addr<<endl;
        return -1;
    }


    // get all nodes that contain the current article in tree 1 and tree 2
    vector<string> t1_loc;
    vector<string> t2_loc;
    string line = "";
    // counter=0->new article id, counter=1->tree encodings for the current article in tree 1
    // counter=2->tree encodings for the current article in tree 2, counter=3->empty line
    int counter = 0; 
    int line_counter = 0;
    int article_counter = 0;
    while(!sampled_art.eof()){
        line = "";
        getline(sampled_art, line);

        if(line_counter>=start_index) {
            //new article
            if(counter == 0){
                if (line.size()!=0){
                    outfile<<"-----------"<<line<<"-----------"<<endl;
                }
            }
            else if(counter == 1){
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
               
                article_counter++;
                // cout<<article_counter<<endl;

                // ensure the non-wiki tree does not have duplicated articles
                if (t1_loc.size()!=1 && t2_loc.size()!=1) {
                    cerr<<"There are duplicated articles in the generated tree"<<endl;
                    cerr<<"t1_loc.size() = "<<t1_loc.size()<<endl;
                    cerr<<"t2_loc.size() = "<<t2_loc.size()<<endl;
                    return -1;
                }
                //ensure the smaller tree is the first parameter
                if(t1_loc.size()==1){
                    calculate_path_similarity(t1_loc, t2_loc, t1, t2, outfile);
                }
                else{
                    calculate_path_similarity(t2_loc, t1_loc, t2, t1, outfile);
                }
            }
                
        }
        
        counter++;
    }

    // process the last article
    if(counter == 3){
        counter = -1;
        article_counter++;
        // cout<<article_counter<<endl;
        
        // ensure the non-wiki tree does not have duplicated articles
        if (t1_loc.size()!=1 && t2_loc.size()!=1) {
            cerr<<"There are duplicated articles in the generated tree"<<endl;
            cerr<<"t1_loc.size() = "<<t1_loc.size()<<endl;
            cerr<<"t2_loc.size() = "<<t2_loc.size()<<endl;
            return -1;
        }
        //ensure the smaller tree is the first parameter
        if(t1_loc.size()==1){
            calculate_path_similarity(t1_loc, t2_loc, t1, t2, outfile);
        }
        else{
            calculate_path_similarity(t2_loc, t1_loc, t2, t1, outfile);
        }
    }


    
    sampled_art.close();
    outfile.close();

    return 1;
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

            if (tree!="wiki_tree") {
                files.push_back(tree);
            }
        }
    }
    closedir(dp);
    return 0;
}

int main(int argc, const char * argv[]) {
    if(argc < 5){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Start Folder Number | End folder number (include) | sampled_groups_folder | Tree_folder_addr"<<endl;
        cout<<"(e.g 0 5 ./all_trees)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
//
    int start_folder = stoi(argv[1]);
    int end_folder = stoi(argv[2]);
    
    string tree_folder_addr = argv[4];
    vector<string> tree_names;
    getTreeNames(tree_folder_addr, tree_names);
    // tree_names.push_back("BoW_tree");
    // tree_names.push_back("rand_tree");
    // tree_names.push_back("rev_tree");
    string folder_path = argv[3];
    vector<string> subFolder;
//    getdir(folder_path, subFolder);

    for (int i=start_folder;i<=end_folder;i++){
        subFolder.push_back(to_string(i));
    }
    
    
    for (int i=0;i<subFolder.size();i++){
        string curr_folder_path = folder_path+"/"+subFolder[i];
        cout<<"Getting similarity score for "<<curr_folder_path<<endl;
        for (int j=0;j<tree_names.size();j++){
            //create a folder to hold each comparison result
            string comp_folder_path = curr_folder_path+"/"+tree_names[j]+" vs wiki";
            // cout<<comp_folder_path<<endl;
            
            string customized_tree_addr = curr_folder_path+"/random_"+tree_names[j]+".txt";
            string wiki_tree_addr = tree_folder_addr+"/"+tree_names[j]+".txt";
            string output_addr = comp_folder_path+"/reported_similartity.txt";
            string sampled_file = comp_folder_path+"/sampled_arts.txt";
            
            int start_index = 0;
            calculate_and_output_similarity_score(customized_tree_addr, wiki_tree_addr, sampled_file, output_addr, start_index);
            
        }
        
    }
    
    
    
    
    return 0;
}
