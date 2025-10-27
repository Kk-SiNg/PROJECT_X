#include <iostream>
#include <vector>
#include <array>

using namespace std;

const int N = 10e3; // for max possible size of 2-d array N*N
int visited_array[N];

void dfs(array<vector<int>, N> graph2, int vertex){

    // if(visited_array[vertex] == 1){           //M1 to use visited_array--->check after entering a node: if it had been previously visited
    //     return;
    // }
    visited_array[vertex] = 1;
    for(int child : graph2[vertex]){
        if(visited_array[child] == 1){          //M2 to use visited_array--->check before jusmping to a node if it had been previously visited 
            continue;
        }
        dfs(graph2, child);
    }
}


int main(){

    int number_of_nodes, number_of_edges;
    cin >> number_of_nodes >> number_of_edges;
    int x, y;
    //m1 to store graph (adjacency matrix)
    int graph[number_of_nodes][number_of_nodes];
    for(int i = 0; i < number_of_edges; i++){
        cin >> x >> y;                          // there is an edge(taking bidirectional until mentioned) b/w node x and y
       
        //now to denote a connection we put that (x,y) and (y,x) in matrix to be 1(or any other weights) and others unconnected are still 0
        graph[x][y] = 1;
        graph[y][x] = 1;
        // now think like i want to see who all are childrens of node0 ? ---> for this go to 0th row and check for those j's thart are non-zero
   
        // space complexixity = O(N*N)
        // time complexixity for retrival of a wt.= O(1) as we can do graph[1][0] to find wt of edge connecting node 0 and 1 ...
    }


    //m2 to store graph (adjacency list)
    array <vector<int>, N> graph2;
    for(int i = 0; i < number_of_edges; i++){
        cin >> x >> y;

        graph2[y].push_back(x);
        graph2[x].push_back(y);
        // in this method we will have have a array of vectors where ith vector of array represent ith node and components of that vector represent all connected nodes with that ith node.
        // to store weights we will have to create a pair of all node numbers pushed_back and wt, i.e. array<vector<pair<int, int>>> graph2;
   
        // Space complexicity = O(N + number_of_edges)
        // Time complexicity for retrival of a wt. = O(N)   as we may have to traverse all N components of ith row to find the weight of j or first if j is even in ith vector.
    }
    
    dfs(graph2, 0);

    return 0;
}