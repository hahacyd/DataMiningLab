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
class FPTreeNode {
public:
    FPTreeNode(string name)
        : supply(0)
        , item_name(name){
        child = nullptr;
        sibling = nullptr;
        parent = nullptr;
    };
    // FPTreeNode():supply(0),item_name(){
    //     child = nullptr;
    //     sibling = nullptr;
    //     parent = nullptr; 
    // }
    int addSupply(){
        supply++;
    }
    int getSupply(){
        return supply;
    }
    string getItemName(){
        return item_name;
    }
    FPTreeNode *child, *sibling,*parent;

private:
    int supply; //每个节点的支持度
    string item_name;
};
class DataBase {
public:
//TODO: load 的项集 必须是已排序
    DataBase():fptree_root(nullptr){
        fptree_root = new FPTreeNode("null");
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

    int FP_growth();
    int FP_growth_subprocess(FPTreeNode* localroot, CandidateKey alpha);
    bool checkOnePath(FPTreeNode* root);
    int buildFP_growthTree(FPTreeNode* node);
    int buildFP_growthTree_SubProcess(FPTreeNode* node, vector<string>::iterator item_iter,vector<string>::iterator item_end);
    int addsibling(FPTreeNode* p, string& item_name);
    int addchild(FPTreeNode* p, string& item_name);
    void printtree(FPTreeNode* node, int layer);
    int addItemAddress2ItemTable(string& item_name, FPTreeNode* address);

    FPTreeNode* getRoot(){
        return fptree_root;
    }

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

class ItemTableElement {
public:
    ItemTableElement(string name)
        : item_name(name),supply(1){};
    // private:
    int supply;
    string item_name;
    set<FPTreeNode*> fp_treenode_chains; //记录 FPTree_growth 树中相同 项的位置
};