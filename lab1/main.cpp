// #include"SimpleLoader.h"
#include"BaseStructure.h"
#include<string>
#include<vector>
#include<list>
using namespace std;
int main(int argv,char** argc)
{
    // cout << argc[0] << endl;
    // map<string, int> testmap;
    // string test;
    // while(getline(cin,test) && test != "quit"){
    //     testmap[test]++;
    // }
    // for(auto&& i : testmap)
    // {
    //     cout << i.first;
    //     cout << " ";
    //     cout <<i.second << endl;
    // }

    // string filepath = "dataset/UNIX_usage/USER0/sanitized_all.981115184025";
    string filepath = "dataset/GroceryStore/Groceries.csv";
    if (argv > 1) {
        filepath = argc[1];
    }

    // DataBase database;

    GroceryDataBase grocerybase(filepath);
    cout << grocerybase.size() << endl;
    grocerybase.Apriori();
    // vector<UserComandHistory> record = readUnixUsageDataSet(filepath);

    return 1;
}