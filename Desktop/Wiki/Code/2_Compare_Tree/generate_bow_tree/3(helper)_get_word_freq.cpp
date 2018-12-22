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




int main(int argc, const char * argv[]) {
    if(argc < 2){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Word_w_Arts_file_addr"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    string infile_addr = argv[1];
    
    
    
    
    
//    //initiate map for storing weight pair
//    unordered_map<int, map<int, int> >weight;
//    //initiate map for storing article_name and its index (for look up)
//    unordered_map<string, int> article_index;
    
    //local variables
    vector<string> parsed;
    string line = "";

    //////////////////////////
    //  Create edge list    //
    //////////////////////////
    ifstream infile;
    infile.open (infile_addr);
    
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
            i++;
            parsed.clear();
            parse(line, parsed);
            cout<<parsed.size()<<endl;
        }
    }
    infile.close();
//
    return 0;
}



