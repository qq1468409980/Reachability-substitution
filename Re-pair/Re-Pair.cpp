#include <iostream>
#include <vector>
#include <utility>
#include <string> 
#include <map>
#include <queue>
#include <tuple>
#include <fstream>
#include <dirent.h>
using namespace std;


class Repair{
private:
    int points_size;                                    // ｜A｜
    int reachable_size;                                 // ｜B｜
    int add_num;                                        // 添加点的序号
    int cnt_edge;                                       // 边数
    vector<vector<bool> > reachable_table;              // reachable表
    unordered_map<int, pair<int, int> > added_dict;     // 添加点的字典
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int> >, less<tuple<int, int, int> > > to_repair_queue;  //大根堆
   
    void init_repair_queue();
    void update_repair_queue();
    void update_reachable_table(tuple<int, int, int>);
    bool check(tuple<int, int, int>);                   // 检查频率是否改变
public:
    Repair(int, int);   // 随机生成矩阵
    Repair(int, int, int); // 按密度生成矩阵
    Repair(vector<vector<bool>>); // 给定矩阵
    void run_Repair();
    void show();
    void show(ofstream&);
    void show_convise(ofstream&);
    void show_graph(ofstream&);
    void show_graph_only_A2B(ofstream&);
    vector<vector<bool>> recov();
};
// 生成图

void readFiles2(const std::string& path) {
    DIR *dir;
    struct dirent *entry;
    std::ofstream outFile("repair_result.txt");
    std::ofstream outFile_consize("repair_result_consise.txt");
    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) {
                std::string filePath = path + "/" + entry->d_name;
                cout << entry->d_name << endl;
                string s = entry->d_name;
                if(s == ".DS_Store")
                    continue;
                std::string filePath2 = path + "/graph/" + entry->d_name;
                filePath2 = filePath2.substr(0, filePath2.size() - 4) + "_oriGraph.txt";
                std::string filePath3 = path + "/graph/" + entry->d_name;
                filePath3 = filePath3.substr(0, filePath3.size() - 4) + "_genGraph.txt";
                std::string filePath4 = path + "/data_repaired/" + entry-> d_name; 
                cout << filePath << endl;
                std::ifstream file(filePath);
                std::ofstream outFile2(filePath2);
                std::ofstream outFile3(filePath3);
                std::ofstream outFile4(filePath4);
                outFile << entry->d_name << endl;
                outFile_consize << entry->d_name << endl;
                if (file.is_open()) {
                    int n;
                    file >> n;
                    outFile2 << n << endl;
                    outFile3 << n << endl;
                    vector<vector<bool>> mat(n, vector<bool>(n));
                    for(int i = 0; i < n; ++i)
                    {
                        for(int j = 0; j < n; ++j)
                        {
                            int x;
                            file >> x;
                            mat[i][j] = x == 1;
                            cout << x << " ";
                        }
                        cout << endl;
                    }
                    auto mat_copy = mat;
                    Repair rp(mat);
                    cout << "pre:" << endl;
                    rp.show_graph(outFile2);
                    rp.run_Repair();
                    outFile << "Final: " << endl;
                    cout << "final:" << endl;
                    rp.show_graph(outFile3);
                    rp.show_graph_only_A2B(outFile4);
                    rp.show(outFile);
                    rp.show_convise(outFile_consize);
                    
                    cout <<"kk" << endl;
                    auto mat_recov = rp.recov();
                    for(int i = 0; i < n; ++i)
                    {
                        for(int j = 0; j < n; ++j)
                        {
                            if(mat_recov[i][j] != mat_copy[i][j])
                                getchar();
                        }
                    }
            
                    outFile << endl << endl;
                    outFile_consize << endl;
                    file.close();
                } else {
                    std::cerr << "Unable to open file: " << filePath << std::endl;
                }
            }
        }
        closedir(dir);
    } else {
        perror("opendir");
    }
}

