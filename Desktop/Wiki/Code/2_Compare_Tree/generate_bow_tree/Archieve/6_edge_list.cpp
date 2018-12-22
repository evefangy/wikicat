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

//the function assigns an index to represent the article
int getArtIndex(string& a, unordered_map<string, int>& article_index, ofstream&artname){
    int c = 0; //count the number of appearance of an article in the map
    unordered_map<string, int>::iterator it;
    int in_a = 0; //article index
    c = article_index.count(a);
    //if new article
    if(c == 0){
//        curr_in+=1;
        article_index[a] = stoi(a);
        in_a = stoi(a);
        artname<<in_a<<", "<<a<<endl;
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
    unordered_map<int, map<int,int> >::iterator it;
    map<int,int>::iterator it2;
    
    
    
    //skip the first element (author name), take each pair
    for(int i=1;i<parsed.size();i++){
        for(int j = i+1;j<parsed.size();j++){
            in_a1 = getArtIndex(parsed[i], article_index, artname);
            
            in_a2 = getArtIndex(parsed[j], article_index, artname);
          
//            cout<<parsed[i]<<" "<<in_a1<<" "<<parsed[j]<<" "<<in_a2<<endl;
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
    if(argc < 5){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Address_for_major_editor_w_article_file | Output_address | category_range_and_output_version | upper-bound_on_num_art_edited_by_one_ed"<<endl;
        cout<<"(e.g ./major_editor_w_art_cate_0_to_100000.txt ./ 0_to_100000_2)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    string output_address = argv[2];
    string cate_range = argv[3];
    string edge_list_addr = output_address+"/art_weight_pair_cate_"+cate_range+".txt";
    string art_index_addr = output_address+"/art_name_cate_"+cate_range+".txt";
    string num_in = argv[4];
    int art_num_upper_bound = stoi(num_in);
    
    
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

    //////////////////////////
    //  Create edge list    //
    //////////////////////////
    ifstream infile;
    infile.open (argv[1]);
    int i = 0;

    if(!infile){
        cerr<<"Can't open infile"<<endl;
        exit(1);
    }
    int l = 0;
    while(!infile.eof()){
        getline(infile,line);
        l+=1;
        if (!line.empty()){
            if(l %1000 == 0){
                cout<<l<<endl;
            }
//            if(curr_in%100 == 0){
//                cout<<"\t"<<curr_in<<endl;
//            }
            i++;
            parsed.clear();
            parse(line, parsed);
            
            //if the number of artilces edited by an editor is too many, skip the editor
            if((parsed.size()-1) > art_num_upper_bound){
                continue;
            }
            
            addEdge(parsed, weight, article_index, art_index_name);
            
        }
    }
    infile.close();

    cout<<"Finish creating weight pairs"<<endl;
    
    int real_art_pair_counter = 0;
    //*********** print out art_weight pair ************//
    int num_weight_pair = weight.size(); //not a real number of article pairs
    int c = 0;
    unordered_map<int, map<int,int> >::iterator it;
    map<int,int>::iterator it2;
    for (it = weight.begin();it!=weight.end();it++){
        real_art_pair_counter+= (it->second).size();
        for(it2 = (it->second).begin();it2!=(it->second).end();it2++){
            art_weight_pair<<it->first<<", "<<it2->first<<", "<<it2->second<<endl;
        }
        c++;
        if(c%10000==0){
            cout<<"stored "<<c<<"/"<<num_weight_pair<<endl;
        }
    }
   art_weight_pair.close();

    cout<<"Completed"<<endl;
    
    cout<<"-------------------------------"<<endl;
    cout<<"There are "<<article_index.size()<<" articles with at least one edge"<<endl;
    cout<<"There are "<<real_art_pair_counter<<" weight pairs in total"<<endl;

    return 0;
}

