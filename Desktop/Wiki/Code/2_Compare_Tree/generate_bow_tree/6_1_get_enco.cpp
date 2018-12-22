/////////////////////////////////////////////////////////////////////////////////////
//  The code calculates the new group assignment after merging all                 //
//  pairs for each level.                                                          //
//                                                                                 //
//  Boost library (a sample code prepared by Alejandro was used as a reference):   //
//  At first, we created a node for every article. Each article was                //
//  originally assigned to itself as a group. Union-find was run to                //
//  merge two articles. Once the number of pairs we grabbed was as                 //
//  desired, we check the new group assignments for every article:                 //
//  1) If an article has a new group or has some other articles joined in          //
//      (become the parent), record the group number in output file                //
//  2) If an article hasn't been merged, record the group number as -1             //
//                                                                                 //
//  Output: Group Assignment File                                                  //
//  Example: {[-1, -1, 2, 2], [2, 2, 2, 2]}                                        //
//  ------------------------------------------------------------------             //
//  Note:                                                                          //
//  1. Each row of the output file shows the group assignment of each iteration,   //
//  each index of of row corresponds to the article_index. -1 means the article    //
//  stays in its own group (has never merged with any group). Once an article      //
//  is merged into a group, the group number is recorded.                          //
//                                                                                 //
//  2. All article index in the input file must be consecutive and start from 0    //
//      for the boost library to work                                              //
/////////////////////////////////////////////////////////////////////////////////////


