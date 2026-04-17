#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

class Building {
private:
    std::string name;
    std::vector<Building*> neighbors;

public:
    /**
     * Constructor that creates a Building with only a name.
     */
    Building(const std::string& name) : name(name) {}

    /**
     * Alternative constructor if you want to provide neighbors at creation time.
     */
    Building(const std::string& name, const std::vector<Building*>& neighbors) 
        : name(name), neighbors(neighbors) {}

    /**
     * Assign or update the vector of neighboring buildings later.
     */
    void setNeighbors(const std::vector<Building*>& neighbors) {
        this->neighbors = neighbors;
    }

    // Getters
    std::string getName() const {
        return name;
    }

    std::vector<Building*> getNeighbors() const {
        return neighbors;
    }

    /**
     * Add a single neighbor.
     * In C++, std::vector handles the resizing automatically.
     */
    void addNeighbor(Building* neighbor) {
        if (neighbor == nullptr) return;
        neighbors.push_back(neighbor);
    }

    /**
     * Equivalent to Java's toString()
     */
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
    /**
     * Main constructor for a directed Path between two neighboring buildings.
     */
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

    // Getters
    Building* getFrom() const { return from; }
    Building* getTo() const { return to; }
    float getTimeSpare() const { return timeSpare; }
    float getTimePopular() const { return timePopular; }
    std::string getSpecials() const { return specials; }
    bool getIfIndoors() const { return ifIndoors; }
    bool getHasElevatorOrEscalator() const { return hasElevatorOrEscalator; }
    bool getHasStairsOrRamp() const { return hasStairsOrRamp; }

    /**
     * Returns a nice string representation of the path
     */
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