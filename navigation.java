public class Building {
    private String name;                    // Name of the building (e.g., "Main Building")
    private Building[] neighbors;           // Array of buildings directly connected to this one

    /**
     * Constructor that creates a Building with only a name.
     * The neighbors array can be assigned later (as you requested).
     */
    public Building(String name) {
        this.name = name;
        this.neighbors = new Building[0];   // empty array by default
    }

    /**
     * Alternative constructor if you want to provide neighbors at creation time.
     * (You can still change them later with setNeighbors())
     */
    public Building(String name, Building[] neighbors) {
        this.name = name;
        this.neighbors = (neighbors != null) ? neighbors : new Building[0];
    }

    /**
     * Assign or update the array of neighboring buildings later.
     * This matches exactly what you asked: "the constructor could be assigned later."
     */
    public void setNeighbors(Building[] neighbors) {
        this.neighbors = (neighbors != null) ? neighbors : new Building[0];
    }

    // Getters
    public String getName() {
        return name;
    }

    public Building[] getNeighbors() {
        return neighbors;
    }

    // Optional: add a single neighbor (useful when building the graph)
    public void addNeighbor(Building neighbor) {
        if (neighbor == null) return;
        // Create a new larger array and copy existing neighbors
        Building[] newNeighbors = new Building[neighbors.length + 1];
        System.arraycopy(neighbors, 0, newNeighbors, 0, neighbors.length);
        newNeighbors[neighbors.length] = neighbor;
        this.neighbors = newNeighbors;
    }

    @Override
    public String toString() {
        return name;
    }
}

public class Path {
    private final Building from;          // Starting building (order matters!)
    private final Building to;            // Destination building
    private final float timeSpare;        // Time estimate when you have spare time (e.g., leisurely walk)
    private final float timePopular;      // Time estimate when following the popular/fast route
    private final String specials;        // Any special notes (e.g., "scenic", "stairs only", "elevator available")
    private final boolean ifIndoors;      // true = mostly indoors, false = outdoors
    private final boolean hasElevatorOrEscalator;  // true = has elevator or escalator
    private final boolean hasStairsOrRamp;         // true = has stairs or ramp

    /**
     * Main constructor for a directed Path between two neighboring buildings.
     * Order is important: from → to
     */
    public Path(Building from, Building to,
                float timeSpare, float timePopular,
                String specials, boolean ifIndoors,
                boolean hasElevatorOrEscalator, boolean hasStairsOrRamp) {
        
        if (from == null || to == null) {
            throw new IllegalArgumentException("Both buildings must be provided");
        }
        
        this.from = from;
        this.to = to;
        this.timeSpare = timeSpare;
        this.timePopular = timePopular;
        this.specials = (specials != null) ? specials : "";
        this.ifIndoors = ifIndoors;
        this.hasElevatorOrEscalator = hasElevatorOrEscalator;
        this.hasStairsOrRamp = hasStairsOrRamp;
    }

    // Getters
    public Building getFrom() {
        return from;
    }

    public Building getTo() {
        return to;
    }

    public float getTimeSpare() {
        return timeSpare;
    }

    public float getTimePopular() {
        return timePopular;
    }

    public String getSpecials() {
        return specials;
    }

    public boolean isIfIndoors() {
        return ifIndoors;
    }

    public boolean hasElevatorOrEscalator() {
        return hasElevatorOrEscalator;
    }

    public boolean hasStairsOrRamp() {
        return hasStairsOrRamp;
    }

    /**
     * Returns a nice string representation of the path
     */
    @Override
    public String toString() {
        return from.getName() + " → " + to.getName() +
               " | Spare: " + timeSpare + "min | Popular: " + timePopular + "min" +
               " | Indoors: " + (ifIndoors ? "Yes" : "No") +
               (specials.isEmpty() ? "" : " | " + specials);
    }
}