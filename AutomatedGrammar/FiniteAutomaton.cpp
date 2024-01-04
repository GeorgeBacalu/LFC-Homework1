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
	return out;
}