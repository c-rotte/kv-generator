#include "KVParser.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <bitset>

using namespace std;

void KVParser::generateTable(void) {

	cout << "Generating truth table for F := " + function << endl << endl;

	vector<char> chars = getChars();
	const int rows = pow(2.0, (double) chars.size());

	// loop through all rows of the table
	for (int i = 0; i < rows; i++) {
		// Integer to bit representation
		string values = bitset<32>(i).to_string();
		values = values.substr(values.size() - chars.size(), values.size());
		vector<bool> boolVec;
		vector<char> charVec;
		int index = 0;
		cout << " ";
		for (size_t a = 0; a < chars.size(); a++) {
			// save current char and its bool value
			boolVec.push_back(values[index] == '0' ? false : true);
			charVec.push_back(chars[index]);
			cout << charVec[index] << "=" << boolVec[index] << "; ";
			index++;
		}
		
		bool eval = evaluate(charVec, boolVec);
		tableEntries.push_back(i);
		tableValues.push_back(eval);

		cout << "evaluates to " << eval << endl;

	}
}

void KVParser::printDiagram(void) {
	
	vector<char> chars = getChars();
	int xDim = 2, yDim = 1;
	int* kv = new int[xDim * yDim];
	
	// diagram for one atom
	kv[0 * yDim + 0] = 0;
	kv[1 * yDim + 0] = 1;

	bool horizontal = false;
	for (int i = 1; i < chars.size(); i++) {
		// expand the diagram for every additional char
		kv = expandKV(kv, xDim, yDim, horizontal);
		if (i % 2 == 0) {
			xDim *= 2;
		} else {
			yDim *= 2;
		}
		horizontal = !horizontal;
	}

	
	for (int y = 0; y < yDim; y++) {
		cout << " ";
		for (int x = 0; x <= xDim * 4; x++) {
			cout << "-";
		}
		cout << endl << " |";
		for (int x = 0; x < xDim; x++) {
			kv[x * yDim + y] = getValueForEntry(kv[x * yDim + y]) ? 'X' : ' ';
			cout << " " << (char) kv[x * yDim + y] << " ";
			cout << "|";
		}
		cout << endl;
	}
	cout << " ";
	for (int x = 0; x <= xDim * 4; x++) {
		cout << "-";
	}
}

// lookup bool for given entry ((binary) number representation)
bool KVParser::getValueForEntry(int entry) {
	for (int i = 0; i < tableEntries.size(); i++) {
		int e = tableEntries[i];
		if (entry == e) {
			return tableValues[i];
		}
	}
	cout << "ERROR" << endl;
	return false;
}

// first to last bit -> a, b, ....
int* KVParser::expandKV(int* currentKV, int xDim, int yDim, bool horizonzal) {
	
	int* newChar = new int[xDim * 2 * yDim];

	if (horizonzal) {
		xDim *= 2;
		//copy
		for (int x = 0; x < xDim / 2; x++) {
			for (int y = 0; y < yDim; y++) {
				// append 0
				newChar[x * yDim + y] = currentKV[x * yDim + y] * 2;
			}
		}
	} else {
		yDim *= 2;
		//copy
		for (int x = 0; x < xDim; x++) {
			for (int y = 0; y < yDim / 2; y++) {
				// append 0
				newChar[x * yDim + y] = currentKV[x * yDim / 2 + y] * 2;
			}
		}
	}

	if (horizonzal) {
		for (int x = xDim / 2; x < xDim; x++) {
			for (int y = 0; y < yDim; y++) {
				// mirror
				newChar[x * yDim + y] = newChar[(xDim - 1 - x) * yDim + y];
				// set last Bit
				newChar[x * yDim + y]++;
			}
		}
	} else {
		for (int x = 0; x < xDim; x++) {
			for (int y = yDim / 2; y < yDim; y++) {
				// mirror
				newChar[x * yDim + y] = newChar[x * yDim + (yDim - 1 - y)];
				// set last Bit
				newChar[x * yDim + y]++;
			}
		}
	}

	return newChar;

}

vector<char> KVParser::getChars(void) {
	vector<char> chars;
	for (int i = 0; i < function.length(); i++) {
		char currChar = function[i];
		if (find(chars.begin(), chars.end(), currChar) == chars.end() && currChar >= 'a' && currChar <= 'z') { // a - z
			chars.push_back(currChar);
		}
	}
	sort(chars.begin(), chars.end());
	return chars;
}

bool KVParser::evaluate(vector<char> chars, vector<bool> values) {
	if (values.size() != chars.size()) {
		cout << "ERROR" << endl;
		return false;
	}
	string insertedValues = function;
	for (int i = 0; i < chars.size(); i++) {
		//cout << "char: " << chars[i] << endl;
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

	if (currFunction.size() == 1) {
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


