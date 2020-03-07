#pragma once
#include <iostream>
#include <vector> 

using namespace std;

class KVParser {

private:

	string function;
	std::string kv = "empty";
	char* table;

	std::vector<char> getChars(void);

	bool evaluate(char chars[], bool values[]);

	std::string evaluateWithBrackets(std:: string currFunction);

	std::string evaluateRecursive(std::string currFunction);

public:

	KVParser(string function) {
		this->function = function;
	}

	void generateTable(void);

	void printDiagram(void);

	~KVParser() {
		delete table;
		table = NULL;
	}

};