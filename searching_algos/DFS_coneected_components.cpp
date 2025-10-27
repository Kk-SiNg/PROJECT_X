#include <iostream>
#include <vector>
#include <array>

using namespace std;

const int N = 10e5; // for max possible size of 2-d array N*N
bool visited_array[N];
vector <vector<int>> list_of_all_connected_components;
vector<int> temp_comoponet_list;

void dfs(array <vector<int>, N> graph2, int vertex){

    visited_array[vertex] = 1;
    temp_comoponet_list.push_back(vertex);

    for(int child : graph2[vertex]){

        if(visited_array[child] == 1){
            continue;
        }

        dfs(graph2, child);
    }
}


int main(){

    int number_of_nodes, number_of_edges;
    cin >> number_of_nodes >> number_of_edges;
    int x, y;
    
    array <vector<int>, N> graph2;
    for(int i = 0; i < number_of_edges; i++){
        cin >> x >> y;

        graph2[y].push_back(x);
        graph2[x].push_back(y);
    }

    //now counting and storing connected components:
    //note ---> 1 dfs starting from any node in a connected component will visit every other node in that connected component, so count = number of dfs required.
    int count = 0;

    for(int i = 1; i <= number_of_nodes; i++){
        //this if() ensures that all connected components are visited exactly once
        if(visited_array[i] == true){
            continue;
        }
        else{
            temp_comoponet_list.clear();
            dfs(graph2, i);
            list_of_all_connected_components.push_back(temp_comoponet_list);
            count ++;
        }
    }

    for(int i = 0; i < count; i++){
        for(auto j: list_of_all_connected_components[i]){
            cout << j <<" ";
        }
        cout << endl;
    }
    return 0;
}