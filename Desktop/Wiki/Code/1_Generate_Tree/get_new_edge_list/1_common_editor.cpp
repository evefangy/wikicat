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
        if(s[i]==','){
            if(tmp.size()!=0 && tmp!=" "){
                parsed.push_back(tmp);
                
            }
            tmp = "";
        }
        else{
            if(s[i]!= ' '){
                tmp+=s[i];
            }
        }
    }
    if(tmp.size()!=0 && tmp!=" "){
        parsed.push_back(tmp);
        
    }
}


vector<int> calculate_intersection(unordered_set<string>& s1, unordered_set<string>& s2){
    // cout<<"here3"<<endl;
    int intersection = 0;

    int total = s1.size()+s2.size();

    int union_ = 0;

    for (auto i=s2.begin(); i!=s2.end();i++){
        if (s1.count(*i)==1){
            intersection++;
        }
    }

    union_ = total-intersection;

    vector<int> ans;
    ans.push_back(intersection);
    ans.push_back(union_);

    return ans;
}


int main(int argc, const char * argv[]) {
    if(argc < 7){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Address_for_article_editor | Output_address | q | artname | edge_list | used_art"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    string infile = argv[1];
    string output_address = argv[2];
    string q_str = argv[3];
    double q = stod(q_str);

    string artname_addr = argv[4];
    string edge_list_addr = argv[5];

    string used_art_addr = argv[6];

    output_address += "/edge_list_with_q_";
    output_address = output_address + q_str +".txt";

    // cout<<"here0.0"<<endl;

    bool isLog = false;
    if (q==-1){
        isLog = true;
    }

    ifstream used_art_file;
    used_art_file.open(used_art_addr);
    if (!used_art_file){
        cout<<"can't open artname"<<endl;
        exit(0);
    }



    ifstream artname;
    artname.open(artname_addr);
    if (!artname){
        cout<<"can't open artname"<<endl;
        exit(0);
    }

    ifstream art_ed;
    art_ed.open(infile);
    if (!art_ed){
        cout<<"can't open arted"<<endl;
        exit(0);
    }
  

// cout<<"here1.0"<<endl;

    unordered_set<string> used_art;
    vector<string> parsed;
    string line;

    
    while(!used_art_file.eof()){   // store used article ids
        getline(used_art_file, line);
        if (line.size()!=0){

            parsed.clear();
            parse(line, parsed);
            if (parsed.size()==1){
                used_art.insert(parsed[0]);
            }
            else{
                used_art.insert(parsed[1]);
            }
        }
    }

    cout<<"collected "<<used_art.size()<<" used articles"<<endl;
    used_art_file.close();


    unordered_map<string,string> artname_map;

    int line_ctr = 0;
    while(!artname.eof()){
        getline(artname, line);
        if (line.size()!=0){

            parsed.clear();
            parse(line, parsed);
            if (parsed.size()==1){
                if (used_art.count(parsed[0])==1){
                    artname_map[parsed[0]] = line_ctr;  // store key: article id, value: encoded
                }
                
            }
            else{
                if (used_art.count(parsed[1])==1){
                    artname_map[parsed[1]] = parsed[0];
                }
            }
            line_ctr++;
        }
    }

    cout<<"collected "<<artname_map.size()<<" artnames"<<endl;
    artname.close();


    ofstream outfile;
    outfile.open(output_address);
    if (!outfile){
        cout<<"can't open outfile"<<endl;
        exit(0);
    }

// cout<<"here0"<<endl;

    unordered_set<string> ed;
    unordered_map<string, unordered_set<string> > article_editor_map;


    // store all article along with its editors
    while(!art_ed.eof()){
        getline(art_ed, line);
        if (line.size()!=0){
            parsed.clear();
            parse(line, parsed);
            string a = parsed[0];
            ed.clear();
            for (int i=1;i<parsed.size();i++){
                ed.insert(parsed[i]);
            }

            article_editor_map[artname_map[a]] = ed;
        }
    }

    cout<<"collected "<<article_editor_map.size()<<" article_editor sets"<<endl;
   
    // cout<<"here1"<<endl;

    art_ed.close();

    ifstream edge_list;
    edge_list.open(edge_list_addr);
    if(!edge_list){
        cout<<"Can't open egde list"<<endl;
        exit(0);
    }

    string a1;
    string a2;

// cout<<"here2"<<endl;
    vector<int> inter_union;
    while(!edge_list.eof()){
        getline(edge_list, line);
        if(line.size()!=0){
            parsed.clear();
            parse(line, parsed);
            a1 = parsed[0];
            a2 = parsed[1];

            if (article_editor_map.count(a1)==0 || article_editor_map.count(a2)==0) continue;


            inter_union.clear();
            inter_union = calculate_intersection(article_editor_map[a1], article_editor_map[a2]);

            // cout<<inter_union[0]<<"---"<<inter_union[1]<<endl;
            double new_weight = 0.0;
            if (isLog){
                new_weight = inter_union[0]*1.0/log10(inter_union[1]);
            }

            else{
                new_weight = inter_union[0]*1.0/pow(inter_union[1], q);
            }

            if (!isinf(new_weight)){
                outfile<<a1<<", "<<a2<<", "<<new_weight<<endl;
            }
            
            
        }
    }

// cout<<"here4"<<endl;




    return 0;
}

