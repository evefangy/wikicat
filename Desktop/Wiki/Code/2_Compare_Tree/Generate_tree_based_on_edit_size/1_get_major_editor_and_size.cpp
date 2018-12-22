//
//  main.cpp
//  convert_name_to_id
//
//  Created by Eve on 2/6/18.
//  Copyright © 2018 Eve. All rights reserved.
//

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

void parse(vector<string>& parsed, string& line){
    string tmp = "";
    for(int i=0;i<line.size();i++){
        if(line[i]==','){
            if (tmp.size()!=0 && tmp!=" "){
                parsed.push_back(tmp);
            }
           
            tmp = "";
        }
        else{
            if(line[i]!=' '){
                tmp+=line[i];
                
            }
        }
    }
    if (tmp.size()!=0 && tmp!=" "){
        parsed.push_back(tmp);
    }
}

int main(int argc, const char * argv[]) {
    if(argc < 5){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"used_art_file | parsed_rev_history_addr | Output_Address | version"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string used_art_file = argv[1];
    string parsed_rev_history_addr = argv[2];
    string output_addr = argv[3];
    string v = argv[4];
    output_addr+="/1_article_editor_edit_size_"+v+".txt";
    
    
    ifstream used_art, rev_history;
    used_art.open(used_art_file);
    rev_history.open(parsed_rev_history_addr);
    
    ofstream outfile;
    outfile.open(output_addr);

    if(!outfile){
        cerr<<"can't open output file"<<endl;
        exit(0);
    }


    
    //read in id file
    unordered_set<string> used_art_set;

    string line;
    vector<string> parsed;
    int c=0;
    while(!used_art.eof()){
        line = "";
        getline(used_art, line);
        if(line.size()>0){
            parsed.clear();
            parse(parsed, line);
            if(parsed.size()!=1){
                cerr<<"parsed.size()!=1"<<endl;
                exit(0);
            }
            used_art_set.insert(parsed[0]);
            c++;
        }
    }
    cout<<"Stored "<<c<<" articles"<<endl;
    
    
    used_art.close();

    unordered_map<string, double> editor_edit_size;

    c = 0;
    
    bool isNewArt = true;
    bool isUsedArt = false;

    int total_num_art = 0;
    int num_processed_art = 0;

    while(!rev_history.eof()){
        line = "";
        getline(rev_history, line);

        // if it's an empty line (new article)
        if (line.size()==0 || line==" "){
            if (!isNewArt){
                for(auto it=editor_edit_size.begin(); it!=editor_edit_size.end();it++){
                    if (it->second>=0.10){
                        outfile<<", "<<it->first<<", "<<it->second;
                    }
                }
                
                isNewArt = true;
                isUsedArt = false;
                editor_edit_size.clear();
                total_num_art++;
            }
            
        }
        else if(line.size()!=0){
            parsed.clear();
            parse(parsed, line);

            // article id
            if (isNewArt){
                if (parsed.size()!=1){
                    cerr<<"Article ID format is wrong"<<endl;
                    cout<<line<<endl;
                    exit(0);
                }

                if (used_art_set.count(parsed[0])==1){
                    isUsedArt  = true;
                    outfile<<endl;
                    outfile<<parsed[0];
                    num_processed_art++;
                }

            }
            else{
                if (isUsedArt){
                    // article size
                    if (parsed.size()==1){
                        
                    }

                    // ed history
                    else{
                        int n = parsed.size();

                        if(parsed[1]!="0"){
                            try{
                                editor_edit_size[parsed[n-3]] += stod(parsed[n-1]);
                            }
                            
                            catch(std::exception& e){
                                cout << "Could not convert string to double" << endl;
                                cout<<"["<<line<<"]"<<endl;
                            }
                        }
                    }
                }
                
            }
            
            
            isNewArt = false;
        }
        
    }
    
    rev_history.close();
    outfile.close();

    cout<<"There are "<<num_processed_art<<"/"<<total_num_art<<" articles processed in the rev history"<<endl;
    return 0;
}

