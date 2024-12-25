// #include <iostream>
// #include <fstream>
// #include <string>
// #include <sstream>
// #include <vector>
// #include <algorithm>
// #include <set>
// #include <queue>

// using namespace std;

// struct P {
//     int64_t x, y;
//     bool operator<(const P& other) const {
//         return (y < other.y) ||
//                (y == other.y && x < other.x);
//     }
//     bool operator==(const P& other) const {
//         return (y == other.y) &&
//                (x == other.x);
//     }
// };

// struct E {
//     int64_t key;
//     int64_t score;
//     int64_t prevKey;
// };

// vector<vector<vector<int64_t>>> priceDirect(5);

// const int64_t A = 0;
// const int64_t UP = 1;
// const int64_t DOWN = 2;
// const int64_t LEFT = 3;
// const int64_t RIGHT = 4;

// P getPositionAlpha(int64_t number) {
//     switch (number) {
//     case 0: return P{ 1, 3 };
//     case 10: return P{ 2, 3 };
//     case 1: return P{ 0, 2 };
//     case 2: return P{ 1, 2 };
//     case 3: return P{ 2, 2 };
//     case 4: return P{ 0, 1 };
//     case 5: return P{ 1, 1 };
//     case 6: return P{ 2, 1 };
//     case 7: return P{ 0, 0 };
//     case 8: return P{ 1, 0 };
//     case 9: return P{ 2, 0 };
//     }
//     return P{ 0, 3 };
// }

// int64_t getNumberAlpha(P p) {
//     if (p.y == 3) {
//         switch (p.x) {
//         case 0: return -1;
//         case 1: return 0;
//         case 2: return 10;
//         }
//     }
//     if (p.y == 2) {
//         switch (p.x) {
//         case 0: return 1;
//         case 1: return 2;
//         case 2: return 3;
//         }
//     }
//     if (p.y == 1) {
//         switch (p.x) {
//         case 0: return 4;
//         case 1: return 5;
//         case 2: return 6;
//         }
//     }
//     if (p.y == 0) {
//         switch (p.x) {
//         case 0: return 7;
//         case 1: return 8;
//         case 2: return 9;
//         }
//     }
//     return -1;
// }

// P getPositionDirect(int64_t number) {
//     switch (number) {
//     case A: return P{ 2, 0 };
//     case UP: return P{ 1, 0 };
//     case LEFT: return P{ 0, 1 };
//     case DOWN: return P{ 1, 1 };
//     case RIGHT: return P{ 2, 1 };
//     }
//     return P{ 0, 0 };
// }

// int64_t getNumberDirect(P p) {
//     if (p.y == 0) {
//         switch (p.x) {
//         case 0: return -1;
//         case 1: return UP;
//         case 2: return A;
//         }
//     }
//     if (p.y == 1) {
//         switch (p.x) {
//         case 0: return LEFT;
//         case 1: return DOWN;
//         case 2: return RIGHT;
//         }
//     }
//     return -1;
// }

// int64_t maxLevel = 2;

// void computePriceDirect(int64_t level) {
//     if (level == 0) {
//         priceDirect[0].resize(5);
//         for (int64_t x = 0; x < 5; ++x) {
//             priceDirect[0][x].resize(5);
//             for (int64_t y = 0; y < 5; ++y) {
//                 priceDirect[0][x][y] = 1;
//             }
//         }
//         return;
//     }
//     computePriceDirect(level - 1);
//     priceDirect[level].resize(5);
//     for (int64_t start = 0; start < 5; ++start) {
//         vector<vector<int64_t>> test(5, vector<int64_t>(5, -1));
//         vector<E> stack;
//         stack.push_back(E{ start, 0, A });
//         priceDirect[level][start].resize(5, -1);
//         auto& out = priceDirect[level][start];
//         while (!stack.empty()) {
//             E e = stack.back();
//             stack.pop_back();
//             int64_t scorePress = e.score + priceDirect[level - 1][e.prevKey][A];
//             if (test[e.key][e.prevKey] != -1 && test[e.key][e.prevKey] <= e.score) {
//                 continue;
//             }
//             test[e.key][e.prevKey] = e.score;
//             if (out[e.key] == -1 || out[e.key] >= scorePress) {
//                 out[e.key] = scorePress;
//             }
//             P curr = getPositionDirect(e.key);
//             const auto add = [&](P p, int64_t direction) {
//                 int64_t key = getNumberDirect(p);
//                 if (key != -1) {
//                     stack.push_back(E{ key, e.score + priceDirect[level - 1][e.prevKey][direction], direction });
//                 }
//             };
//             add(P{ curr.x, curr.y - 1 }, UP);
//             add(P{ curr.x, curr.y + 1 }, DOWN);
//             add(P{ curr.x - 1, curr.y }, LEFT);
//             add(P{ curr.x + 1, curr.y }, RIGHT);
//         }
//     }
// }

