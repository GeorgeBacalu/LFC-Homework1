#include "FiniteAutomaton.h"

FiniteAutomaton::FiniteAutomaton() : m_states{}, m_alphabet{}, m_initialState{}, m_finalStates{}, m_transition{} {}

FiniteAutomaton::FiniteAutomaton(const std::set<std::string>& states, const std::set<char>& alphabet, const std::string& initialState, const std::set<std::string>& finalStates, const Transition& transition)
	: m_states{ states }, m_alphabet{ alphabet }, m_initialState{ initialState }, m_finalStates{ finalStates }, m_transition{ transition } {}

const std::set<std::string>& FiniteAutomaton::GetStates() const { return m_states; }

const std::set<char>& FiniteAutomaton::GetAlphabet() const { return m_alphabet; }

const std::string& FiniteAutomaton::GetInitialState() const { return m_initialState; }

const std::set<std::string>& FiniteAutomaton::GetFinalStates() const { return m_finalStates; }

const Transition& FiniteAutomaton::GetTransition() const { return m_transition; }

void FiniteAutomaton::SetStates(const std::set<std::string>& states) { m_states = states; }

void FiniteAutomaton::SetAlphabet(const std::set<char>& alphabet) { m_alphabet = alphabet; }

void FiniteAutomaton::SetInitialState(const std::string& initialState) { m_initialState = initialState; }

void FiniteAutomaton::SetFinalStates(const std::set<std::string>& finalStates) { m_finalStates = finalStates; }

void FiniteAutomaton::SetTransition(const Transition& transition) { m_transition = transition; }

std::istream& operator>>(std::istream& in, FiniteAutomaton& finiteAutomaton) {
	int nrStates, nrValuesAlphabet, nrFinalStates, nrTransitions;
	in >> nrStates;
	for (size_t i = 0; i < nrStates; ++i) {
		std::string state;
		in >> state;
		finiteAutomaton.m_states.insert(state);
	}
	in >> nrValuesAlphabet;
	for (size_t i = 0; i < nrValuesAlphabet; ++i) {
		char symbol;
		in >> symbol;
		finiteAutomaton.m_alphabet.insert(symbol);
	}
	in >> finiteAutomaton.m_initialState >> nrFinalStates;
	for (size_t i = 0; i < nrFinalStates; ++i) {
		std::string finalState;
		in >> finalState;
		finiteAutomaton.m_finalStates.insert(finalState);
	}
	in >> nrTransitions;
	for (size_t i = 0; i < nrTransitions; ++i) {
		std::string currentState, nextState;
		char symbol;
		in >> currentState >> symbol >> nextState;
		finiteAutomaton.m_transition[{currentState, symbol}].push_back(nextState);
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, const FiniteAutomaton& finiteAutomaton) {
	out << "AFN = ({ ";
	for (const auto& state : finiteAutomaton.m_states)
		out << state << " ";
	out << "}, { ";
	for (const auto& symbol : finiteAutomaton.m_alphabet)
		out << symbol << " ";
	out << "}, delta, " << finiteAutomaton.m_initialState << ", { ";
	for (const auto& finalState : finiteAutomaton.m_finalStates)
		out << finalState << " ";
	out << "})\n\n";
	out << "delta:\n";
	for (const auto& [key, value] : finiteAutomaton.m_transition) {
		const auto& [currentState, symbol] = key;
		for (const auto& nextState : value)
			out << " (" << currentState << ", " << symbol << ") -> " << nextState << "\n";
	}
	return out << "\n";
}

bool FiniteAutomaton::isValid() const {
	return checkNonEmptyStates() && checkNonEmptyAlphabet() && checkValidInitialState() && checkValidFinalStates() && checkValidTransition();
}

bool FiniteAutomaton::checkNonEmptyStates() const {
	if (m_states.empty()) {
		std::cout << "Invalid automaton: empty set of states!\n";
		return false;
	}
	return true;
}

bool FiniteAutomaton::checkNonEmptyAlphabet() const {
	if (m_alphabet.empty()) {
		std::cout << "Invalid automaton: empty alphabet!\n";
		return false;
	}
	return true;
}

bool FiniteAutomaton::checkValidInitialState() const {
	if (m_initialState.empty()) {
		std::cout << "Invalid automaton: empty initial state!\n";
		return false;
	}
	if (m_states.find(m_initialState) == m_states.end()) {
		std::cout << "Invalid automaton: initial state not in set of states!\n";
		return false;
	}
	return true;
}

bool FiniteAutomaton::checkValidFinalStates() const {
	if (m_finalStates.empty()) {
		std::cout << "Invalid automaton: empty set of final states!\n";
		return false;
	}
	for (const auto& finalState : m_states)
		if (m_states.find(finalState) == m_states.end()) {
			std::cout << "Invalid automaton: final state not in set of states!\n";
			return false;
		}
	return true;
}

bool FiniteAutomaton::checkValidTransition() const {
	if (m_transition.empty()) {
		std::cout << "Invalid automaton: empty transition function!\n";
		return false;
	}
	for (const auto& [key, value] : m_transition) {
		const auto& [currentState, symbol] = key;
		if (m_states.find(currentState) == m_states.end()) {
			std::cout << "Invalid automaton: State " << currentState << "from transition function not in set of states!\n";
			return false;
		}
		if (m_alphabet.find(symbol) == m_alphabet.end()) {
			std::cout << "Invalid automaton: Symbol " << symbol << "from transition function not in alphabet!\n";
			return false;
		}
		for (const auto& nextState : value)
			if (m_states.find(nextState) == m_states.end()) {
				std::cout << "Invalid automaton: State " << nextState << "from transition function not in set of states!\n";
				return false;
			}
	}
	return true;
}

bool FiniteAutomaton::checkWord(const std::string& currentState, const std::string& word) const {
	if (word.empty())
		return m_finalStates.find(currentState) != m_finalStates.end();
	char symbol = word[0];
	for (const auto& [_, nextStates] : m_transition)
		for (const auto& nextState : nextStates)
			if (checkWord(nextState, word.substr(1)))
				return true;
	return false;
}

bool FiniteAutomaton::isDeterministic() const {
	for (const auto& state : m_states)
		for (const auto& symbol : m_alphabet) {
			if (m_transition.find({ state, symbol }) == m_transition.end()) {
				std::cout << "Non-deterministic automaton: missing transition for state " << state << " and symbol " << symbol << "!\n";
				return false;
			}
			const auto& nextStates = m_transition.find({ state, symbol })->second;
			if (nextStates.size() > 1) {
				std::cout << "Non-deterministic automaton: multiple transitions for state " << state << " and symbol " << symbol << "!\n";
				return false;
			}
		}
	std::cout << "Deterministic automaton!\n";
	return true;
}