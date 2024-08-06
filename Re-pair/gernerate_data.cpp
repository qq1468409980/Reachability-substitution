#include <iostream>
#include <vector>
#include <utility>
#include <string> 
#include <map>
#include <queue>
#include <tuple>
#include <fstream>
using namespace std;

int main()
{
    vector<int> arr_size = {10, 200, 500, 1000};
    for(auto &mat_size : arr_size)
    {
        int points_size = mat_size;
        int reachable_size = mat_size;
        for(int p = 1; p < 10; ++p)
        {
            std::ofstream outFile("data/data_" + to_string(mat_size)+ "_" + to_string(p) + ".txt");
            outFile << mat_size << endl;
            for(int i = 0; i < points_size; i++)
            {
                for(int j = 0; j < reachable_size; j++)
                {
                    int x = (rand() % 10) < p;
                    outFile << x << " ";
                }
                outFile << endl;
            }
            outFile.close();
        }
    }
    cout << "Finished!" << endl;
    return 0;
}