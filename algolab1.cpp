#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <limits>

class PerfectHash {
public:
    PerfectHash(const std::vector<double>& input, int a, int b, int p)
            : a(a), b(b), p(p), emptyMarker(std::numeric_limits<double>::infinity()) {

        // Remove duplicates and initialize main table
        std::unordered_map<double, bool> unique;
        for (double num : input) {
            unique[num] = true;
        }

        mainTable.resize(unique.size());

        // First level hashing
        for (const auto& pair : unique) {
            double num = pair.first;
            int index = hash(num, a, b, mainTable.size());
            mainTable[index].push_back(num);
        }

        // Second level hashing to resolve collisions
        subTables.resize(mainTable.size());
        for (size_t i = 0; i < mainTable.size(); ++i) {
            if (mainTable[i].size() > 1) {
                handleCollision(i);
            } else if (!mainTable[i].empty()) {
                subTables[i] = { mainTable[i][0] };
            }
        }
    }

    void printHashTable() const {
        for (size_t i = 0; i < subTables.size(); ++i) {
            std::cout << "Cell " << i << ": ";
            for (double num : subTables[i]) {
                if (num != emptyMarker) {
                    std::cout << num << " ";
                }
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<std::vector<double>> mainTable;
    std::vector<std::vector<double>> subTables;
    int a, b, p;
    double emptyMarker;

    int hash(double num, int a, int b, int tableSize) const {
        return static_cast<int>(std::fmod((a * num + b), p)) % tableSize;
    }

    void handleCollision(int index) {
        int subTableSize = mainTable[index].size() * mainTable[index].size();
        std::vector<double> subTable(subTableSize, emptyMarker);

        bool collisionResolved = false;
        srand(static_cast<unsigned>(time(0)));  // Initialize random seed

        while (!collisionResolved) {
            subTable.assign(subTableSize, emptyMarker);
            int subA = rand() % p;
            int subB = rand() % p;
            collisionResolved = true;

            for (double num : mainTable[index]) {
                int subIndex = hash(num, subA, subB, subTableSize);
                if (subTable[subIndex] == emptyMarker) {
                    subTable[subIndex] = num;
                } else {
                    collisionResolved = false;
                    break;
                }
            }
        }

        subTables[index] = subTable;
    }
};

int main() {
    std::vector<double> input = {1, 2, 3};
    int a = 1, b = 2, p = 7;
    PerfectHash perfectHash(input, a, b, p);
    std::cout << "The hash table has been created successfully.\n";
    perfectHash.printHashTable();
    return 0;
}