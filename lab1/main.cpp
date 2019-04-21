// #include"SimpleLoader.h"
#include"BaseStructure.h"
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<unistd.h>
using namespace std;
int main(int argv,char** argc)
{
    // string filepath = "dataset/UNIX_usage/USER0/sanitized_all.981115184025";
    string filepath = "dataset/GroceryStore/Groceries.csv";
    // string filepath = "dataset/GroceryStore/mytran.csv";
    if (argv > 1) {
        filepath = argc[1];
    }

    // DataBase database;

    GroceryDataBase grocerybase(filepath);
    cout << grocerybase.size();
    cout << " 个样例" << endl;
    grocerybase.Apriori(100);
    // grocerybase.buildFP_growthTree(200);
    // grocerybase.print_database();
    // grocerybase.FP_growth();
    // vector<UserComandHistory> record = readUnixUsageDataSet(filepath);

    return 1;
}