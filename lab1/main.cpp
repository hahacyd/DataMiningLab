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
    // cout << sizeof(DataItem) << endl;
    // return 1;
    // vector<string> s = { "0","2","3","4" };
    // vector<string> m = { "0","2","3","4" };
    // cout << (s == m) << endl;
    // return 1;
    //     for (vector<string>::iterator it = m.begin(); it != m.end();it++) {
    //         if(*it == "2"){
    //             m.erase(it);
    //         }
    //     }

    //     for(auto&& i : m)
    //     {
    //         cout << i << " ";
    //     }
    // return 1;
    // cout << includes(s.begin(), s.end(), m.begin(), m.end()) << endl;
    // return 1;
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
    // cout << "输入要删除的字符:" << endl;
    // getline(cin, test);
    // for(auto&& i : testmap)
    // {
    //     if(i.first == "test"){
    //         testmap.erase(i.first);
    //     }
    // }
    //     for(auto&& i : testmap)
    // {
    //     cout << i.first;
    //     cout << " ";
    //     cout <<i.second << endl;
    // }
    // return 1;
    // string filepath = "dataset/UNIX_usage/USER0/sanitized_all.981115184025";
    string filepath = "dataset/GroceryStore/Groceries.csv";
    if (argv > 1) {
        filepath = argc[1];
    }

    // DataBase database;

    GroceryDataBase grocerybase(filepath);
    cout << grocerybase.size();
    cout << " 个样例" << endl;
    // grocerybase.Apriori(100);
    grocerybase.buildFP_growthTree();
    // vector<UserComandHistory> record = readUnixUsageDataSet(filepath);

    return 1;
}