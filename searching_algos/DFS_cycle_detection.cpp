#include <iostream>
#include <vector>
#include <array>

using namespace std;

const int N = 10e3;
array <vector<int>, N> graph;
bool visited_lst[N];
bool isCycleExist = false;

bool dfs(int vertex, int parent){               // parent node of current vertex to keep in mind so that we can ignore it checking for condition of cycle.

    visited_lst[vertex] = 1;
    for(int child : graph[vertex]){
        if(child == parent){
            continue;
        }
        if(visited_lst[child]){
            return true;                //as soon as even a single cycle is found return true and then fall back to parent node(we could add something to even exit dfs right now to optimise)
        }
        isCycleExist |= dfs(child, vertex);
    }
}

int main(){
    int n, m;
    cin >> n >> m;
    int x, y;

    for(int i = 0; i < m; i++){
        cin >> x >> y;
        graph[x].push_back(y);
        graph[y].push_back(x);
    }

    for(int i = 1; i <= n; i++){
        if(visited_lst[i] == 1){
            continue;
        }
        dfs(i,1);
        if(isCycleExist == 1){
            cout << "cycle exist\n";
            return 1;
        }
    }
    return 0;
}