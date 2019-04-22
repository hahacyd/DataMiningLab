#include "BaseStructure.h"
#include "debug.h"
#include <algorithm>
#include <map>
#include <stdio.h>
#include <sstream>
// using namespace std;
/**
 * 1 使用迭代的方法基于候选 找出频繁项集
 */ 
int DataBase::apriori_gen(CandidateSet& L)
{
    // 自动在 L 中的 k - 1 频繁项集 自连接 生成 k 项频繁项集
    // assert(L.size() > 0);
    int length = L.begin()->first.size();
    CandidateKey intersect_result, union_result;
    CandidateSet candidate_set;
    bool same = 0;
    // 连接步
    for (auto&& i : L) {
        for (auto&& j : L) {
            if(i.first == j.first){
                continue;
            }
            intersect_result.clear();
            union_result.clear();

            CandidateKey::const_iterator it1_begin = i.first.begin(),
                                         it1_end = i.first.end(),
                                         it2_begin = j.first.begin(),
                                         it2_end = j.first.end();
            set_intersection(i.first.begin(), i.first.end(), j.first.begin(), j.first.end(), std::back_inserter(intersect_result));

            if (intersect_result.size() >= length - 1) { //&& *(it1_end - 1) < *(it2_end - 1)
                // 这里 candidate 中的项集 中的项自然是有序的，这是 set_union 所决定的
                set_union(i.first.begin(), i.first.end(), j.first.begin(), j.first.end(), std::back_inserter(union_result));
                sortItem(union_result);
                candidate_set[union_result] = 0;
            }
        }
    }
    // 剪枝步
    int count_cut = 0;
    for (auto&& i : candidate_set) {

        if (has_infrequent_subset(i.first, L)) {
            count_cut += candidate_set.erase(i.first);
        }
    }
    // print(candidate_set);
    // exit(-1);
    L.swap(candidate_set); // 交换 输入集合 与 输出集合,减少返回时的 复制开销
    return length;
}
/**
 * cond 是一个 k 项集，如果其中任意 k - 1项不在 集合 container 中，则返回true,ps: contain 是一个 k 项集 集合
 */
bool DataBase::has_infrequent_subset(CandidateKey cand, CandidateSet& container)
{
    //TODO: 判断某一个 自连接 后的候选项集 是否 在原来的候选项集中
    int item_num = cand.size();

    // 假设 cand 是一个k 项集，那么要产生 k - 2 个 k - 1项集
    CandidateKey candkey;
    candkey.assign(cand.begin(), cand.end());
    CandidateKey temp = candkey;

    // CandidateKey::iterator it = 
    for (auto&& i : candkey) {
        temp.erase(find(temp.begin(), temp.end(), i));
        bool spaceX = 0;
        for (auto&& j : container) {
            if (includes(j.first.begin(), j.first.end(), temp.begin(), temp.end())) {
                spaceX = 1;
            }
        }
        if (spaceX == 0) { //说明 原始数据中没有 这个频繁项，返回 不存在
            return 1;
        }
        temp.clear();
        temp.assign(candkey.begin(), candkey.end());
    }
    return 0;
}
int DataBase::Apriori(int min_sup)
{

    CandidateSet frequent_set = frequent_one_set;
    for (int k = 1; frequent_set.size() != 0; k++) {
        int test = apriori_gen(frequent_set);
        // assert(k == test);
        // 检验 apriori_gen 产生的候选项集是否在 原始数据集中，
        float progress_count = 0.0;
        float progress_length = dataset.size();
        if (progress_length == 0)
            progress_length = 1;

        // 用于打印 执行 进度
        printf("正在挖掘 %d 项集 : %.2f %%",k+1, 100 * progress_count / progress_length);

        for (auto&& j : dataset) { //从 数据库 中筛查 候选项集 frequent_set 中每个频繁项集的出现次数,
            progress_count += 1;
            printf("\r");
            printf("正在挖掘 %d 项集 : %.2f %%",k+1, 100 * progress_count / progress_length);
            fflush(stdout);
            for (auto&& i : frequent_set) {
                if (includes(j.first.begin(), j.first.end(), i.first.begin(), i.first.end())) {
                    frequent_set[i.first] += 1;
                }
            }
        }
        printf(" 完成 \n");
        //将 frequent_set 中小于 最小支持度的 频繁项集 删除
        for (CandidateSet::iterator it = frequent_set.begin(); it != frequent_set.end();) {
            // int freq = it->second;
            if (it->second < min_sup) {
                it = frequent_set.erase(it);
            } else {
                it++;
            }
        }

        print(frequent_set);
        // printf(" %d 项集收集完毕 \n", k + 1);
        good_frequent_set.insert(frequent_set.begin(), frequent_set.end());

        progress_count = 0;
    }
    return 1;
}

