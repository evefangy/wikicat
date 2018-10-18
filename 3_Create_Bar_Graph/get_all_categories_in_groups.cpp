// The code takes in the category tree and a level request. The code goes to the desired level and gets all subcategories and ancestors for the nodes in the level.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>

using namespace std;


void parse(string& s, vector<string>& parsed){
    string tmp = "";
    for(int i=0; i<s.length();i++){
        if(s[i] == ':'){
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

void parse_group(string& s, vector<int>& decode){
    string tmp="";
    for(int i=0;i<s.size();i++){
        
        if(s[i]=='_'){
            if(tmp.size()==0) continue;
            decode.push_back(stoi(tmp));
            tmp = "";
        }
        else{
            tmp+=s[i];
        }
    }
    decode.push_back(stoi(tmp));
   
    
//    for(int i=0;i<decode.size();i++){
//        cout<<decode[i]<<" ";
//    }
//    cout<<endl;
}

int main(int argc, const char * argv[]) {
    if(argc < 4){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Address_for_categories | Output_address | Number_of_level_to_extract "<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    string in_addr = argv[1];
    string output_address = argv[2];
    output_address+="/combined_category.txt";
    string lv_range_str = argv[3];
    int lv_range = stoi(lv_range_str);
   
    //ofstram
    ofstream outfile;
    outfile.open(output_address);
    
    
    //local variables
    vector<string> parsed;
    string line = "";
    vector<int> decode;
   
    //read in category file
    ifstream infile;
    infile.open (in_addr);
   
    
    //vector for storing ancestors
    vector <int> ancestors;
    map<string, vector <int> > cate_at_curr_lv;
    
    if(!infile){
        cerr<<"Can't open infile"<<endl;
        exit(1);
    }
   
    int line_c = 0;
    while(!infile.eof()){
        line_c++;
        if(line_c%100 == 0){
            cout<<line_c<<endl;
        }
        getline(infile,line);
        if (!line.empty()){
            parsed.clear();
            parse(line, parsed);
            decode.clear();
            parse_group(parsed[0], decode);
            
            //if we haven't reached the level
            if(decode.size()<lv_range){
                ancestors.push_back(stoi(parsed[1]));
            }
            //if the current level is the level we want to examine
            else if(decode.size()==lv_range){
                
                string group_code = parsed[0];
                string article = parsed[1];
                //doesn't exist
                if(cate_at_curr_lv.count(group_code)==0){
                    cate_at_curr_lv[group_code] = ancestors;
                    
                }
                cate_at_curr_lv[group_code].push_back(stoi(article));
            }
            else{
                string main_group = "";
                for(int i=0;i<lv_range;i++){
                    main_group = main_group+"_"+to_string(decode[i]);
                }
                
                cate_at_curr_lv[main_group].push_back(stoi(parsed[1]));
            }
        }
    }
    infile.close();

   
    for(auto it = cate_at_curr_lv.begin();it!=cate_at_curr_lv.end(); it++){
        outfile<<it->first;
        for(int i = 0;i<it->second.size();i++){
            outfile<<", "<<it->second[i];
        }
        outfile<<endl;
    }
    
    
    
    cout<<"There are "<<cate_at_curr_lv.size()<<" category groups we are interested in"<<endl;

    return 0;
}

