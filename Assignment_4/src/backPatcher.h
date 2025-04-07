#ifndef BACKPATCHER_H
#define BACKPATCHER_H

#include <vector>
#include <string>

class BackPatcher {
    private:
        std::vector<std::string> trueList;  // List of temporary labels for true conditions
        std::vector<std::string> falseList; // List of temporary labels for false conditions
        std::vector<std::string> nextList;  // List of temporary labels for next statements
    
    public:
        // Default constructor
        BackPatcher() 
        : trueList(0), falseList(0), nextList(0) {}
        
        // Copy constructor
        BackPatcher(const BackPatcher& other) {
            trueList = other.trueList;
            falseList = other.falseList;
            nextList = other.nextList;
        }
        
        BackPatcher& operator=(const BackPatcher& other) {
            if (this != &other) {
                trueList = other.trueList;
                falseList = other.falseList;
                nextList = other.nextList;
            }
            return *this;
        }

        // Static function to create a copy from a pointer
        static BackPatcher* copy(const BackPatcher* src) {
            if (!src) return nullptr;
            return new BackPatcher(*src); // Uses copy constructor
        }
        
        // Functions to add temporary labels to lists
        void addToTrueList(const std::string& tempLabel) {
            trueList.push_back(tempLabel);
        }
        
        void addToFalseList(const std::string& tempLabel) {
            falseList.push_back(tempLabel);
        }
        
        void addToNextList(const std::string& tempLabel) {
            nextList.push_back(tempLabel);
        }
        
        // Merge two lists into one
        static std::vector<std::string> merge(const std::vector<std::string>& list1, 
                                             const std::vector<std::string>& list2) {
            std::vector<std::string> result = list1;
            result.insert(result.end(), list2.begin(), list2.end());
            return result;
        }
        
        // Function to replace all occurrences of tempLabels in a given list with actualLabel
        std::string backPatch(const std::vector<std::string>& list, std::string instruction, const std::string& actualLabel) {
            for (const std::string& tempLabel : list) {
                // Find and replace all occurrences of tempLabel with actualLabel
                size_t pos = 0;
                while ((pos = instruction.find(tempLabel, pos)) != std::string::npos) {
                    instruction.replace(pos, tempLabel.length(), actualLabel);
                    pos += actualLabel.length();
                }
            }
            return instruction;
        }
        
        // Static version of backPatch for use with static methods
        static std::string staticBackPatch(const std::vector<std::string>& list, 
                                         std::string instruction, 
                                         const std::string& actualLabel) {
            for (const std::string& tempLabel : list) {
                size_t pos = 0;
                while ((pos = instruction.find(tempLabel, pos)) != std::string::npos) {
                    instruction.replace(pos, tempLabel.length(), actualLabel);
                    pos += actualLabel.length();
                }
            }
            return instruction;
        }
        
        // Specialized back-patching functions that also clear the list afterward
        std::string backPatchTrueList(std::string instruction, const std::string& actualLabel) {
            std::string result = backPatch(trueList, instruction, actualLabel);
            trueList.clear();
            return result;
        }
        
        std::string backPatchFalseList(std::string instruction, const std::string& actualLabel) {
            std::string result = backPatch(falseList, instruction, actualLabel);
            falseList.clear();
            return result;
        }
        
        std::string backPatchNextList(std::string instruction, const std::string& actualLabel) {
            std::string result = backPatch(nextList, instruction, actualLabel);
            nextList.clear();
            return result;
        }
        
        // Assign function for vectors - assigns all three lists at once
        void assignLists(const std::vector<std::string>& newTrueList,
            const std::vector<std::string>& newFalseList,
            const std::vector<std::string>& newNextList) {
            trueList = newTrueList;
            falseList = newFalseList;
            nextList = newNextList;
        }

        // Assign function for individual lists
        void assignTrueList(const std::vector<std::string>& newTrueList) {
            trueList = newTrueList;
        }

        void assignFalseList(const std::vector<std::string>& newFalseList) {
            falseList = newFalseList;
        }

        void assignNextList(const std::vector<std::string>& newNextList) {
            nextList = newNextList;
        }

        // Accessor functions for the lists (for debugging)
        const std::vector<std::string>& getTrueList() const {
            return trueList;
        }
        
        const std::vector<std::string>& getFalseList() const {
            return falseList;
        }
        
        const std::vector<std::string>& getNextList() const {
            return nextList;
        }
    };

    #endif