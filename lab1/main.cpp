// #include"SimpleLoader.h"
#include"BaseStructure.h"
#include"GroceryStore.h"
#include"Unix_usage.h"
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<unistd.h>
#include<cstdlib>
#include<cstdio>
using namespace std;
int main(int argv,char** argc)
{
    //定义 挖掘哪个 Unix 用户的使用记录 
    string userno = "7";
    string unixfilepath
        = string("dataset/UNIX_usage/USER") + userno + string("/sanitized_all.981115184025");
    
    string grocery_filepath = "dataset/GroceryStore/Groceries.csv";
    // string filepath = "dataset/GroceryStore/mytran.csv";
    // string filepath = "dataset/GroceryStore/IBM.csv";
    string filepath = unixfilepath;
    if (argv > 1) {
        filepath = string(argc[1]);
    }
    // GroceryDataBase grocerybase(filepath);
    // grocerybase.Apriori(100);
    // grocerybase.printResult();

    UnixUserDataBase unixusage(filepath);
    // unixusage.Apriori(200);
    unixusage.FP_growth(100);

    unixusage.printResult();
    // vector<UserComandHistory> record = readUnixUsageDataSet(filepath);

    return 1;
}