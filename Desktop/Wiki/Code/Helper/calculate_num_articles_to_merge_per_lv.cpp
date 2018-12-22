
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

//ofstream artname;
//bool first = false;

void parse(vector<string>& parsed, string& line){
    string tmp = "";
    for(int i=0;i<line.size();i++){
        if(line[i]==' '){
            if (tmp!=" "&&tmp.size()!=0){
                parsed.push_back(tmp);
            }
            
            tmp = "";
        }
        else if(line[i]!=','){
            tmp+=line[i];
        }
    }
    if (tmp!=" "&&tmp.size()!=0){
        parsed.push_back(tmp);
    }
}

int main(int argc, const char * argv[]) {
    if(argc < 3){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"sorted_weight_Address | outputfolder"<<endl;
        cout<<"(e.g ./sorted_weight.txt ./)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string weight_addr = argv[1]; //weight, a1, a2

    string output_addr = argv[2];

    output_addr += "/ctr.txt";

    ofstream outfile;
    outfile.open(output_addr);


    unordered_set<string> visited;
    unordered_map<string, int> num_art_merged; //weight, num new articles

    vector<string> v;

    ifstream infile;
    infile.open(weight_addr);
    string line="";
    vector<string> parsed;
    while(!infile.eof()){
        getline(infile, line);
        if (line!="" && line.size()!=0){
            parsed.clear();
            parse(parsed, line);

            if (parsed.size()!=3){
                cerr<<"parsed.size()!=3"<<endl;
                cout<<"["<<line<<"]"<<endl;
                exit(0);
            }

            string weight = parsed[0];
            string a1 = parsed[1];
            string a2 = parsed[2];

            if (visited.count(a1)==0){
                num_art_merged[weight] += 1;
                visited.insert(a1);
                v.push_back(a1);
            }
            if (visited.count(a2)==0){
                num_art_merged[weight] += 1;
                visited.insert(a2);
                v.push_back(a2);
            }

        }
    }

    int num_art = 0;
    for (auto it=num_art_merged.begin();it!=num_art_merged.end();it++){
        cout<<it->first<<" "<<it->second<<" "<<(it->second*1.0/visited.size())*100<<"%"<<endl;
        num_art+=it->second;
    }

    sort(v.begin(), v.end());
    for (auto i=0;i<v.size();i++){
        outfile<<v[i]<<endl;
    }


    outfile.close();

    cout<<"There are in total "<<num_art<<" articles"<<endl;
    cout<<"There are in total "<<visited.size()<<" articles encountered"<<endl;
  
    return 0;
    
}
