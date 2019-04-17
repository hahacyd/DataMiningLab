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
    int no;
    vector<string> item_set;
    // ~Transaction(){
    //     item_set.clear();
    // }
};

vector<Transaction> readDataSet(string filename);