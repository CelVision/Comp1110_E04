# Campus Navigation System - Project Structure

**Last Updated:** 2026-05-02

## Directory Organization

The repository has been reorganized into a clean, modular structure:

```
Comp1110_E04/
│
├── README.md                    # Main documentation (root copy)
├── LICENSE                      # License file
├── PROJECT_STRUCTURE.md        # This file
│
├── src/                         # Source Code
│   ├── navigation.cpp           # Core navigation classes (Building, Path)
│   ├── interface.cpp            # User interface and system interaction
│   └── Read_data.cpp            # Data file processing utilities
│
├── data/                        # Data Files
│   ├── node.txt                 # 54 HKU buildings and their names
│   ├── neighbor.txt             # Adjacency relationships between buildings
│   └── Paths.txt                # Path information (200+ paths with time data)
│
├── tests/                       # Test Files
│   ├── test_navigation.cpp      # Main C++ test suite
│   ├── test_menu.cpp            # Menu system tests
│   ├── test_preference.cpp      # Preference handling tests
│   ├── test_preference_detailed.cpp  # Detailed preference tests
│   ├── minimal_test.cpp         # Minimal test suite
│   ├── test_fixes.py            # Python test suite (15 tests)
│   ├── simple_verify.py         # Windows-compatible test runner
│   ├── verify_navigation.py     # Original test suite
│   └── check_path.py            # Path verification utility
│
├── docs/                        # Documentation & Reports
│   ├── README.md                # Full documentation copy
│   ├── QUICK_REFERENCE.md       # One-page quick reference
│   ├── ISSUES_FIXED.md          # Detailed fix explanations
│   ├── FIXES_COMPLETE.md        # Comprehensive analysis
│   ├── TEST_REPORT.md           # Test summary report
│   ├── DETAILED_TEST_RESULTS.md # Detailed test results
│   ├── VERIFICATION_SUMMARY.md  # Verification summary
│   ├── VERSION_CHANGELOG.md     # Version history
│   ├── FINAL_REPORT.md          # Final project report
│   ├── DEBUG_CHECKLIST.md       # Debugging reference
│   └── reference_picture.png    # Campus reference image
│
├── build/                       # Compiled Executables
│   ├── campus_nav.exe           # Main program executable
│   ├── minimal_test.exe         # Minimal test executable
│   ├── test_menu.exe            # Menu test executable
│   ├── test_preference.exe      # Preference test executable
│   └── test_preference_detailed.exe  # Detailed preference test
│
└── .git/                        # Version control (Git)
```

## Important Changes

### Path References Updated
All source files have been updated to reference data files with correct relative paths:

- **interface.cpp**: 
  - `"node.txt"` → `"../data/node.txt"`
  - `"neighbor.txt"` → `"../data/neighbor.txt"`
  - `"Paths.txt"` → `"../data/Paths.txt"`

- **Read_data.cpp**:
  - `"node.txt"` → `"../data/node.txt"`
  - `"neighbor.txt"` → `"../data/neighbor.txt"`

### Relative Path Structure
The path `../data/` works because:
- **Compilation**: Typically run from the root directory (`Comp1110_E04/`)
- **Execution**: The program expects to run from the root, allowing it to find `./data/` files
- **From src/**: Relative path goes up one level (`..`) then into `data/`

## Building & Running

### Compilation
```bash
# From root directory
g++ -o build/campus_nav.exe src/interface.cpp src/navigation.cpp src/Read_data.cpp

# Or with modern C++ standard
g++ -std=c++17 -o build/campus_nav.exe src/interface.cpp src/navigation.cpp src/Read_data.cpp
```

### Running Tests
```bash
# Python tests
python tests/simple_verify.py

# Or run compiled tests
./build/minimal_test.exe
./build/test_preference.exe
```

### Running Main Program
```bash
# From root directory
./build/campus_nav.exe
```

## File Categories

### Source Code (`src/`)
- **navigation.cpp**: Core classes
- **interface.cpp**: User interaction layer
- **Read_data.cpp**: Data loading utilities

### Data Files (`data/`)
- **node.txt**: Building data (names)
- **neighbor.txt**: Building connections
- **Paths.txt**: Path information (CSV format)

### Tests (`tests/`)
- C++ tests: `.cpp` files (compile to `.exe`)
- Python tests: `.py` files (run with Python)

### Documentation (`docs/`)
- Markdown documentation (`.md`)
- Reference images (`.png`)

### Binaries (`build/`)
- Executable programs (`.exe`)
- Generated from compilation

## Best Practices

1. **Run from root**: Always execute programs from the repository root
2. **Data access**: Keep data in `data/` folder for consistency
3. **Testing**: Use `tests/` folder for all test files
4. **Documentation**: Keep docs in `docs/` folder
5. **Compilation**: Output binaries to `build/` folder

## Notes

- All relative paths use `../data/` format for cross-platform compatibility
- Programs expect to be run from the root `Comp1110_E04/` directory
- Data files (*.txt) should remain in `data/` folder
- Test files can be modified for custom testing

