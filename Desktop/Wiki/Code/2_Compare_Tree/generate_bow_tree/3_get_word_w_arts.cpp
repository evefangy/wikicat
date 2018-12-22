//
//  main.cpp
//  get_editor_with_art
//
//  Created by Eve on 9/19/17.
//  Copyright © 2017 Eve. All rights reserved.
//

/////////////////////////////////////////////////////////////////////////
//  Code rearrage the parsed revision history                          //
//  (following article, major word 1, major word 2,... format)     //
//  to the following format                                            //
//  word 1, majorly edited article 1, majorly edited article 2, ...  //
/////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_set>
using namespace std;

int num_art = 0;
unordered_set<string> all_articles;

void parse_info(string& line, unordered_map <string, unordered_set<string> >& info){
    string article;
    vector<string> parsed;
    parsed.clear();
    bool name = false;
    if(line.length() == 0){
        cerr<<"empty string"<<endl;
        exit(0);
    }
    string tmp;
    tmp = "";
    for (int i=0;i<line.size();i++){
        if (line[i]==','){
            parsed.push_back(tmp);
            tmp = "";
        }
        else if (line[i]!=' '){
            tmp+=line[i];
        }
    }
    parsed.push_back(tmp);

    //go through each word and add the article to the set
    unordered_set<string> unique_article_for_word;
    int appear = 0;
    // go through each word
    for (unsigned int j = 1; j<parsed.size();j++){
        //if the word is aready in the map
        unordered_map <string, unordered_set<string> >::iterator it;
        appear = info.count(parsed[j]);
        if (appear == 1){
            it = info.find(parsed[j]);
            // if the word doesn't have the article in the set
            if(it->second.find(parsed[0])==it->second.end()){
                it->second.insert(parsed[0]);
            }
        }
        // if the editor is not in the map
        else{
            unique_article_for_word.clear();
            unique_article_for_word.insert(parsed[0]);
            info[parsed[j]] = unique_article_for_word;
        }
    }
    
}

int main(int argc, const char * argv[]) {
    if(argc < 3){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Address_for_art_w_major_editor_file | Output_address"<<endl;
        cout<<"(e.g ./art_word.txt ./Output)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    string output_address = argv[2];

    string output_file_address = output_address+"/3_word_w_art.txt";
    
    string count_addr = output_address+"/3_counts.txt";
    
    
    ofstream outfile, count_file;
    outfile.open (output_file_address);
    count_file.open(count_addr);
    
  

    string input_address = argv[1];
    fstream infile;
    infile.open(input_address);

    
    
    //use a set to remove duplicate edits of the same article by the same author
    unordered_map <string, unordered_set<string> > info; // [editor name, <art1, art2 ,...>]
  
    
    
    int counter = 0; //current line number

    num_art = 0;    string line = "";
    

    
    if(!infile){
        cerr<<"Can't open the file"<<endl;
        exit(1);
    }
    
    
    while(!infile.eof()){
        getline(infile,line);
        if (!line.empty()){
            if(counter >= 0){
                num_art++;
                parse_info(line, info);
            }
            
        }
        counter++;
    }
    infile.close();
    
    

    int num_tf_art = 0;
    //print to a text file
    unordered_map <string, unordered_set<string> >::iterator it2;
    unordered_set<string>::iterator it3;
    for (it2 = info.begin(); it2!=info.end();it2++){
        //skip words that only appears in one article
        if(it2->second.size()<2) continue;
        count_file<<it2->first<<" "<<it2->second.size()<<endl;
        //record the td-idf word and articles that contain the word
        outfile<<it2->first<<", ";
        it3 = it2->second.begin();
        outfile<< *it3;
        it3++;

        //record the current article
        all_articles.insert(*it3);
        
        for(; it3 != it2->second.end();it3++){
            outfile<<", "<< *it3;
            all_articles.insert(*it3);
        }
        outfile<<endl;;
    }
    
    outfile.close();
    cout<<"Number of articles with TF-IDF words: "<<all_articles.size()<<endl;
    cout<<"Total number of articles: "<<num_art<<endl;
    
    cout<<"Completed"<<endl;

    
    return 1;
}
