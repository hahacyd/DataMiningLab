#pragma once
#include<list>
#include<string>
#include<vector>
#include<fstream>
#include<map>
#include<set>
#include<iostream>
using namespace std;

typedef list<string> CandidateKey;
// typedef list<string> CandidateKey;


class DataItem {
public:
    int load(string str); 
    int push_back(string item){
        item_set.push_back(item);
        return 1;
    }
    friend ostream& operator<<(ostream& out, DataItem& item);
    CandidateKey item_set;
    private:
        int transaction_no;
        
        // ~Transaction(){
        //     item_set.clear();
        // }
};
ostream& operator<<(ostream& out, DataItem& item);
vector<DataItem> readGroceryDataSet(string filename);
CandidateKey get_comma_str(string& str);
CandidateKey linesplit(string str, string delime);

class UserComandHistory{
public:
    int load(string str);
    int push_back(string str);
    friend ostream& operator<<(ostream& out, UserComandHistory& comandhistory);
private:
    CandidateKey comands;
};
vector<UserComandHistory> readUnixUsageDataSet(string filename);
ostream& operator<<(ostream& out, UserComandHistory& comandhistory);