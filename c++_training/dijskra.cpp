#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

typedef pair<int, int> pii;

// Funkcja Dijkstry
void dijkstra(int start, vector<vector<pii>>& graph, vector<int>& distances) {
    int n = graph.size();  // Liczba wierzchołków
    priority_queue<pii, vector<pii>, greater<pii>> pq;  // Kolejka priorytetowa (min-heap)

    distances[start] = 0;  // Odległość od źródła do źródła to 0
    pq.push({0, start});   // Wstawienie startowego wierzchołka do kolejki

    while (!pq.empty()) {
        int current_dist = pq.top().first;
        int current_vertex = pq.top().second;
        pq.pop();

        // Jeżeli znaleźliśmy krótszą ścieżkę, pomijamy
        if (current_dist > distances[current_vertex]) continue;

        // Przeglądamy wszystkich sąsiadów bieżącego wierzchołka
        for (auto& neighbor : graph[current_vertex]) {
            int neighbor_vertex = neighbor.first;
            int edge_weight = neighbor.second;

            // Jeśli nowa odległość jest krótsza
            if (distances[current_vertex] + edge_weight < distances[neighbor_vertex]) {
                distances[neighbor_vertex] = distances[current_vertex] + edge_weight;
                pq.push({distances[neighbor_vertex], neighbor_vertex});
            }
        }
    }
}

int main() {
    int n, m;  // Liczba wierzchołków i krawędzi
    cout << "Podaj liczbę wierzchołków i krawędzi: ";
    cin >> n >> m;

    vector<vector<pii>> graph(n);  // Graf jako lista sąsiedztwa

    cout << "Podaj krawędzie (wierzchołek1, wierzchołek2, waga):" << endl;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});  // Jeśli graf jest nieskierowany
    }

    int start;
    cout << "Podaj wierzchołek początkowy: ";
    cin >> start;

    vector<int> distances(n, INT_MAX);  // Odległości od źródła, początkowo nieskończone

    dijkstra(start, graph, distances);

    cout << "Najkrótsze odległości od wierzchołka " << start << ":\n";
    for (int i = 0; i < n; i++) {
        cout << "Do wierzchołka " << i << ": " << distances[i] << endl;
    }

    return 0;
}
