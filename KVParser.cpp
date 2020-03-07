#include "KVParser.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <bitset>

using namespace std;

void KVParser::generateTable(void) {

	cout << "Generating truth table for F := " + function << endl;

	vector<char> chars = getChars();
	const int rows = pow(2.0, (double) chars.size());

	for (int i = 0; i < rows; i++) {
		string values = bitset<32>(i).to_string();
		values = values.substr(values.size() - chars.size(), values.size());
		bool* boolArray = new bool[(int) chars.size()];
		char* charArray = new char[(int) chars.size()];
		int index = 0;
		for (size_t a = 0; a < chars.size(); a++) {
			boolArray[index] = values[index] == '0' ? false : true;
			charArray[index] = chars[index];
			cout << charArray[index] << "=" << boolArray[index] << "; ";
			index++;
		}
		
		bool eval = evaluate(charArray, boolArray);

		cout << "evaluates to " << eval << endl;



		delete[] boolArray;
		boolArray = NULL;
		delete[] charArray;
		charArray = NULL;
	}
}

void KVParser::printDiagram(void) {
	cout << kv << endl;
}

vector<char> KVParser::getChars(void) {
	std::vector<char> chars;
	for (int i = 0; i < function.length(); i++) {
		char currChar = function[i];
		if (find(chars.begin(), chars.end(), currChar) == chars.end() && currChar >= 'a' && currChar <= 'z') { // a - z
			chars.push_back(currChar);
		}
	}
	sort(chars.begin(), chars.end());
	return chars;
}

bool KVParser::evaluate(char chars[], bool values[]) {
	if (sizeof(values) != sizeof(chars)) {
		cout << "ERROR" << endl;
		return false;
	}
	string insertedValues = function;
	for (int i = 0; i < sizeof(chars); i++) {
		replace(insertedValues.begin(), insertedValues.end(), chars[i], values[i] ? '1' : '0');
	}

	return evaluateWithBrackets(insertedValues) == "1" ? true : false;
}

string KVParser::evaluateWithBrackets(string currFunction) {

	int openBracketOccurence = currFunction.find("(");
	while (openBracketOccurence != string::npos) {
		
		int nextOpenBracket = currFunction.find("(", openBracketOccurence + 1);
		int nextClosedBracket = currFunction.find(")", openBracketOccurence + 1);
		
		//cout << openBracketOccurence << "; " << nextClosedBracket << "   " << function << endl;

		if (nextClosedBracket < nextOpenBracket || nextOpenBracket == string::npos) { // Found pair: (openBracketOccurence, nextClosedBracket)
			
			currFunction = currFunction.substr(0, openBracketOccurence) + 
				evaluateRecursive(currFunction.substr(openBracketOccurence + 1, nextClosedBracket - (openBracketOccurence + 1))) +
					currFunction.substr(nextClosedBracket + 1, currFunction.size() - (nextClosedBracket + 1));
			openBracketOccurence = currFunction.find("(");
			
		}
		else {
			openBracketOccurence = nextOpenBracket;
		}
	}

	//cout << currFunction << endl;

	return evaluateRecursive(currFunction);

}

// ->; !; ^; v; <-> (no brackets)
string KVParser::evaluateRecursive(string currFunction) {
	
	//cout << " evaluating " << currFunction << endl;

	if (currFunction.size() <= 1) {
		return currFunction;
	}
	
	int occurence = currFunction.find("!");
	if (occurence != string::npos) {
		string first = currFunction.substr(0, occurence);
		string middle = currFunction.substr(occurence + 1, 1) == "1" ? "0" : "1";
		string last = currFunction.substr(occurence + 2, currFunction.size() - (occurence + 2));
		currFunction = evaluateRecursive(first + middle + last);
	}

	occurence = currFunction.find("&");
	if (occurence != string::npos) {
		string first = evaluateRecursive(currFunction.substr(0, occurence));
		string last = evaluateRecursive(currFunction.substr(occurence + 1, currFunction.size() - (occurence + 1)));
		currFunction = (first == "1" && last == "1") ? "1" : "0";
	}

	occurence = currFunction.find("|");
	if (occurence != string::npos) {
		string first = evaluateRecursive(currFunction.substr(0, occurence));
		string last = evaluateRecursive(currFunction.substr(occurence + 1, currFunction.size() - (occurence + 1)));
		currFunction = (first == "1" || last == "1") ? "1" : "0";
	}

	occurence = currFunction.find("+");
	if (occurence != string::npos) {
		string first = evaluateRecursive(currFunction.substr(0, occurence));
		string last = evaluateRecursive(currFunction.substr(occurence + 1, currFunction.size() - (occurence + 1)));
		currFunction = (first != last) ? "1" : "0";
	}

	occurence = currFunction.find("<->");
	if (occurence != string::npos) {
		string first = evaluateRecursive(currFunction.substr(0, occurence));
		string last = evaluateRecursive(currFunction.substr(occurence + 3, currFunction.size() - (occurence + 3)));
		currFunction = (first == last) ? "1" : "0";
	}

	occurence = currFunction.find("->");
	if (occurence != string::npos) {
		string first = evaluateRecursive(currFunction.substr(0, occurence));
		string last = evaluateRecursive(currFunction.substr(occurence + 2, currFunction.size() - (occurence + 2)));
		currFunction = (first == "0" || last == "1") ? "1" : "0";
	}

	return currFunction;

}