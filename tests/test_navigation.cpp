#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <cassert>

class Building {
private:
    std::string name;
    std::vector<Building*> neighbors;

public:
    Building(const std::string& name) : name(name) {}

    Building(const std::string& name, const std::vector<Building*>& neighbors) 
        : name(name), neighbors(neighbors) {}

    void setNeighbors(const std::vector<Building*>& neighbors) {
        this->neighbors = neighbors;
    }

    std::string getName() const {
        return name;
    }

    std::vector<Building*> getNeighbors() const {
        return neighbors;
    }

    void addNeighbor(Building* neighbor) {
        if (neighbor == nullptr) return;
        neighbors.push_back(neighbor);
    }

    std::string to_string() const {
        return name;
    }
};

class Path {
private:
    Building* from;
    Building* to;
    float timeSpare;
    float timePopular;
    std::string specials;
    bool ifIndoors;
    bool hasElevatorOrEscalator;
    bool hasStairsOrRamp;

public:
    Path(Building* from, Building* to, float timeSpare, float timePopular,
         const std::string& specials, bool ifIndoors, 
         bool hasElevatorOrEscalator, bool hasStairsOrRamp)
        : from(from), to(to), timeSpare(timeSpare), timePopular(timePopular),
          specials(specials), ifIndoors(ifIndoors), 
          hasElevatorOrEscalator(hasElevatorOrEscalator), hasStairsOrRamp(hasStairsOrRamp) {
        
        if (from == nullptr || to == nullptr) {
            throw std::invalid_argument("Both buildings must be provided");
        }
    }

    Building* getFrom() const { return from; }
    Building* getTo() const { return to; }
    float getTimeSpare() const { return timeSpare; }
    float getTimePopular() const { return timePopular; }
    std::string getSpecials() const { return specials; }
    bool getIfIndoors() const { return ifIndoors; }
    bool getHasElevatorOrEscalator() const { return hasElevatorOrEscalator; }
    bool getHasStairsOrRamp() const { return hasStairsOrRamp; }

    std::string to_string() const {
        std::ostringstream oss;
        oss << from->getName() << " -> " << to->getName()
            << " | Spare: " << timeSpare << "min | Popular: " << timePopular << "min"
            << " | Indoors: " << (ifIndoors ? "Yes" : "No");
            
        if (!specials.empty()) {
            oss << " | " << specials;
        }
        
        return oss.str();
    }
};

// ============================================================================
// TEST SUITE
// ============================================================================

void test_building_constructor_name_only() {
    std::cout << "\n[TEST 1] Building Constructor (name only)...";
    Building b("Main Building");
    assert(b.getName() == "Main Building");
    assert(b.getNeighbors().empty());
    std::cout << " PASS" << std::endl;
}

void test_building_constructor_with_neighbors() {
    std::cout << "[TEST 2] Building Constructor (with neighbors)...";
    Building b1("Building A");
    Building b2("Building B");
    Building b3("Building C");
    
    std::vector<Building*> neighbors = {&b2, &b3};
    Building main("Main", neighbors);
    
    assert(main.getName() == "Main");
    assert(main.getNeighbors().size() == 2);
    assert(main.getNeighbors()[0]->getName() == "Building B");
    assert(main.getNeighbors()[1]->getName() == "Building C");
    std::cout << " PASS" << std::endl;
}

void test_building_set_neighbors() {
    std::cout << "[TEST 3] Building setNeighbors()...";
    Building b1("Building 1");
    Building b2("Building 2");
    Building b3("Building 3");
    
    std::vector<Building*> neighbors = {&b2, &b3};
    b1.setNeighbors(neighbors);
    
    assert(b1.getNeighbors().size() == 2);
    assert(b1.getNeighbors()[0]->getName() == "Building 2");
    std::cout << " PASS" << std::endl;
}

void test_building_add_neighbor() {
    std::cout << "[TEST 4] Building addNeighbor()...";
    Building b1("Building 1");
    Building b2("Building 2");
    Building b3("Building 3");
    
    b1.addNeighbor(&b2);
    assert(b1.getNeighbors().size() == 1);
    assert(b1.getNeighbors()[0]->getName() == "Building 2");
    
    b1.addNeighbor(&b3);
    assert(b1.getNeighbors().size() == 2);
    assert(b1.getNeighbors()[1]->getName() == "Building 3");
    std::cout << " PASS" << std::endl;
}

void test_building_add_null_neighbor() {
    std::cout << "[TEST 5] Building addNeighbor(nullptr)...";
    Building b1("Building 1");
    
    b1.addNeighbor(nullptr);
    assert(b1.getNeighbors().empty());
    std::cout << " PASS" << std::endl;
}

void test_building_to_string() {
    std::cout << "[TEST 6] Building to_string()...";
    Building b("Central Library");
    assert(b.to_string() == "Central Library");
    std::cout << " PASS" << std::endl;
}

