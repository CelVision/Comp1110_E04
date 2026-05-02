#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <limits>
#include "navigation.cpp"

/**
 * Helper function to find the data directory
 * Tries multiple paths: ./data/, ../data/, and absolute path
 */
std::string findDataPath(const std::string& filename) {
    // Try current directory
    std::ifstream test1("./data/" + filename);
    if (test1.good()) {
        test1.close();
        return "./data/" + filename;
    }
    
    // Try parent directory
    std::ifstream test2("../data/" + filename);
    if (test2.good()) {
        test2.close();
        return "../data/" + filename;
    }
    
    // Try absolute path (Windows specific - adjust for your system)
    std::string absPath = "D:\\Comp1110_E04\\data\\" + filename;
    std::ifstream test3(absPath);
    if (test3.good()) {
        test3.close();
        return absPath;
    }
    
    // Default to ./data/ path
    return "./data/" + filename;
}

/**
 * NavigationSystem manages the campus navigation graph and pathfinding.
 */
class NavigationSystem {
private:
    std::map<std::string, Building*> buildings;
    std::vector<Path*> paths;
    
public:
    /**
     * Constructor
     */
    NavigationSystem() {}
    
    /**
     * Destructor - clean up memory
     */
    ~NavigationSystem() {
        for (auto& pair : buildings) {
            delete pair.second;
        }
        for (auto& path : paths) {
            delete path;
        }
    }
    
    /**
     * Add a building to the system
     */
    void addBuilding(const std::string& name) {
        if (buildings.find(name) == buildings.end()) {
            buildings[name] = new Building(name);
        }
    }
    
    /**
     * Connect two buildings
     */
    void connectBuildings(const std::string& from, const std::string& to) {
        if (buildings.find(from) != buildings.end() && 
            buildings.find(to) != buildings.end()) {
            buildings[from]->addNeighbor(buildings[to]);
        }
    }
    
    /**
     * Add a path between two buildings
     */
    void addPath(const std::string& fromName, const std::string& toName, 
                 float timeSpare, float timePopular, const std::string& specials,
                 bool ifIndoors, bool hasElevator, bool hasStairs) {
        if (buildings.find(fromName) != buildings.end() && 
            buildings.find(toName) != buildings.end()) {
            Path* newPath = new Path(buildings[fromName], buildings[toName],
                                     timeSpare, timePopular, specials,
                                     ifIndoors, hasElevator, hasStairs);
            paths.push_back(newPath);
        }
    }
    
    /**
     * Get all building names
     */
    std::vector<std::string> getAllBuildingNames() const {
        std::vector<std::string> names;
        for (const auto& pair : buildings) {
            names.push_back(pair.first);
        }
        std::sort(names.begin(), names.end());
        return names;
    }
    
    /**
     * Find the SHORTEST TIME path between two buildings using Dijkstra
     * (CORRECT for weighted graph - uses timeSpare as weight)
     * preference: 0 = no preference, 1 = avoid outdoors, 2 = avoid stairs
     */
    std::vector<std::string> findPath(const std::string& start, const std::string& end, int preference = 0) {
        if (buildings.find(start) == buildings.end() ||
            buildings.find(end) == buildings.end()) {
            return {};
        }
    
        // initialize Dijkstra's algorithm
        std::map<std::string, float> dist;
        std::map<std::string, std::string> parent;
        std::map<std::string, bool> visited;
    
        for (auto& pair : buildings) {
            std::string name = pair.first;
            dist[name] = 1e9; 
            parent[name] = "";
            visited[name] = false;
        }
        dist[start] = 0.0f;
    

        using P = std::pair<float, std::string>;
        std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
        pq.push({0.0f, start});
    
        while (!pq.empty()) {
            P top = pq.top();
            std::string current = top.second;
            pq.pop();
    
            if (visited[current]) continue;
            visited[current] = true;
    
            if (current == end) break; // exit if we reached the destination
    
            // go through neighbors
            Building* b = buildings[current];
            for (Building* neighbor : b->getNeighbors()) {
                std::string next = neighbor->getName();
    
                // get the weight
                Path* p = findPathInfo(current, next);
                if (!p) continue;
    
                float weight = p->getTimeSpare();
                
                // Apply preference multiplier
                if (preference == 1 && !p->getIfIndoors()) {
                    // Preference 1: Avoid outdoors - multiply outdoor paths by 5
                    weight *= 5.0f;
                } else if (preference == 2 && p->getHasStairsOrRamp()) {
                    // Preference 2: Avoid stairs - multiply paths with stairs by 5
                    weight *= 5.0f;
                }
    

                if (dist[next] > dist[current] + weight) {
                    dist[next] = dist[current] + weight;
                    parent[next] = current;
                    pq.push({dist[next], next});
                }
            }
        }
    
        // no path found
        if (parent[end].empty()) {
            return {};
        }
    
        // get path
        std::vector<std::string> path;
        std::string node = end;
        while (!node.empty()) {
            path.push_back(node);
            node = parent[node];
        }
        std::reverse(path.begin(), path.end());
    
        return path;
    }
    
