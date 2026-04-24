#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

/**
 * Building class represents a campus building in the navigation system.
 * Stores building name and connections to neighboring buildings.
 */
class Building {
private:
    std::string name;
    std::vector<Building*> neighbors;

    // Prevent accidental copying and moving (use rule of five)
    Building(const Building&) = delete;
    Building(Building&&) = delete;
    Building& operator=(const Building&) = delete;
    Building& operator=(Building&&) = delete;

public:
    /**
     * Constructor that creates a Building with only a name.
     * @param name The building's name (must not be empty)
     * @throws std::invalid_argument if name is empty
     */
    explicit Building(const std::string& name) : name(name) {
        if (name.empty()) {
            throw std::invalid_argument("Building name cannot be empty");
        }
    }

    /**
     * Destructor - explicitly defaulted for clarity.
     */
    ~Building() = default;

    /**
     * Alternative constructor if you want to provide neighbors at creation time.
     * @param name The building's name (must not be empty)
     * @param neighbors Vector of neighboring building pointers (must not contain nulls)
     * @throws std::invalid_argument if name is empty or any neighbor is null
     */
    Building(const std::string& name, const std::vector<Building*>& neighbors) 
        : name(name), neighbors(neighbors) {
        if (name.empty()) {
            throw std::invalid_argument("Building name cannot be empty");
        }
        // Validate all neighbors are non-null
        for (const auto& neighbor : neighbors) {
            if (neighbor == nullptr) {
                throw std::invalid_argument("Neighbors cannot contain null pointers");
            }
        }
    }

    /**
     * Assign or update the vector of neighboring buildings later.
     * @param neighbors Vector of neighboring building pointers (must not contain nulls)
     * @throws std::invalid_argument if any neighbor is null
     */
    void setNeighbors(const std::vector<Building*>& neighbors) {
        // Validate all neighbors are non-null before accepting
        for (const auto& neighbor : neighbors) {
            if (neighbor == nullptr) {
                throw std::invalid_argument("Neighbors cannot contain null pointers");
            }
        }
        this->neighbors = neighbors;
    }

    /**
     * Get the building's name.
     * @return Const reference to the building name
     */
    const std::string& getName() const {
        return name;
    }

    /**
     * Get the list of neighboring buildings.
     * @return Const reference to the neighbors vector (efficient for reading)
     */
    const std::vector<Building*>& getNeighbors() const {
        return neighbors;
    }

    /**
     * Add a single neighbor.
     * @param neighbor Pointer to a neighboring building (must not be null)
     * @throws std::invalid_argument if neighbor is null
     */
    void addNeighbor(Building* neighbor) {
        if (neighbor == nullptr) {
            throw std::invalid_argument("Cannot add null neighbor");
        }
        neighbors.push_back(neighbor);
    }

    /**
     * Convert building to string representation.
     * @return The building name as a string
     */
    std::string toString() const {
        return name;
    }
};

/**
 * Path class represents a directed path between two neighboring buildings.
 * Stores travel times, accessibility features, and special information about the path.
 */
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

    // Prevent accidental copying and moving (use rule of five)
    Path(const Path&) = delete;
    Path(Path&&) = delete;
    Path& operator=(const Path&) = delete;
    Path& operator=(Path&&) = delete;

public:
    /**
     * Main constructor for a directed Path between two neighboring buildings.
     * @param from Source building pointer (must not be null)
     * @param to Destination building pointer (must not be null)
     * @param timeSpare Travel time in spare hours (minutes, must be >= 0)
     * @param timePopular Travel time in popular hours (minutes, must be >= 0)
     * @param specials Special notes about the path
     * @param ifIndoors Whether the path is indoors
     * @param hasElevatorOrEscalator Whether the path has elevator or escalator
     * @param hasStairsOrRamp Whether the path has stairs or ramp
     * @throws std::invalid_argument if inputs are invalid
     */
    Path(Building* from, Building* to, float timeSpare, float timePopular,
         const std::string& specials, bool ifIndoors, 
         bool hasElevatorOrEscalator, bool hasStairsOrRamp)
        : from(from), to(to), timeSpare(timeSpare), timePopular(timePopular),
          specials(specials), ifIndoors(ifIndoors), 
          hasElevatorOrEscalator(hasElevatorOrEscalator), hasStairsOrRamp(hasStairsOrRamp) {
        
        // Validate both buildings are provided
        if (from == nullptr || to == nullptr) {
            throw std::invalid_argument("Both buildings must be provided (cannot be null)");
        }
        
        // Validate time values are non-negative
        if (timeSpare < 0.0f) {
            throw std::invalid_argument("Spare time cannot be negative");
        }
        if (timePopular < 0.0f) {
            throw std::invalid_argument("Popular time cannot be negative");
        }
        
        // Validate time values are reasonable (not exceeding 24 hours = 1440 minutes)
        if (timeSpare > 1440.0f) {
            throw std::invalid_argument("Spare time exceeds reasonable limit (24 hours)");
        }
        if (timePopular > 1440.0f) {
            throw std::invalid_argument("Popular time exceeds reasonable limit (24 hours)");
        }
    }

    /**
     * Destructor - explicitly defaulted for clarity.
     */
    ~Path() = default;

    /**
     * Get the source building.
     * @return Pointer to the source building
     */
    Building* getFrom() const { 
        return from; 
    }

    /**
     * Get the destination building.
     * @return Pointer to the destination building
     */
    Building* getTo() const { 
        return to; 
    }

    /**
     * Get travel time during spare hours.
     * @return Time in minutes
     */
    float getTimeSpare() const { 
        return timeSpare; 
    }

    /**
     * Get travel time during popular hours.
     * @return Time in minutes
     */
    float getTimePopular() const { 
        return timePopular; 
    }

    /**
     * Get special notes about the path.
     * @return Const reference to the special notes string
     */
    const std::string& getSpecials() const { 
        return specials; 
    }

    /**
     * Check if the path is indoors.
     * @return True if indoors, false otherwise
     */
    bool getIfIndoors() const { 
        return ifIndoors; 
    }

    /**
     * Check if the path has elevator or escalator.
     * @return True if has elevator/escalator, false otherwise
     */
    bool getHasElevatorOrEscalator() const { 
        return hasElevatorOrEscalator; 
    }

    /**
     * Check if the path has stairs or ramp.
     * @return True if has stairs/ramp, false otherwise
     */
    bool getHasStairsOrRamp() const { 
        return hasStairsOrRamp; 
    }

    /**
     * Returns a detailed string representation of the path.
     * @return Formatted string with all path information
     */
    std::string toString() const {
        std::ostringstream oss;
        
        // Basic path information
        oss << from->getName() << " -> " << to->getName();
        
        // Time information
        oss << " | Time(spare): " << timeSpare << "m";
        oss << " | Time(popular): " << timePopular << "m";
        
        // Environment information
        oss << " | Indoor: " << (ifIndoors ? "Yes" : "No");
        
        // Accessibility information
        oss << " | Elevator/Escalator: " << (hasElevatorOrEscalator ? "Yes" : "No");
        oss << " | Stairs/Ramp: " << (hasStairsOrRamp ? "Yes" : "No");
        
        // Special notes (if any)
        if (!specials.empty()) {
            oss << " | Note: " << specials;
        }
        
        return oss.str();
    }
};