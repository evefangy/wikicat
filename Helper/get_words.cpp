//
//  main.cpp
//  parse
//
//  Created by Eve on 11/28/17.
//  Copyright © 2017 Eve. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
bool brackets;
bool useless;
bool xml;


void parse(string& s, ofstream& outfile){
    
//    if(s=="== See also =="||s=="== References =="||s=="== Sources =="||s=="== External links =="){
//        useless = true;
//        return;
//    }

    int i=0;
    while(i<s.size()){
        if(xml){
            //remove xml
            if(s[i] == '<'){
                brackets = true;
            }
            if(brackets){
                if (s[i]=='>'){
                    brackets = false;
                }
            }
            else{
                //copy the content
                //replace quote
                if(s[i]=='&'){
                    if(i+6<s.size()&&s.substr(i,6)=="&quot;"){
                        outfile<<'"';
                        i=i+6;
                        continue;
                    }
                }
                
                outfile<<s[i];
                
            }
        }
        
        else{
            //replace quote
            if(s[i]=='&'){
                if(i+6<s.size()&&s.substr(i,6)=="&quot;"){
                    outfile<<'"';
                    i=i+6;
                    continue;
                }
            }
            
            outfile<<s[i];
            
        }
        i++;
        
    }
    
}


int main(int argc, const char * argv[]) {
    ifstream infile;
    ofstream outfile;
    string address = argv[1];
    string o_address = argv[2];
    
    

    infile.open(address);
    outfile.open(o_address);
    
    
    
    brackets = false;
    useless = false;
    xml = true;
    int num_art = 0;
    string line = "";
    
    
    
    while(!infile.eof()){
        getline(infile, line);
        //skip empty lines
        if(line.empty()){
            continue;
        }
        //veriy if it's a new article
        if(line[0] == '\r'){
            //if it's "new art"
            if(line[1]!='<' && line!="\r"){
                xml = true;
                brackets = false;
                useless = false;
                num_art+=1;
                cout<<num_art<<endl;
                outfile<<endl<<"************************** New Article *************************"<<endl<<endl;
            }
            //if it's xml
            else if(line[1]!='<'){
                parse(line, outfile);
                outfile<<endl;
                xml = false;

            }
        }

        else if(!useless){
            parse(line, outfile);
            outfile<<endl;
            xml = false;
    
        }
        
    }
    infile.close();
    outfile.close();
    cout<<num_art<<endl;
    cout<<"Verify if there're more articles than feeded (error may happens for **)"<<endl;
    

    cout<<"completed"<<endl;
    return 0;
}

