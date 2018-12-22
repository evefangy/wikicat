//
//  main.cpp
//  get_editor_with_art
//
//  Created by Eve on 9/19/17.
//  Copyright © 2017 Eve. All rights reserved.
//

/////////////////////////////////////////////////////////////////////////
//  Code rearrage the parsed revision history                          //
//  (following article, major editor 1, major editor 2,... format)     //
//  to the following format                                            //
//  editor 1, majorly edited article 1, majorly edited article 2, ...  //
/////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_set>
using namespace std;

int num_art = 0;
int num_none_ed = 0;
int min_ed = 0;

void parse(string& line, vector<string>& parsed){
    string tmp = "";
    bool name = true;
    for(int i=0;i<line.size();i++){
        if((line[i] == ',') && (name == true)){
            name = false;
            parsed.push_back(tmp);
            tmp = "";
        }
        else{
            tmp += line[i];
        }
    }
    parsed.push_back(tmp);
        
}




int main(int argc, const char * argv[]) {
    if(argc < 4){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Address_for_art_w_major_editor_file | Output_address | category_range_and_output_version"<<endl;
//        cout<<"(e.g ./art_w_major_editor_cate_0_to_100000.txt ./Parsed 0_to_100000_2)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    

    
    string output_address = argv[2];
    string cate_range = argv[3];
    string output_file_address = output_address+"/filtered_ed_size.txt";
    
    
    ofstream outfile;
    outfile.open (output_file_address);
    
    
    string input_address = argv[1];
    fstream infile;
    infile.open(input_address);
    
  

    unordered_map <string, string> info;
    
    
    int counter = 0; //current line number
    
    
    string line = "";
    
    
    if(!infile){
        cerr<<"Can't open the file"<<endl;
        exit(1);
    }
    
    vector<string> parsed;
    while(!infile.eof()){
        getline(infile,line);
        if (!line.empty()){
            parsed.clear();
            parse(line, parsed);
            if(parsed.size()<2){
                continue;
            
            }
            if(info.count(parsed[0])==0){
                info[parsed[0]] = parsed[1];
            }
            else{
                if(info[parsed[0]].size()<parsed[1].size()){
                    info[parsed[0]] = parsed[1];
                }
            }
            
        }
       
    }
    infile.close();
    
    cout<<"There are "<<info.size()<<" articles"<<endl;
    
    //print to a text file

    for (auto it = info.begin();it!=info.end();it++){
        
        
        outfile<<it->first<<",";
        
        outfile<<it->second<<endl;
        
    }
    
    outfile.close();

    cout<<"Completed"<<endl;
    
    
    return 1;
}

