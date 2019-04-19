#include"BaseStructure.h"
#include"SimpleLoader.h"
#include<map>
#include<algorithm>
// using namespace std;

int GroceryDataBase::load(string filename)
{
    //read whole liens from filename
    // vector<DataItem> transet;

    ifstream fp(filename.c_str());

    if (!fp.is_open()) { // check file whether or not exist
        cout << "打开文件 ";
        cout << filename;
        cout << "失败 " << endl;
    }
 
    cout << fp.is_open() << endl;
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
}
int UnixUserDataBase::load(string filename){
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
        if(line == string("**SOF**")){
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
 * 1 使用注册那个迭代的方法基于候选 找出频繁项集
 */ 
CandidateSet DataBase::apriori_gen(CandidateSet& L){
    // 自动在 L 中的 k - 1 频繁项集 自连接 生成 k 项频繁项集
    assert(L.size() > 0);
    int length = L.begin()->first.size();
    CandidateKey intersect_result,union_result;
    CandidateSet candidate_set;
    bool same = 0;
    for (auto&& i : L) {
        for(auto&& j : L)
        {
            intersect_result.clear();
            union_result.clear();

            if (i.first == j.first) {
                continue;
            }
            // 连接步
            CandidateKey::const_iterator it1_begin = i.first.begin(),
                                         it1_end = i.first.end(),
                                         it2_begin = j.first.begin(),
                                         it2_end = j.first.end();
            // for(int m = 0; m < length; m++,it1++,it2++)
            // {

            //     // if()
            //     /* code */
            // }
            
            set_intersection(i.first.begin(), i.first.end(), j.first.begin(), j.first.end(), std::back_inserter(intersect_result));

            if (intersect_result.size() >= length - 1 && *(it1_end - 1) < *(it2_end - 1)){
                // cout << "可以连接" << endl;
                set_union(i.first.begin(), i.first.end(), j.first.begin(), j.first.end(), std::back_inserter(union_result));
                
                candidate_set[union_result]++;
            }
            
            // 剪枝步
            for(auto&& i : candidate_set)
            {
                has_infrequent_subset(i.first, L);
            }
        }
    }
    // print(candidate_set);

}
bool DataBase::has_infrequent_subset(CandidateKey cand,CandidateSet& container){
    //TODO: 判断某一个 自连接 后的候选项集 是否 在原来的候选项集中
    for(auto&& i : container)
    {
        // set<string> candset(cand.begin(), cand.end());
        // set<string> containerset(container.begin(), container.end());
        
    }
    
}
int DataBase::Apriori(){
    // 找出 1 频繁项集
    CandidateSet frequent_one_set;
    CandidateSet good_frequent_set;
    for (auto&& i : database) {
        for(auto&& j : i.item_set)
        {
            CandidateKey s;
            s.push_back(j);
            frequent_one_set[s]++;
        }
    }

    print(frequent_one_set);
    // FrequentItem
    for(int k = 0; frequent_one_set.size() != 0; k++)
    {
        apriori_gen(frequent_one_set);
        /* code */
        

        break;
    }

    return 1;
}
int DataBase::print(const CandidateSet& candset)const{
    for(auto&& i : candset)
    {
        for(auto&& j : i.first)
        {
            cout << j << ",";
        }

        cout << " ";
        cout <<i.second << endl;
    }
    return 1;
}