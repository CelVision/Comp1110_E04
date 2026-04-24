#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "navigation.cpp"

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
     * Find a path between two buildings using BFS
     */
    std::vector<std::string> findPath(const std::string& start, const std::string& end) {
        if (buildings.find(start) == buildings.end() || 
            buildings.find(end) == buildings.end()) {
            return {};
        }
        
        std::map<std::string, std::string> parent;
        std::map<std::string, bool> visited;
        std::queue<std::string> q;
        
        q.push(start);
        visited[start] = true;
        
        while (!q.empty()) {
            std::string current = q.front();
            q.pop();
            
            if (current == end) {
                // Reconstruct path
                std::vector<std::string> path;
                std::string node = end;
                while (node != start) {
                    path.push_back(node);
                    node = parent[node];
                }
                path.push_back(start);
                std::reverse(path.begin(), path.end());
                return path;
            }
            
            Building* building = buildings[current];
            for (Building* neighbor : building->getNeighbors()) {
                std::string neighborName = neighbor->getName();
                if (!visited[neighborName]) {
                    visited[neighborName] = true;
                    parent[neighborName] = current;
                    q.push(neighborName);
                }
            }
        }
        
        return {};  // No path found
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
        std::vector<std::string> route;
        float totalTimeSpare;
        float totalTimePopular;
        std::vector<PathSegment> segments;
        
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
    };
    
    RouteInfo calculateRoute(const std::string& start, const std::string& end) {
        RouteInfo info;
        info.totalTimeSpare = 0;
        info.totalTimePopular = 0;
        
        info.route = findPath(start, end);
        
        if (info.route.empty()) {
            return info;
        }
        
        // Calculate times for each segment
        for (size_t i = 0; i < info.route.size() - 1; ++i) {
            Path* pathInfo = findPathInfo(info.route[i], info.route[i + 1]);
            if (pathInfo) {
                info.totalTimeSpare += pathInfo->getTimeSpare();
                info.totalTimePopular += pathInfo->getTimePopular();
                
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
 * Display menu and get user choice
 */
int displayMainMenu() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════╗\n";
    std::cout << "║     HKU CAMPUS NAVIGATION SYSTEM               ║\n";
    std::cout << "╠════════════════════════════════════════════════╣\n";
    std::cout << "║  1. Find Path Between Buildings                ║\n";
    std::cout << "║  2. List All Buildings                         ║\n";
    std::cout << "║  3. Exit                                       ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n";
    std::cout << "Enter your choice (1-3): ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore();  // Clear input buffer
    return choice;
}

/**
 * Display all buildings with numbering
 */
int selectBuilding(const std::vector<std::string>& buildings, const std::string& prompt) {
    std::cout << "\n" << prompt << "\n";
    std::cout << "═══════════════════════════════════════════════════════\n";
    
    for (size_t i = 0; i < buildings.size(); ++i) {
        std::cout << std::setw(3) << (i + 1) << ". " << buildings[i] << "\n";
    }
    
    std::cout << "═══════════════════════════════════════════════════════\n";
    std::cout << "Enter building number: ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    
    if (choice < 1 || choice > (int)buildings.size()) {
        std::cout << "Invalid selection!\n";
        return -1;
    }
    
    return choice - 1;
}

/**
 * Display path results
 */
void displayPathResults(const NavigationSystem::RouteInfo& route) {
    if (route.route.empty()) {
        std::cout << "\n❌ No path found between selected buildings!\n";
        return;
    }
    
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    NAVIGATION RESULTS                      ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    // Route overview
    std::cout << "\n📍 COMPLETE ROUTE:\n";
    std::cout << "─────────────────────────────────────────────────────────────\n";
    for (size_t i = 0; i < route.route.size(); ++i) {
        std::cout << "  " << std::setw(2) << (i + 1) << ". " << route.route[i] << "\n";
        if (i < route.route.size() - 1) {
            std::cout << "       ↓\n";
        }
    }
    
    // Time summary
    std::cout << "\n⏱️  TIME SUMMARY:\n";
    std::cout << "─────────────────────────────────────────────────────────────\n";
    std::cout << "  Spare Time (less crowded):    " 
              << std::fixed << std::setprecision(1) << route.totalTimeSpare 
              << " minutes\n";
    std::cout << "  Popular Time (crowded):       " 
              << std::fixed << std::setprecision(1) << route.totalTimePopular 
              << " minutes\n";
    
    // Detailed segments
    std::cout << "\n📋 DETAILED ROUTE BREAKDOWN:\n";
    std::cout << "─────────────────────────────────────────────────────────────\n";
    
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
            std::cout << "🏢 Indoor";
        } else {
            std::cout << "🌳 Outdoor";
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
            std::cout << "    📝 Notes: " << segment.specials << "\n";
        }
    }
    
    std::cout << "\n═════════════════════════════════════════════════════════════\n";
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
    try {
        NavigationSystem system;
        
        // Load test data
        std::cout << "Loading campus data...\n";
        loadTestData(&system);
        std::cout << "✓ Campus data loaded successfully!\n";
        
        bool running = true;
        while (running) {
            int choice = displayMainMenu();
            
            switch (choice) {
                case 1: {
                    // Find path
                    std::vector<std::string> buildings = system.getAllBuildingNames();
                    
                    int startIdx = selectBuilding(buildings, "SELECT STARTING BUILDING:");
                    if (startIdx == -1) break;
                    
                    int endIdx = selectBuilding(buildings, "SELECT ENDING BUILDING:");
                    if (endIdx == -1) break;
                    
                    std::string startBuilding = buildings[startIdx];
                    std::string endBuilding = buildings[endIdx];
                    
                    if (startBuilding == endBuilding) {
                        std::cout << "\n⚠️  Start and end buildings must be different!\n";
                        break;
                    }
                    
                    std::cout << "\nCalculating route...\n";
                    NavigationSystem::RouteInfo route = system.calculateRoute(startBuilding, endBuilding);
                    displayPathResults(route);
                    break;
                }
                
                case 2: {
                    // List all buildings
                    std::vector<std::string> buildings = system.getAllBuildingNames();
                    std::cout << "\n";
                    std::cout << "╔════════════════════════════════════════════╗\n";
                    std::cout << "║         AVAILABLE BUILDINGS                ║\n";
                    std::cout << "╠════════════════════════════════════════════╣\n";
                    for (size_t i = 0; i < buildings.size(); ++i) {
                        std::cout << "║ " << std::setw(2) << (i + 1) << ". " 
                                  << std::setw(40) << std::left << buildings[i] << "║\n";
                    }
                    std::cout << "╚════════════════════════════════════════════╝\n";
                    break;
                }
                
                case 3: {
                    std::cout << "\nThank you for using HKU Campus Navigation System!\n";
                    running = false;
                    break;
                }
                
                default:
                    std::cout << "\n❌ Invalid choice! Please enter 1-3.\n";
            }
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
