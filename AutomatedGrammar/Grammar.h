#pragma once

#include <iostream>
#include <string>
#include <set>

using Production = std::pair<std::string, std::string>;

class Grammar {
	std::set<char> m_VN;
	std::set<char> m_VT;
	std::set<Production> m_P;
	char m_S;
public:
	Grammar();
	Grammar(const std::set<char>& VN, const std::set<char>& VT, const std::set<Production>& P, char S);
	friend std::istream& operator>>(std::istream& in, Grammar& grammar);
	friend std::ostream& operator<<(std::ostream& out, const Grammar& grammar);

	const std::set<char>& GetVN() const;
	const std::set<char>& GetVT() const;
	const std::set<Production>& GetP() const;
	char GetS() const;
	void SetVN(const std::set<char>& VN);
	void SetVT(const std::set<char>& VT);
	void SetP(const std::set<Production>& P);
	void SetS(char S);
};