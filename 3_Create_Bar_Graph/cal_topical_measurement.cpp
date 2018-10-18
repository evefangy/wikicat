

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <math.h>

using namespace std;


int curr_in = -1; //current index of the new inserted article [used in addEdge]
int num_noEdge = 0;

bool new_art;
bool store_art1;
bool store_art2;
string art_index1;
string art_index2;


void parsed_by_space(string& s, vector<string>& parsed){
    string tmp = "";
    for(int i=0; i<s.length();i++){
        if(s[i]==' '){
            parsed.push_back(tmp);
            tmp = "";
        }
        else{
            tmp+=s[i];
        }
    }
    if(tmp.size()!=0){
        parsed.push_back(tmp);
    }
    
}

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





string most_freq(vector<string>& g){
    sort(g.begin(), g.end());
   
    //Finding the mode of the entered words
    int _count = 0;
    int _max = 0;
    string mode;
    for (unsigned int test = 0, j = 1; test<g.size(); test++, j++) {
        if (g[test] == g[j]) {
            _count++;
        }
        else if (g[test] != g[j]) {
            if (_count>_max) {
                _max = _count;
                mode = g[test];
            }
            _count = 0;
        }
    }
    return mode;
}


double cal_top(vector<string>& g, vector<double>& ed_size){
    if(ed_size.size()==1){
        return 1.0;
    }
    
    double max_v = 0.0;
    double min_v = 1.0;
//    //normalize the ed_size
//    for(int i=0;i<ed_size.size();i++){
//        cout<<ed_size[i]<<" ";
//        if (ed_size[i]<min_v) {
//            min_v = ed_size[i];
//        }
//        if(ed_size[i]>max_v){
//            max_v = ed_size[i];
//        }
//    }
//    cout<<endl;
    
    double sum_v = 0.0;
    //calculate edit percentage
    for(int i=0;i<ed_size.size();i++){
        sum_v += ed_size[i];
        
    }



    
    double entro = 0.0;
    for(int i=0;i<ed_size.size();i++){
//        ed_size[i] = (ed_size[i]-min_v)*1.0/(max_v-min_v);
        ed_size[i] = ed_size[i]/sum_v;
        entro = entro+(ed_size[i]*log(ed_size[i]));
        cout<<ed_size[i]<<" ";
    }
    cout<<endl;
    cout<<"-------"<<endl;
    
    //calculate the entrope
    double top_m = 0.0;
    top_m = (log(ed_size.size())+entro)/log(ed_size.size());
    cout<<"top_m = "<<top_m<<endl;
    return top_m;
    
    
//    //count number of topics
//    double top_m = 0.0;
//
//    sort(g.begin(), g.end());
//    vector<int> counter;
//
//    int curr = 0;
//    int num = 0;
//    for(int i=1;i<g.size();i++){
//        if(g[curr]==g[i]){
//            num++;
//        }
//        else{
//            curr = i;
//            counter.push_back(num);
//            num = 0;
//        }
//    }
//    return counter.size();
    
//    //Finding the mode of the entered words
//    int _count = 0;
//    int _max = 0;
////    string mode;
//    for (unsigned int test = 0, j = 1; test<g.size(); test++, j++) {
//        if (g[test] == g[j]) {
//            _count++;
//        }
//        else if (g[test] != g[j]) {
//            if (_count>_max) {
//                _max = _count;
////                mode = g[test];
//            }
//            _count = 0;
//        }
//    }
//
//    top_m = _max*1.0/g.size();
//    return top_m;
    
}



