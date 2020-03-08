#pragma once
#include <iostream>
#include <vector> 

using namespace std;

class KVParser {

private:

	string function;
	std::vector<int> tableEntries;
	std::vector<int> tableValues;

	std::vector<char> getChars(void);

	bool evaluate(vector<char> chars, vector<bool> values);

	std::string evaluateWithBrackets(std:: string currFunction);

	std::string evaluateRecursive(std::string currFunction);

	int* expandKV(int* currentKV, int xDim, int yDim, bool horizonzal);

	bool getValueForEntry(int entry);

public:

	KVParser(string function) {
		this->function = function;
	}

	void generateTable(void);

	void printDiagram(void);

	~KVParser() {
		
	}

};