#ifndef __PARSER_INCLUDED__
#define __PARSER_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class Parser
{
private:
	std::vector<std::string> tokens_type;
	std::vector<std::string> tokens_actual;
	std::vector<std::string> lookup_table;
	std::vector<std::string> saved_expression_actual;
	std::vector<std::string> saved_expression_type;
	std::vector<int> nested_counter;
	std::string current_token_type;
	std::string current_token_actual;
	int index;
	int accumulator = 0;
	int counter = 0;
	bool compare = true;

public:
	Parser(std::vector<std::string>, std::vector<std::string>);
	void nextToken();
	int accept(std::string, bool);
	int expect(std::string, bool);
	void factor();
	void term();
	void expression();
	void condition();
	void statement();
	void block();
	void program();
	int getNum(std::string);
	void expressionHelper();
	void termHelper();
	void parenHelper();
	std::string lookUp(std::string);
	void printVector();
	bool lookUpBool(std::string);
	void addToLookUp(std::string, std::string);
	void addArrayToLookUp(std::string, std::string);
	void updateArrayLookUp(std::string, std::string, std::string);
	void printLookUp();
	void updateLookUp(std::string, std::string);
	int lookUpInt(std::string name);
	void goToEquality();
	void goToParen();
	bool isNumber(const std::string&);
	void goToEnd();
	void goToWhile(int);
	void copyExpression();
	void copyExpressionArray();
	void insertExpression();
	void printExpression();
	void printLine();
};
#endif