int main(int argc, const char * argv[]) {
    if(argc < 5){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Address_for_major_editor_w_article_file | Output_address | category_group_file | article_category_file "<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    string output_address = argv[2];
    output_address+="/topical_measure.txt";
    string err_address = argv[2];
    err_address+="/unstored_cate.txt";
    string top_address = argv[2];
    top_address+="/top_bar.txt";
    
    
    string cate_addr = argv[3];
    string art_cate_addr = argv[4];
    
    //store each category's corresponding group
    unordered_map<string, string> cate_map;
    
    //local variables
    vector<string> parsed;
    string line = "";
    
    //store counter
    ofstream outfile, errfile, top_bar;
    outfile.open (output_address);
    errfile.open(err_address);
    top_bar.open(top_address);

    
    ifstream infile, cate_group, art_cate;
    
    //store categories to their corresponding group
    cate_group.open(cate_addr);
    while(!cate_group.eof()){
        getline(cate_group,line);
        if (!line.empty()){
            parsed.clear();
            parse(line, parsed);
            for(int i=0;i<parsed.size();i++){
                if(i==0) continue;
                cate_map[parsed[i]] = parsed[0];
//                cout<<parsed[i]<<" "<<parsed[0]<<endl;
            }
        }
    }
    cate_group.close();
    
    cout<<"Stored "<<cate_map.size()<<" categories"<<endl;
    
    
    // Go through each article and its categories. Find the groups those categories belong to.
    // Assign the article to a group that appeared the most frequently
    unordered_map<string, string> art_cate_group;
    vector<string> tmp;
    art_cate.open(art_cate_addr);
    while(!art_cate.eof()){
        getline(art_cate,line);
        if (!line.empty()){
            parsed.clear();
            parsed_by_space(line, parsed);
            tmp.clear();
            if(art_cate_group.count(parsed[0])!=0){
                cerr<<parsed[0]<< "has existed in the map"<<endl;
                continue;
            }

            for(int i=0;i<parsed.size();i++){
                if(i==0) continue;
                //if the category doesn't exist
                if(cate_map.count(parsed[i])==0){
                    errfile<<parsed[i]<<endl;
                    continue;
                }
                tmp.push_back(cate_map[parsed[i]]);
            }
            
            //use the most frequent group
            art_cate_group[parsed[0]] = most_freq(tmp);
        }
    }
    art_cate.close();
    
    
    infile.open (argv[1]);

    double top_m = 0.0;
    
    if(!infile){
        cerr<<"Can't open infile"<<endl;
        exit(1);
    }
   
    vector<double> ed_size;
    int index = 0;
    string art_group = "";
    
    while(!infile.eof()){
        getline(infile,line);
        if (!line.empty()){
            parsed.clear();
            parse(line, parsed);
            
            tmp.clear();
            ed_size.clear();
            if(parsed.size()<2){
                cerr<<"parsed.size()<2"<<endl;
                exit(0);
            }
            for(int i=1;i<parsed.size();i+=2){
                
             
                //locate index
                art_group = art_cate_group[parsed[i]];
//                cout<<art_group<<" ";
//                cout<<art_group<<"----"<<endl;
                if(tmp.size()<1){
                    tmp.push_back(art_group);
                    ed_size.push_back(stod(parsed[i+1]));
                    continue;
                }
                
                
                index=0;
                while(index<tmp.size() && tmp[index] != art_group){
                    index++;
                }
            
                //can't locate
                if(index==tmp.size()){
                    tmp.push_back(art_group);
                    ed_size.push_back(stod(parsed[i+1]));
                }
                else {
                    ed_size[index]+=stod(parsed[i+1]);
                }
            }
//            cout<<"+++++"<<endl;
            
            top_m = cal_top(tmp, ed_size);
            // <------------------------------------------------------
            if(top_m!=0.0){
                outfile<<parsed[0]<<" "<<parsed.size()/2<<" "<<top_m;
                if(top_m>=0.3){
                    top_bar<<parsed.size()/2<<endl;
                    outfile<<" "<<parsed.size()/2;
                }
                else{
                    outfile<<" -1";
                }
               
                
                outfile<<endl;
            }
            
            
        }
    }
    infile.close();

   
    cout<<"Completed"<<endl;
   
    return 0;
}

