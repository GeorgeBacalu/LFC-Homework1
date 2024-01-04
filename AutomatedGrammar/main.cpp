#include <fstream>
#include "Grammar.h"
#include "FiniteAutomaton.h"

int main() {
	Grammar grammar;
	FiniteAutomaton finiteAutomaton;
	std::ifstream finG{ "inputG.txt" };
	if (!finG.is_open()) {
		std::cerr << "Could not open grammar file!\n";
		return 1;
	}
	std::ifstream finFA{ "inputFA.txt" };
	if (!finFA.is_open()) {
		std::cerr << "Could not open automaton file!\n";
		return 1;
	}
	finG >> grammar;
	std::cout << grammar;
	if (grammar.isValid()) {
		std::cout << "Grammar is valid\n";
		if (grammar.isRegular()) {
			std::cout << "Grammar is regular\n\n";
			grammar.generateWords(3);
		}
	}
	finFA >> finiteAutomaton;
	std::cout << finiteAutomaton;
}