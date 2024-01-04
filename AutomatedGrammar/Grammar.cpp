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
	for (int i = 0; i < nrNonTerminals; ++i) {
		char nonTerminal;
		in >> nonTerminal;
		grammar.m_VN.insert(nonTerminal);
	}
	in >> nrTerminals;
	for (int i = 0; i < nrTerminals; ++i) {
		char terminal;
		in >> terminal;
		grammar.m_VT.insert(terminal);
	}
	in >> grammar.m_S >> nrProductions;
	std::string left, right;
	for (int i = 0; i < nrProductions; ++i) {
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