/*!
 * \brief Uses boost disjoint_sets with a custom class.
 *
 * \author Alejandro Mallea <janoma@gmail.com>
 *
 * \copyright The MIT License
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <set>
#include <map>
#include <unordered_set>
#include <string>

#include <boost/pending/disjoint_sets.hpp>
#include <boost/pending/property.hpp>
using namespace std;

int current_counter = 0;

//////////////////////////////////////////////////
//      Disjoint Set Class & Helpers            //
//////////////////////////////////////////////////

class Element
{
public:
    explicit
    Element(int n) {art_index = n;}
    
    int get_art_index() const { return art_index; }
    
    size_t dsID;    //in this implementation dsID=art_index
    size_t dsRank;
    size_t dsParent;
    
private:
    int art_index;
};

inline bool
operator==(Element const& lhs, Element const& rhs)
{
    return lhs.get_art_index() == rhs.get_art_index();
}

inline bool
operator!=(Element const& lhs, Element const& rhs)
{
    return ! operator==(lhs, rhs);
}

class Parent
{
public:
    Parent(std::vector<Element>& e) : mElements(e) { }
    std::vector<Element>& mElements;
};

class Rank
{
public:
    Rank(std::vector<Element>& e) : mElements(e) { }
    std::vector<Element>& mElements;
};

template <>
struct boost::property_traits<Rank*>
{
    typedef size_t value_type;
};

inline Element const&
get(Parent* pa, Element const& k)
{
    return pa->mElements.at(k.dsParent);
}

inline void
put(Parent* pa, Element k, Element const& val)
{
    pa->mElements.at(k.dsID).dsParent = val.dsID;
}

inline size_t const&
get(Rank*, Element const& k)
{
    return k.dsRank;
}

inline void
put(Rank* pa, Element k, size_t const& val)
{
    pa->mElements.at(k.dsID).dsRank = val;
}


void printElements(std::vector<Element>& elements)
{
    cout<<"---------------- ID [Group] ---------------------"<<endl;
    for (size_t i = 0; i < elements.size(); ++i)
    {
        std::cout << std::setw(4) << elements[i].get_art_index()<<"["<<elements[i].dsParent<<"]"<<endl;;
    }
    
    cout<<"---------------------------------------------------"<<endl;
}

inline bool
compareByParent(Element const& lhs, Element const& rhs)
{
    return lhs.dsParent < rhs.dsParent;
}

inline bool
compareByArtIndex(Element const& lhs, Element const& rhs)
{
    return lhs.get_art_index() < rhs.get_art_index();
}

void display_groups(vector<Element> elements, boost::disjoint_sets<Rank*, Parent*> sets){
    
    //^^^ do not pass elements by reference since the function sorts it!!!!! ^^^
    
    cout << "Found " << sets.count_sets(elements.begin(), elements.end()) << " groups" << endl;
    
    sets.compress_sets(elements.begin(), elements.end());
    std::sort(elements.begin(), elements.end(), compareByParent);
    
    size_t first = 0;
    while (first < elements.size())
    {
        size_t currentParent = elements.at(first).dsParent;
        size_t last = first;
        while (last < elements.size() && elements.at(last).dsParent == currentParent)
        {
            ++last;
        }
        std::cout << "\tRange: [" << first << "," << last << "). <dsParent = "<<currentParent<<">. Sorted elements: ";
        for (size_t i = first; i < last; ++i)
        {
            std::cout << elements.at(i).get_art_index() << " ";
        }
        std::cout << std::endl;
        first = last;
    }
    
}

void display_certain_groups(vector<Element>& elements_original, size_t& g1, size_t& g2, size_t& g3, size_t& g4){
//    map<size_t, vector<size_t> > g; //group_id, list of article_ids
    //create "map"
    vector<vector<size_t> > g;
    vector<size_t> emp;
    for(int i=0;i<elements_original.size();i++){
        g.push_back(emp);
    }
    
    size_t group = 0; //group id
    size_t artid = 0; //article id
    //store all articles in a group to a map
    for(int i=0;i<elements_original.size();i++){
        group = elements_original[i].dsParent;
        artid = elements_original[i].dsID;
        
        //store
        g[group].push_back(artid);
    }
    
    cout<<"Group "<<g1<<": ";
    for(int i=0;i<g[g1].size();i++){
        cout<<g[g1][i]<<" ";
    }
    cout<<endl;
    
    cout<<"Group "<<g2<<": ";
    for(int i=0;i<g[g2].size();i++){
        cout<<g[g2][i]<<" ";
    }
    cout<<endl;
    
    cout<<"Group "<<g3<<": ";
    for(int i=0;i<g[g3].size();i++){
        cout<<g[g3][i]<<" ";
    }
    cout<<endl;
    
    cout<<"Group "<<g4<<": ";
    for(int i=0;i<g[g4].size();i++){
        cout<<g[g4][i]<<" ";
    }
    cout<<endl;
    
    
}


////////////////////////////////////
//         Parse Function         //
////////////////////////////////////
void parse(string& s, vector<double>& parsed){
    parsed.clear();
    string tmp = "";
    for(int i=0; i<s.length();i++){
        if(s[i]==','){
            if (tmp.size()!=0 && tmp!=" "){
                parsed.push_back(stod(tmp));
            }
            
            tmp = "";
        }
        else{
            if(s[i]!= ' '){
                tmp+=s[i];
            }
        }
    }
    if (tmp.size()!=0 && tmp!=" "){
        parsed.push_back(stod(tmp));
    }
}

////////////////////////////////////
//      Create First Level        //
////////////////////////////////////
void create_first_level(std::vector<Element>& elements, vector<int>& prev_g, ofstream& outfile)
{
    // for (int i = 0; i < elements.size(); ++i){
    //     cout<<elements[i].dsParent<<" ";
    //  }
    //  cout<<endl;

    //initiate every group to -1
    vector<int> new_group(prev_g.size(), -1);
    
    for (int i = 0; i < elements.size(); ++i)
    {
        int new_group_id = elements[i].dsParent;
        int old_group_id = prev_g[i]; //group assignment from previous iteration
        
        //if the article has been merged
        if(new_group_id!=i){
            prev_g[i] = new_group_id; //update for next iteration
            new_group[i] = new_group_id;
            
            new_group[new_group_id] = new_group_id; //also update the assignment for the group this article is merged into
        }
        
    }
    
    //output
    for(int i=0;i<new_group.size()-1;i++){
        outfile<<new_group[i]<<", ";
    }
    outfile<<new_group[new_group.size()-1]<<endl;
    
    
}


void create_level(std::vector<Element>& elements, vector<int>& prev_g, ofstream& outfile)
{
     // for (int i = 0; i < elements.size(); ++i){
     //    cout<<elements[i].dsParent<<" ";
     // }
     // cout<<endl;

    vector<int> new_group(prev_g.size(), -1);
    for (int i = 0; i < elements.size(); ++i)
    {
        int new_group_id = elements[i].dsParent;
        int old_group_id = prev_g[i];
        if(new_group_id!=i){
            prev_g[i] = new_group_id;
            new_group[i] = new_group_id;
            
            prev_g[new_group_id] = new_group_id;
            new_group[new_group_id] = new_group_id;
        }
        //if the article has been merged before, update the group number
        else if(new_group[i]==-1 && old_group_id!=-1){
            new_group[i] = old_group_id;
            
        }
    }
    
    //output
    for(int i=0;i<new_group.size()-1;i++){
        outfile<<new_group[i]<<", ";
    }
    outfile<<new_group[new_group.size()-1]<<endl;
    
}



int main(int argc, const char * argv[])
{
    
    //==================== load inputs =====================
    if(argc < 6){
        cout<<"----------------------------------------------------------------"<<endl;
        cout<<"Too few arguments. Please provide the information in the following format. "<<endl;
        cout<<"Address_for_sorted_edge_list_file | Output_address | Minimum_Weight_to_Consider | Number_Articles | min_num_articles_to_merge"<<endl;
        cout<<"(e.g ./sorted_weight.txt ./ 3 33000 168)"<<endl;
        cout<<"----------------------------------------------------------------"<<endl;
        return 0;
    }
    string output_add = argv[2];
    string enc_address = output_add+"/group_ite.txt";
    double common_ed_limit = stod(argv[3]);
    int num_art = atoi(argv[4]);
    
    ofstream group_ite;
    group_ite.open(enc_address);

    string min_merge = argv[5];
    double min_num_articles_to_merge = stod(min_merge);
    // int min_num_articles_to_merge = 1;
    cout<<"We will merge at least "<<min_num_articles_to_merge<<" articles per level"<<endl;
    
    
    //-------------------------- Create Node for Each Article -----------------------------
    vector<Element> elements;
    vector<int> initial_group;
    
    elements.reserve(num_art);
    for (int i=0;i<num_art;i++){
        elements.push_back(Element(i));
        elements[i].dsID = i;
        initial_group.push_back(-1);
    }
    
    Rank rank(elements);
    Parent parent(elements);
    
    boost::disjoint_sets<Rank*, Parent*> sets(&rank, &parent);
    
    for (size_t i = 0; i < elements.size(); ++i)
    {
        sets.make_set(elements.at(i));
    }
    
    
    /*------------------------- Read In Edge List ---------------------------*/
    //local variables
    vector<double> parsed;
    double current_weight = -10.0;
    std::unordered_set<int> merged_articles;
    int num_article_merged = 0; // count number of articles merged for the current level
    
    //get edge list for weight,  article_pair (in descending order)
    ifstream edge_list;
    edge_list.open (argv[1]);
    
    string line = "";
    int c = 0;  //line counter
    bool first_lv = true;
    
    
    size_t g1 = 0;
    size_t g2 = 0;
    size_t new_g1 = 0;
    size_t new_g2 = 0;
    int num_lvs = 0;
    
    while(!edge_list.eof()){
        line = "";
        parsed.clear();
        getline(edge_list,line);
        if (!line.empty()){
            parse(line, parsed);
            if(c==0){
                current_weight = parsed[0];
            }
            
            //if the current weight is smaller than desired, stop grabbing edges <--------------
            if(parsed[0]<common_ed_limit) break;
            
            //grabbed enough edges, start creating the level (update group status in output file)
            if(parsed[0]!=current_weight && num_article_merged >= min_num_articles_to_merge){

                num_lvs++;
                num_article_merged = 0;
                sets.compress_sets(elements.begin(), elements.end());
                
                
                cout<<num_lvs<<" Creating level for weight "<<current_weight<<"..."<<endl;
                current_weight = parsed[0];
                
                if(first_lv == true){
                    first_lv = false;
                    create_first_level(elements, initial_group, group_ite);
                }
                else{
                    create_level(elements, initial_group, group_ite);
                }
                
            }

            // error check: index for the current article is out of range
            if (parsed[1]>=num_art){
                cerr<<"Detected article index ["<<parsed[1]<<"] out of range (number of article initialized = "<<num_art<<")"<<endl;
                exit(0);
            }
            if (parsed[2]>=num_art){
                cerr<<"Detected article index ["<<parsed[2]<<"] out of range (number of article initialized = "<<num_art<<")"<<endl;
                exit(0);
            }

            g1 = elements[parsed[1]].dsParent;
            g2 = elements[parsed[2]].dsParent;

            // count the number of newly merged articles of the level
            if (merged_articles.count(parsed[1])==0){
                merged_articles.insert(parsed[1]);
                num_article_merged++;
            }

            if (merged_articles.count(parsed[2])==0){
                merged_articles.insert(parsed[2]);
                num_article_merged++;
            }

            if(g1==g2){
                continue;
            }
            sets.union_set(elements[parsed[1]], elements[parsed[2]]);

            // cout<<"merging "<<parsed[1]<<" and "<<parsed[2]<<endl;
            // cout<<"[";
            // for (int i = 0; i < elements.size(); ++i){
            //     cout<<i<<" ";
            //  }
            //  cout<<"]"<<endl;
            // for (int i = 0; i < elements.size(); ++i){
            //     cout<<elements[i].dsParent<<" ";
            //  }
            //  cout<<endl;

        }
        c++;
    }
    edge_list.close();
    
    
    //Store last level
    sets.compress_sets(elements.begin(), elements.end());
    num_lvs++;

    cout<<"Creating level for weight "<<current_weight<<"..."<<endl;
    create_level(elements, initial_group, group_ite);

    cout<<"There are in total "<<num_lvs<<" levels in the tree"<<endl;
    
    return 0;
}


