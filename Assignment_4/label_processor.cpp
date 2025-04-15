#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>

using namespace std;

struct CodeLine {
    string text;
    bool isLabel;
    string labelName;
};

// Function to trim whitespace from the beginning and end of a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

    ifstream inFile(argv[1]);
    if (!inFile) {
        cerr << "Error: Cannot open input file " << argv[1] << endl;
        return 1;
    }

    // First pass: Parse the input file and identify labels
    vector<CodeLine> codeLines;
    regex labelRegex("^\\s*label\\s+(@L\\d+|\\w+):\\s*$");
    regex gotoRegex("\\s*goto\\s+(@L\\d+)\\s*");
    regex ifGotoRegex("\\s*if\\s+.*\\s+goto\\s+(@L\\d+)\\s*");
    
    string line;
    while (getline(inFile, line)) {
        CodeLine codeLine;
        codeLine.text = line;
        codeLine.isLabel = false;
        
        smatch matches;
        if (regex_match(line, matches, labelRegex)) {
            codeLine.isLabel = true;
            codeLine.labelName = matches[1].str();
        }
        
        codeLines.push_back(codeLine);
    }
    
    // Second pass: Identify consecutive labels and create mapping
    unordered_map<string, string> labelMap;
    
    for (size_t i = 0; i < codeLines.size(); i++) {
        if (codeLines[i].isLabel) {
            string keepLabel = codeLines[i].labelName;
            
            // Look ahead for consecutive labels
            size_t j = i + 1;
            while (j < codeLines.size() && codeLines[j].isLabel) {
                // Map this label to the first label
                labelMap[codeLines[j].labelName] = keepLabel;
                j++;
            }
            
            // Skip the consecutive labels in the outer loop
            i = j - 1;
        }
    }
    
    // Third pass: Process the code with the label mapping
    ofstream outFile(argv[2]);
    if (!outFile) {
        cerr << "Error: Cannot open output file " << argv[2] << endl;
        return 1;
    }
    
    bool skipLine = false;
    
    for (size_t i = 0; i < codeLines.size(); i++) {
        const CodeLine& current = codeLines[i];
        
        // Check if this is a label that should be skipped
        if (current.isLabel && labelMap.find(current.labelName) != labelMap.end()) {
            skipLine = true;
        } else {
            skipLine = false;
        }
        
        if (!skipLine) {
            string processedLine = current.text;
            
            // Replace goto statements
            smatch gotoMatches;
            if (regex_search(processedLine, gotoMatches, gotoRegex)) {
                string targetLabel = gotoMatches[1].str();
                if (labelMap.find(targetLabel) != labelMap.end()) {
                    // Replace the target label with the mapped label
                    size_t pos = processedLine.find(targetLabel);
                    processedLine.replace(pos, targetLabel.length(), labelMap[targetLabel]);
                }
            }
            
            // Replace if goto statements
            smatch ifGotoMatches;
            if (regex_search(processedLine, ifGotoMatches, ifGotoRegex)) {
                string targetLabel = ifGotoMatches[1].str();
                if (labelMap.find(targetLabel) != labelMap.end()) {
                    // Replace the target label with the mapped label
                    size_t pos = processedLine.find(targetLabel);
                    processedLine.replace(pos, targetLabel.length(), labelMap[targetLabel]);
                }
            }
            
            outFile << processedLine << endl;
        }
    }
    
    inFile.close();
    outFile.close();
    
    cout << "Label processing completed successfully." << endl;
    cout << "Mapped labels:" << endl;
    for (const auto& mapping : labelMap) {
        cout << "  " << mapping.first << " -> " << mapping.second << endl;
    }
    
    return 0;
}