int DataBase::sortItem(CandidateKey& s)
{
    s.sort([&](string& s1, string& s2) -> bool { return (initialize[s1] > initialize[s2])
                                                     || (initialize[s1] == initialize[s2] && s1 < s2); });
};
int DataBase::createFPtree(FPTreeNode* node, list<pair<CandidateKey, int>>& prefix_path,vector<ItemTableElement>& headpoint_table)
{
    // assert(node != nullptr);
    // vector<ItemTableElement> local_itemtable;
    for (auto&& i : prefix_path) {
        // 调用此程序来将每个 Trans 加入到FP 树中
        // CandidateKey s = i.first;
        buildFPtree(node, i.first.begin(), i.first.end(), headpoint_table, i.second);
    }
    return 1;
}
int DataBase::FP_growth(int support)
{
    this->min_sup = support;

    int deletecount = 0;
    // 将数据库中支持度过小的项删除 并把保留下的项排序
    CandidateKey::iterator it1;
    list<pair<CandidateKey,int>>::iterator datait1 = dataset.begin();
    for (; datait1 != dataset.end();) {
        for (it1 = datait1->first.begin(); it1 != datait1->first.end();) {
            if (initialize[*it1] < support) {
                it1 = datait1->first.erase(it1);
                deletecount++;
            } else {
                it1++;
            }
        }
        
        sortItem(datait1->first);  // 对数据集中的项 按出现次数从大到小排序

        if (datait1->first.size() < 1) {
            datait1 = dataset.erase(datait1);
        } else {
            datait1++;
        }
    }

    // 创建 FP 树
    // buildFP_growthTree(support);
    createFPtree(this->fptree_root, dataset,this->item_table);
    // printtree(fptree_root, 0);
    // cout << "结束" << deletecount <<"数据集大小 "<<dataset.size() <<endl;
    // return 1;

    // assert(fptree_root != nullptr);

    // 挖掘 FP 树
    CandidateKey backfix;
    minFPtree(fptree_root, backfix, this->item_table);

    destroyTree(fptree_root);
    fptree_root = nullptr;

    
    return 1;
}
void DataBase::destroyTree(FPTreeNode* root)
{
    FPTreeNode* s1 = root;

    if (root->child != nullptr) {
        destroyTree(s1->child);
    }
    if (s1->sibling != nullptr) {
        destroyTree(s1->sibling);
    }

    delete s1;
    // root = nullptr;
}
static list<pair<CandidateKey, int>> prefix_path; //获取前缀路径

int DataBase::minFPtree(FPTreeNode* localroot, CandidateKey& alpha, vector<ItemTableElement>& item_table)
{
    // assert(localroot != nullptr);
    // if(item_table.size() == 1 && alpha.size() >= 1 && item_table.begin()->supply >= min_sup){
    //     alpha.push_front(item_table.begin()->item_name);
    //     good_frequent_set[alpha] = item_table.begin()->supply;
    //     alpha.pop_front();
    //     return 1;
    // }
    // if (localroot->child == nullptr) {
    //     return 1;
    // }
    //对 item_table 排序
    sort(item_table.begin(),
       item_table.end(), [](ItemTableElement s1, ItemTableElement s2) -> bool { return s1.supply < s2.supply; });

    //progress bar
    float progress_count = 0.0;
    float progress_length = item_table.size();
    if (localroot == fptree_root) {

        if (progress_length == 0)
            progress_length = 1;
        printf("%.2f %%", 100 * progress_count / progress_length);
    }

    // 遍历 树 localroot 中的项头表，为每一项都生成它的 条件模式基
    for (auto&& i : item_table) {
        if (localroot == fptree_root) {
            progress_count += 1;
            printf("\r");
            printf("%.2f %%", 100 * progress_count / progress_length);
            fflush(stdout);
        }

        alpha.push_front(i.item_name);//这是性能考量，对于vector,在前面插入数据太慢了
        // alpha.insert(alpha.begin(), i.item_name);

        if (alpha.size() >= 2 && i.supply >= min_sup) {
            good_frequent_set[alpha] = i.supply;
            // FPresult.push_back(make_pair(alpha, i.supply));
        }

        CandidateKey conditem_name;

        FPTreeNode* it = nullptr;
        for (auto&& j : i.fp_treenode_chains) {
            // 找出 某个头表项的每一条路径
            it = j->parent;
            for (; it->getItemName() != "null"; it = it->parent) {
                // conditem_name.insert(conditem_name.begin(), it->getItemName());
                conditem_name.push_front(it->getItemName());
            }
            // 如果对某个项来说 它就是离根最近的节点，那么就忽略
            if (conditem_name.size() < 1) {
                continue;
            }
            // sortItem(conditem_name);
            // prefix_path[conditem_name] = j->getSupply();
            prefix_path.push_back(make_pair(conditem_name, j->getSupply()));
            conditem_name.clear();
        }

        //此时获得一个项的所有条件模式基 prefix_path,再给它创建一个条件模式树 和 这个树的项头表
        if (prefix_path.size() > 0) {
            FPTreeNode* condroot = new FPTreeNode("null");
            vector<ItemTableElement> local_itemtable;
            createFPtree(condroot, prefix_path,local_itemtable);
            prefix_path.clear(); //建完树就没用了
            if (local_itemtable.size() > 0) {
                minFPtree(condroot, alpha, local_itemtable);
            }
            // 树挖掘完毕后 回收空间，
            destroyTree(condroot);
        }
        // alpha.erase(alpha.begin());
        alpha.pop_front();
    }
    if (localroot == fptree_root) {
        printf("\n");
    }
    return 1;
}
/** 这是 将要递归的程序:
 * @param node: 当前的树节点(位置)
 * @param item: 处理 Trans 的一个 item
 */
