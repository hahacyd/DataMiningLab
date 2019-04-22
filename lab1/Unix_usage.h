#pragma once
#include"BaseStructure.h"
#include<string>
using namespace std;

class UnixUserDataBase:public DataBase{
    public:
        int load(string filename);
        UnixUserDataBase(string filename){
            load(filename);
        }
};