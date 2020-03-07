#include <iostream>
#include "KVParser.h"

using namespace std;

int main(void) {

	cout << "Simple KV parser." << endl << "Enter a function using {\"!\" (not), \"&\" (and), \"|\" (or), \"->\" (if), \"<->\" (iff), \"(\", \")\"} and get the corresponding truth table and kv diagram." << endl << endl;
	cout << "Note: \"!\" > \"&\" > \"|\" > \"+\" > \"<->\" > \"->\" so make sure to use brackets!" << endl << endl;
	cout << "F := ";
	string function;
	cin >> function;
	KVParser parser = KVParser(function);
	parser.generateTable();

	cout << "TODO: KV" << endl;

	cout << "[ENTER]" << endl;
	cin.ignore();
	cin.get();

	return 0;
}