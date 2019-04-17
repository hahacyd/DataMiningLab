#include"SimpleLoader.h"
#include<string>
#include<vector>
#include<list>
using namespace std;
int main()
{
    // string filepath = "dataset/GroceryStore/Groceries.csv";
    // vector<Transaction> record = readGroceryDataSet(filepath);

    string filepath = "dataset/UNIX_usage/USER1/sanitized_all.981115184025";
    vector<UserComandHistory> record = readUnixUsageDataSet(filepath);
    
    cout << "共计 :";
    cout << record.size() << endl;

    return 1;
}