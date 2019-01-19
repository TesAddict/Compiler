#ifndef _LEXICALANALYZER_H_INCLUDED_
#define _LEXICALANALYZER_H_INCLUDED_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

class LexicalAnalyzer
{
private:
	std::string token;
	std::vector<std::string> token_actual;
	std::vector<std::string> token_type;
	std::vector<std::string> keywords = {";", "int", "(", ")", "if", "begin", "end", "then", "while", "do", "array"};

public:
	LexicalAnalyzer(std::string);
	std::vector<std::string> getActual();
	std::vector<std::string> getType();
	bool isNumber(std::string);
};
#endif


