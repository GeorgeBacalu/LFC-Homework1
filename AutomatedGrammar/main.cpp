#include <fstream>
#include "Grammar.h"

int main() {
	Grammar grammar;
	std::ifstream fin{ "input.txt" };
	if (!fin.is_open()) {
		std::cerr << "Could not open file!\n";
		return 1;
	}
	fin >> grammar;
	std::cout << grammar;
	if (grammar.isValid()) {
		std::cout << "Grammar is valid\n";
		if (grammar.isRegular()) {
			std::cout << "Grammar is regular\n\n";
			grammar.generateWords(3);
		}
	}
}