#include<vector>
#include<list>
#include<map>
#include<string>
#include"SimpleLoader.h"
#include"debug.h"
using namespace std;

typedef set<string> FrequentItem ;
typedef map<set<string>, int> CandidateSet;

class DataBase {
public:
    virtual int load(string filename)=0;
    int size(){
        return database.size();
    }
    CandidateSet apriori_gen(CandidateSet& L);   //
    bool has_infrequent_subset();
    int Apriori();
    int print(const CandidateSet&)const;

protected:
    vector<DataItem> database;
};

class GroceryDataBase:public DataBase{
    public:
        int load(string filename);
        GroceryDataBase(string filename){
            load(filename);
        }
    private:
};
class UnixUserDataBase:public DataBase{
    public:
        int load(string filename);
        UnixUserDataBase(string filename){
            load(filename);
        }
};