void test_path_constructor_valid() {
    std::cout << "[TEST 7] Path Constructor (valid)...";
    Building from("Building A");
    Building to("Building B");
    
    Path p(&from, &to, 5.0, 10.0, "Covered pathway", true, true, false);
    
    assert(p.getFrom()->getName() == "Building A");
    assert(p.getTo()->getName() == "Building B");
    assert(p.getTimeSpare() == 5.0);
    assert(p.getTimePopular() == 10.0);
    assert(p.getSpecials() == "Covered pathway");
    assert(p.getIfIndoors() == true);
    assert(p.getHasElevatorOrEscalator() == true);
    assert(p.getHasStairsOrRamp() == false);
    std::cout << " PASS" << std::endl;
}

void test_path_constructor_null_from() {
    std::cout << "[TEST 8] Path Constructor (null from building)...";
    Building to("Building B");
    
    try {
        Path p(nullptr, &to, 5.0, 10.0, "", true, false, false);
        assert(false && "Should have thrown an exception");
    } catch (const std::invalid_argument&) {
        std::cout << " PASS (exception caught as expected)" << std::endl;
    }
}

void test_path_constructor_null_to() {
    std::cout << "[TEST 9] Path Constructor (null to building)...";
    Building from("Building A");
    
    try {
        Path p(&from, nullptr, 5.0, 10.0, "", true, false, false);
        assert(false && "Should have thrown an exception");
    } catch (const std::invalid_argument&) {
        std::cout << " PASS (exception caught as expected)" << std::endl;
    }
}

void test_path_getters() {
    std::cout << "[TEST 10] Path Getters...";
    Building from("Building A");
    Building to("Building B");
    
    Path p(&from, &to, 30.5, 45.75, "Elevator access", false, true, true);
    
    assert(p.getFrom() != nullptr);
    assert(p.getTo() != nullptr);
    assert(p.getTimeSpare() == 30.5f);
    assert(p.getTimePopular() == 45.75f);
    assert(p.getSpecials() == "Elevator access");
    assert(p.getIfIndoors() == false);
    assert(p.getHasElevatorOrEscalator() == true);
    assert(p.getHasStairsOrRamp() == true);
    std::cout << " PASS" << std::endl;
}

void test_path_to_string() {
    std::cout << "[TEST 11] Path to_string()...";
    Building from("Main Hall");
    Building to("Science Building");
    
    Path p(&from, &to, 5.0, 8.0, "Scenic route", true, false, true);
    
    std::string result = p.to_string();
    assert(result.find("Main Hall") != std::string::npos);
    assert(result.find("Science Building") != std::string::npos);
    assert(result.find("5") != std::string::npos);
    assert(result.find("8") != std::string::npos);
    assert(result.find("Yes") != std::string::npos);
    assert(result.find("Scenic route") != std::string::npos);
    std::cout << " PASS" << std::endl;
}

void test_path_to_string_no_specials() {
    std::cout << "[TEST 12] Path to_string() (no special notes)...";
    Building from("Building X");
    Building to("Building Y");
    
    Path p(&from, &to, 2.5, 3.5, "", false, false, false);
    
    std::string result = p.to_string();
    assert(result.find("Building X") != std::string::npos);
    assert(result.find("Building Y") != std::string::npos);
    assert(result.find("2.5") != std::string::npos);
    assert(result.find("3.5") != std::string::npos);
    assert(result.find("No") != std::string::npos);
    std::cout << " PASS" << std::endl;
}

void test_complex_graph() {
    std::cout << "[TEST 13] Complex Graph Structure...";
    
    // Create a small graph of buildings
    Building central("Central Hub");
    Building lib("Library");
    Building lab("Science Lab");
    Building cafe("Cafe");
    
    // Set up connections
    std::vector<Building*> centralNeighbors = {&lib, &lab};
    central.setNeighbors(centralNeighbors);
    
    lib.addNeighbor(&cafe);
    lab.addNeighbor(&cafe);
    
    // Create paths
    Path p1(&central, &lib, 2.0, 3.0, "Covered", true, false, false);
    Path p2(&central, &lab, 3.0, 4.5, "Outdoor", false, true, false);
    Path p3(&lib, &cafe, 1.5, 2.0, "", true, false, true);
    
    // Verify the graph structure
    assert(central.getNeighbors().size() == 2);
    assert(lib.getNeighbors().size() == 1);
    assert(lab.getNeighbors().size() == 1);
    
    assert(p1.getFrom()->getName() == "Central Hub");
    assert(p2.getTo()->getName() == "Science Lab");
    assert(p3.getIfIndoors() == true);
    
    std::cout << " PASS" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   Navigation Classes Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;

    try {
        test_building_constructor_name_only();
        test_building_constructor_with_neighbors();
        test_building_set_neighbors();
        test_building_add_neighbor();
        test_building_add_null_neighbor();
        test_building_to_string();
        
        test_path_constructor_valid();
        test_path_constructor_null_from();
        test_path_constructor_null_to();
        test_path_getters();
        test_path_to_string();
        test_path_to_string_no_specials();
        
        test_complex_graph();

        std::cout << "\n========================================" << std::endl;
        std::cout << "   ALL TESTS PASSED!" << std::endl;
        std::cout << "========================================" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\n[ERROR] Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