// void compute(int64_t start, vector<int64_t>& out) {
//     vector<E> stack;
//     stack.push_back(E{ start, 0, A });
//     out.resize(11, -1);
//     vector<vector<int64_t>> test(11, vector<int64_t>(5, -1));
//     while (!stack.empty()) {
//         E e = stack.back();
//         stack.pop_back();
//         int64_t scorePress = e.score + priceDirect[maxLevel][e.prevKey][A];
//         if (test[e.key][e.prevKey] != -1 && test[e.key][e.prevKey] <= e.score) {
//             continue;
//         }
//         test[e.key][e.prevKey] = e.score;
//         if (out[e.key] == -1 || out[e.key] >= scorePress) {
//             out[e.key] = scorePress;
//         }
//         P curr = getPositionAlpha(e.key);
//         const auto add = [&](P p, int64_t direction) {
//             int64_t key = getNumberAlpha(p);
//             if (key != -1) {
//                 stack.push_back(E{ key, e.score + priceDirect[maxLevel][e.prevKey][direction], direction });
//             }
//         };
//         add(P{ curr.x, curr.y - 1 }, UP);
//         add(P{ curr.x, curr.y + 1 }, DOWN);
//         add(P{ curr.x - 1, curr.y }, LEFT);
//         add(P{ curr.x + 1, curr.y }, RIGHT);
//     }
// }

// int main()
// {
//     ifstream stream("input21.txt");
//     string line;
//     int64_t sum = 0;
//     priceDirect.resize(maxLevel + 1);
//     computePriceDirect(maxLevel);
//     vector<vector<int64_t>> price(11, vector<int64_t>());
//     for (int64_t i = 0; i < price.size(); ++i) {
//         compute(i, price[i]);
//     }
//     while (getline(stream, line)) {
//         int64_t value = 0;
//         int64_t prev = 10;
//         int64_t num = 0;
//         for (size_t i = 0; i < line.size(); ++i) {
//             if (line[i] == 'A') {
//                 value += price[prev][10];
//                 prev = 10;
//             }
//             else {
//                 int64_t curr = line[i] - '0';
//                 value += price[prev][curr];
//                 prev = curr;
//                 num = 10 * num + curr;
//             }
//         }
//         sum += value * num;
//     }

//     cout << sum << endl;
//     return 0;
// }






















#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;

struct P {
    int64_t x, y;
    bool operator<(const P& other) const {
        return (y < other.y) ||
               (y == other.y && x < other.x);
    }
    bool operator==(const P& other) const {
        return (y == other.y) &&
               (x == other.x);
    }
};

struct E {
    int64_t key;
    int64_t score;
    int64_t prevKey;
};

vector<vector<vector<int64_t>>> priceDirect(5);

const int64_t A = 0;
const int64_t UP = 1;
const int64_t DOWN = 2;
const int64_t LEFT = 3;
const int64_t RIGHT = 4;

P getPositionAlpha(int64_t number) {
    switch (number) {
    case 0: return P{ 1, 3 };
    case 10: return P{ 2, 3 };
    case 1: return P{ 0, 2 };
    case 2: return P{ 1, 2 };
    case 3: return P{ 2, 2 };
    case 4: return P{ 0, 1 };
    case 5: return P{ 1, 1 };
    case 6: return P{ 2, 1 };
    case 7: return P{ 0, 0 };
    case 8: return P{ 1, 0 };
    case 9: return P{ 2, 0 };
    }
    return P{ 0, 3 };
}

int64_t getNumberAlpha(P p) {
    if (p.y == 3) {
        switch (p.x) {
        case 0: return -1;
        case 1: return 0;
        case 2: return 10;
        }
    }
    if (p.y == 2) {
        switch (p.x) {
        case 0: return 1;
        case 1: return 2;
        case 2: return 3;
        }
    }
    if (p.y == 1) {
        switch (p.x) {
        case 0: return 4;
        case 1: return 5;
        case 2: return 6;
        }
    }
    if (p.y == 0) {
        switch (p.x) {
        case 0: return 7;
        case 1: return 8;
        case 2: return 9;
        }
    }
    return -1;
}

P getPositionDirect(int64_t number) {
    switch (number) {
    case A: return P{ 2, 0 };
    case UP: return P{ 1, 0 };
    case LEFT: return P{ 0, 1 };
    case DOWN: return P{ 1, 1 };
    case RIGHT: return P{ 2, 1 };
    }
    return P{ 0, 0 };
}

int64_t getNumberDirect(P p) {
    if (p.y == 0) {
        switch (p.x) {
        case 0: return -1;
        case 1: return UP;
        case 2: return A;
        }
    }
    if (p.y == 1) {
        switch (p.x) {
        case 0: return LEFT;
        case 1: return DOWN;
        case 2: return RIGHT;
        }
    }
    return -1;
}

