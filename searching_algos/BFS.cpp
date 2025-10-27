#include <iostream>
#include <vector>
#include <array>
#include <queue>

using namespace std;

const int N = 10e5;

array<vector<int>, N> g;
int vis[N];
int level[N];

void bfs(int source){           //source node from where to start BFS
    
                                /*we use queue of BFS cuz we want to traverse
                                breadth wise or horizontally, so say: 1st node1 is inserted in queue
                                then it is removed and all of its child are inserted and now .pop() will remove
                                1st added child and then we add this childs child(only those that are not yet visited
                                to give the shortest path to any node from source node) 
                                but .pop() will still remove 1st node's child and so on...*/
    queue<int> q;
    q.push(source);
    vis[source] = 1;

    while(!q.empty()){
        int current_val = q.front();
        q.pop();
        cout << current_val << " ";
        for(int child : g[current_val]){
            if(vis[child]){
                continue;
            }
            q.push(child);
            vis[child] = 1;
            level[child] = level[current_val] + 1;
        }
    }

    cout << endl;
}

int main(){

    int n, m;
    cin >> n >> m;
    int x, y;

    for(int i = 0; i < m; i++){
        cin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);

    }
    bfs(1);
    for(int i = 1; i <= n; i++){
        cout << i << " : " << level[i] << endl;
    }

    return 0;
}