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

class ItemTableElement;
class FPTreeNode;
class DataBase {
public:
//TODO: load 的项集 必须是已排序
    DataBase():fptree_root(nullptr){

    }
    virtual int load(string filename)=0;
    int size(){
        return database.size();
    }
    int apriori_gen(CandidateSet& L);   //
    bool has_infrequent_subset(CandidateKey cand, CandidateSet& container);
    int Apriori(int min_sup);
    int print(const CandidateSet&)const;
    int print(const vector<pair<string, int> >& candset) const;
    int buildFP_growthTree();

protected:
    CandidateSet frequent_one_set; //初始 集合
    vector<pair<string, int>> vfrequent_one_set;
    vector<DataItem> database;
    vector<ItemTableElement> item_table;
    FPTreeNode* fptree_root;
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
class FPTreeNode {
public:
    FPTreeNode()
        : supply(0)
        , item_name(){};

private:
    int supply; //每个节点的支持度
    string item_name;
};
class ItemTableElement {
public:
private:
    int supply;
    string item_name;
    vector<FPTreeNode*> fp_treenode_chains; //记录 FPTree_growth 树中相同 项的位置
};