#include <iostream>
#include "KVParser.h"
#include <algorithm>
#include <string>

using namespace std;

int main(void) {

	cout << "Simple KV parser." << endl << "Enter a function using {\"!\" (not), \"&\" (and), \"|\" (or), \"->\" (if), \"<->\" (iff), \"(\", \")\"}"
		<< endl << "and get the corresponding truth table and kv diagram." << endl;
	cout << "Note: \"!\" >> \"&\" >> \"|\" >> \"+\" >> \"<->\" >> \"->\", so make sure to use brackets!" << endl << "Spaces will be deleted." << endl << endl;
	cout << " F := ";
	string function;
	getline(cin, function);
	// remove whitespace
	function.erase(remove_if(function.begin(), function.end(), isspace), function.end());
	cout << endl;

	KVParser parser = KVParser(function);
	parser.generateTable();

	cout << endl << endl << " KV: " << endl << endl;
	parser.printDiagram();
	
	cout << endl << endl;
	cout << "[Press enter to exit] ";
	cin.get();

	return 0;
}