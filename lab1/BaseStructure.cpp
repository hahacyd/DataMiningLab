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
    // int length =
    assert(L.size() > 0);
    // int length = L[0].first.size();
    int length = L.begin()->first.size();
    vector<string> result;
    bool same = 0;
    for (auto&& i : L) {

        for(auto&& j : L)
        {
            set<string>::iterator it1 = i.first.begin(),it2 = j.first.begin();
            for(int m = 0; m < length; m++,it1++,it2++)
            {
                
                // if()
                /* code */
            }
            
            // set_intersection(i.first.begin(), i.first.end(), j.first.begin(), j.first.end(), std::back_inserter(result));

            if (result.size() > length - 1){

            }
            // 连接步


            // 剪枝步
        }
        
    }
    
}
int DataBase::print(const CandidateSet& candset)const{
    for(auto&& i : candset)
    {
        for(auto&& j : i.first)
        {
            cout << j << " ";
        }

        cout << " ";
        cout <<i.second << endl;
    }
    return 1;
}
int DataBase::Apriori(){
    // 找出 1 频繁项集
    map<set<string>, int> frequent_one_set;
    for (auto&& i : database) {
        for(auto&& j : i.item_set)
        {
            set<string> s;
            s.insert(j);
            frequent_one_set[s]++;
        }
    }

    print(frequent_one_set);
    // FrequentItem
    for(int k = 0; frequent_one_set.size() != 0; k++)
    {
        apriori_gen(frequent_one_set);
        /* code */
    }
    
    return 1;
}