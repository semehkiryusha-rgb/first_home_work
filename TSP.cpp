#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>

using namespace std;

struct City {
    double x, y;
};

double dist(const City& a, const City& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    
    string data = R"(70
67 99
48 83
75 81
8 19
20 18
54 38
63 36
44 33
52 18
12 13
25 5
58 85
5 67
90 9
41 76
25 76
37 64
56 63
10 55
98 7
16 74
89 60
48 82
81 76
29 60
17 22
5 45
79 70
9 100
17 82
74 67
10 68
48 19
83 86
84 94
64 96
80 39
69 23
72 42
48 67
58 43
81 34
79 17
30 23
42 67
7 76
29 51
78 92
64 8
95 57
57 91
40 35
68 40
92 34
62 1
28 43
76 73
67 88
93 54
6 8
87 18
30 9
77 13
78 94
55 3
82 88
73 28
20 55
27 43
95 86)";

    
    stringstream ss(data);
    int n;
    ss >> n;

    vector<City> cities(n);
    for (int i = 0; i < n; i++) {
        ss >> cities[i].x >> cities[i].y;
    }

    vector<int> tour(n);
    for (int i = 0; i < n; i++) {
        tour[i] = i;
    }

    // вычисление длины
    auto tour_len = [&](vector<int>& t) {
        double len = 0;
        for (int i = 0; i < n; i++) {
            len += dist(cities[t[i]], cities[t[(i + 1) % n]]);
        }
        return len;
        };

    // 2-opt 
    bool improved = true;
    int iterations = 0;

    while (improved && iterations < 100) {
        improved = false;

        for (int i = 1; i < n - 1; i++) {
            for (int j = i + 1; j < n; j++) {
                int a = tour[i - 1];
                int b = tour[i];
                int c = tour[j - 1];
                int d = tour[j % n];

                double old_len = dist(cities[a], cities[b]) + dist(cities[c], cities[d]);
                double new_len = dist(cities[a], cities[c]) + dist(cities[b], cities[d]);

                if (new_len < old_len) {
                    reverse(tour.begin() + i, tour.begin() + j);
                    improved = true;
                    iterations++;
                    break;
                }
            }
            if (improved) break;
        }
    }

    double final_len = tour_len(tour);

    cout << final_len << endl;

    for (int i = 0; i < n; i++) {
        cout << tour[i];
        if (i < n - 1) cout << endl;
    }
    cout << endl;

    return 0;
}