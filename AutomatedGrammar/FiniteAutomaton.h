#pragma once

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>

using Transition = std::map<std::pair<std::string, char>, std::vector<std::string>>;

class FiniteAutomaton {
	std::set<std::string> m_states;
	std::set<char> m_alphabet;
	std::string m_initialState;
	std::set<std::string> m_finalStates;
	Transition m_transition;
public:
	FiniteAutomaton();
	FiniteAutomaton(const std::set<std::string>& states, const std::set<char>& alphabet, const std::string& initialState, const std::set<std::string>& finalStates, const Transition& transition);
	friend std::istream& operator>>(std::istream& in, FiniteAutomaton& finiteAutomaton);
	friend std::ostream& operator<<(std::ostream& out, const FiniteAutomaton& finiteAutomaton);

	const std::set<std::string>& GetStates() const;
	const std::set<char>& GetAlphabet() const;
	const std::string& GetInitialState() const;
	const std::set<std::string>& GetFinalStates() const;
	const Transition& GetTransition() const;
	void SetStates(const std::set<std::string>& states);
	void SetAlphabet(const std::set<char>& alphabet);
	void SetInitialState(const std::string& initialState);
	void SetFinalStates(const std::set<std::string>& finalStates);
	void SetTransition(const Transition& transition);
};