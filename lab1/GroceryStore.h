#pragma once
#include"BaseStructure.h"

using namespace std;

class GroceryDataBase:public DataBase{
    public:
        int load(string filename);
        GroceryDataBase(string filename){
            load(filename);
            cout << "载入数据成功,共 " << dataset.size()<<" 条数据." <<endl;
        }

    private:
};
