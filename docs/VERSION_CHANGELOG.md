# Version Changelog - Campus Navigation System

## Version 2.2 - May 2, 2026
**Status:** ✅ Released  
**Commit:** `6067740`  
**Branch:** main

### What's New

#### Documentation Updates
- **README.md Enhanced** - Version updated from 2.1 to 2.2
- **New Section:** "Using Custom/New Data" (285 new lines)
- **Updated Table of Contents** - Added custom data section

#### New Documentation Content

##### 1. Format Specifications (NEW)
Comprehensive guides for each data file:

- **node.txt Format**
  - Purpose: Define all buildings/nodes
  - One building per line
  - Case-sensitive names
  - Example with 3 buildings

- **neighbor.txt Format**
  - Format: `BuildingName : Neighbor1, Neighbor2, ...`
  - Bidirectional connections
  - Case-sensitive matching
  - Example with adjacencies

- **Paths.txt Format (CSV)**
  - 8 required columns
  - Time validation rules (0-1440 seconds)
  - Boolean columns (Y/N only)
  - Detailed column definitions table
  - Example with 3 paths

##### 2. Step-by-Step Guide (NEW)
- Step 1: Prepare node.txt with building list
- Step 2: Prepare neighbor.txt with connections
- Step 3: Prepare Paths.txt with path details
- Step 4: Replace files in workspace
- Step 5: Test your data

##### 3. Validation & Troubleshooting (NEW)
- 5 common issues with fixes
- 9-item validation checklist
- Complete mini-system example with 3 buildings

##### 4. Complete Mini Example (NEW)
Full working example demonstrating:
- node.txt with 3 buildings
- neighbor.txt with connections
- Paths.txt with full path definitions
- Navigation graph with different path types

### File Organization Changes
**Test Files Reorganization:**
- Moved to `test reports/` directory:
  - `minimal_test.cpp`
  - `minimal_test.exe`
  - `simple_verify.py`
  - `test_fixes.py`
  - `test_menu.cpp`
  - `test_menu.exe`
  - `test_navigation.cpp`
  - `test_preference.cpp`
  - `test_preference.exe`
  - `test_preference_detailed.cpp`
  - `test_preference_detailed.exe`
  - `verify_navigation.py`

### Testing Results
```
Python Test Suite: ✅ 15/15 PASSED
- Test 1: Building Constructor (valid name)... PASS
- Test 2: Building Constructor (empty name)... PASS
- Test 3: Constructor (null in neighbors)... PASS
- Test 4: Building Constructor (with valid neighbors)... PASS
- Test 5: setNeighbors (with null)... PASS
- Test 6: addNeighbor(None)... PASS
- Test 7: addNeighbor (valid)... PASS
- Test 8: Path (negative spare time)... PASS
- Test 9: Path (negative popular time)... PASS
- Test 10: Path (time > 24h)... PASS
- Test 11: Path Constructor (valid)... PASS
- Test 12: Path (null from)... PASS
- Test 13: Path (null to)... PASS
- Test 14: Path toString() (new format)... PASS
- Test 15: Complex graph structure... PASS

C++ Compilation: ✅ SUCCESS
- Compiled: interface.cpp + navigation.cpp
- Executable: campus_nav.exe
- Size: Standard executable

Program Execution: ✅ SUCCESS
- Loaded 54 buildings from node.txt
- Loaded 54 building connections from neighbor.txt
- Loaded 140 paths from Paths.txt
- Data validation: All passed
```

### Statistics
| Metric | Value |
|--------|-------|
| README lines added | 285 |
| New sections | 1 major |
| Code examples | 15+ |
| Files moved | 12 |
| Documentation tables | 2 |
| Validation checklist items | 9 |
| Common issues documented | 5 |

### Compatibility
- ✅ Backward compatible with all previous versions
- ✅ All 15 unit tests passing
- ✅ C++ compilation successful
- ✅ Data loading operational
- ✅ Navigation graph construction verified

### Breaking Changes
None - This is a pure documentation release.

### Deprecations
None

### Security Changes
None

### Known Issues
None

### Future Roadmap
- Version 2.3: API reference documentation
- Version 2.4: Advanced navigation algorithms
- Version 2.5: Performance optimization guide

---

## Previous Versions

### Version 2.1 - April 24, 2026
**Status:** ✅ Released  
**Focus:** Real Data Integration
- Integrated 54 HKU campus buildings
- Loaded 140 paths from CSV
- Added file-based data loading

### Version 2.0
**Status:** ✅ Released  
**Focus:** Core Classes
- Building class (8 methods)
- Path class (9 methods)
- Complete validation framework

---

## How to Use This Version

### For Users with Custom Data:
1. Read "Using Custom/New Data" section in README.md
2. Follow step-by-step guide (5 steps)
3. Use mini-example as reference
4. Run validation checklist
5. Consult troubleshooting if issues occur

### For Developers:
1. Review new format specifications
2. Validate input data against checklist
3. Test with mini-example first
4. Scale up to production data
5. Run full test suite: `python test_fixes.py`

### For Contributors:
1. All tests must pass (15/15)
2. Documentation must be updated
3. Follow existing code style
4. Test with custom data before PR

---

## Release Notes

### What Changed
- **Documentation:** +286 lines (primarily README.md)
- **Organization:** 12 test files moved to test reports/
- **Functionality:** No changes (fully backward compatible)

### Why These Changes
1. Users requested guidance on custom data
2. Test files cluttered main directory
3. Documentation gap existed for data format
4. Need for validation guidance

### Testing Completed
- [x] Python unit tests (15/15 passing)
- [x] C++ compilation
- [x] Program execution
- [x] Data loading
- [x] Navigation graph construction

### Deployment Checklist
- [x] All tests passing
- [x] Documentation updated
- [x] Backward compatibility verified
- [x] Code compiled successfully
- [x] Ready for production

---

**Generated:** May 2, 2026  
**Version:** 2.2 (Custom Data Support)  
**Status:** Production Ready ✅
