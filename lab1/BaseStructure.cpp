#include "BaseStructure.h"
#include "SimpleLoader.h"
#include "debug.h"
#include <algorithm>
#include <map>
#include <stdio.h>
// using namespace std;

int GroceryDataBase::load(string filename)
{
    ifstream fp(filename.c_str());

    if (!fp.is_open()) { // check file whether or not exist
        cout << "打开文件 ";
        cout << filename;
        cout << "失败 " << endl;
    }

    string line;
    getline(fp, line); //the first line is nosense

    // line 样例: "2","{tropical fruit,yogurt,coffee}"
    while (getline(fp, line)) {
        // stringstream line_buffer(line);
        DataItem one_transaction;
        one_transaction.load(line);
        database.push_back(one_transaction);

        // break;
    }

    fp.close();

    // 生成 1 频繁项集

    for (auto&& i : database) {
        for (auto&& j : i.item_set) {
            initialize[j]++;
        }
    }
    vector<string> onetemp;
    for (auto&& i : initialize) {
        onetemp.push_back(i.first);
        frequent_one_set[onetemp] = i.second;
        onetemp.clear();
    }
}
int UnixUserDataBase::load(string filename)
{
    // vector<UserComandHistory> record;

    ifstream fp(filename.c_str());
    if (!fp.is_open()) { // check file whether or not exist
        cout << "打开文件 ";
        cout << filename;
        cout << "失败 " << endl;
    }
    string line;
    bool start_user = 0;
    while (getline(fp, line)) {
        if (line == string("**SOF**")) {
            start_user = 1;
            DataItem comand;
            while (getline(fp, line) && line != string("**EOF**")) {
                comand.push_back(line);
            }
            database.push_back(comand);
            // cout << comand;
        }
        // break;
    }
    return 1;
}
/**
 * 1 使用迭代的方法基于候选 找出频繁项集
 */
