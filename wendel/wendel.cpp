#include <bits/stdc++.h>

using namespace std;

vector<tuple<int, int, char, char>> input;
vector<bool> visited;

vector<bool> transactions;

// vetor de outputs(identificador, quais transações fizeram parte, serialização, equivalência)
vector<tuple<int, vector<int>, string, string>> output;

// structure for graph node
struct node {
    set<pair<int, int>> edges;
};

// Custom compare for tuples
bool sortInput(const tuple<int, int, char, char>& a, const tuple<int, int, char, char>& b) {
    return (get<0>(a) < get<0>(b));
}

bool isCyclicUtil(int v, bool visited[], bool *recStack, vector <node> &graph) {
    if(visited[v] == false) {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        recStack[v] = true;
 
        // Recur for all the vertices adjacent to this vertex
        for (set<pair<int,int>>:: iterator it = graph[v].edges.begin(); it != graph[v].edges.end();/* blank */) {
            if ( !visited[it->second] && isCyclicUtil(it->second, visited, recStack, graph) ) {
                return true;
            }
            else if (recStack[it->second]) {
                return true;
            }

            ++it;
        }
 
    }
    recStack[v] = false;  // remove the vertex from recursion stack

    return false;
}
 
// Returns true if the graph contains a cycle, else false.
// This function is a variation of DFS() in https://www.geeksforgeeks.org/archives/18212
bool isCyclic(vector <node> &graph) {
    // Mark all the vertices as not visited and not part of recursion stack
    bool *visited = new bool[graph.size()];
    bool *recStack = new bool[graph.size()];
    for(size_t i = 1; i < graph.size(); i++) {
        visited[i] = false;
        recStack[i] = false;
    }
 
    // Call the recursive helper function to detect cycle in different
    // DFS trees
    for(size_t i = 1; i < graph.size(); i++) {
        if ( !visited[i] && isCyclicUtil(i, visited, recStack, graph)) {
            return true;
        }
    }
 
    return false;
}

// Create an Agenda based on the param index
vector<tuple<int, int, char, char>> create_agenda(int index) {
    set<int> B;
    vector<tuple<int, int, char, char>> aux;

    B.insert(get<1>(input[index]));

    

    int element = get<1>(input[index]);

    while (!B.empty()){ 
        //cout << "aa" << endl;
       
        // for (size_t i = 0; i < input.size(); i++) {
        //     cout << visited[i] << ' ';
        // }
        // cout << endl;
        
        for (size_t i = 0; i < input.size(); i++) {
            // if (visited[i]) {
            //     continue;
            // }
            
            if (get<2>(input[i]) != 'C' && !visited[i]) {
                visited[i] = 1;
                B.insert(get<1>(input[i]));
                aux.push_back(make_tuple(get<0>(input[i]), get<1>(input[i]), get<2>(input[i]), get<3>(input[i])));
            }
            else if (get<2>(input[i]) == 'C' && get<1>(input[i]) == element) {
                // cout << "aqui" << B.size() << '\n';
                visited[i] = 1;
                break;
            }
            else {
                //visited[i] = 1;
            }
        }

        B.erase(element);

        // cout << "aqui2" << B.size() << '\n';

        element = *B.begin();

        // cout << index << endl;
    }

    return aux;
}

// test seriability
bool test_seriability(vector<tuple<int, int, char, char>> &agenda) {
    set<int> B;
    set<char> attributes;

    cout << "Local Agenda: " << endl;
    for (size_t i = 0; i < agenda.size(); i++) {
        cout << get<0>(agenda[i]) << ' ' << get<1>(agenda[i]) << ' ' << get<2>(agenda[i]) << ' ' << get<3>(agenda[i]) << endl;
        B.insert(get<1>(agenda[i]));
        attributes.insert(get<3>(agenda[i]));
    }

    // create the graph    
    int greater = 0;
    for (set<int>:: iterator it = B.begin(); it != B.end();/* blank */) {
        if (*it > greater) {
            greater = *it;
        }
        ++it;
    }

    vector<node> graph(greater + 1);
    cout << '\n' << "Nodes: ";
    for (size_t i = 1; i < graph.size(); i++) {
        cout << i << ' ';
    }
    cout << '\n';

    // look for dependencies for each attribute
    for (set<char>:: iterator it = attributes.begin(); it != attributes.end();/* blank */) {
        //cout << *it << '\n';
        for (int j = agenda.size() - 1; j >= 0; j--) {
            // If is not the attribute of iteration, go to next it
            if (get<3>(agenda[j]) != *it) {
                continue;
            }

            // break;
            for (int i = j - 1; i >= 0; i--) {
                // If is same transaction or Different attributes, go to next it
                if (get<1>(agenda[i]) == get<1>(agenda[j]) || get<3>(agenda[i]) != get<3>(agenda[j])) {
                    continue;
                }

                if (get<2>(agenda[j]) == 'R') {                     
                    // Reading after write
                    if (get<2>(agenda[i]) == 'W') {
                        graph[get<1>(agenda[i])].edges.insert(make_pair(get<1>(agenda[i]), get<1>(agenda[j])));
                    }                
                }                
                else if (get<2>(agenda[j]) == 'W') {
                    // Writing after Read
                    if (get<2>(agenda[i]) == 'R') {
                        graph[get<1>(agenda[i])].edges.insert(make_pair(get<1>(agenda[i]), get<1>(agenda[j])));
                    }
                    // Writing after Write
                    else if (get<2>(agenda[i]) == 'W') {
                        graph[get<1>(agenda[i])].edges.insert(make_pair(get<1>(agenda[i]), get<1>(agenda[j])));
                    }
                }
            }
        }

        ++it;
    }

    bool hasCycle = isCyclic(graph);
    // Check if has a cycle
    cout << "Graph has cycle? " << hasCycle << '\n';

    // Print the graph's nodes and its edges
    for (size_t i = 1; i < graph.size(); i++) {
        cout << "Node " << i << ":\n";
        for (set<pair<int,int>>:: iterator it = graph[i].edges.begin(); it != graph[i].edges.end();/* blank */) {
            cout << '<' << it->first << ',' << it->second << '>' << endl;

            ++it;
        }
        cout << endl;
    }
    cout << '\n';

    return hasCycle;
}