void readFiles(const std::string& path) {
    DIR *dir;
    struct dirent *entry;
    std::ofstream outFile("repair_result.txt");
    std::ofstream outFile_consize("repair_result_consise.txt");
    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) {
                std::string filePath = path + "/" + entry->d_name;
                std::ifstream file(filePath);
                outFile << entry->d_name << endl;
                outFile_consize << entry->d_name << endl;
                if (file.is_open()) {
                    int n;
                    file >> n;
                    vector<vector<bool>> mat(n, vector<bool>(n));
                    for(int i = 0; i < n; ++i)
                    {
                        for(int j = 0; j < n; ++j)
                        {
                            int x;
                            file >> x;
                            mat[i][j] = x == 1;
                        }
                    }
                    
                    Repair rp(mat);
                    cout << "pre:" << endl;
                    //rp.show(outFile);
                    rp.run_Repair();
                    outFile << "Final: " << endl;
                    cout << "final:" << endl;
                    rp.show(outFile);
                    rp.show_convise(outFile_consize);
                    outFile << endl << endl;
                    outFile_consize << endl;
                    file.close();
                } else {
                    std::cerr << "Unable to open file: " << filePath << std::endl;
                }
            }
        }
        closedir(dir);
    } else {
        perror("opendir");
    }
}
int main()
{
    /*
    for(int j = 0; j < 10; j++)
    {
        cout << j <<":" << endl;
        int n = 100;
        //cin >> n;
        std::ofstream outFile("test" + to_string(n) + ".txt");
        for(int i = 1; i < 10; ++i)
        {
            outFile << "Ex: " << i << endl;
            outFile << "Original: " << endl;
            cout << "#####################"<<endl;
            cout << "Ex: " << i << endl;
            
            Repair rp(n,n,i);
            cout << "pre:" << endl;
            rp.show(outFile);
            rp.run_Repair();
            outFile << "Final: " << endl;
            cout << "final:" << endl;
            rp.show(outFile);
            outFile << endl << endl;
        }
        outFile.close();
    }
    */
    std::string folderPath = "data"; // 指定文件夹路径
    readFiles2(folderPath);
    return 0;
}

Repair::Repair(int n, int m)
{
    srand((unsigned)time(NULL)); 
    points_size = n;
    reachable_size = m;
    add_num = reachable_size;
    reachable_table.resize(n, vector<bool>(m));
    for(int i = 0; i < points_size; i++)
    {
        for(int j = 0; j < reachable_size; j++)
        {
            reachable_table[i][j] = rand() % 2;
        }
    }
    init_repair_queue();
}

Repair::Repair(int n, int m, int p)
{
    srand((unsigned)time(NULL)); 
    points_size = n;
    reachable_size = m;
    add_num = reachable_size;
    reachable_table.resize(n, vector<bool>(m));
    for(int i = 0; i < points_size; i++)
    {
        for(int j = 0; j < reachable_size; j++)
        {
            reachable_table[i][j] = (rand() % 10) < p;
        }
    }
    init_repair_queue();
}

Repair::Repair(vector<vector<bool>> init_table)
{
    reachable_table = init_table;
    points_size = init_table.size();
    reachable_size = init_table[0].size();
    add_num = reachable_size;
    init_repair_queue();
}

// 初始化优先队列
void Repair::init_repair_queue()
{
    for(int i = 0; i < reachable_size; ++i)
    {
        for(int j = i + 1; j < reachable_size; ++j)
        {
            int cnt = 0;
            for(int k = 0; k < points_size; ++k)
            {
                cnt += reachable_table[k][i] == true && reachable_table[k][j] == true;
            }
            if(cnt > 2)
            {
                to_repair_queue.push(make_tuple(cnt, i, j));
            }
        }
    }
}

// 更新优先队列
void Repair::update_repair_queue()
{
    for(int i = 0; i < add_num; ++i)
    {
        int cnt = 0;
        for(int j = 0; j < points_size; ++j)
        {
            cnt += reachable_table[j][add_num] && reachable_table[j][i];
        }
        if(cnt > 2)
        {
            to_repair_queue.push(make_tuple(cnt, i, add_num));
        }
    }
}

// 更新reachable表
void Repair::update_reachable_table(tuple<int, int, int> chosen_pair_info)
{
    int p1 = get<1>(chosen_pair_info);
    int p2 = get<2>(chosen_pair_info);
    added_dict[add_num] = make_pair(p1, p2);
    for(int k = 0; k < points_size; ++k)
    {
        if(reachable_table[k][p1] == true && reachable_table[k][p2] == true)
        {
            reachable_table[k].push_back(true);
            reachable_table[k][p1] = false;
            reachable_table[k][p2] = false;
        }
        else
        {
            reachable_table[k].push_back(false);
        }
    }
}

