#include<vector>
#include<list>
#include<map>
#include<string>
#include"SimpleLoader.h"
#include"debug.h"
using namespace std;

typedef set<string> FrequentItem ;
// typedef vector<string> CandidateKey;
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
    int addSupply(int support){
        supply += support;
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
    DataBase():min_sup(0),fptree_root(nullptr){
        fptree_root = new FPTreeNode("null");
    }
    virtual int load(string filename)=0;
    int size(){
        return database.size();
    }
    int apriori_gen(CandidateSet& L);   //
    bool has_infrequent_subset(CandidateKey cand, CandidateSet& container);
    int Apriori(int min_sup);

    int sortItem(CandidateKey& s);
    int buildFP_growthTree(int min_sup);
    int FP_growth(int support);
    int minFPtree(FPTreeNode* localroot, CandidateKey& alpha,vector<ItemTableElement>& item_table);
    bool checkOnePath(FPTreeNode* root);
    int buildFP_growthTree(FPTreeNode* node);
    int buildFPtree(FPTreeNode* node, CandidateKey::iterator item_iter,CandidateKey::iterator item_end,vector<ItemTableElement>& item_table,int supply);
    vector<ItemTableElement> createFPtree(FPTreeNode* node, vector<pair<CandidateKey,int>>& prefix_path);

    int addsibling(FPTreeNode* p, string& item_name);
    int addchild(FPTreeNode* p, string& item_name);
    
    int addItemAddress2ItemTable(string& item_name, FPTreeNode* address,vector<ItemTableElement>& item_table,int support);

    void destroyTree(FPTreeNode* root);
    int print(const CandidateSet&) const;
    int print(const vector<pair<string, int> >& candset) const;
    int print_database() const;
    void printtree(FPTreeNode* node, int layer);
    FPTreeNode* getRoot()
    {
        return fptree_root;
    }
 
protected:
    int min_sup;
    map<string, int> initialize;
    
    CandidateSet frequent_one_set; //初始 集合
    CandidateSet good_frequent_set; //最终集合,包含各个达到最小支持度 的频繁项集的集合
    list<pair<CandidateKey, int>> FPresult;
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
        : item_name(name),supply(0){};
    // private:
    int supply;
    string item_name;
    set<FPTreeNode*> fp_treenode_chains; //记录 FPTree_growth 树中相同 项的位置
};