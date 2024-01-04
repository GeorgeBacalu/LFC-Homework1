#include "Grammar.h"
#include <vector>
#include <random>

Grammar::Grammar() : m_VN{}, m_VT{}, m_P{}, m_S{ 'S' } {}

Grammar::Grammar(const std::set<char>& VN, const std::set<char>& VT, const std::set<Production>& P, char S) : m_VN{ VN }, m_VT{ VT }, m_P{ P }, m_S{ S } {}

const std::set<char>& Grammar::GetVN() const { return m_VN; }

const std::set<char>& Grammar::GetVT() const { return m_VT; }

const std::set<Production>& Grammar::GetP() const { return m_P; }

char Grammar::GetS() const { return m_S; }

void Grammar::SetVN(const std::set<char>& VN) { m_VN = VN; }

void Grammar::SetVT(const std::set<char>& VT) { m_VT = VT; }

void Grammar::SetP(const std::set<Production>& P) { m_P = P; }

void Grammar::SetS(char S) { m_S = S; }

std::istream& operator>>(std::istream& in, Grammar& grammar) {
	int nrNonTerminals, nrTerminals, nrProductions;
	in >> nrNonTerminals;
	for (size_t i = 0; i < nrNonTerminals; ++i) {
		char nonTerminal;
		in >> nonTerminal;
		grammar.m_VN.insert(nonTerminal);
	}
	in >> nrTerminals;
	for (size_t i = 0; i < nrTerminals; ++i) {
		char terminal;
		in >> terminal;
		grammar.m_VT.insert(terminal);
	}
	in >> grammar.m_S >> nrProductions;
	std::string left, right;
	for (size_t i = 0; i < nrProductions; ++i) {
		in >> left >> right;
		grammar.m_P.emplace(left, right);
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, const Grammar& grammar) {
	out << "G = ({ ";
	for (char ch : grammar.m_VN)
		out << ch << " ";
	out << "}, { ";
	for (char ch : grammar.m_VT)
		out << ch << " ";
	out << "}, " << grammar.m_S << ", P})\n";
	for (const auto& [left, right] : grammar.m_P)
		if (!left.empty())
			out << left << " -> " << right << "\n";
	return out << "\n";
}

const std::set<std::string>& Grammar::generateWords(int nrWords) {
	std::string generatedWord;
	std::random_device rd;
	std::mt19937 eng(rd());
	m_generatedWords.clear();
	while (m_generatedWords.size() < nrWords) {
		generatedWord = std::string{ m_S };
		while (true) {
			std::set<Production> applicableProductions;
			for (const auto& production : m_P)
				if (generatedWord.find(production.first) != std::string::npos)
					applicableProductions.insert(production);
			if (applicableProductions.empty()) {
				m_generatedWords.insert(generatedWord);
				break;
			}
			std::uniform_int_distribution<int> distribution(0, applicableProductions.size() - 1);
			size_t productionIndex = distribution(eng);
			auto it = applicableProductions.begin();
			std::advance(it, productionIndex);
			const auto& [left, right] = *it; // selected production
			// choose random position to apply production
			std::vector<size_t> possiblePositions;
			size_t position = generatedWord.find(left);
			while (position != std::string::npos) {
				possiblePositions.push_back(position);
				position = generatedWord.find(left, position + 1);
			}
			std::uniform_int_distribution<int> distribution2(0, possiblePositions.size() - 1);
			size_t replacePosition = possiblePositions[distribution2(eng)];
			std::cout << generatedWord << " => ";
			generatedWord.replace(replacePosition, left.length(), right == "@" ? "" : right);
		}
		std::cout << generatedWord << "\n\n";
	}
	return m_generatedWords;
}

bool Grammar::isValid() const {
	return checkNonEmptySets() && checkDisjunctVNVT() && checkSInVN() && checkProductionsLeftNonTerminal() && checkProductionValidCharacters() && checkProductionWithS();
}

bool Grammar::checkNonEmptySets() const {
	if (m_VN.empty() || m_VT.empty()) {
		std::cout << "Invalid grammar: VN or VT is empty!\n";
		return false;
	}
	return true;
}

bool Grammar::checkDisjunctVNVT() const {
	for (char nonTerminal : m_VN)
		if (m_VT.find(nonTerminal) != m_VT.end()) {
			std::cout << "Invalid grammar: VN characters are in VT!\n";
			return false;
		}
	for (char terminal : m_VT)
		if (m_VN.find(terminal) != m_VN.end()) {
			std::cout << "Invalid grammar: VT characters are in VN!\n";
			return false;
		}
	return true;
}

bool Grammar::checkSInVN() const {
	if (m_VN.find(m_S) == m_VN.end()) {
		std::cout << "Invalid grammar: S is not in VN!\n";
		return false;
	}
	return true;
}

bool Grammar::checkProductionsLeftNonTerminal() const {
	bool hasNonTerminal;
	for (const auto& [left, _] : m_P) {
		hasNonTerminal = false;
		for (char nonTerminal : m_VN) {
			if (left.find(nonTerminal) != std::string::npos) {
				hasNonTerminal = true;
				break;
			}
		}
		if (!hasNonTerminal) {
			std::cout << "Invalid grammar: production without left side non-terminal!\n";
			return false;
		}
	}
	return true;
}

bool Grammar::checkProductionValidCharacters() const {
	for (const auto& [left, right] : m_P)
		for (char symbol : left + right)
			if (m_VN.find(symbol) == m_VN.end() && m_VT.find(symbol) == m_VT.end()) {
				std::cout << "Invalid grammar: production with invalid characters!\n";
				return false;
			}
	return true;
}

bool Grammar::checkProductionWithS() const {
	bool existsProductionWithS = false;
	for (const auto& [left, _] : m_P)
		if (left[0] == m_S && left.size() == 1)
			existsProductionWithS = true;
	if (!existsProductionWithS)
		std::cout << "Invalid grammar: no production with left side S!\n";
	return existsProductionWithS;
}

bool Grammar::isRegular() const {
	for (const auto& [left, right] : m_P)
		if (!(checkRegularLeft(left) || checkRegularRight(right) || checkRegularS({ left, right })))
			return false;
	return true;
}

bool Grammar::checkRegularLeft(const std::string& left) const {
	bool hasNonTerminalLeft = left.size() == 1 && m_VN.find(left[0]) != m_VN.end();
	if (!hasNonTerminalLeft) {
		std::cout << "Non regular grammar: left side must have only one non-terminal!\n";
		return false;
	}
	return true;
}

bool Grammar::checkRegularRight(const std::string& right) const {
	bool hasTerminalRight = right.size() == 1 && m_VT.find(right[0]) != m_VT.end();
	bool hasTerminalNonTerminalRight = right.size() == 2 && m_VT.find(right[0]) != m_VT.end() && m_VN.find(right[1]) != m_VN.end();
	if (!(hasTerminalRight || hasTerminalNonTerminalRight || right != "@")) {
		std::cout << "Non regular grammar: right side must have one terminal / one terminal + one non-terminal / null (lambda)!\n";
		return false;
	}
	return true;
}

bool Grammar::checkRegularS(const Production& production) const {
	const auto& [left, right] = production;
	if (right == "@" && left[0] == m_S)
		for (const auto& [_, right1] : m_P)
			if (right1.find(m_S) != std::string::npos) {
				std::cout << "Non regular grammar: S can't be on the right side if production S->@ exists!\n";
				return false;
			}
	return true;
}

FiniteAutomaton Grammar::convertToFiniteAutomaton() const {
	if (!isRegular()) {
		std::cout << "Non regular grammar\n";
		return FiniteAutomaton();
	}
	FiniteAutomaton finiteAutomaton;
	std::set<std::string> states;
	std::set<std::string> finalStates;
	Transition transition;
	for (char nonTerminal : m_VN)
		states.insert(std::string{ nonTerminal });
	states.insert("T");
	for (const auto& [left, right] : m_P) {
		std::string currentState{ left };
		char symbol = right[0];
		std::string nextState = right.size() == 2 ? std::string{ right[1] } : "T";
		transition[{currentState, symbol}].push_back(nextState);
		if (right.size() == 1)
			finalStates.insert(currentState);
	}
	if (!finalStates.empty())
		finalStates.insert("T");
	finiteAutomaton.SetStates(states);
	finiteAutomaton.SetAlphabet(m_VT);
	finiteAutomaton.SetInitialState(std::string{ m_S });
	finiteAutomaton.SetFinalStates(finalStates);
	finiteAutomaton.SetTransition(transition);
	return finiteAutomaton;
}