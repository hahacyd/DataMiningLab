#include"GroceryStore.h"
#include<sstream>
#include<fstream>
#include<iostream>
#include<string>
int GroceryDataBase::load(string filename)
{
    ifstream fp(filename.c_str());

    if (!fp.is_open()) { // check file whether or not exist
        cout << "打开文件 ";
        cout << filename;
        cout << "失败 " << endl;
    }
    string line;
    getline(fp, line); //the first line is nosense

    // line 样例: "2","{tropical fruit,yogurt,coffee}"
    CandidateKey item_set;
    while (getline(fp, line)) {
        // stringstream line_buffer(line);
        // DataItem one_transaction;
        // one_transaction.load(line);
        // database.push_back(one_transaction);

        unsigned int pos_start = 1, pos_end = 0; //因为 第一个字符肯定是 "

        pos_end = line.find("\"", pos_start);
        // transaction_no = stoi(line.substr(pos_start, pos_end - pos_start));
        pos_end += 4;
        pos_start = pos_end;
        pos_end = line.find("\"", pos_start);

        string substr = line.substr(pos_start, pos_end - pos_start - 1);

        istringstream s(substr);
        
        string item;
        while (getline(s, item, ',')) {
            item_set.push_back(item);
        }
        dataset.push_back(make_pair(item_set, 1));

        item_set.clear();
        // break;
    }

    fp.close();

    // for(auto&& i : dataset)
    // {
    //     for(auto&& j : i.first)
    //     {
    //         cout << j << " ";
    //     }
    //     cout << i.second << endl;
    // }

    
    //记录单一的项的出现次序
    for (auto&& i : dataset) {
        for (auto&& j : i.first) {
            initialize[j]++;
        }
    }
    // for(auto&& i : initialize)
    // {
    //     cout << i.first << " " << i.second << endl;
    // }
    // exit(-1);
    // 排序每个条目:
    for(auto&& i : dataset)
    {
        sortItem(i.first);
    }
    
    // 生成 1 频繁项集
    CandidateKey onetemp;
    for (auto&& i : initialize) {
        onetemp.push_back(i.first);
        frequent_one_set[onetemp] = i.second;
        onetemp.clear();
    }
}