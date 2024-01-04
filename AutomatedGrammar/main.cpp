#include <fstream>
#include "Grammar.h"

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
	if (finiteAutomaton.isValid()) {
		std::cout << finiteAutomaton;
		finiteAutomaton.isDeterministic();
		std::string word;
		std::cout << "Enter word to check: ";
		std::cin >> word;
		std::cout << "Word is" << (finiteAutomaton.checkWord(finiteAutomaton.GetInitialState(), word) ? " " : " not ") << "accepted!\n";
	}
	std::cout << "Grammar -> Finite Automaton: " << grammar.convertToFiniteAutomaton();
}