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
/* The code takes in editors and the articles they did major revisions for. The  */
/* code outputs the edge list for article pairs in the following format:         */
/* [art_weight_pair]: article_decode 1, article_decode 2, weight                 */
/* [art_index_name]: article_decode, article_name                                */
/*********************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>

using namespace std;

unordered_map<string, string> real_article_id;

int curr_in = -1; //current index of the new inserted article [used in addEdge]
int num_noEdge = 0;

bool new_art;
bool store_art1;
bool store_art2;
string art_index1;
string art_index2;

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

int getArtIndex(string& a, unordered_map<string, int>& article_index, ofstream&artname){
    int c = 0; //count the number of appearance of an article in the map
    unordered_map<string, int>::iterator it;
    int in_a = 0; //article index
    c = article_index.count(a);
    //if new article
    if(c == 0){
        curr_in+=1;
        article_index[a] = curr_in;
        in_a = curr_in;
        artname<<curr_in<<", "<<a<<endl;
    }
    //otherwise, directly get the index of the article
    else if (c == 1){
        it = article_index.find(a);
        in_a = it->second;
    }
    
    return in_a;
}


void addEdge(vector<string>& parsed, unordered_map<int, map<int,int> >& weight, unordered_map<string, int>& article_index, ofstream& artname){
    int in_a1 = 0;
    int in_a2 = 0;
    int tmp = 0;
    int c = 0; //counter for article appearances in weight
    
    string real_a1 = "";
    string real_a2 = "";
    unordered_map<int, map<int,int> >::iterator it;
    map<int,int>::iterator it2;
    
    
    
    //skip the first element (author name), take each pair
    for(int i=1;i<parsed.size();i++){
        for(int j = i+1;j<parsed.size();j++){
            real_a1 = real_article_id[parsed[i]];
            real_a2 = real_article_id[parsed[j]];
            
            in_a1 = getArtIndex(real_a1, article_index, artname);
            
            in_a2 = getArtIndex(real_a2, article_index, artname);
            
            //swap a1 a2 index to ensure a1 has smaller index (avoid repetition)
            if(in_a1>in_a2){
                tmp = in_a1;
                in_a1 = in_a2;
                in_a2 = tmp;
            }
            c = weight.count(in_a1);
            //if new article
            if(c == 0){
                map<int, int> edge;
                edge[in_a2] = 1;
                weight[in_a1] = edge;
            }
            //if the article is not in weight
            else{
                it = weight.find(in_a1); //locate article 1
                it2 = (it->second).find(in_a2); //locate article 2 from article 1'a edges
                // if article 2 doesnt have an edge with article 1
                if(it2 == (it->second).end()){
                    (it->second)[in_a2] = 1;
                }
                else{
                    it2->second+=1;
                }
            }
        }
        
    }
}



int main(int argc, const char * argv[]) {
    if(argc < 4){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Address_for_word_articles_file Address_for_real_article_id Output_address"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    string output_address = argv[3];
    string real_id_addr = argv[2];
    string edge_list_addr = output_address+"/art_weight_pair_2.txt";
    string art_index_addr = output_address+"/art_name_2.txt";
    string freq_w_addr = output_address+"/freq_w_2.txt";
    
    
    
    
    //initiate map for storing weight pair
    unordered_map<int, map<int, int> >weight;
    //initiate map for storing article_name and its index (for look up)
    unordered_map<string, int> article_index;
    
    //local variables
    vector<string> parsed;
    string line = "";
    
    new_art = false;
    store_art1 = false;
    store_art2 = false;
    //********** articles, weight pair *******//
    ofstream art_weight_pair;
    art_weight_pair.open (edge_list_addr);
    
    //********** article index, names *******//
    ofstream art_index_name;
    art_index_name.open (art_index_addr);
    
    
    
    //load and store article ids (since article ids in word_articles are only article index
    ifstream real_id;
    real_id.open(real_id_addr);
    int num_art = 0;
    if(!real_id) {
        cerr<<"Can't open file"<<endl;
        exit(1);
    }
    while (!real_id.eof()){
        getline(real_id, line);
        if(line.size()!=0){
            parsed.clear();
            parse(line, parsed);
            if(parsed.size()==2){
                real_article_id[to_string(parsed[1])] = parsed[1];
                num_art++;
            }
            else{
                cerr<<"parsed.size()!=2"<<endl;
                exit(0);
            }
            
        }
    }
    cout<<"There are in total "<<num_art<<" articles"<<endl;
    
    //////////////////////////
    //  Create edge list    //
    //////////////////////////
    ifstream infile;
    infile.open (argv[1]);
    
    ofstream freq;
    freq.open(freq_w_addr);
    freq<<"The file stores the most appeared words (freq>number of articles/4) and the number of articles it appears"<<endl;
    int i = 0;
    
    if(!infile){
        cerr<<"Can't open infile"<<endl;
        exit(1);
    }
    int l = 0;
    while(!infile.eof()){
        getline(infile,line);
        l+=1;
        cout<<l<<endl;
        if (!line.empty()){
            i++;
            parsed.clear();
            parse(line, parsed);
            cout<<"parsed.size() = "<<parsed.size()<<endl;
            if(parsed.size()>2000) {
                freq<<parsed[0]<<": "<<parsed.size()<<endl;
                continue;
            }
            addEdge(parsed, weight, article_index, art_index_name);
            
        }
    }
    infile.close();
    freq.close();
    
    if(curr_in+1!=num_art){
        cerr<<"Found "<<curr_in+1<<" articles when creating indices compared to a total of"<< num_art<<" articles"<<endl;
        //        exit(0);
    }
    
    //*********** print out art_weight pair ************//
    unordered_map<int, map<int,int> >::iterator it;
    map<int,int>::iterator it2;
    for (it = weight.begin();it!=weight.end();it++){
        for(it2 = (it->second).begin();it2!=(it->second).end();it2++){
            if(it2->second==1) continue; //filter out weight=1 pairs
            art_weight_pair<<it->first<<", "<<it2->first<<", "<<it2->second<<endl;
        }
    }
    art_weight_pair.close();
    
    
    cout<<"Completed"<<endl;
    
    return 0;
}