bool check_views(int index, vector<tuple<int, int, char, char>> &agenda, vector<tuple<int, int, char, char>> &aux, set<int> &B) {
    //cout << "index " << index << endl;
    set<int> temp;
    for (int it:temp) {
        if (it != index) {
            temp.insert(it);            
        }
    }

    vector<tuple<int, int, char, char>> view;
    for (size_t i = 0; i < aux.size(); i++) {
        view.push_back(make_tuple(get<0>(aux[i]), get<1>(aux[i]), get<2>(aux[i]), get<3>(aux[i])));
    }

    for (size_t i = 0; i < agenda.size(); i++) {
        if (get<1>(agenda[i]) == index) {
            view.push_back(make_tuple(get<0>(agenda[i]), get<1>(agenda[i]), get<2>(agenda[i]), get<3>(agenda[i])));
        }
    }

    for (int it:temp) {
        bool check = check_views(it, agenda, view, temp);

        if (check) {
            return check;
        }
    }

    // Check if View is Equivalent
    if (temp.empty()) {
        cout << "Local View: " << endl;
        for (size_t i = 0; i < view.size(); i++) {
            cout << get<0>(view[i]) << ' ' << get<1>(view[i]) << ' ' << get<2>(view[i]) << ' ' << get<3>(view[i]) << endl;
        }
        cout << "\n\n\n\n\n";

        set<char> attributes;
        for (size_t i = 0; i < agenda.size(); i++) {
            attributes.insert(get<3>(agenda[i]));
        }

        // for (set<char>:: iterator it = attributes.begin(); it != attributes.end();/* blank */) {
        //     //cout << *it << '\n';
        //     for (int j = view.size() - 1; j >= 0; j--) {
        //         // If is not the attribute of iteration, go to next it
        //         if (get<3>(view[j]) != *it) {
        //             continue;
        //         }

        //         if (get<3>(view[j]) == 'R')) {
        //             for (int i = j - 1; i >= 0; i--) {
        //                 // If is same transaction or Different attributes, go to next it
        //                 if (get<1>(view[i]) == get<1>(view[j]) || get<3>(view[i]) != get<3>(view[j])) {
        //                     continue;
        //                 }

        //                 if (get<2>(view[j]) == 'R') {
        //                     // Reading after write
        //                     if (get<2>(view[i]) == 'W') {
        //                     }                
        //                 }                
        //                 else if (get<2>(view[j]) == 'W') {
        //                     // Writing after Read
        //                     if (get<2>(view[i]) == 'R') {
        //                     }
        //                     // Writing after Write
        //                     else if (get<2>(view[i]) == 'W') {
        //                     }
        //                 }
        //             }
        //         }
        //     }

        //     ++it;
        // }
    }

    return false;
}

// Test equivalency of an Agenda
bool create_views(vector<tuple<int, int, char, char>> &agenda) {
    set<int> B;

    vector<tuple<int, int, char, char>> aux;

    for (size_t i = 0; i < agenda.size(); i++) {
        // cout << get<0>(agenda[i]) << ' ' << get<1>(agenda[i]) << ' ' << get<2>(agenda[i]) << endl;
        B.insert(get<1>(agenda[i]));
    }

    return check_views(0, agenda, aux, B);
}


// main
int main() {
    
    ios::sync_with_stdio(false);
    cin.tie(0);

    int prior;
    int ident;
    char oper;
    char var;
    while (cin >> prior) {

        cin >> ident;
        cin >> oper;
        cin >> var;

        input.push_back(make_tuple(prior, ident, oper, var));
    }
    sort(input.begin(), input.end(), sortInput);
    for (size_t i = 0; i < input.size(); i++) {
        visited.push_back(false);
    }

    int tSize = 0;
    for (size_t i = 0; i < input.size(); i++) {
        if (tSize < get<1>(input[i])) {
            transactions.push_back(false);

            tSize = transactions.size();
        }
    }

    for (size_t i = 0; i < input.size(); i++) {
        //cout << visited[i] << "here\n";
        if (!visited[i]) {
            vector<tuple<int, int, char, char>> aux = create_agenda(i);
            test_seriability(aux);
            create_views(aux);
            cout << endl;
        }
        //break;
    }

    cout << "All: ";
    for (size_t i = 0; i < transactions.size(); i++) {
        cout << i + 1 << ' ';
    }
    cout << endl;

    for (size_t i = 0; i < input.size(); i++) {
        cout << get<0>(input[i]) << ' ';
        cout << get<1>(input[i]) << ' ';
        cout << get<2>(input[i]) << ' ';
        cout << get<3>(input[i]) << '\n';
    }
}