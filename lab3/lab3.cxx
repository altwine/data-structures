#include <iostream>
#include <vector>
#include <deque>
#include <chrono>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace std::chrono;

vector<bool> simulateArray(const vector<bool>& initial, int S, int K, int& finalCount) {
    vector<bool> coins = initial;
    const int totalCoins = coins.size();
    int currentPos = 0;
    for (int move = 0; move < K; ++move) {
        int step = 0;
        const int start = currentPos;
        bool fullCircle = false;
        while (step < S) {
            if (coins[currentPos]) step++;
            currentPos = (currentPos + 1) % totalCoins;
            if (currentPos == start && step < S) {
                if (fullCircle) {
                    finalCount = -1;
                    return {};
                }
                fullCircle = true;
            }
        }
        const int flipPos = (currentPos - 1 + totalCoins) % totalCoins;
        coins[flipPos] = !coins[flipPos];
    }
    finalCount = count(coins.begin(), coins.end(), true);
    return coins;
}

vector<bool> simulateLinkedList(const vector<bool>& initial, int S, int K, int& finalCount) {
    struct Node {
        bool isHeads;
        Node* next;
        Node(bool state) : isHeads(state), next(nullptr) {}
    };
    Node* head = new Node(initial[0]);
    Node* current = head;
    for (size_t i = 1; i < initial.size(); ++i) {
        current->next = new Node(initial[i]);
        current = current->next;
    }
    current->next = head;
    current = head;
    for (int move = 0; move < K; ++move) {
        int step = 0;
        Node* prev = nullptr;
        Node* start = current;
        bool fullCircle = false;
        while (step < S) {
            if (current->isHeads) {
                step++;
            }
            prev = current;
            current = current->next;
            if (current == start && step < S) {
                if (fullCircle) {
                    Node* toDelete = head->next;
                    while (toDelete != head) {
                        Node* temp = toDelete;
                        toDelete = toDelete->next;
                        delete temp;
                    }
                    delete head;
                    finalCount = -1;
                    return {};
                }
                fullCircle = true;
            }
        }
        prev->isHeads = !prev->isHeads;
    }
    vector<bool> result;
    current = head;
    do {
        result.push_back(current->isHeads);
        current = current->next;
    } while (current != head);
    finalCount = count(result.begin(), result.end(), true);
    Node* toDelete = head->next;
    while (toDelete != head) {
        Node* temp = toDelete;
        toDelete = toDelete->next;
        delete temp;
    }
    delete head;
    return result;
}

vector<bool> simulateDeque(const vector<bool>& initial, int S, int K, int& finalCount) {
    deque<bool> coins(initial.begin(), initial.end());
    const int totalCoins = coins.size();
    int currentPos = 0;
    for (int move = 0; move < K; ++move) {
        int step = 0;
        const int start = currentPos;
        bool fullCircle = false;
        while (step < S) {
            if (coins[currentPos]) step++;
            currentPos = (currentPos + 1) % totalCoins;
            if (currentPos == start && step < S) {
                if (fullCircle) {
                    finalCount = -1;
                    return {};
                }
                fullCircle = true;
            }
        }
        const int flipPos = (currentPos - 1 + totalCoins) % totalCoins;
        coins[flipPos] = !coins[flipPos];
    }
    finalCount = count(coins.begin(), coins.end(), true);
    return vector<bool>(coins.begin(), coins.end());
}

vector<vector<bool>> generateInitialArrangements(int N, int M) {
    vector<vector<bool>> result;
    int total = N + M;
    for (int heads = 0; heads <= N; ++heads) {
        vector<bool> arrangement(total, false);
        fill(arrangement.begin(), arrangement.begin() + heads, true);
        do {
            result.push_back(arrangement);
        } while (prev_permutation(arrangement.begin(), arrangement.end()));
    }
    return result;
}

int main() {
    int N, M, S, K, L;
    cout << "Enter N M S K L: " << endl;
    cin >> N >> M >> S >> K >> L;

    // Прегенерация расположений
    cout << "\n1. Pregeneration" << endl;
    auto start_gen = high_resolution_clock::now();
    vector<vector<bool>> initialArrangements = generateInitialArrangements(N, M);
    auto end_gen = high_resolution_clock::now();
    auto duration_gen = duration_cast<milliseconds>(end_gen - start_gen);
    cout << "\tGenerated " << initialArrangements.size() << " arrangements in " 
         << duration_gen.count() << "ms" << endl;

    bool found = false;
    vector<bool> solution;

    // Замеры для массива
    start_gen = high_resolution_clock::now();
    for (const auto& initial : initialArrangements) {
        int count;
        vector<bool> finalState = simulateArray(initial, S, K, count);
        if (count == L) {
            solution = initial;
            found = true;
            break;
        }
    }
    end_gen = high_resolution_clock::now();
    duration_gen = duration_cast<milliseconds>(end_gen - start_gen);
    cout << "\n2. Array time: " << duration_gen.count() << "ms" << endl;
    if (found) {
        cout << "\tArray solution found: ";
        for (bool b : solution) cout << (b ? "H" : "T");
        cout << endl;
    } else {
        cout << "\tNo array solution found." << endl;
    }

    // Замеры для связного списка
    found = false;
    start_gen = high_resolution_clock::now();
    for (const auto& initial : initialArrangements) {
        int count;
        vector<bool> finalState = simulateLinkedList(initial, S, K, count);
        if (count == L) {
            solution = initial;
            found = true;
            break;
        }
    }
    end_gen = high_resolution_clock::now();
    duration_gen = duration_cast<milliseconds>(end_gen - start_gen);
    cout << "\n3. Linked List time: " << duration_gen.count() << "ms" << endl;
    if (found) {
        cout << "\tLinked List solution found: ";
        for (bool b : solution) cout << (b ? "H" : "T");
        cout << endl;
    } else {
        cout << "\tNo linked list solution found." << endl;
    }

    // Замеры для двусторонней очереди
    found = false;
    start_gen = high_resolution_clock::now();
    for (const auto& initial : initialArrangements) {
        int count;
        vector<bool> finalState = simulateDeque(initial, S, K, count);
        if (count == L) {
            solution = initial;
            found = true;
            break;
        }
    }
    end_gen = high_resolution_clock::now();
    duration_gen = duration_cast<milliseconds>(end_gen - start_gen);
    cout << "\n4. Deque time: " << duration_gen.count() << "ms" << endl;
    if (found) {
        cout << "\tDeque solution found: ";
        for (bool b : solution) cout << (b ? "H" : "T");
        cout << endl;
    } else {
        cout << "\tNo deque solution found." << endl;
    }

    return 0;
}