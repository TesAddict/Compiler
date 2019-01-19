#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(std::string fileName)
{
	std::ifstream inFile;
	inFile.open(fileName);
	if (!inFile)
	{
		std::cout << "Source file does not exist.\n";
		exit(1);
	}
	while (inFile >> token)
	{
		auto index = find(keywords.begin(), keywords.end(), token);
		if (index != keywords.end())
		{
			token_actual.push_back(token);
			token_type.push_back("keyword");
		}
		else
		{
			if (isNumber(token))
			{	
				token_actual.push_back(token);
				token_type.push_back("literal");
			}
			else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "=")
			{
				token_actual.push_back(token);
				token_type.push_back("operator");
			}
			else if (token == "==" || token == ">=" || token == "<=" || token == "!=")
			{
				token_actual.push_back(token);
				token_type.push_back("conditional");
			}
			else
			{
				token_actual.push_back(token);
				token_type.push_back("identifier");
			}
		}
	}
}

std::vector<std::string> LexicalAnalyzer::getActual()
{
	return token_actual;
}

std::vector<std::string> LexicalAnalyzer::getType()
{
	return token_type;
}

bool LexicalAnalyzer::isNumber(std::string check)
{
	if (check[0] == '-')
	{
		std::string::const_iterator it = check.begin()+1;
		while (it != check.end() && std::isdigit(*it)) ++it;
		return !check.empty() && it == check.end();
	}
	else
	{
		std::string::const_iterator it = check.begin();
		while (it != check.end() && std::isdigit(*it)) ++it;
		return !check.empty() && it == check.end();
	}
}