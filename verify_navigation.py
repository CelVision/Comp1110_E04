#!/usr/bin/env python3
"""
Navigation Classes - Python Verification Script
Simulates C++ Building and Path classes to verify logic correctness
"""

class Building:
    """Simulates C++ Building class"""
    
    def __init__(self, name, neighbors=None):
        self.name = name
        self.neighbors = neighbors if neighbors is not None else []
    
    def set_neighbors(self, neighbors):
        """Set neighbors list"""
        self.neighbors = neighbors if neighbors is not None else []
    
    def get_name(self):
        """Get building name"""
        return self.name
    
    def get_neighbors(self):
        """Get neighbors list"""
        return self.neighbors
    
    def add_neighbor(self, neighbor):
        """Add single neighbor"""
        if neighbor is not None:
            self.neighbors.append(neighbor)
    
    def to_string(self):
        """Convert to string"""
        return self.name


class Path:
    """Simulates C++ Path class"""
    
    def __init__(self, from_building, to_building, time_spare, time_popular,
                 specials, if_indoors, has_elevator_or_escalator, has_stairs_or_ramp):
        if from_building is None or to_building is None:
            raise ValueError("Both buildings must be provided")
        
        self.from_building = from_building
        self.to_building = to_building
        self.time_spare = time_spare
        self.time_popular = time_popular
        self.specials = specials
        self.if_indoors = if_indoors
        self.has_elevator_or_escalator = has_elevator_or_escalator
        self.has_stairs_or_ramp = has_stairs_or_ramp
    
    def get_from(self):
        return self.from_building
    
    def get_to(self):
        return self.to_building
    
    def get_time_spare(self):
        return self.time_spare
    
    def get_time_popular(self):
        return self.time_popular
    
    def get_specials(self):
        return self.specials
    
    def get_if_indoors(self):
        return self.if_indoors
    
    def get_has_elevator_or_escalator(self):
        return self.has_elevator_or_escalator
    
    def get_has_stairs_or_ramp(self):
        return self.has_stairs_or_ramp
    
    def to_string(self):
        """Generate human-readable path description"""
        result = f"{self.from_building.get_name()} -> {self.to_building.get_name()}"
        result += f" | Spare: {self.time_spare}min | Popular: {self.time_popular}min"
        result += f" | Indoors: {'Yes' if self.if_indoors else 'No'}"
        
        if self.specials:
            result += f" | {self.specials}"
        
        return result


# ============================================================================
# TEST SUITE
# ============================================================================

def test_building_constructor_name_only():
    print("[TEST 1] Building Constructor (name only)...", end=" ")
    b = Building("Main Building")
    assert b.get_name() == "Main Building"
    assert len(b.get_neighbors()) == 0
    print("✓ PASS")


def test_building_constructor_with_neighbors():
    print("[TEST 2] Building Constructor (with neighbors)...", end=" ")
    b1 = Building("Building A")
    b2 = Building("Building B")
    b3 = Building("Building C")
    
    main = Building("Main", [b2, b3])
    
    assert main.get_name() == "Main"
    assert len(main.get_neighbors()) == 2
    assert main.get_neighbors()[0].get_name() == "Building B"
    assert main.get_neighbors()[1].get_name() == "Building C"
    print("✓ PASS")


def test_building_set_neighbors():
    print("[TEST 3] Building set_neighbors()...", end=" ")
    b1 = Building("Building 1")
    b2 = Building("Building 2")
    b3 = Building("Building 3")
    
    b1.set_neighbors([b2, b3])
    
    assert len(b1.get_neighbors()) == 2
    assert b1.get_neighbors()[0].get_name() == "Building 2"
    print("✓ PASS")


def test_building_add_neighbor():
    print("[TEST 4] Building add_neighbor()...", end=" ")
    b1 = Building("Building 1")
    b2 = Building("Building 2")
    b3 = Building("Building 3")
    
    b1.add_neighbor(b2)
    assert len(b1.get_neighbors()) == 1
    assert b1.get_neighbors()[0].get_name() == "Building 2"
    
    b1.add_neighbor(b3)
    assert len(b1.get_neighbors()) == 2
    assert b1.get_neighbors()[1].get_name() == "Building 3"
    print("✓ PASS")


def test_building_add_null_neighbor():
    print("[TEST 5] Building add_neighbor(None)...", end=" ")
    b1 = Building("Building 1")
    
    b1.add_neighbor(None)
    assert len(b1.get_neighbors()) == 0
    print("✓ PASS")


def test_building_to_string():
    print("[TEST 6] Building to_string()...", end=" ")
    b = Building("Central Library")
    assert b.to_string() == "Central Library"
    print("✓ PASS")


