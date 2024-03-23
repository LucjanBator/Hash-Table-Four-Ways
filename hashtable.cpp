#include <iostream>
#include <list>
#include <vector>
#include <functional>
#include <stdexcept>
#include <limits>


class HashTable {
private:
    int size;
    int collisionResolutionMethod;
    std::vector<std::list<int>> table;
    std::function<int(int)> hashFunction;

public:
    HashTable(int size, int collisionResolutionMethod, std::function<int(int)> hashFunction)
        : size(size), collisionResolutionMethod(collisionResolutionMethod), table(size), hashFunction(hashFunction) {}

    void insert(int element) {
        int index = hashFunction(element) % size;

        if (collisionResolutionMethod == 1) {
            table[index].push_back(element); // Metoda łańcuchowa
        }
        else {
            int i = 1;
            while (table[index].size() != 0 && table[index].front() != -1) {
                if (collisionResolutionMethod == 2) {
                    index = (index + i) % size; // Adresowanie liniowe
                }
                else if (collisionResolutionMethod == 3) {
                    index = (index + i * i) % size; // Adresowanie kwadratowe
                }
                else if (collisionResolutionMethod == 4) {
                    index = (index + i * hashFunction(element)) % size; // Haszowanie dwukrotne
                }
                i++;
            }
            table[index].push_back(element);
        }
    }

    void remove(int element) {
        int index = hashFunction(element) % size;

        if (collisionResolutionMethod == 1) {
            table[index].remove(element); // Metoda łańcuchowa
        }
        else {
            int i = 1;
            while (table[index].size() != 0) {
                if (table[index].front() == element) {
                    table[index].front() = -1; // Oznaczamy jako usunięte
                    break;
                }
                if (collisionResolutionMethod == 2) {
                    index = (index + i) % size; // Adresowanie liniowe
                }
                else if (collisionResolutionMethod == 3) {
                    index = (index + i * i) % size; // Adresowanie kwadratowe
                }
                else if (collisionResolutionMethod == 4) {
                    index = (index + i * hashFunction(element)) % size; // Haszowanie dwukrotne
                }
                i++;
            }
        }
    }

    int search(int element) {
        int index = hashFunction(element) % size;

        if (collisionResolutionMethod == 1) {
            // Wyszukiwanie dla metody łańcuchowej
            if (table[index].size() > 0) {
                return index;
            }
        }
        else {
            int i = 1;
            while (table[index].size() != 0) {
                if (table[index].front() == element) {
                    return index;
                }
                if (collisionResolutionMethod == 2) {
                    index = (index + i) % size; // Adresowanie liniowe
                }
                else if (collisionResolutionMethod == 3) {
                    index = (index + i * i) % size; // Adresowanie kwadratowe
                }
                else if (collisionResolutionMethod == 4) {
                    index = (index + i * hashFunction(element)) % size; // Haszowanie dwukrotne
                }
                i++;
            }
        }
        return -1; // Element nieznaleziony
    }

    void display() {
        std::cout << "[";
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                std::cout << ", ";
            }
            if (table[i].empty()) {
                std::cout << "None";
            }
            else {
                std::cout << "[";
                for (const auto& element : table[i]) {
                    if (element != -1) {
                        std::cout << element << ", ";
                    }
                }
                std::cout << "]";
            }
        }
        std::cout << "]" << std::endl;
    }

    int getSize() const {
        return size;
    }
};

int main() {
    int size, collisionResolutionMethod;
    std::cin >> size >> collisionResolutionMethod;

    HashTable hashTable(size, collisionResolutionMethod, [](int value)  {
        return value; // Prosta funkcja haszująca, używamy wartości jako indeksu bez modyfikacji
    });

    int element;
    while (std::cin >> element) {
        if (element == -1) {
            break;
        }
        hashTable.insert(element);
    }

    hashTable.display();

    int operation, searchElement;

    while (true) {
        try {
            if (!(std::cin >> operation)) {
                throw std::runtime_error("Nieprawidłowe dane wejściowe.");
            }

            if (operation == -1) {
                break;
            } else if (operation == 0) {
                int element;
                if (!(std::cin >> element)) {
                    throw std::runtime_error("Nieprawidłowe dane wejściowe.");
                }
                hashTable.insert(element);
                hashTable.display();
            } else if (operation == 1) {
                if (!(std::cin >> searchElement)) {
                    throw std::runtime_error("Nieprawidłowe dane wejściowe.");
                }
                int position = hashTable.search(searchElement);
                std::cout << position << std::endl;
            } else if (operation == 2) {
                int element;
                if (!(std::cin >> element)) {
                    throw std::runtime_error("Nieprawidłowe dane wejściowe.");
                }
                hashTable.remove(element);
                hashTable.display();
            } else {
                std::cout << "Nieprawidłowa liczba, kliknij enter aby móc wybrać jeszcze raz." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Wystąpił błąd: " << e.what() << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return 0;
}