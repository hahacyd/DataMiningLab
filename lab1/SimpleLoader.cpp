#include "SimpleLoader.h"
#include <algorithm>
#include <sstream>
using namespace std;
vector<Transaction> readGroceryDataSet(string filename)
{
    //read whole liens from filename
    vector<Transaction> record;

    ifstream fp(filename.c_str());

    if (!fp.is_open()) { // check file whether or not exist
        cout << "打开文件 ";
        cout << filename;
        cout << "失败 " << endl;
    }

    // cout << fp.is_open() << endl;
    string line;
    getline(fp, line); //the first line is nosense

    // line 样例: "2","{tropical fruit,yogurt,coffee}"
    while (getline(fp, line)) {
        // stringstream line_buffer(line);
        Transaction one_transaction;
        one_transaction.load(line);
        record.push_back(one_transaction);

        // break;
    }

    fp.close();

    return record;
}
int Transaction::load(string str)
{
    // TODO: read single line
    unsigned int pos_start = 1, pos_end = 0;  //因为 第一个字符肯定是 "

    pos_end = str.find("\"", pos_start);
    transaction_no = stoi(str.substr(pos_start, pos_end - pos_start));
    pos_end += 4;
    pos_start = pos_end;
    pos_end = str.find("\"", pos_start);

    string substr = str.substr(pos_start, pos_end - pos_start - 1);
    item_set = linesplit(substr, string(","));

    // for(auto&& i : items)
    // {
    //     cout << i <<" ";
    // }
    // cout << endl;
    return 1;
}
vector<string> linesplit(string str, string split_str)
{
    int i = 0;
    vector<string> items;

    unsigned int pos_start = 0, pos_end = 0;
    int length = str.length();

    pos_end = str.find(split_str, pos_end);
    while (pos_end <= length) {
        
        items.push_back(str.substr(pos_start, pos_end - pos_start));

        pos_end += 1;
        pos_start = pos_end;

        pos_end = str.find(split_str, pos_end);
    }
    items.push_back(str.substr(pos_start, length - pos_start));
    return items;
}
vector<UserComandHistory> readUnixUsageDataSet(string filename){
    vector<UserComandHistory> record;

    ifstream fp(filename.c_str());
    if (!fp.is_open()) { // check file whether or not exist
        cout << "打开文件 ";
        cout << filename;
        cout << "失败 " << endl;
    }
    string line;
    bool start_user = 0;
    while (getline(fp, line)) {
        if(line == string("**SOF**")){
            start_user = 1;
            UserComandHistory comand;
            while (getline(fp, line) && line != string("**EOF**")) {
                comand.push_back(line);
            }
            record.push_back(comand);
            cout << comand;
        }
        // break;
    }
    return record;
}
int UserComandHistory::push_back(string str){
    comands.push_back(str);
    return 1;
}
ostream& operator<<(ostream& out, UserComandHistory& comandhistory){
    for(auto&& i : comandhistory.comands)
    {
        out << i << " ";
    }
    out << std::endl;
}