int DataBase::apriori_gen(CandidateSet& L)
{
    // 自动在 L 中的 k - 1 频繁项集 自连接 生成 k 项频繁项集
    assert(L.size() > 0);
    int length = L.begin()->first.size();
    CandidateKey intersect_result, union_result;
    CandidateSet candidate_set;
    bool same = 0;
    // 连接步
    for (auto&& i : L) {
        for (auto&& j : L) {
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
    // CandidateKey::iterator cand_itend = cand.end() - 2,cand_itbegin = cand.begin();
    // cand.erase()
    vector<string> candkey;
    candkey.assign(cand.begin(), cand.end());
    vector<string> temp = candkey;

    // vector<CandidateKey> alternativeSet;

    for (auto&& i : candkey) {
        temp.erase(find(temp.begin(), temp.end(), i));
        bool spaceX = 0;
        for (auto&& j : container) {
            if (includes(j.first.begin(), j.first.end(), temp.begin(), temp.end())) {
                spaceX = 1;
            }
        }
        if (spaceX == 0) {
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
        assert(k == test);
        // 检验 apriori_gen 产生的候选项集是否在 原始数据集中，
        float progress_count = 0.0;
        float progress_length = database.size();
        if (progress_length == 0)
            progress_length = 1;

        // 用于打印 执行 进度
        printf("%.2f %%", 100 * progress_count / progress_length);

        for (auto&& j : database) { //从 数据库 中筛查 候选项集 frequent_set 中每个频繁项集的出现次数,
            progress_count += 1;
            printf("\r");
            printf("%.2f %%", 100 * progress_count / progress_length);
            fflush(stdout);
            for (auto&& i : frequent_set) {
                if (includes(j.item_set.begin(), j.item_set.end(), i.first.begin(), i.first.end())) {
                    frequent_set[i.first] += 1;
                }
            }
        }

        printf("\n");
        //将 frequent_set 中小于 最小支持度的 频繁项集 删除
        CandidateSet::iterator ittemp;
        for (CandidateSet::iterator it = frequent_set.begin(); it != frequent_set.end();) {
            int freq = it->second;
            if (it->second < min_sup) {
                ittemp = it;
                it++;
                frequent_set.erase(ittemp);
            } else {
                it++;
            }
        }

        print(frequent_set);
        printf(" %d 项集收集完毕 \n", k + 1);
        good_frequent_set.insert(frequent_set.begin(), frequent_set.end());

        progress_count = 0;
    }
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
int DataBase::buildFP_growthTree(int min_sup)
{
    this->min_sup = min_sup;
    buildFP_growthTree(getRoot());
    // printtree(fptree_root,0);
}
/**以下是 FPgrowth 算法的实现;
*/
int DataBase::buildFP_growthTree(FPTreeNode* node)
{
    assert(node != nullptr && min_sup != 0);

    CandidateKey::iterator it1, it2;

    vector<DataItem>::iterator datait2 = database.begin(),datait1 = datait2;
    for (; datait1 != database.end();) {

        for (it1 = datait1->item_set.begin(); it1 != datait1->item_set.end();) {
            if (initialize[*it1] < min_sup) {
                it1 = datait1->item_set.erase(it1);
            }
            else{
                it1++;
            }
        }
        if(datait1->item_set.size() < 1){
            datait1 = database.erase(datait1);
        }
        else{
            datait1++;
        }
    }
    for (auto&& i : database) {
        // 先 将 database 中的Trans的 不满足最小支持度的条目删除

        // sort(i.item_set.begin(),
        //     i.item_set.end(),
        //     [&](string& s1, string& s2) -> bool { return (initialize[s1] > initialize[s2])|| (initialize[s1] == initialize[s2] && s1 < s2) ; });//|| (initialize[s1] == initialize[s2] && s1 < s2)
        sortItem(i.item_set);
        // 调用此程序来将每个 Trans 加入到FP 树中
        buildFP_growthTree_SubProcess(node, i.item_set.begin(), i.item_set.end(), this->item_table);
    }
    cout << "建树完成" << endl;
}
int DataBase::sortItem(CandidateKey& s){
    sort(s.begin(),
            s.end(),
            [&](string& s1, string& s2) -> bool { return (initialize[s1] > initialize[s2])|| (initialize[s1] == initialize[s2] && s1 < s2) ; });//|| (initialize[s1] == initialize[s2] && s1 < s2)
};
int DataBase::buildcondTree(FPTreeNode* node, CandidateSet& condpat, vector<ItemTableElement>& local_itemtable)
{
    assert(node != nullptr);
    for (auto&& i : condpat) {
        //sort(i.item_set.begin(),
        //    i.item_set.end(),
        //   [&](string& s1, string& s2) -> bool { return initialize[s1] > initialize[s2]; });
        // 调用此程序来将每个 Trans 加入到FP 树中
        CandidateKey s = i.first;
        buildFP_growthTree_SubProcess(node, s.begin(), s.end(), local_itemtable);
    }
    // cout << "建条件模式树完成" << endl;
}
int DataBase::FP_growth()
{
    if (fptree_root == nullptr) {
        buildFP_growthTree(fptree_root);
    }

    //TODO: 需要完成 FP-growth 树的挖掘
    assert(fptree_root != nullptr);

    CandidateKey test;
    // test.push_back("null");
    FP_growth_subprocess(fptree_root, test, this->item_table);

    CandidateSet::iterator it = good_frequent_set.begin();
    for (; it != good_frequent_set.end();) {
        if(it->second < min_sup){
            it = good_frequent_set.erase(it);
        }
        else{
            it++;
        }
    }
    // for(auto i : good_frequent_set)
    // {
    //     if(i.second < min_sup){
    //         good_frequent_set.erase(i.first);
    //     }
    // }
    print(good_frequent_set);
    return 1;
}
int DataBase::FP_growth_subprocess(FPTreeNode* localroot, CandidateKey alpha, vector<ItemTableElement>& item_table)
{
    assert(localroot != nullptr);
    if(localroot->child == nullptr){
        return 1;
    }
    //对 item_table 排序
    sort(item_table.begin(),
        item_table.end(), [](ItemTableElement s1, ItemTableElement s2) -> bool { return s1.supply < s2.supply; });
    // for(auto&& i : item_table)
    // {
    //     cout << i.item_name << " " << i.supply << endl;

    // }
    // return 1;
    // if (checkOnePath(localroot)) {
    //     // tree 包含单个路径
    //     //去这个路径上的每个组合，与 alpha 取并，其支持度是这个组合中的各项支持度的最小值
    //     cout << "welcome " << alpha.size()<<endl;
    //     // printtree(localroot, 0);
    //     // for (auto&& i : item_table) {
    //     //     cout << i.item_name << " " << i.supply << endl;
    //     // }
    //     // for(auto&& i : alpha)
    //     // {
    //     //     cout << i << endl;
    //     // }
    //     return 1;
    // } else {
        // 遍历 树 localroot 中的项头表，为每一项都生成它的 条件模式基
        float progress_count = 0.0;
        float progress_length = item_table.size();
        if (localroot == fptree_root) {

            if (progress_length == 0)
                progress_length = 1;
            printf("%.2f %%", 100 * progress_count / progress_length);
        }

        for (auto&& i : item_table) {
            if (localroot == fptree_root) {
                progress_count += 1;
                printf("\r");
                printf("%.2f %%", 100 * progress_count / progress_length);
                fflush(stdout);
            }

            alpha.insert(alpha.begin(), i.item_name);
            if (alpha.size() >= 2) {
                good_frequent_set[alpha] = i.supply;
            }

            CandidateSet condpat; //获取前缀路径
            CandidateKey conditem_name;

            for (auto&& j : i.fp_treenode_chains) {
                conditem_name.clear();
                FPTreeNode* it = j->parent;
                for (; it->getItemName() != "null"; it = it->parent) {
                    conditem_name.insert(conditem_name.begin(), it->getItemName());
                }
                // 得到的 conditem_name 默认是已排好序的
                if (conditem_name.size() > 0) {
                    sortItem(conditem_name);
                    condpat[conditem_name] += j->getSupply();
                }
            }
            // cout << endl<<i.item_name << endl;
            // print(condpat);
            // break;
            //此时获得一个项的所有条件模式基 condpat,再给它创建一个条件模式树 和 这个树的项头表
            if(condpat.size() > 0){
                FPTreeNode* condroot = new FPTreeNode("null");
                vector<ItemTableElement> local_itemtable;
                buildcondTree(condroot, condpat, local_itemtable);
                if(local_itemtable.size() > 0){
                    FP_growth_subprocess(condroot, alpha, local_itemtable);
                }
            }
            alpha.erase(alpha.begin());
            // break;
        }
        if (localroot == fptree_root) {
            printf("\n");
        }
    // }
}
bool DataBase::checkOnePath(FPTreeNode* root)
{
    assert(root != nullptr);
    for (; root != nullptr; root = root->child) {
        if (root->sibling != nullptr) {
            return false;
        }
    }
    return true;
}
/** 这是 将要递归的程序:
 * @param node: 当前的树节点(位置)
 * @param item: 处理 Trans 的一个 item
 */
int DataBase::buildFP_growthTree_SubProcess(FPTreeNode* node, CandidateKey::iterator item_iter, CandidateKey::iterator item_end, vector<ItemTableElement>& item_table)
{
    assert(node != nullptr);
    if (item_iter == item_end) { //某一个 Trans 的所有项都处理完成了
        return 1;
    }
    assert(item_iter != item_end);
    FPTreeNode* child = node->child;
    if (child == nullptr) {
        // node->child = new FPTreeNode(*item_iter);
        addchild(node, *item_iter);
        node->child->addSupply();

        addItemAddress2ItemTable(*item_iter, node->child, item_table);

        buildFP_growthTree_SubProcess(node->child, ++item_iter, item_end, item_table);
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

            assert(cur->sibling != nullptr);
            cur->sibling->addSupply();
            addItemAddress2ItemTable(*item_iter, cur->sibling, item_table);

            buildFP_growthTree_SubProcess(cur->sibling, ++item_iter, item_end, item_table);
        } else { //这一层找到了对应的节点
            sibling->addSupply();
            addItemAddress2ItemTable(*item_iter, sibling, item_table);
            buildFP_growthTree_SubProcess(sibling, ++item_iter, item_end, item_table);
        }
    }
    return 1;
}
void DataBase::printtree(FPTreeNode* node, int layer)
{
    if (node == nullptr) {
        return;
    }
    assert(node != nullptr);

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
    assert(p->sibling == nullptr);
    p->sibling = new FPTreeNode(item_name);

    p->sibling->parent = p->parent;
    return 1;
}
int DataBase::addchild(FPTreeNode* p, string& item_name)
{
    assert(p->child == nullptr);
    p->child = new FPTreeNode(item_name);

    p->child->parent = p;
    return 1;
};
int DataBase::addItemAddress2ItemTable(string& item_name, FPTreeNode* address, vector<ItemTableElement>& item_table)
{
    for (auto&& i : item_table) {
        if (i.item_name == item_name) {
            // i.fp_treenode_chains.push_back(address);
            i.fp_treenode_chains.push_back(address);
            i.supply++;

            return 1;
        }
    }
    // 执行 到这里 说明 项头表中没有 这个 名为 item_name 的项
    item_table.push_back(ItemTableElement(item_name));
    item_table.rbegin()->fp_treenode_chains.push_back(address);
    return 1;
}
int DataBase::print_database() const
{
    for (auto i : database) {
        cout << i;
    }
}