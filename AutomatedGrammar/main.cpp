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
	if (grammar.isValid())
		grammar.generateWords(3);
	else
		std::cout << "Invalid grammar!\n";
}