    /**
     * Find path information (time, notes) between two buildings
     */
    Path* findPathInfo(const std::string& fromName, const std::string& toName) {
        for (Path* path : paths) {
            if (path->getFrom()->getName() == fromName && 
                path->getTo()->getName() == toName) {
                return path;
            }
        }
        return nullptr;
    }
    
    /**
     * Calculate route with times
     */
    struct RouteInfo {
        struct PathSegment {
            std::string from;
            std::string to;
            float timeSpare;
            float timePopular;
            std::string specials;
            bool indoors;
            bool elevator;
            bool stairs;
        };
        
        std::vector<std::string> route;
        float totalTimeSpare;
        float totalTimePopular;
        std::vector<PathSegment> segments;
    };
    
    RouteInfo calculateRoute(const std::string& start, const std::string& end, int preference = 0) {
        RouteInfo info;
        info.totalTimeSpare = 0;
        info.totalTimePopular = 0;
        
        info.route = findPath(start, end, preference);
        
        if (info.route.empty()) {
            return info;
        }
        
        // Calculate times for each segment (always use original times for display)
        for (size_t i = 0; i < info.route.size() - 1; ++i) {
            Path* pathInfo = findPathInfo(info.route[i], info.route[i + 1]);
            if (pathInfo) {
                // Always use original times for display
                float timeSpareWithReward = pathInfo->getTimeSpare();
                float timePopularWithReward = pathInfo->getTimePopular();
                
                // Add 10 seconds reward/penalty for elevator or indoor paths
                if (pathInfo->getHasElevatorOrEscalator() || pathInfo->getIfIndoors()) {
                    timeSpareWithReward -= 10.0f / 60.0f;
                    timePopularWithReward -= 10.0f / 60.0f;
                }
                
                info.totalTimeSpare += timeSpareWithReward;
                info.totalTimePopular += timePopularWithReward;
                
                RouteInfo::PathSegment segment;
                segment.from = info.route[i];
                segment.to = info.route[i + 1];
                segment.timeSpare = pathInfo->getTimeSpare();
                segment.timePopular = pathInfo->getTimePopular();
                segment.specials = pathInfo->getSpecials();
                segment.indoors = pathInfo->getIfIndoors();
                segment.elevator = pathInfo->getHasElevatorOrEscalator();
                segment.stairs = pathInfo->getHasStairsOrRamp();
                
                info.segments.push_back(segment);
            }
        }
        
        return info;
    }
};

/**
 * Read buildings from node.txt file
 */
std::vector<std::string> readBuildingsFromFile(const std::string& filename) {
    std::vector<std::string> buildings;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return buildings;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        if (!line.empty()) {
            buildings.push_back(line);
        }
    }
    
    file.close();
    return buildings;
}

/**
 * Read neighbor connections from neighbor.txt file
 */
