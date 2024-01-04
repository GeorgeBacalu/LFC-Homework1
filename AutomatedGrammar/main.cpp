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
	int option;
	std::string word;
	finG >> grammar;
	finFA >> finiteAutomaton;
	do {
		std::cout << "Command menu:\n";
		std::cout << "0. Exit\n";
		std::cout << "1. Display grammar\n";
		std::cout << "2. Generate words in grammar\n";
		std::cout << "3. Get equivalent finite automaton with grammar G\n";
		std::cout << "4. Check if word is accepted by finite automaton\n";
		std::cout << "5. Generate word in G + check if it is accepted by finite automaton\n";
		std::cout << "Choose option: ";
		std::cin >> option;
		switch (option) {
		case 0: break;
		case 1: std::cout << "\nGrammar:\n" << grammar; break;
		case 2:
			int nrWords;
			std::cout << "\nEnter number of words to generate: ";
			std::cin >> nrWords;
			if (grammar.isValid())
				grammar.generateWords(nrWords);
			break;
		case 3:
			if (grammar.isValid()) {
				std::cout << "\nGrammar is valid\n";
				if (grammar.isRegular()) {
					std::cout << "Grammar is regular\n";
					std::cout << "Grammar -> Finite Automaton:\n\n" << grammar.convertToFiniteAutomaton();
				}
			}
			break;
		case 4:
			std::cout << "\nEnter word to check: ";
			std::cin >> word;
			std::cout << "Word is" << (finiteAutomaton.checkWord(finiteAutomaton.GetInitialState(), word) ? " " : " not ") << "accepted!\n\n";
			break;
		case 5:
			std::cout << "\n";
			std::set<std::string> generatedWordSet = grammar.generateWords(1);
			std::string generatedWord = *generatedWordSet.begin();
			std::cout << "Word: " << generatedWord << " generated from grammar is" << (finiteAutomaton.checkWord(finiteAutomaton.GetInitialState(), generatedWord) ? " " : " not ") << "accepted by automaton\n";
			break;
		}
	} while (option);
}