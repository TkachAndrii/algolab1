#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <limits>
#include <ctime>

class PerfectHash {
public:
    PerfectHash(const std::vector<double>& input, int a, int b, int p)
            : a(a), b(b), p(p), emptyMarker(std::numeric_limits<double>::infinity()) {

        std::unordered_map<double, bool> unique;
        for (double num : input) {
            unique[num] = true;
        }

        mainTable.resize(unique.size());

        for (const auto& pair : unique) {
            double num = pair.first;
            int index = hash(num, a, b, mainTable.size());
            mainTable[index].push_back(num);
        }

        subTables.resize(mainTable.size());
        for (size_t i = 0; i < mainTable.size(); ++i) {
            if (mainTable[i].size() > 1) {
                createSubTable(i);
            } else if (!mainTable[i].empty()) {
                subTables[i] = { mainTable[i][0] };
            }
        }
    }

    void printMainTableWithCollisions() const {
        std::cout << "Main hash table (with collisions):\n";
        for (size_t i = 0; i < mainTable.size(); ++i) {
            std::cout << "Cell " << i << ": ";
            for (double num : mainTable[i]) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }
    }

    void printSubTables() const {
        std::cout << "Sub hash tables (size n^2):\n";
        for (size_t i = 0; i < subTables.size(); ++i) {
            if (subTables[i].size() > 1) {
                std::cout << "Cell " << i << ": ";
                for (double num : subTables[i]) {
                    if (num != emptyMarker) {
                        std::cout << num << " ";
                    } else {
                        std::cout << "_ ";
                    }
                }
                std::cout << std::endl;
            }
        }
    }

private:
    std::vector<std::vector<double>> mainTable;
    std::vector<std::vector<double>> subTables;
    int a, b, p;
    double emptyMarker;

    int hash(double num, int a, int b, int tableSize) const {
        int h = static_cast<int>(std::fmod((a * num + b), p));
        if (h < 0) {
            h += p;
        }
        return h % tableSize;
    }

    void createSubTable(int index) {
        int subTableSize = mainTable[index].size() * mainTable[index].size();
        std::vector<double> subTable(subTableSize, emptyMarker);

        bool collisionResolved = false;
        srand(static_cast<unsigned>(time(0)));  // Ініціалізація випадкового зерна

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
    std::vector<double> input = {1, 2.2, 3, 4, sqrt(5), 6, -7, 8, 9, 10};
    int a = 1, b = 2, p = 101;
    PerfectHash perfectHash(input, a, b, p);
    std::cout << "The hash table has been created successfully.\n";
    perfectHash.printMainTableWithCollisions();
    perfectHash.printSubTables();
    return 0;
}