int64_t maxLevel = 25;

void computePriceDirect(int64_t level) {
    if (level == 0) {
        priceDirect[0].resize(5);
        for (int64_t x = 0; x < 5; ++x) {
            priceDirect[0][x].resize(5);
            for (int64_t y = 0; y < 5; ++y) {
                priceDirect[0][x][y] = 1;
            }
        }
        return;
    }
    computePriceDirect(level - 1);
    priceDirect[level].resize(5);
    for (int64_t start = 0; start < 5; ++start) {
        vector<vector<int64_t>> test(5, vector<int64_t>(5, -1));
        vector<E> stack;
        stack.push_back(E{ start, 0, A });
        priceDirect[level][start].resize(5, -1);
        auto& out = priceDirect[level][start];
        while (!stack.empty()) {
            E e = stack.back();
            stack.pop_back();
            int64_t scorePress = e.score + priceDirect[level - 1][e.prevKey][A];
            if (test[e.key][e.prevKey] != -1 && test[e.key][e.prevKey] <= e.score) {
                continue;
            }
            test[e.key][e.prevKey] = e.score;
            if (out[e.key] == -1 || out[e.key] >= scorePress) {
                out[e.key] = scorePress;
            }
            P curr = getPositionDirect(e.key);
            const auto add = [&](P p, int64_t direction) {
                int64_t key = getNumberDirect(p);
                if (key != -1) {
                    stack.push_back(E{ key, e.score + priceDirect[level - 1][e.prevKey][direction], direction });
                }
            };
            add(P{ curr.x, curr.y - 1 }, UP);
            add(P{ curr.x, curr.y + 1 }, DOWN);
            add(P{ curr.x - 1, curr.y }, LEFT);
            add(P{ curr.x + 1, curr.y }, RIGHT);
        }
    }
}

void compute(int64_t start, vector<int64_t>& out) {
    vector<E> stack;
    stack.push_back(E{ start, 0, A });
    out.resize(11, -1);
    vector<vector<int64_t>> test(11, vector<int64_t>(5, -1));
    while (!stack.empty()) {
        E e = stack.back();
        stack.pop_back();
        int64_t scorePress = e.score + priceDirect[maxLevel][e.prevKey][A];
        if (test[e.key][e.prevKey] != -1 && test[e.key][e.prevKey] <= e.score) {
            continue;
        }
        test[e.key][e.prevKey] = e.score;
        if (out[e.key] == -1 || out[e.key] >= scorePress) {
            out[e.key] = scorePress;
        }
        P curr = getPositionAlpha(e.key);
        const auto add = [&](P p, int64_t direction) {
            int64_t key = getNumberAlpha(p);
            if (key != -1) {
                stack.push_back(E{ key, e.score + priceDirect[maxLevel][e.prevKey][direction], direction });
            }
        };
        add(P{ curr.x, curr.y - 1 }, UP);
        add(P{ curr.x, curr.y + 1 }, DOWN);
        add(P{ curr.x - 1, curr.y }, LEFT);
        add(P{ curr.x + 1, curr.y }, RIGHT);
    }
}

int main()
{
    ifstream stream("input21.txt");
    if (!stream.is_open()) {
        cerr << "Error: Could not open input21.txt" << endl;
        return 1;
    }

    string line;
    int64_t sum = 0;
    priceDirect.resize(maxLevel + 1);
    computePriceDirect(maxLevel);
    vector<vector<int64_t>> price(11, vector<int64_t>());
    for (int64_t i = 0; i < price.size(); ++i) {
        compute(i, price[i]);
    }
    while (getline(stream, line)) {
        int64_t value = 0;
        int64_t prev = 10;
        int64_t num = 0;
        for (size_t i = 0; i < line.size(); ++i) {
            if (line[i] == 'A') {
                if (price[prev].size() > 10) { // Ensure index 10 exists
                    value += price[prev][10];
                } else {
                    cerr << "Error: price[" << prev << "][10] is out of bounds." << endl;
                    return 1;
                }
                prev = 10;
            }
            else if (isdigit(line[i])) {
                int64_t curr = line[i] - '0';
                if (prev < price.size() && curr < price[prev].size()) { // Ensure indices are within bounds
                    value += price[prev][curr];
                } else {
                    cerr << "Error: price[" << prev << "][" << curr << "] is out of bounds." << endl;
                    return 1;
                }
                prev = curr;
                num = 10 * num + curr;
            }
            else {
                cerr << "Error: Invalid character '" << line[i] << "' in input." << endl;
                return 1;
            }
        }
        sum += value * num;
    }

    cout << sum << endl;
    return 0;
}
