// symbolTable.cpp : This file contains the 'main' function. Program execution begins and ends there.

// We will build a symbol-table in this assignment. At this initial stage of the project,
// we will count  many details regarding an actual symbol-table and we will simply adhere to the basic concept that
// "a symbol-table is an efficient data-dictionary for the symbols used in a program".
// Thus, our focus in this assignment is to construct a simple hash-based data-dictionary.

// Input:
// The input to your program will be a sequence of two-tuples, where each element in each tuple is a string.
// An example of input sequence is given below.

// -> int, INTEGER
// -> myFunction, FUNCTION
// -> x, IDENTIFIER
// -> 5, NUMBER

// NOTE: Use these console input as an example.
// -> int INTEGER
// -> myFunction FUNCTION
// -> x IDENTIFIER
// -> 5 NUMBER

// The first element of each tuple will be the key of the record to be stored in the symbol-table.
// Hence, you have to apply the hash function on the first element of each tuple.

#include <iostream>
#include <cstring>
#include <iomanip>
#include <windows.h>

using namespace std;

#define MAX_LENGTH 20

class SymbolInfo { // class for storing a symbol and its type with a next pointer
public:
	string symbol;
	string type;
	SymbolInfo* next;

public:
	friend class SymbolTable;

	SymbolInfo() {
		next = NULL;
	}

	SymbolInfo(string symbol, string type) {
		this->symbol = symbol;
		this->type = type;
		next = NULL;
	}
};

SymbolInfo* block[MAX_LENGTH];

class SymbolTable {
	// Class creating a hash table, functions to insert, search, dump, delete symbols

public:
	// Set the SymbolTable to NULL at the beginning of the program
	SymbolTable() {
		for (int i = 0; i < MAX_LENGTH; i++)
			block[i] = NULL;
	}

public:
	int hashFunction(string symbol);         // Function to apply hash function on the key
	void insert(string symbol, string type); // Function to insert new symbols in hash table
	void dump();                             // Function to print the whole hash table
	void lookup(string symbol);              // Function to search desired symbol
	bool erase(string symbol);               // Function to delete a certain input symbol
};

