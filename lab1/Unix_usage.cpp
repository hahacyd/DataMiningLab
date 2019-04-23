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
    if (!fp.is_open()) { // check file whether or not exist
        cout << "打开文件 ";
        cout << filename;
        cout << "失败 " << endl;
    }
    string line;
    bool start_user = 0;
    string paste;
    // bool connect = 0; //用于处理 | 符号
    while (getline(fp, line)) {
        if (line == string("**SOF**")) {
            start_user = 1;
            CandidateKey comand;
            while (getline(fp, line) && line != string("**EOF**")) {
                // if(connect == 1){
                //     paste = comand.back();
                //     comand.pop_back();
                //     paste =  paste + line;
                //     comand.push_back(paste);
                //     connect = 0;
                //     continue;
                // }
                if(line[0] == '<' || line[0] == '|' || line[0] == '&'||line[0] == '-'||line[0] == '>'){//|| line[0] == '|' || line[0] == '&'||line[0] == '-'||line[0] == '>'
                    // paste = line;
                    // if(line[0] == '|' || line[0] == '>'){
                    //     connect = 1;
                    // }
                    // paste = comand.back();
                    // comand.pop_back();
                    // paste =  paste + line;
                    // comand.push_back(paste);
                    continue;
                }
                comand.push_back(line);
            }
            // database.push_back(comand);
            dataset.push_back(make_pair(comand, 1));
            // cout << comand;
        }
        // break;
    }

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