
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

void parse(vector<string>& parsed, string& s){
	string tmp = "";

	string curr_char = "";
	for (int i=0;i<s.size();i++){
		curr_char = s[i];
		if (curr_char==":" || curr_char == "•"){
			if (tmp.size()!=0 && tmp!= " "){
				parsed.push_back(tmp);
			}
			tmp = "";
		}
		else if (s[i]!=' '){
			tmp+=s[i];
		}
	}

	if (tmp.size()!=0 && tmp!= " "){
		parsed.push_back(tmp);
	}

}


int main(int argc, const char * argv[]){
	string infile_addr="/Users/eve/Desktop/input_string.txt";
	
	ifstream infile;
	vector<string> parsed;
	infile.open(infile_addr);
	string line="";
	while(!infile.eof()){
		getline(infile, line);
		if (line!=""){
			parsed.clear();
			parse(parsed, line);
			for (int i=0;i<parsed.size();i++){
				cout<<parsed[i]<<endl;
			}

			// line.erase(remove(line.begin(), line.end(), '\n'), line.end());
			// line.erase(remove(line.begin(), line.end(), '\t'), line.end());
			// line = "Lunch "+line;
			// dish_name = dish_name+"\""+line+"\", ";
			// // cout<<endl;
			// cout<<"\""<<line<<"\":{"<<endl;
			// cout<<"\tprice:{"<<endl;
			// cout<<"\t\tsm: 6.10,"<<endl;
			// cout<<"\t},"<<endl;
			// cout<<"\tsides: lunch_sides"<<endl;;
			// cout<<"},"<<endl;

		}
	}

	// cout<<dish_name<<endl;


}