std::map<std::string, std::vector<std::string>> readNeighborsFromFile(const std::string& filename) {
    std::map<std::string, std::vector<std::string>> neighbors;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return neighbors;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Format: "Building : Neighbor1, Neighbor2, ..."
        size_t colonPos = line.find(":");
        if (colonPos == std::string::npos) continue;
        
        std::string building = line.substr(0, colonPos);
        std::string neighborsList = line.substr(colonPos + 1);
        
        // Trim building name
        building.erase(0, building.find_first_not_of(" \t\r\n"));
        building.erase(building.find_last_not_of(" \t\r\n") + 1);
        
        // Parse neighbors
        std::vector<std::string> buildingNeighbors;
        std::stringstream ss(neighborsList);
        std::string neighbor;
        
        while (std::getline(ss, neighbor, ',')) {
            // Trim neighbor name
            neighbor.erase(0, neighbor.find_first_not_of(" \t\r\n"));
            neighbor.erase(neighbor.find_last_not_of(" \t\r\n") + 1);
            
            if (!neighbor.empty()) {
                buildingNeighbors.push_back(neighbor);
            }
        }
        
        if (!building.empty() && !buildingNeighbors.empty()) {
            neighbors[building] = buildingNeighbors;
        }
    }
    
    file.close();
    return neighbors;
}

/**
 * Read paths from Paths.txt file
 */
struct PathData {
    std::string from;
    std::string to;
    float timeSpare;
    float timePopular;
    std::string specials;
    bool isIndoors;
    bool hasStairs;
    bool hasElevator;
};

std::vector<PathData> readPathsFromFile(const std::string& filename) {
    std::vector<PathData> paths;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return paths;
    }
    
    std::string line;
    bool firstLine = true;
    
    while (std::getline(file, line)) {
        // Skip header line
        if (firstLine) {
            firstLine = false;
            continue;
        }
        
        // Skip empty lines
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string field;
        PathData pathData;
        int fieldCount = 0;
        
        while (std::getline(ss, field, ',') && fieldCount < 8) {
            // Trim field
            field.erase(0, field.find_first_not_of(" \t\r\n"));
            field.erase(field.find_last_not_of(" \t\r\n") + 1);
            
            try {
                switch (fieldCount) {
                    case 0: pathData.from = field; break;
                    case 1: pathData.to = field; break;
                    case 2: 
                        pathData.timeSpare = (field.empty() || field == "Nil") ? 0 : std::stof(field);
                        break;
                    case 3: 
                        pathData.timePopular = (field.empty() || field == "Nil") ? 0 : std::stof(field);
                        break;
                    case 4: pathData.specials = field; break;
                    case 5: pathData.isIndoors = (field == "Y" || field == "y"); break;
                    case 6: pathData.hasStairs = (field == "Y" || field == "y"); break;
                    case 7: pathData.hasElevator = (field == "Y" || field == "y"); break;
                }
            } catch (...) {
                // Skip invalid rows
                break;
            }
            
            fieldCount++;
        }
        
        // Only add if we have all required fields
        if (fieldCount == 8 && !pathData.from.empty() && !pathData.to.empty()) {
            paths.push_back(pathData);
        }
    }
    
    file.close();
    return paths;
}

/**
 * Load real data from files
 */
void loadRealData(NavigationSystem& system) {
    // Read buildings
    std::cout << "Reading buildings from node.txt...\n" << std::flush;
    std::vector<std::string> buildings = readBuildingsFromFile(findDataPath("node.txt"));
    std::cout << "  Loaded " << buildings.size() << " buildings\n" << std::flush;
    
    for (const auto& building : buildings) {
        system.addBuilding(building);
    }
    
    // Read neighbors and connect buildings
    std::cout << "Reading connections from neighbor.txt...\n" << std::flush;
    std::map<std::string, std::vector<std::string>> neighbors = readNeighborsFromFile(findDataPath("neighbor.txt"));
    std::cout << "  Loaded " << neighbors.size() << " building connections\n" << std::flush;
    
    for (const auto& pair : neighbors) {
        const std::string& building = pair.first;
        for (const auto& neighbor : pair.second) {
            system.connectBuildings(building, neighbor);
        }
    }
    
    // Read paths
    std::cout << "Reading paths from Paths.txt...\n" << std::flush;
    std::vector<PathData> paths = readPathsFromFile(findDataPath("Paths.txt"));
    std::cout << "  Loaded " << paths.size() << " paths\n" << std::flush;
    
    for (const auto& path : paths) {
        // Convert seconds to minutes for display
        float timeSpareMin = path.timeSpare / 60.0f;
        float timePopularMin = path.timePopular / 60.0f;
        
        system.addPath(path.from, path.to, timeSpareMin, timePopularMin, 
                      path.specials, path.isIndoors, path.hasElevator, path.hasStairs);
    }
}

