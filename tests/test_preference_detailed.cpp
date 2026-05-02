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
#include "../src/navigation.cpp"

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
    
            if (current == end) break;
    
            Building* b = buildings[current];
            for (Building* neighbor : b->getNeighbors()) {
                std::string next = neighbor->getName();
    
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
    
        if (parent[end].empty()) {
            return {};
        }
    
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
        size_t colonPos = line.find(":");
        if (colonPos == std::string::npos) continue;
        
        std::string building = line.substr(0, colonPos);
        std::string neighborsList = line.substr(colonPos + 1);
        
        building.erase(0, building.find_first_not_of(" \t\r\n"));
        building.erase(building.find_last_not_of(" \t\r\n") + 1);
        
        std::vector<std::string> buildingNeighbors;
        std::stringstream ss(neighborsList);
        std::string neighbor;
        
        while (std::getline(ss, neighbor, ',')) {
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
        if (firstLine) {
            firstLine = false;
            continue;
        }
        
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string field;
        PathData pathData;
        int fieldCount = 0;
        
        while (std::getline(ss, field, ',') && fieldCount < 8) {
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
                break;
            }
            
            fieldCount++;
        }
        
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
    std::cout << "Reading buildings from node.txt...\n" << std::flush;
    std::vector<std::string> buildings = readBuildingsFromFile("node.txt");
    std::cout << "  Loaded " << buildings.size() << " buildings\n" << std::flush;
    
    for (const auto& building : buildings) {
        system.addBuilding(building);
    }
    
    std::cout << "Reading connections from neighbor.txt...\n" << std::flush;
    std::map<std::string, std::vector<std::string>> neighbors = readNeighborsFromFile("neighbor.txt");
    std::cout << "  Loaded " << neighbors.size() << " building connections\n" << std::flush;
    
    for (const auto& pair : neighbors) {
        const std::string& building = pair.first;
        for (const auto& neighbor : pair.second) {
            system.connectBuildings(building, neighbor);
        }
    }
    
    std::cout << "Reading paths from Paths.txt...\n" << std::flush;
    std::vector<PathData> paths = readPathsFromFile("Paths.txt");
    std::cout << "  Loaded " << paths.size() << " paths\n" << std::flush;
    
    for (const auto& path : paths) {
        float timeSpareMin = path.timeSpare / 60.0f;
        float timePopularMin = path.timePopular / 60.0f;
        
        system.addPath(path.from, path.to, timeSpareMin, timePopularMin, 
                      path.specials, path.isIndoors, path.hasElevator, path.hasStairs);
    }
}

/**
 * Test function - find routes with different preferences and show details
 */
void testPreference(NavigationSystem& system, const std::string& start, const std::string& end) {
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "TESTING: " << start << " -> " << end << "\n";
    std::cout << std::string(80, '=') << "\n";
    
    // Test with no preference
    std::cout << "\n[ROUTE 1: NO PREFERENCE]\n";
    auto route0 = system.calculateRoute(start, end, 0);
    if (route0.route.empty()) {
        std::cout << "No path found\n";
        return;
    }
    std::cout << "Time: " << std::fixed << std::setprecision(2) << route0.totalTimeSpare << " min\n";
    int outdoorCount0 = 0;
    for (const auto& seg : route0.segments) {
        if (!seg.indoors) outdoorCount0++;
    }
    std::cout << "Outdoor: " << outdoorCount0 << " seg(s) | Indoor: " << (route0.segments.size() - outdoorCount0) << " seg(s)\n";
    std::cout << "Path: ";
    for (size_t i = 0; i < route0.route.size(); ++i) {
        std::cout << route0.route[i];
        if (i < route0.route.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";
    
    // Test with avoid outdoor preference
    std::cout << "\n[ROUTE 2: AVOID OUTDOORS]\n";
    auto route1 = system.calculateRoute(start, end, 1);
    if (route1.route.empty()) {
        std::cout << "No path found\n";
        return;
    }
    std::cout << "Time: " << std::fixed << std::setprecision(2) << route1.totalTimeSpare << " min\n";
    int outdoorCount1 = 0;
    for (const auto& seg : route1.segments) {
        if (!seg.indoors) outdoorCount1++;
    }
    std::cout << "Outdoor: " << outdoorCount1 << " seg(s) | Indoor: " << (route1.segments.size() - outdoorCount1) << " seg(s)\n";
    std::cout << "Path: ";
    for (size_t i = 0; i < route1.route.size(); ++i) {
        std::cout << route1.route[i];
        if (i < route1.route.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";
    
    // Compare
    std::cout << "\n[ANALYSIS]\n";
    if (route0.route == route1.route) {
        std::cout << "✗ Routes IDENTICAL (no preference change)\n";
        if (outdoorCount0 > 0) {
            std::cout << "  Note: Both routes have " << outdoorCount0 << " outdoor segment(s)\n";
            std::cout << "  Reason: No better indoor-only alternative exists\n";
        }
    } else {
        std::cout << "✓ Routes DIFFERENT - PREFERENCE WORKING!\n";
        if (outdoorCount1 < outdoorCount0) {
            std::cout << "  Outdoor segments reduced: " << outdoorCount0 << " -> " << outdoorCount1 << "\n";
            std::cout << "  Time impact: " << std::fixed << std::setprecision(2) 
                      << route1.totalTimeSpare << " min (vs " << route0.totalTimeSpare << " min)\n";
        }
    }
}

/**
 * Main program
 */
int main() {
    std::cout << "Loading campus data...\n" << std::flush;
    
    NavigationSystem system;
    loadRealData(system);
    std::cout << "Campus data loaded successfully!\n" << std::flush;
    
    std::vector<std::string> buildings = system.getAllBuildingNames();
    std::cout << "\nTotal buildings: " << buildings.size() << "\n";
    
    // Scan for routes with outdoor paths
    std::cout << "\nScanning for routes with outdoor segments...\n";
    int testedCount = 0;
    int differentCount = 0;
    
    for (size_t i = 0; i < buildings.size() && testedCount < 10; ++i) {
        for (size_t j = i + 1; j < buildings.size() && testedCount < 10; ++j) {
            auto route0 = system.calculateRoute(buildings[i], buildings[j], 0);
            if (route0.route.empty()) continue;
            
            // Check if this route has outdoor segments
            int outdoorCount = 0;
            for (const auto& seg : route0.segments) {
                if (!seg.indoors) outdoorCount++;
            }
            
            if (outdoorCount > 0) {
                testPreference(system, buildings[i], buildings[j]);
                testedCount++;
            }
        }
    }
    
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "Testing completed! Found " << testedCount << " routes with outdoor segments.\n";
    
    return 0;
}