// 检查频率是否改变
bool Repair::check(tuple<int, int, int> chosen_pair_info)
{
    int cnt = get<0>(chosen_pair_info);
    int p1 = get<1>(chosen_pair_info);
    int p2 = get<2>(chosen_pair_info);
    int temp_cnt = 0;
    for(int i = 0; i < reachable_size; ++i)
    {
        temp_cnt += reachable_table[i][p1] && reachable_table[i][p2];
    }
    if(cnt == temp_cnt)
        return true;
    else
    {
        if(temp_cnt > 2)
        {
            to_repair_queue.push(make_tuple(temp_cnt, p1, p2));
        }
        return false;
    }
}

// 运行算法
void Repair::run_Repair()
{
    while (!to_repair_queue.empty())
    {
        auto chosen_pair = to_repair_queue.top();
        to_repair_queue.pop();
        if(!check(chosen_pair))
            continue;
        //cout << get<0> (chosen_pair) << " " << get<1> (chosen_pair) <<" " << get<2> (chosen_pair)<<endl;
        update_reachable_table(chosen_pair);
        update_repair_queue();
        add_num++;
        cout <<add_num << ": join！" << endl;
    }
}

void Repair::show(ofstream& outFile)
{
    // 输出reachable表
    cnt_edge = 0;
    outFile << "Reachable table :" << endl;
    for(int i = 0; i < reachable_size; ++i)
    {
        outFile << "   ";
        for(int j = 0; j < add_num; ++j)
        {
            cnt_edge += reachable_table[i][j];
            outFile << reachable_table[i][j] << " ";
        }
        outFile << endl;
    }
    //输出字典
    if(added_dict.size() > 0)
    {
        outFile << "Dictionary :"  << endl;
        cout << added_dict.size();
        for(auto &x : added_dict)
        {
            outFile << "   " << x.first << ": " << x.second.first << " " << x.second.second;
            outFile << endl;
        }
    }
    // 边数
    outFile <<"Edges count :   " << cnt_edge + added_dict.size() * 2 << endl;
    cout <<" "<< cnt_edge + added_dict.size() * 2 << endl;
}
void Repair::show_convise(ofstream& outFile)
{
    outFile << "Dictionary size:"  << added_dict.size() << endl <<"Edges count :   " << cnt_edge + added_dict.size() * 2 << endl;
}
// 显示结果
void Repair::show()
{
    //输出reachable表
    cnt_edge = 0;
    cout << "Reachable table :" << endl;
    for(int i = 0; i < reachable_size; ++i)
    {
        cout << "   ";
        for(int j = 0; j < add_num; ++j)
        {
            cnt_edge += reachable_table[i][j];
            cout << reachable_table[i][j] << " ";
        }
        cout << endl;
    }
    //输出字典
    if(added_dict.size() > 0)
    {
        cout << "Dictionary :"  << endl;
        cout << "Dictionary size:" << added_dict.size()<<endl;
        for(auto &x : added_dict)
        {
            cout << "   " << x.first << ": " << x.second.first << " " << x.second.second;
            cout << endl;
        }
    }
    // 边数
    cout <<"Edges count :   " << cnt_edge + added_dict.size() * 2 << endl;
}

void Repair::show_graph(ofstream& outFile)
{
    for(int i = 0; i < reachable_size; ++i)
    {
        for(int j = 0; j < add_num; ++j)
        {
            if(reachable_table[i][j] == 1)
            outFile << "s" << i << " t" << j << endl;
        }
    }
    for(auto &x : added_dict)
    {
        outFile << "t" << x.first << " t" << x.second.first << endl;
        outFile << "t" << x.first << " t" << x.second.second << endl;
    }
}

void Repair::show_graph_only_A2B(ofstream &outFile)
{
    outFile << reachable_size << endl;
    for(int i = 0; i < reachable_size; ++i)
    {
        for(int j = 0; j < add_num; ++j)
        {
            if(reachable_table[i][j] == 1)
                outFile << "1 ";
            else
                outFile << "0 ";
        }
        outFile << endl;
    }
}

vector<vector<bool>> Repair::recov()
{
    vector<vector<bool>> ret(points_size, vector<bool>(points_size, false));
    for(int i = 0; i < points_size; i++)
    {
        for(int j = 0; j < add_num; j++)
        {
            if(reachable_table[i][j])
            {
                queue<int> q;
                q.push(j);
                while(!q.empty())
                {
                    int x = q.front();
                    q.pop();
                    if(x < points_size)
                    {
                        ret[i][x] = true;
                    }
                    else
                    {
                        q.push(added_dict[x].first);
                        q.push(added_dict[x].second);
                    }
                }
            }        
        }
    }
    return ret;
}