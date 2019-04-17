#include"SimpleLoader.h"
#include<sstream>
using namespace std;
vector<Transaction> readDataSet(string filename)
{
    //read whole liens from filename
    vector<Transaction> record;

    ifstream fp(filename.c_str());
    
    if(!fp.is_open()){ // check file whether or not exist
        cout << "打开文件 ";
        cout << filename;
        cout<<"失败 " << endl;
    }

    // cout << fp.is_open() << endl;
    string line;
    getline(fp, line); //the first line is nosense

    // line 样例: "2","{tropical fruit,yogurt,coffee}"
    while(getline(fp,line)){
        // stringstream line_buffer(line);
        Transaction one_transaction;
        one_transaction.load(line);
        record.push_back(one_transaction);
    }

    fp.close();

    return record;
}
int Transaction::load(string str){
// TODO: read single line 


    return 1;
}