// Function for setting the font color in the console
void SetColor(int ForegroundColor) {
	WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	// We use csbi for the wAttributes word.
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
		// Mask out all but the background attribute, and add in the foreground color
		wColor = (csbi.wAttributes & 0xF0) + (ForegroundColor & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}

int SymbolTable::hashFunction(string symbol) {
	
	int sum = 0;
	
	for (int i = 0; i < symbol.length(); i++) {
		// Adding up the ASCII values of each character in the symbol
		sum = sum + symbol[i];
	}
	// Then modulo the index with the MAX_LENGTH and return the result
	return (sum % MAX_LENGTH);
}

void SymbolTable::insert(string symbol, string type) {
	
	// Hash the symbol and get the position
	int position = hashFunction(symbol);
	SymbolInfo* currentSymbol = new SymbolInfo(symbol, type);

	// If the bucket is empty, create a new SymbolInfo object and store the symbol and type in it
	if (block[position] == NULL) {
		block[position] = currentSymbol;
	}
	// If the bucket is not empty, add the new SymbolInfo node to the front of the linked list
	else {
		SymbolInfo* start = block[position];
		while (start->next != NULL)
			start = start->next;
		start->next = currentSymbol;
	}
	SetColor(2);
	cout
		<< endl
		<< "Inserted successfully"
		<< endl;
}

void SymbolTable::dump() {
	int symbolFieldWidth = 12;
	int typeFieldWidth = 12;

	cout << "--------------------------------------------------" << endl;
	cout << "\t\t|"
		<< "Symbol Table"
		<< "|" << endl;
	cout << "--------------------------------------------------" << endl;

	for (int i = 0; i < MAX_LENGTH; i++) {

		SymbolInfo* start = block[i];
		if (start == NULL)
			cout << "\n";
		while (start != NULL) {
			cout << endl;

			SetColor(15);
			cout << setw(3) << left << i;
			cout << "-> ";

			SetColor(11);
			cout << "Symbol: ";
			cout << setw(symbolFieldWidth)
				<< left
				<< start->symbol;

			SetColor(14);
			cout << " Type: ";
			cout << setw(typeFieldWidth)
				<< left
				<< start->type;

			start = start->next;
		}
	}
}

void SymbolTable::lookup(string symbol) {
	
	int position = hashFunction(symbol);
	int found = 0;

	// Hash the input value and put it on temp
	SymbolInfo* temp = block[position];

	// Compare temp with all element in the SymbolTable
	// Return true if found otherwise, false
	while (temp != NULL) {
		if (temp->symbol == symbol) {
			SetColor(11);
			cout << "\n"
				<< "Symbol: "
				<< temp->symbol;
			SetColor(14);
			cout << " Type: "
				<< temp->type
				<< endl;

			found = 1;
		}
		temp = temp->next;
	}
	if (found == 1) {
		SetColor(2);
		cout << "[INFO] Element found at " << position << "\n";
	}
	else {
		SetColor(4);
		cout << "\n[INFO] Symbol not found\n";
	}
}

bool SymbolTable::erase(string symbol) {
	
	int position = hashFunction(symbol);
	
	// Hash the input value and put it on temp
	SymbolInfo* temp = block[position];
	SymbolInfo* parameter = block[position];

	// If the bucket is empty
	if (temp == NULL) {
		return false;
	}
	// If a bucket has single element
	else if (temp->next == NULL && temp->symbol == symbol) {
		block[position] = NULL;
		delete temp;
		return true;
	}
	// If a bucket has multiple elements,
	while (temp->symbol != symbol && temp->next != NULL) {
		parameter = temp;
		temp = temp->next;
	}

	if (temp->symbol == symbol && temp->next != NULL) {
		if (parameter->symbol == temp->symbol) {
			block[position] = temp->next;
		}
		else {
			parameter->next = temp->next;
			temp->next = NULL;
		}
		delete temp;
		return true;
	}
	else {
		parameter->next = NULL;
		temp->next = NULL;
		delete temp;
		return true;
	}
	return false;
}

void menu() {
	SetColor(13);
	cout << "\n Press 1 to INSERT a new symbol along with its type into the symbol-table\n";
	SetColor(9);
	cout << "\n Press 2 to LOOKUP a in the symbol-table\n";
	SetColor(10);
	cout << "\n Press 3 to DUMP the contents of the symbol-table\n";
	SetColor(11);
	cout << "\n Press 4 to DELETE a symbol in the symbol-table\n";
	SetColor(14);
	cout << "\n Press 0 to EXIT\n";
	SetColor(15);
	cout << "\n Enter your choice: ";
}

int main() {
	SymbolTable ST;

	int choice;

	string symbol, type;

	menu();
	cin >> choice;

	while (choice != 0) {

		switch (choice) {
		case 0:
			SetColor(12);
			cout << "\n[INFO] Exiting the program\n";
			exit(1);
			break;

		case 1:
			cout << "\nEnter SYMBOL and TYPE to insert: ";
			cin >> symbol >> type;

			ST.insert(symbol, type);
			SetColor(2);
			cout << "\n"
				<< "[INFO] Value inserted at: "
				<< ST.hashFunction(symbol)
				<< "\n";
			break;

		case 2:
			cout << "\nInput symbol and type to lookup: ";
			cin >> symbol;
			ST.lookup(symbol);
			break;

		case 3:
			ST.dump();
			break;

		case 4:
			cout << "\nInput symbol to delete: ";
			cin >> symbol;
			if (ST.erase(symbol)) {
				SetColor(2);
				cout << "\n[INFO] Element deleted\n";
			}
			else {
				SetColor(4);
				cout << "\n[INFO] Element not found\n";
			}
			break;

		default:
			SetColor(4);
			cout << "\n[WARNING] Invalid choice\n";
		}
		menu();
		cin >> choice;
	}
	return 0;
}