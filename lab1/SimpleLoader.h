#pragma once
#include<list>
#include<string>
#include<vector>
#include<fstream>
#include<iostream>
using namespace std;


class Transaction {
public:
    int load(string str);
private:
    int transaction_no;
    vector<string> item_set;
    // ~Transaction(){
    //     item_set.clear();
    // }
};

vector<Transaction> readGroceryDataSet(string filename);
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