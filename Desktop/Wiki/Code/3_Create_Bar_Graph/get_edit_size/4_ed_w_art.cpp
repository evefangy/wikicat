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
#include <map>
using namespace std;

int num_art = 0;
int num_none_ed = 0;
int min_ed = 0;

void parse(string& line, vector<string>& parsed ){
    string tmp = "";
    for(int i=0;i<line.size();i++){
        if(line[i] == ','){
            parsed.push_back(tmp);
            tmp = "";
        }
        else{
            if(line[i]!=' '){
                tmp += line[i];
            }
        }
    }
    parsed.push_back(tmp);
        
}


void parse_info(string& line, unordered_map <string, map<string, double> >& info){
    string article;
    vector<string> parsed;
    parsed.clear();
    parse(line, parsed);
    
    
    
    //if the format of infile is not right
    if(parsed.size()%2 == 0){
        cerr<<"format is incorrect"<<endl;
        exit(0);
        
    }
    
    
    
    //go through each editor and add the article to the set
    map<string, double> tmp_s;
    int appear = 0;
    for (unsigned int j = 1; j<parsed.size();j+=2){
        //record only positive major edits
        if(stod(parsed[j+1])>=0.1){
            //if the editor is aready in the map
            unordered_map <string, map<string, double> >::iterator it;
            
            appear = info.count(parsed[j]);
            if (appear == 1){
                it = info.find(parsed[j]);
                // if the editor doesn't have the article in the set
                if(it->second.find(parsed[0])==it->second.end()){
                    (it->second)[parsed[0]] = stod(parsed[j+1]);
                }
                else{
                    (it->second)[parsed[0]] += stod(parsed[j+1]);
                }
            }
            // if the editor is not in the map
            else{
                tmp_s.clear();
                tmp_s[parsed[0]] = stod(parsed[j+1]);
                info[parsed[j]] = tmp_s;
            }
        }
    }
    
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
    string output_file_address = output_address+"/major_editor_w_art_cate_"+cate_range+".txt";
    
    
    ofstream outfile;
    outfile.open (output_file_address);
    
    
    string input_address = argv[1];
    fstream infile;
    infile.open(input_address);
    
  
    
    //use a set to remove duplicate edits of the same article by the same author
    unordered_map <string, map<string, double> > info; // [editor name, <[art1, edit size], [art2, edit size] ,...>]
    
    int counter = 0; //current line number
 
    string line = "";
    
   
    if(!infile){
        cerr<<"Can't open the file"<<endl;
        exit(1);
    }
    
    
    while(!infile.eof()){
        getline(infile,line);
        if (!line.empty()){
            parse_info(line, info);
        }
    }
    infile.close();
    
    
    
    //print to a text file
    unordered_map <string, map<string, double> >::iterator it2;
    map<string, double>::iterator it3;
    for (it2 = info.begin(); it2!=info.end();it2++){
        it3 = it2->second.begin();
        //if an author edited fewer than 3 edits, disregard him
        if(it2->second.size()<3){
            continue;
        }
        //editor name
        outfile<<it2->first;
        //edited article and size
        
        for(it3 = it2->second.begin(); it3 != it2->second.end();it3++){
            outfile<<", "<< it3->first<<", "<<it3->second;
        }
        outfile<<endl;
    }
    
    outfile.close();
    
    cout<<"Completed"<<endl;
    
    
    return 1;
}