def test_path_constructor_valid():
    print("[TEST 7] Path Constructor (valid)...", end=" ")
    from_bldg = Building("Building A")
    to_bldg = Building("Building B")
    
    p = Path(from_bldg, to_bldg, 5.0, 10.0, "Covered pathway", True, True, False)
    
    assert p.get_from().get_name() == "Building A"
    assert p.get_to().get_name() == "Building B"
    assert p.get_time_spare() == 5.0
    assert p.get_time_popular() == 10.0
    assert p.get_specials() == "Covered pathway"
    assert p.get_if_indoors() == True
    assert p.get_has_elevator_or_escalator() == True
    assert p.get_has_stairs_or_ramp() == False
    print("✓ PASS")


def test_path_constructor_null_from():
    print("[TEST 8] Path Constructor (null from building)...", end=" ")
    to_bldg = Building("Building B")
    
    try:
        p = Path(None, to_bldg, 5.0, 10.0, "", True, False, False)
        assert False, "Should have thrown an exception"
    except ValueError:
        print("✓ PASS (exception caught as expected)")


def test_path_constructor_null_to():
    print("[TEST 9] Path Constructor (null to building)...", end=" ")
    from_bldg = Building("Building A")
    
    try:
        p = Path(from_bldg, None, 5.0, 10.0, "", True, False, False)
        assert False, "Should have thrown an exception"
    except ValueError:
        print("✓ PASS (exception caught as expected)")


def test_path_getters():
    print("[TEST 10] Path Getters...", end=" ")
    from_bldg = Building("Building A")
    to_bldg = Building("Building B")
    
    p = Path(from_bldg, to_bldg, 30.5, 45.75, "Elevator access", False, True, True)
    
    assert p.get_from() is not None
    assert p.get_to() is not None
    assert p.get_time_spare() == 30.5
    assert p.get_time_popular() == 45.75
    assert p.get_specials() == "Elevator access"
    assert p.get_if_indoors() == False
    assert p.get_has_elevator_or_escalator() == True
    assert p.get_has_stairs_or_ramp() == True
    print("✓ PASS")


def test_path_to_string():
    print("[TEST 11] Path to_string()...", end=" ")
    from_bldg = Building("Main Hall")
    to_bldg = Building("Science Building")
    
    p = Path(from_bldg, to_bldg, 5.0, 8.0, "Scenic route", True, False, True)
    
    result = p.to_string()
    assert "Main Hall" in result
    assert "Science Building" in result
    assert "5" in result
    assert "8" in result
    assert "Yes" in result
    assert "Scenic route" in result
    print("✓ PASS")


def test_path_to_string_no_specials():
    print("[TEST 12] Path to_string() (no special notes)...", end=" ")
    from_bldg = Building("Building X")
    to_bldg = Building("Building Y")
    
    p = Path(from_bldg, to_bldg, 2.5, 3.5, "", False, False, False)
    
    result = p.to_string()
    assert "Building X" in result
    assert "Building Y" in result
    assert "2.5" in result
    assert "3.5" in result
    assert "No" in result
    print("✓ PASS")


def test_complex_graph():
    print("[TEST 13] Complex Graph Structure...", end=" ")
    
    # Create a small graph of buildings
    central = Building("Central Hub")
    lib = Building("Library")
    lab = Building("Science Lab")
    cafe = Building("Cafe")
    
    # Set up connections
    central.set_neighbors([lib, lab])
    
    lib.add_neighbor(cafe)
    lab.add_neighbor(cafe)
    
    # Create paths
    p1 = Path(central, lib, 2.0, 3.0, "Covered", True, False, False)
    p2 = Path(central, lab, 3.0, 4.5, "Outdoor", False, True, False)
    p3 = Path(lib, cafe, 1.5, 2.0, "", True, False, True)
    
    # Verify the graph structure
    assert len(central.get_neighbors()) == 2
    assert len(lib.get_neighbors()) == 1
    assert len(lab.get_neighbors()) == 1
    
    assert p1.get_from().get_name() == "Central Hub"
    assert p2.get_to().get_name() == "Science Lab"
    assert p3.get_if_indoors() == True
    print("✓ PASS")


def main():
    print("=" * 50)
    print("   Navigation Classes - Python Verification")
    print("=" * 50)
    print()

    try:
        test_building_constructor_name_only()
        test_building_constructor_with_neighbors()
        test_building_set_neighbors()
        test_building_add_neighbor()
        test_building_add_null_neighbor()
        test_building_to_string()
        
        test_path_constructor_valid()
        test_path_constructor_null_from()
        test_path_constructor_null_to()
        test_path_getters()
        test_path_to_string()
        test_path_to_string_no_specials()
        
        test_complex_graph()

        print()
        print("=" * 50)
        print("   ✓ ALL TESTS PASSED!")
        print("=" * 50)
        
        # Print summary
        print()
        print("SUMMARY:")
        print("  - Building class: 7 methods verified")
        print("  - Path class: 9 methods verified")
        print("  - Integration tests: 1 complex scenario verified")
        print("  - Total tests: 13 test cases")
        print()
        print("STATUS: All functions work correctly! ✓")
        
        return 0

    except AssertionError as e:
        print(f"\n✗ Test failed: {e}")
        return 1
    except Exception as e:
        print(f"\n✗ Error: {e}")
        return 1


if __name__ == "__main__":
    exit(main())
