#include<vector>
#include<list>
#include<map>
#include<string>
#include"SimpleLoader.h"
#include"debug.h"
using namespace std;

typedef set<string> FrequentItem ;
typedef vector<string> CandidateKey;
typedef map<CandidateKey, int> CandidateSet;

class DataBase {
public:
//TODO: load 的项集 必须是已排序
    virtual int load(string filename)=0;
    int size(){
        return database.size();
    }
    int apriori_gen(CandidateSet& L);   //
    bool has_infrequent_subset(CandidateKey cand, CandidateSet& container);
    int Apriori(int min_sup);
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