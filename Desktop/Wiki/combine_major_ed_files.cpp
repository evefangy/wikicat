//
//  main.cpp
//  combine_two_files_side_by_side
//
//  Created by Eve on 2/6/18.
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
#include <unordered_set>

using namespace std;

int main(int argc, const char * argv[]) {
    if(argc < 4){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"file1 | file2 | Output_Address"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    
    string f1_addr = argv[1];
    string f2_addr = argv[2];
    string output_addr = argv[3];
    output_addr+="/combined_art_ed.txt";
    
    
    ifstream f1;
    f1.open(f1_addr);
    ifstream f2;
    f2.open(f2_addr);
    
    
    ofstream outfile;
    outfile.open(output_addr);
   
    if(!outfile){
        cerr<<"can't open output file"<<endl;
        exit(0);
    }
    
    //read in file1
    string line;
    string artIndex = "";
    int indexOfComma = 0;


    unordered_set<string> visited;

    int dup1 = 0;
    int dup2 = 0;

    while(!f1.eof()){
        line = "";
        getline(f1, line);
        indexOfComma = line.find(",");
        if (indexOfComma!=std::string::npos){
            artIndex = line.substr(0, indexOfComma);
        }
        else{
            artIndex = line;
        }
        
        if (visited.count(artIndex)==0){
            visited.insert(artIndex);
            outfile<<line<<endl;
        }
        else{
            dup1++;
        }
        
    }
    cout<<"dup1 = "<<dup1<<endl;
  
    f1.close();
    while(!f2.eof()){
        line = "";
        getline(f2, line);
        indexOfComma = line.find(",");
        if (indexOfComma!=std::string::npos){
            artIndex = line.substr(0, indexOfComma);
        }
        else{
            artIndex = line;
        }
        
        if (visited.count(artIndex)==0){
            visited.insert(artIndex);
            outfile<<line<<endl;
        }
        else{
            dup2++;
        }
        
    }
    f2.close();
    outfile.close();
    

    cout<<"dup2 = "<<dup2<<endl;
    cout<<dup1+dup2<<endl;
    return 0;
}

