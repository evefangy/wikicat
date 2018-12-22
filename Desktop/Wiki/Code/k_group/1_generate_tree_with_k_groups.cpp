//
//  main.cpp
//  generate_random
//
//  Created by Eve on 3/9/18.
//  Copyright © 2018 Eve. All rights reserved.
//


/****************************************************************/
// The code is written for randomly choose an article of a given
// structure, where the structure (tree enco) and number of
// articles in each node is the same.
//
// [Input format]
// <Tree.txt>
// tree_enc_, art_id1, art_id2, ...
//
// <Artname.txt>
// index, article id
//
//
// [Output format]
// (random tree has articles in their real article ids)
// Tree encoding, random article 1, random article 2 ...
//
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
#include <time.h>       /* time */
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand



int main(int argc, const char * argv[]) {
    if(argc < 4){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<" Output_Address | k | number of articles"<<endl;
        cout<<"(e.g ./Output 5 10000)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string k_str = argv[2];
    string output_addr = argv[1];
    output_addr+="/tree_with_"+k_str+"_groups_1.txt";
    string output_addr_2 = argv[1];
    output_addr_2+="/tree_with_"+k_str+"_groups_2.txt";

    string num_art_str = argv[3];
    int num_art = stoi(num_art_str);
    
    int k = stoi(k_str);
    
    srand ( unsigned ( std::time(0) ) );
    
    vector<int> articles;
    // randomly permute the index
    for (int i=0; i<num_art; i++) {
        articles.push_back(i);
    }
    random_shuffle ( articles.begin(), articles.end() );



    ofstream outfile;
    outfile.open(output_addr);
    string tree_encode = "_0";
    outfile<<"_0"<<endl;

    int num_per_group = articles.size()/k;
    for (int i=0;i<k;i++){
        outfile<<tree_encode<<"_"<<i;
        if (i==k-1){
            for (int j=i*num_per_group; j<articles.size();j++){
                if (j==articles.size()) break;
                outfile<<", "<<articles[j];
            }
        }
        else{
            for (int j=i*num_per_group; j<(i+1)*num_per_group;j++){
                if (j==articles.size()) break;
                outfile<<", "<<articles[j];
            }
        }
        
        outfile<<endl;
    }


    outfile.close();


    // randomly permute the index
    random_shuffle ( articles.begin(), articles.end() );

    outfile.open(output_addr_2);
    tree_encode = "_0";
    outfile<<"_0"<<endl;

    num_per_group = articles.size()/k;
    for (int i=0;i<k;i++){
        outfile<<tree_encode<<"_"<<i;
        if (i==k-1){
            for (int j=i*num_per_group; j<articles.size();j++){
                if (j==articles.size()) break;
                outfile<<", "<<articles[j];
            }
        }
        else{
            for (int j=i*num_per_group; j<(i+1)*num_per_group;j++){
                if (j==articles.size()) break;
                outfile<<", "<<articles[j];
            }
        }
        outfile<<endl;
    }


    outfile.close();
    
    
    
    return 0;
}
