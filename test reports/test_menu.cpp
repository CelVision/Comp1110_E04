#include <iostream>

int main() {
    std::cout << "Starting program\n" << std::flush;
    
    bool running = true;
    int count = 0;
    
    while (running && count < 5) {
        std::cout << "Loop iteration " << count << "\n" << std::flush;
        std::cout << "Enter choice (1-2): " << std::flush;
        
        int choice;
        std::cin >> choice;
        
        std::cout << "You entered: " << choice << "\n" << std::flush;
        
        if (choice == 2) {
            running = false;
        }
        count++;
    }
    
    std::cout << "Program ending\n" << std::flush;
    return 0;
}