/**
 * Display preference menu and get user choice
 * Returns: 0 = no preference, 1 = avoid outdoors, 2 = avoid stairs
 */
int displayPreferenceMenu() {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "SELECT YOUR ROUTE PREFERENCE:\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << "1. No preference\n";
    std::cout << "2. Avoid outdoors\n";
    std::cout << "3. Avoid stairs\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << "Choice: ";
    std::cout.flush();
    
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Input error! Please enter a number." << std::endl;
        return 0;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (choice < 1 || choice > 3) {
        std::cout << "Invalid selection! Using no preference.\n";
        return 0;
    }
    
    return choice - 1; // Convert to 0-indexed (0, 1, or 2)
}

/**
 * Display menu and get user choice
 */
int displayMainMenu() {
    std::cout << "Menu about to print" << std::endl;
    std::cout << "\n1. Find Path\n2. List Buildings\n3. Exit\n";
    std::cout << "Choice: ";
    std::cout.flush();
    
    int choice;
    if (!(std::cin >> choice)) {
        // Clear error state and flush input buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Input error! Please enter a number." << std::endl;
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Got choice: " << choice << std::endl;
    return choice;
}

/**
 * Display all buildings with numbering
 */
int selectBuilding(const std::vector<std::string>& buildings, const std::string& prompt) {
    std::cout << "\n" << prompt << "\n";
    std::cout << "=========================================================\n";
    
    for (size_t i = 0; i < buildings.size(); ++i) {
        std::cout << std::setw(3) << (i + 1) << ". " << buildings[i] << "\n";
    }
    
    std::cout << "=========================================================\n";
    std::cout << "Enter building number: ";
    std::cout.flush();
    
    int choice;
    if (!(std::cin >> choice)) {
        // Input failed - clear error state and skip to next line
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input! Please enter a number.\n";
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (choice < 1 || choice > (int)buildings.size()) {
        std::cout << "Invalid selection! Please enter a number between 1 and " << buildings.size() << ".\n";
        return -1;
    }
    
    return choice - 1;
}

/**
 * Display path results
 */
void displayPathResults(const NavigationSystem::RouteInfo& route) {
    if (route.route.empty()) {
        std::cout << "\nNo path found between selected buildings!\n";
        return;
    }
    
    std::cout << "\n";
    std::cout << "=========================================================\n";
    std::cout << "                    NAVIGATION RESULTS                  \n";
    std::cout << "=========================================================\n";
    
    // Route overview
    std::cout << "\nCOMPLETE ROUTE:\n";
    std::cout << "---------\n";
    for (size_t i = 0; i < route.route.size(); ++i) {
        std::cout << "  " << std::setw(2) << (i + 1) << ". " << route.route[i] << "\n";
        if (i < route.route.size() - 1) {
            std::cout << "       |\n";
        }
    }
    
    // Time summary
    std::cout << "\nTIME SUMMARY:\n";
    std::cout << "---------\n";
    std::cout << "  Spare Time (less crowded):    " 
              << std::fixed << std::setprecision(1) << route.totalTimeSpare 
              << " minutes\n";
    std::cout << "  Popular Time (crowded):       " 
              << std::fixed << std::setprecision(1) << route.totalTimePopular 
              << " minutes\n";
    
    // Detailed segments
    std::cout << "\nDETAILED ROUTE BREAKDOWN:\n";
    std::cout << "---------\n";
    
    for (size_t i = 0; i < route.segments.size(); ++i) {
        const auto& segment = route.segments[i];
        
        std::cout << "\n  Segment " << (i + 1) << ":\n";
        std::cout << "    From: " << segment.from << "\n";
        std::cout << "    To:   " << segment.to << "\n";
        std::cout << "    Spare Time: " << std::fixed << std::setprecision(1) 
                  << segment.timeSpare << " min | "
                  << "Popular Time: " << segment.timePopular << " min\n";
        
        // Accessibility info
        std::cout << "    Environment: ";
        if (segment.indoors) {
            std::cout << "Indoor";
        } else {
            std::cout << "Outdoor";
        }
        std::cout << "\n";
        
        std::cout << "    Accessibility: ";
        std::vector<std::string> features;
        if (segment.elevator) features.push_back("Elevator/Escalator");
        if (segment.stairs) features.push_back("Stairs/Ramp");
        if (features.empty()) {
            std::cout << "Standard path";
        } else {
            for (size_t j = 0; j < features.size(); ++j) {
                std::cout << features[j];
                if (j < features.size() - 1) std::cout << " | ";
            }
        }
        std::cout << "\n";
        
        // Special notes
        if (!segment.specials.empty()) {
            std::cout << "    Notes: " << segment.specials << "\n";
        }
    }
    
    std::cout << "\n=========================================================\n";
}

/**
 * Load default test data
 */
void loadTestData(NavigationSystem& system) {
    // Add buildings from the campus
    system.addBuilding("Main Building");
    system.addBuilding("Library Building");
    system.addBuilding("Science Building");
    system.addBuilding("Central Hub");
    system.addBuilding("Cafeteria");
    system.addBuilding("Sports Center");
    system.addBuilding("Student Center");
    
    // Connect buildings
    system.connectBuildings("Main Building", "Library Building");
    system.connectBuildings("Main Building", "Central Hub");
    system.connectBuildings("Library Building", "Science Building");
    system.connectBuildings("Central Hub", "Cafeteria");
    system.connectBuildings("Cafeteria", "Sports Center");
    system.connectBuildings("Central Hub", "Student Center");
    system.connectBuildings("Science Building", "Sports Center");
    
    // Add path details
    system.addPath("Main Building", "Library Building", 5.0, 8.0, "Covered walkway", true, true, false);
    system.addPath("Main Building", "Central Hub", 3.0, 5.0, "", true, false, false);
    system.addPath("Library Building", "Science Building", 10.0, 15.0, "Outdoor", false, false, true);
    system.addPath("Central Hub", "Cafeteria", 2.0, 3.0, "Indoor corridor", true, true, false);
    system.addPath("Cafeteria", "Sports Center", 7.0, 10.0, "Scenic route", false, false, true);
    system.addPath("Central Hub", "Student Center", 4.0, 6.0, "Covered path", true, false, false);
    system.addPath("Science Building", "Sports Center", 8.0, 12.0, "", false, false, true);
}

/**
 * Main program
 */
int main() {
    std::cout << "Loading campus data...\n" << std::flush;
    
    NavigationSystem system;
    loadRealData(system);
    std::cout << "Campus data loaded successfully!\n" << std::flush;
    
    bool running = true;
    while (running) {
        int choice = displayMainMenu();
        
        // Skip invalid input and retry
        if (choice == -1) {
            continue;
        }
        
        switch (choice) {
            case 1: {
                // Find path
                std::vector<std::string> buildings = system.getAllBuildingNames();
                
                int startIdx = selectBuilding(buildings, "SELECT STARTING BUILDING:");
                if (startIdx == -1) {
                    std::cout << "Please try again.\n";
                    break;
                }
                
                int endIdx = selectBuilding(buildings, "SELECT ENDING BUILDING:");
                if (endIdx == -1) {
                    std::cout << "Please try again.\n";
                    break;
                }
                
                std::string startBuilding = buildings[startIdx];
                std::string endBuilding = buildings[endIdx];
                
                if (startBuilding == endBuilding) {
                    std::cout << "\nStart and end buildings must be different!\n";
                    break;
                }
                
                // Display preference menu
                int preference = displayPreferenceMenu();
                
                std::cout << "\nCalculating route...\n";
                NavigationSystem::RouteInfo route = system.calculateRoute(startBuilding, endBuilding, preference);
                displayPathResults(route);
                break;
            }
            
            case 2: {
                // List all buildings
                std::vector<std::string> buildings = system.getAllBuildingNames();
                std::cout << "\nAvailable Buildings:\n";
                    std::cout << "=========================================\n";
                    for (size_t i = 0; i < buildings.size(); ++i) {
                        std::cout << std::setw(2) << (i + 1) << ". " << buildings[i] << "\n";
                    }
                    std::cout << "=========================================\n";
                break;
            }
            
            case 3: {
                // Exit
                std::cout << "\nThank you for using HKU Campus Navigation System!\n";
                running = false;
                break;
            }
            
            default:
                std::cout << "\nInvalid choice! Please enter 1-3.\n";
        }
    }
    
    return 0;
}
