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
    string userno = "2";
    string unixfilepath
        = string("dataset/UNIX_usage/USER") + userno + string("/sanitized_all.981115184025");
    
    string grocery_filepath = "dataset/GroceryStore/Groceries.csv";
    string filepath = unixfilepath;
    if (argv > 1) {
        filepath = string(argc[1]);
    }

    //TODO: 在新建对象时指定路径
    // GroceryDataBase grocerybase(filepath);
    
    
    // TODO: 在调用算法接口时 指定支持度
    // grocerybase.FP_growth(100);
    // grocerybase.Apriori(100);
    // grocerybase.printResult();

    UnixUserDataBase unixusage(filepath);
    unixusage.Apriori(100);
    // unixusage.FP_growth(100);

    unixusage.printResult();
    return 1;
}