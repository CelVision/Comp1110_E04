#include <iostream>
#include <vector>
#include <map>

class Test {
public:
    void add(const std::string& name) {
        items.push_back(name);
    }
    
private:
    std::vector<std::string> items;
};

void loadData(Test& t) {
    t.add("item1");
    t.add("item2");
}

int main() {
    std::cout << "Starting\n" << std::flush;
    try {
        Test t;
        std::cout << "Created\n" << std::flush;
        loadData(t);
        std::cout << "Loaded\n" << std::flush;
        
        std::cout << "Before loop\n" << std::flush;
        bool running = true;
        while (running) {
            std::cout << "In loop\n" << std::flush;
            std::cout << "Enter 1 to continue, 2 to exit: " << std::flush;
            int choice;
            std::cin >> choice;
            if (choice == 2) {
                running = false;
            }
        }
        std::cout << "After loop\n" << std::flush;
        return 0;
    } catch (...) {
        std::cerr << "Exception!" << std::endl;
        return 1;
    }
}
