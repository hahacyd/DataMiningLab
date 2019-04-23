#include"Unix_usage.h"
#include<sstream>
#include<fstream>
#include<iostream>
#include<string>
int UnixUserDataBase::load(string filename)
{
    //在第二次加载前 清空所有数据
    dataset.clear();
    destroyTree(fptree_root);

    ifstream fp(filename.c_str());
    if (!fp.is_open()) { // check the file whether or not exist
        cout << "打开文件 " << filename << "失败 " << endl;
    }
    string line;
    bool start_user = 0;
    string paste;
    while (getline(fp, line)) {
        if (line == string("**SOF**")) {
            start_user = 1;
            CandidateKey comand;
            while (getline(fp, line) && line != string("**EOF**")) {

                if(line[0] == '<' || line[0] == '|' || line[0] == '&'||line[0] == '-'||line[0] == '>'){
                    continue;
                }
                comand.push_back(line);
            }
            // database.push_back(comand);
            dataset.push_back(make_pair(comand, 1));
        }
    }

    //记录单一的项的出现次序
    for (auto&& i : dataset) {
        for (auto&& j : i.first) {
            initialize[j]++;
        }
    }
    // 排序每个条目 并 去重:
    for(auto&& i : dataset)
    {
        sortItem(i.first);
        i.first.unique();
    }

    // 生成 1 频繁项集
    CandidateKey onetemp;
    for (auto&& i : initialize) {
        onetemp.push_back(i.first);
        frequent_one_set[onetemp] = i.second;
        onetemp.clear();
    }
    return 1;
}