int DataBase::buildFPtree(FPTreeNode* node, CandidateKey::iterator item_iter, CandidateKey::iterator item_end, vector<ItemTableElement>& item_table, int supply)
{
    // assert(node != nullptr);
    if (item_iter == item_end) { //某一个 Trans 的所有项都处理完成了
        return 1;
    }

    // assert(item_iter != item_end);
    FPTreeNode* child = node->child;
    if (child == nullptr) {
        // node->child = new FPTreeNode(*item_iter);
        addchild(node, *item_iter);
        node->child->addSupply(supply);

        addItemAddress2ItemTable(*item_iter, node->child, item_table, supply);

        buildFPtree(node->child, ++item_iter, item_end, item_table, supply);
    } else { // 在node 的孩子节点上找 到与当前项(item)相同的节点
        // vector<string>::iterator it = item_iter;
        FPTreeNode *sibling = child, *cur = child;
        //在这一层查找 符合条件的节点
        while (sibling != nullptr && sibling->getItemName() != *item_iter) {
            cur = sibling;
            sibling = sibling->sibling;
        }
        if (sibling == nullptr) { // 说明这一层没有对应的节点，cur 是这一层最后一个节点, 应该再建一个兄弟节点
            addsibling(cur, *item_iter);

            // assert(cur->sibling != nullptr);
            cur->sibling->addSupply(supply);
            addItemAddress2ItemTable(*item_iter, cur->sibling, item_table, supply);

            buildFPtree(cur->sibling,++item_iter, item_end, item_table, supply);
        } else { //这一层找到了对应的节点
            sibling->addSupply(supply);
            addItemAddress2ItemTable(*item_iter, sibling, item_table, supply);
            buildFPtree(sibling, ++item_iter, item_end, item_table, supply);
        }
    }
    return 1;
}
void DataBase::printtree(FPTreeNode* node, int layer)
{
    if (node == nullptr) {
        return;
    }
    // assert(node != nullptr);

    for (int i = 0; i < layer; i++) {
        cout << " ";
    }
    cout << node->getItemName() << "(" << node->getSupply() << ")" << endl;

    FPTreeNode* it = node->child;
    for (; it != nullptr; it = it->sibling) {
        printtree(it, layer + 2);
    }
    // cout << endl;
}
int DataBase::addsibling(FPTreeNode* p, string& item_name)
{
    // assert(p->sibling == nullptr);
    p->sibling = new FPTreeNode(item_name);

    p->sibling->parent = p->parent;
    return 1;
}
int DataBase::addchild(FPTreeNode* p, string& item_name)
{
    // assert(p->child == nullptr);
    p->child = new FPTreeNode(item_name);

    p->child->parent = p;
    return 1;
};

int DataBase::addItemAddress2ItemTable(string& item_name, FPTreeNode* address, vector<ItemTableElement>& item_table, int support)
{
    for (auto&& i : item_table) {
        if (i.item_name == item_name) {
            // i.fp_treenode_chains.push_back(address);
            i.fp_treenode_chains.insert(address);
            i.supply += support;
            return 1;
        }
    }
    // 执行 到这里 说明 项头表中没有 这个 名为 item_name 的项
    item_table.push_back(ItemTableElement(item_name));
    item_table.rbegin()->fp_treenode_chains.insert(address);
    item_table.rbegin()->supply += support;
    return 1;
}
int DataBase::print(const CandidateSet& candset) const
{
    for (auto&& i : candset) {
        for (auto&& j : i.first) {
            cout << j << ",";
        }
        cout << " ";
        cout << i.second << endl;
    }
    return 1;
}
int DataBase::print(const vector<pair<string, int>>& candset) const
{
    for (auto&& i : candset) {
        cout << i.first;
        cout << " ";
        cout << i.second << endl;
    }
    return 1;
}
void DataBase::printResult(){
    print(good_frequent_set);
}