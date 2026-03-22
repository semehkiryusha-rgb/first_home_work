#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cstdlib>

using namespace std;

// функция расстояния Хэмминга
int h(const vector<int>& a, const vector<int>& b) {
    int dist = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) dist++;
    }
    return dist;
}

// подсчет веса решения
int totalWeight(const vector<int>& sol, const vector<int>& weight) {
    int sum = 0;
    for (size_t i = 0; i < sol.size(); i++) {
        if (sol[i]) sum += weight[i];
    }
    return sum;
}

// Ппдсчет ценности
int totalValue(const vector<int>& sol, const vector<int>& value) {
    int sum = 0;
    for (size_t i = 0; i < sol.size(); i++) {
        if (sol[i]) sum += value[i];
    }
    return sum;
}

int main() {

    string data = R"(30 100000
90000  90001
89750  89751
10001  10002
89500  89501
10252  10254
89250  89251
10503  10506
89000  89001
10754  10758
88750  88751
11005  11010
88500  88501
11256  11262
88250  88251
11507  11514
88000  88001
11758  11766
87750  87751
12009  12018
87500  87501
12260  12270
87250  87251
12511  12522
87000  87001
12762  12774
86750  86751
13013  13026
86500  86501
13264  13278
86250  86251
)";

    stringstream ss(data);
    int n, capacity;
    ss >> n >> capacity;

    vector<int> weight(n);
    vector<int> value(n);

    for (int i = 0; i < n; i++) {
        ss >> weight[i] >> value[i];
    }

    // начальное решение (жадное)
    vector<pair<double, int>> ratio(n);
    for (int i = 0; i < n; i++) {
        ratio[i] = { (double)value[i] / weight[i], i };
    }
    sort(ratio.begin(), ratio.end(), greater<pair<double, int>>());

    vector<int> best(n, 0);
    int current_weight = 0;
    for (auto& r : ratio) {
        int idx = r.second;
        if (current_weight + weight[idx] <= capacity) {
            best[idx] = 1;
            current_weight += weight[idx];
        }
    }
    int best_value = totalValue(best, value);

    bool improved = true;
    int iterations = 0;

    while (improved && iterations < 1000) {
        improved = false;
        iterations++;

        for (int i = 0; i < n; i++) {
            vector<int> neighbor = best;
            neighbor[i] = 1 - neighbor[i];

            int w = totalWeight(neighbor, weight);
            int v = totalValue(neighbor, value);

            if (w <= capacity && v > best_value) {
                best = neighbor;
                best_value = v;
                improved = true;
            }
        }

        if (!improved) {
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    vector<int> neighbor = best;
                    neighbor[i] = 1 - neighbor[i];
                    neighbor[j] = 1 - neighbor[j];

                    int w = totalWeight(neighbor, weight);
                    int v = totalValue(neighbor, value);

                    if (w <= capacity && v > best_value) {
                        best = neighbor;
                        best_value = v;
                        improved = true;
                        break;
                    }
                }
                if (improved) break;
            }
        }

        // пробуем случайные решения с большим расстоянием Хэмминга
        if (!improved && iterations % 10 == 0) {
            vector<int> random_sol(n, 0);
            for (int i = 0; i < n; i++) {
                random_sol[i] = rand() % 2;
            }

            // корректируем, чтобы не превышать вес
            int w = totalWeight(random_sol, weight);
            if (w > capacity) {
               
                for (int i = 0; i < n && w > capacity; i++) {
                    if (random_sol[i]) {
                        random_sol[i] = 0;
                        w -= weight[i];
                    }
                }
            }

            int v = totalValue(random_sol, value);

            
            if (v > best_value) {
                best = random_sol;
                best_value = v;
                improved = true;
            }
        }
    }

    cout << best_value << endl;
    for (int i = 0; i < n; i++) {
        if (best[i]) {
            cout << i << " ";
        }
    }
    cout << endl;

    return 0;
}