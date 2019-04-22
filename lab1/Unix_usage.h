#pragma once
#include"BaseStructure.h"
#include<string>
using namespace std;

class UnixUserDataBase:public DataBase{
    public:
        int load(string filename);
        UnixUserDataBase(string filename){
            try{
                load(filename);
            }
            catch(exception& e){
                cout << e.what() << endl;
                cout << "最后一组数据如下:" << endl;
                for(auto&& i : dataset.back().first)
                {
                    cout << i << " ";
                }
                cout << endl;
            }
            cout << "数据读取成功,共 " << dataset.size() << " 个" << endl;
        }
};