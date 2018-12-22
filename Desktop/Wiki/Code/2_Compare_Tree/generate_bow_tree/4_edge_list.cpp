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
/* [art_weight_pair]: article_decode 1, <article_decode 2, weight>               */
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


unordered_set<int> all_articles;

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


void addEdge(vector<string>& parsed, unordered_map<int, unordered_map<int,int> >& weight){
    // article ids (or indices)
    int in_a1 = 0;
    int in_a2 = 0;
    int article_exits = 0;

    // map for storing weight pairs: article 1, <article 2, counter>
    unordered_map<int, unordered_map<int,int> >::iterator it;
    unordered_map<int,int>::iterator it2;
    
    //skip the first element (tf-idf word), take each pair
    for(int i=1;i<parsed.size();i++){
        for(int j = i+1;j<parsed.size();j++){
            in_a1 = stoi(parsed[i]);
            in_a2 = stoi(parsed[j]);

            all_articles.insert(in_a1);
            all_articles.insert(in_a2);

            //swap a1 a2 index to ensure a1 has smaller index (avoid repetition)
            if(in_a1>in_a2){
                swap(in_a1, in_a2);
            }
            article_exits = weight.count(in_a1);
            
            //if new article
            if(article_exits == 0){
                unordered_map<int, int> edge;
                edge[in_a2] = 1;
                weight[in_a1] = edge;
            }
            
            //if the article exists
            else{
                it = weight.find(in_a1); //locate article 1
                it2 = (it->second).find(in_a2); //locate article 2 from article 1'a edges
                // if article 2 doesnt have an edge with article 1
                if(it2 == (it->second).end()){
                    (it->second)[in_a2] = 1;
                }
                else{
                    it2->second += 1;
                }
            }
        }
        
    }
}



int main(int argc, const char * argv[]) {
    if(argc < 5){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Address_for_tf-idf_word_w_article_file | Output_address | upper-bound_on_num_art_uses_the_same_word | number_of_articles"<<endl;
        cout<<"(e.g ./3_word_w_art.txt ./ 1000 33749)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    string output_address = argv[2];
    string edge_list_addr = output_address+"/4_art_weight_pair.txt";
    string num_in = argv[3];
    int art_num_upper_bound = stoi(num_in);
    num_in = argv[4];
    int number_of_articles = stoi(num_in);
    
    //initiate map for storing weight pair: article 1, <article 2, counter>
    unordered_map<int, unordered_map<int, int> >weight;
    
    //local variables
    vector<string> parsed;
    string line = "";
    
    //********** articles, weight pair *******//
    ofstream art_weight_pair;
    art_weight_pair.open (edge_list_addr);


    //////////////////////////
    //  Create edge list    //
    //////////////////////////
    ifstream infile;
    infile.open (argv[1]);

    if(!infile){
        cerr<<"Can't open infile"<<endl;
        exit(1);
    }
    int line_ctr = 0;
    while(!infile.eof()){
        getline(infile,line);
        line_ctr += 1;
        if (!line.empty()){
            // track progress
            if(line_ctr %1000 == 0){
                cout<<line_ctr<<endl;
            }
            
            parsed.clear();
            parse(line, parsed);
            
            //if the number of artilces uses the tf-idf word is bigger than upper-bound, skip
            if((parsed.size()-1) > art_num_upper_bound){
                continue;
            }
            
            addEdge(parsed, weight);
            
        }
    }
    infile.close();


    cout<<"Finish creating weight pairs"<<endl;
    
    int art_pair_counter = 0;
    //*********** print out art_weight pair ************//
    int c = 0;
    unordered_map<int, unordered_map<int,int> >::iterator it;
    unordered_map<int,int>::iterator it2;
    for (it = weight.begin();it!=weight.end();it++){
        art_pair_counter+= (it->second).size();
        for(it2 = (it->second).begin();it2!=(it->second).end();it2++){
            art_weight_pair<<it->first<<", "<<it2->first<<", "<<it2->second<<endl;
        }
        c++;
        if(c%100==0){
            cout<<"stored "<<c<<"/"<<weight.size()<<endl;
        }
    }
    art_weight_pair.close();

    int num_wo_edges = 0;
    if (all_articles.size()<number_of_articles){
        string articles_wo_edges = output_address+"/4_articles_wo_edges.txt";
        ofstream wo_edges;
        wo_edges.open(articles_wo_edges);
        

        for (int i=0;i<number_of_articles;i++){
            if (all_articles.count(i)==0){
                wo_edges<<i<<endl;
                num_wo_edges++;
            }
        }

        wo_edges.close();
    }
    cout<<"Completed"<<endl;
    
    cout<<"-------------------------------"<<endl;
    cout<<"The code did not encode the indices for articles. However, when getting TF-IDF words, articles are encoded. "; 
    cout<<"Please follow the standard of TF-IDF word encoding to decode the tree"<<endl;
    cout<<"There are "<<all_articles.size()<<" articles with at least one edge"<<endl;
    cout<<"There are "<<num_wo_edges<<" articles without edges"<<endl;
    cout<<"There are "<<num_wo_edges+all_articles.size()<<" articles in total [compare to "
        <<number_of_articles<<" as reported]"<<endl;    
    cout<<"There are "<<art_pair_counter<<" weight pairs in total"<<endl;

    return 0;
}

