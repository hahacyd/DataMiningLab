#include"SimpleLoader.h"
#include<string>
#include<vector>
#include<list>
using namespace std;
int main()
{
    string filepath = "dataset/GroceryStore/Groceries.csv";
    // string filepath = "test.txt";
    vector<Transaction> record = readDataSet(filepath);
    cout << record.size() << endl;
    return 1;
}