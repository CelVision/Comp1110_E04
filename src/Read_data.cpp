#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include <iterator>

using namespace std;

/**
 * @brief Trims leading and trailing whitespace from a string.
 * @param s Input string to trim.
 * @return Trimmed string (empty if input was all whitespace).
 */
string trim(const string &s) {
    auto start = s.begin();
    // Skip leading whitespace characters
    while (start != s.end() && isspace(static_cast<unsigned char>(*start))) {
        ++start;
    }
    // If the entire string is whitespace, return empty string immediately
    if (start == s.end()) {
        return "";
    }
    // Find the last non-whitespace character
    auto end = s.end();
    do {
        --end;
    } while (end != start && isspace(static_cast<unsigned char>(*end)));
    // Return the substring from first non-whitespace to last non-whitespace
    return string(start, end + 1);
}

/**
 * @brief Splits a string into tokens using a specified delimiter.
 * @param s Input string to split.
 * @param delimiter Character used to split the string.
 * @return Vector of trimmed tokens.
 */
vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    // Read each segment separated by the delimiter
    while (getline(tokenStream, token, delimiter)) {
        string trimmedToken = trim(token);
        if (!trimmedToken.empty()) {
            tokens.push_back(trimmedToken);
        }
    }
    return tokens;
}

int main() {
    // -------------------------------------------------------------------------
    // Step 1: Read and validate nodes from node.txt
    // -------------------------------------------------------------------------
    unordered_set<string> validNodes;
    ifstream nodeFile("./data/node.txt");
    
    // Check if node.txt can be opened
    if (!nodeFile.is_open()) {
        cerr << "Error: Cannot open ./data/node.txt. Please ensure it is in the data folder." << endl;
        return 1;
    }

    string line;
    while (getline(nodeFile, line)) {
        string nodeName = trim(line);
        // Only add non-empty node names
        if (!nodeName.empty()) {
            validNodes.insert(nodeName);
        }
    }
    nodeFile.close();
    cout << "Successfully loaded " << validNodes.size() << " valid nodes from node.txt." << endl;

    // -------------------------------------------------------------------------
    // Step 2: Read neighbor relationships from neighbor.txt
    // -------------------------------------------------------------------------
    vector<pair<string, string>> pathPairs;
    ifstream neighborFile("./data/neighbor.txt");

    // Check if neighbor.txt can be opened
    if (!neighborFile.is_open()) {
        cerr << "Error: Cannot open ./data/neighbor.txt. Please ensure it is in the data folder." << endl;
        return 1;
    }

    while (getline(neighborFile, line)) {
        string trimmedLine = trim(line);
        // Skip empty lines
        if (trimmedLine.empty()) continue;

        // Parse line in format "FromNode : Neighbor1, Neighbor2, ..."
        size_t splitPos = trimmedLine.find(" : ");
        if (splitPos == string::npos) {
            cerr << "Warning: Skipping invalid line (missing ' : ' separator): " << trimmedLine << endl;
            continue;
        }

        // Extract source node and validate it
        string fromNode = trim(trimmedLine.substr(0, splitPos));
        if (validNodes.find(fromNode) == validNodes.end()) {
            cerr << "Warning: Source node '" << fromNode << "' not found in node.txt. Skipping line." << endl;
            continue;
        }

        // Extract and process neighbor list
        string neighborsPart = trim(trimmedLine.substr(splitPos + 3));
        vector<string> neighbors = split(neighborsPart, ',');
        
        for (const string &toNode : neighbors) {
            // Validate destination node
            if (validNodes.find(toNode) == validNodes.end()) {
                cerr << "Warning: Neighbor node '" << toNode << "' not found in node.txt. Skipping this neighbor." << endl;
                continue;
            }
            // Add valid directed path (from -> to)
            pathPairs.emplace_back(fromNode, toNode);
        }
    }
    neighborFile.close();
    cout << "Successfully parsed " << pathPairs.size() << " directed path segments from neighbor.txt." << endl;

    // -------------------------------------------------------------------------
    // Step 3: Generate output template for manual data entry
    // -------------------------------------------------------------------------
    ofstream outFile("paths_template.txt");
    if (!outFile.is_open()) {
        cerr << "Error: Cannot create paths_template.txt. Check folder write permissions." << endl;
        return 1;
    }

    // Write CSV header (clear labels for manual filling)
    outFile << "FromBuilding,ToBuilding,TimeSpare(min),TimePopular(min),SpecialNotes,IsIndoors(Y/N)" << endl;

    // Write each path segment with empty fields for data entry
    for (const auto &pathPair : pathPairs) {
        outFile << pathPair.first << "," << pathPair.second << ",,,," << endl;
    }

    outFile.close();
    cout << "Success! paths_template.txt has been generated. Please fill in the empty fields." << endl;

    return 0;
}
