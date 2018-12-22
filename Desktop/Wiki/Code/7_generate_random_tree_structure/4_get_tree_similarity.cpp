//
//  main.cpp
//  get_tree_similarity
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
#include <algorithm>
#include <unordered_set>

float max_score = -1.0;
float avg_score = 0.0;
int path_count = 0;

float avg_path_using_max = 0.0;
double avg_path_using_avg = 0.0;
float max_path = -1.0;
float min_path = 1.0;
int tot_path = 0;


using namespace std;
void parse(vector<string>& parsed, string& line){
    string tmp = "";
    //corner case: if the input is an article name
    if(line[1] == '-'){ //use line[1] b/c first char maybe a negative sign
        parsed.push_back("-");
        return;
    }
    for(int i=0;i<line.size();i++){
        if(line[i]==' '){
            parsed.push_back(tmp);
            tmp = "";
        }
        else {
            tmp+=line[i];
        }
    }
    if(tmp != ""){
        parsed.push_back(tmp);
    }
}

void output_score(vector<vector<float> >& sim_matrix, ofstream& outfile){
    vector<vector<float> > opt_score;
    for(int i=0;i<sim_matrix.size();i++){
        vector<float> tmp;
        for(int j=0;j<sim_matrix[0].size();j++){
            tmp.push_back(0.0);
        }
        opt_score.push_back(tmp);
    }
    //initiate the first row with max running score
    opt_score[0][0] = sim_matrix[0][0];
    for (int i=1;i<sim_matrix[0].size();i++){
        if(opt_score[0][i-1]>sim_matrix[0][i]){
            opt_score[0][i] = opt_score[0][i-1];
        }
        else{
            opt_score[0][i] = sim_matrix[0][i];
        }
    }
    for(int i=1;i<sim_matrix.size();i++){
        for(int j=i;j<sim_matrix[0].size();j++){
            opt_score[i][j] = max(opt_score[i-1][j-1]+sim_matrix[i][j], opt_score[i][j-1]);
        }
    }
    // calculate the optimal similarity score for the path 
    float opt = 0.0;
    if (sim_matrix[0].size()!=2){
        // subtract the root and the leaf node
        opt = (opt_score[sim_matrix.size()-1][sim_matrix[0].size()-1]-2)/(sim_matrix[0].size()-2);
    }

    



    outfile<<opt<<" ";
    avg_score += opt;
    path_count += 1;
    if(opt>max_score){max_score = opt;}
    
    
    if(opt>max_path){max_path = opt;}
    if(opt<min_path){min_path = opt;}
}

int main(int argc, const char * argv[]) {
    if(argc < 3){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Similarity_file Output_file"<<endl;
        cout<<"(e.g ./reported_similartity.txt ./)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    
    string sim_addr = argv[1];
    string output_addr = argv[2];
    output_addr+="/new_similartity_score.txt";
    
    string avg_score_addr = argv[2];
    avg_score_addr+="/avg_scores.txt";
    string best_score_addr = argv[2];
    best_score_addr+="/best_score.txt";
    
    //create output file
    ofstream outfile;
    outfile.open(output_addr);
    
    ofstream avg_file;
    avg_file.open(avg_score_addr);
    ofstream best_file;
    best_file.open(best_score_addr);
    
    //get similarity matrix
    ifstream sim_matrix_file;
    sim_matrix_file.open(sim_addr);
    string line = "";
    vector<string> parsed;
    vector<vector<float> > sim_matrix;
    vector<float> row;
    sim_matrix.clear();
    bool first_art = true;
    bool reached_limit = false;
    int c=0;
    while(!sim_matrix_file.eof()){
        line = "";
        getline(sim_matrix_file, line);
        if(line.size()!=0){
            parsed.clear();
            parse(parsed, line);
            
            //start to analyze the similarity score on a new article
            if(parsed[0]=="-"){
                if(!first_art){
                    outfile<<endl;
                    float avg_score_tmp = avg_score/path_count;
                    
                    outfile<<"Average Score: "<<avg_score_tmp<<endl;
                    outfile<<"Best Score: "<<max_score<<endl<<endl;
                    
                    avg_file<<avg_score_tmp<<endl;
                    best_file<<max_score<<endl;
                    
                    avg_path_using_avg = avg_path_using_avg+ avg_score_tmp;
                    avg_path_using_max += max_score;

                    max_score = -1.0;
                    avg_score = 0.0;
                    path_count = 0;
                }
                c++;
                if(c==100000){
                    reached_limit = true;
                }
                first_art = false;
                tot_path+=1;
                outfile<<"Sampled article"<<line<<endl;
                outfile<<"Optimal Score(s): ";
            }
            else{
                row.clear();
                for(int i=0;i<parsed.size();i++){
                    if(parsed[i] == "nan"){
                        cerr<<"<------- NAN appeared in the similarity score ------>"<<endl;
                        exit(0);
                    }
                    row.push_back(stof(parsed[i]));
                }
                sim_matrix.push_back(row);
            }
        }
        //if reach the end of one matrix
        else{
            if(sim_matrix.size()!=0){
                output_score(sim_matrix, outfile);
                sim_matrix.clear();
                if(reached_limit==true){
                    break;
                }
                
            }
            
        }
    }
    sim_matrix_file.close();
    
    //output last pair
    outfile<<endl;
    float avg_score_tmp = avg_score/path_count;
   
    outfile<<"Average Score: "<<avg_score_tmp<<endl;
    outfile<<"Best Score: "<<max_score<<endl<<endl;
    avg_file<<avg_score_tmp<<endl;
    best_file<<max_score<<endl;
    
    avg_path_using_avg = avg_path_using_avg+avg_score_tmp;
    avg_path_using_max += max_score;
    
    outfile<<"-------------------------------------------"<<endl;
    outfile<<"Average Score of All Sampled Path (using max_score): "<<avg_path_using_max/tot_path<<endl;
    outfile<<"Average Score of All Sampled Path (using avg_score): "<<avg_path_using_avg/tot_path<<endl;
    cout<<avg_path_using_max/tot_path<<endl;
    cout<<avg_path_using_avg/tot_path<<endl;
    outfile<<"Maximum Score of All Sampled Path: "<<max_path<<endl;
    outfile<<"Minimum Score of All Sampled Path: "<<min_path<<endl;
    outfile<<"Analyzed "<<tot_path<<" article paths in total"<<endl;
    outfile.close();
    best_file.close();
    avg_file.close();
    
    return 0;

}
