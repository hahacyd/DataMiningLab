#pragma once
#include<list>
#include<string>
#include<vector>
#include<fstream>
#include<map>
#include<set>
#include<iostream>
using namespace std;


class DataItem {
public:
    int load(string str); 
    int push_back(string item){
        item_set.push_back(item);
        return 1;
    }
    vector<string> item_set;
    private:
        int transaction_no;
        
        // ~Transaction(){
        //     item_set.clear();
        // }
};

vector<DataItem> readGroceryDataSet(string filename);
vector<string> get_comma_str(string& str);
vector<string> linesplit(string str, string delime);

class UserComandHistory{
public:
    int load(string str);
    int push_back(string str);
    friend ostream& operator<<(ostream& out, UserComandHistory& comandhistory);
private:
    vector<string> comands;
};
vector<UserComandHistory> readUnixUsageDataSet(string filename);
ostream& operator<<(ostream& out, UserComandHistory& comandhistory);