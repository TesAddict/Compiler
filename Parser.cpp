#include "Parser.h"

//Constructor for Parser. Takes two vectors which correspond to token type and actual token.
Parser::Parser(std::vector<std::string> tokens_type, std::vector<std::string> tokens_actual)
{
	this->tokens_type = tokens_type;
	this->tokens_actual = tokens_actual;
	index = 0;
	current_token_type = tokens_type[index];
	current_token_actual = tokens_actual[index];
}

//Function that will advance the program counter and update all indexes. 
void Parser::nextToken()
{
	index = index + 1;
	if (index < tokens_actual.size())
	{
		current_token_type = tokens_type[index];
		current_token_actual = tokens_actual[index];
	}
}

//If a symbol passes is equal to the current symbol, will return true. 
int Parser::accept(std::string symbol, bool type)
{
	if (current_token_type == symbol && type == 0)
	{
		nextToken();
		return 1;
	}
	else if (current_token_actual == symbol && type == 1)
	{
		nextToken();
		return 1;
	}
	return 0;
}

//Similar to accept, although if the symbols do not match a compiler error will occur.
int Parser::expect(std::string symbol, bool type)
{
	if (accept(symbol, type))
		return 1;
	else
	{
		std::cout << "expect: unexpected symbol: " << current_token_actual << "\n";
		throw std::exception();
	}
	return 0;
}

//A non-terminal of the Recursive Descent grammar. 
void Parser::factor()
{
	if (isNumber(tokens_actual[index]))
	{
		nextToken();
		accumulator = getNum(tokens_actual[index - 1]);
	}
	else if (accept("identifier", 0))
	{
		accumulator = getNum(lookUp(tokens_actual[index - 1]));	
	}
	else if (accept("(", 1))
	{	
		counter++;
		expression();
		goToParen();
		expect(")", 1);
		parenHelper();
		counter--;
		if (counter == 0)
		{
			goToEquality();
		}
	}
	else
	{
		std::cout << "factor: syntax error";
		nextToken();
	}
}

//A non-terminal of the Recursive Descent grammar.
void Parser::term()
{
	factor();
	while (current_token_actual == "*" || current_token_actual == "/")
	{
		nextToken();
		termHelper();
		factor();
	}
}

//A non-terminal of the Recursive Descent grammar.
void Parser::expression()
{
	if (current_token_actual == "+" || current_token_actual == "-")
	{
		nextToken();
		expressionHelper();
	}
	term();
	while (current_token_actual == "+" || current_token_actual == "-")
	{
		nextToken();
		expressionHelper();
		term();
	}
}

//A non-terminal of the Recursive Descent grammar.
void Parser::condition()
{
	int left_expr;
	int right_expr;
	expression();
	if (current_token_actual == "==")
	{
		left_expr = accumulator;
		nextToken();
		expression();
		right_expr = accumulator;
		if (left_expr == right_expr)
			compare = true;
		else
			compare = false;
	}
	else if (current_token_actual == "!=")
	{
		left_expr = accumulator;
		nextToken();
		expression();
		right_expr = accumulator;
		if (left_expr != right_expr)
			compare = true;
		else
			compare = false;
	}
	else if (current_token_actual == "<=")
	{
		left_expr = accumulator;
		nextToken();
		expression();
		right_expr = accumulator;
		if (left_expr <= right_expr)
			compare = true;
		else
			compare = false;
	}
	else if (current_token_actual  == ">=")
	{
		left_expr = accumulator;
		nextToken();
		expression();
		right_expr = accumulator;
		if (left_expr >= right_expr)
			compare = true;
		else 
			compare = false;
	}
	else
	{
		std::cout << "condition: invalid operator " << current_token_actual << " " << index << "\n";
		throw std::exception();
	}
}

//A non-terminal of the Recursive Descent grammar. This checks all possible
//starting token types. ie: All tokens after a ;.
void Parser::statement()
{
	if (accept("identifier", 0))
	{
		std::string name_look = tokens_actual[index-1];
		if (accept("[", 1))
		{
			copyExpressionArray();
			expression();
			std::string index_array = std::to_string(accumulator);
			expect("]", 1);
			insertExpression();
			expect("=", 1);
			copyExpression();
			expression();
			std::string value_array = std::to_string(accumulator);
			expect(";", 1);
			updateArrayLookUp(name_look, index_array, value_array);
			insertExpression();
			printLine();
			printLookUp();
			std::cout << "--------------------\n";
		}
		else
		{
			expect("=", 1);
			copyExpression();
			expression();
			std::string value_look = std::to_string(accumulator);
			expect(";", 1);
			updateLookUp(name_look, value_look);
			insertExpression();
			printLine();
			printLookUp();
			std::cout << "--------------------\n";
		}
		statement();
	}
	else if (accept("callsym", 0))
	{
		expect("identifier", 0);
	}
	else if (accept("begin", 1))
	{
		do
		{
			statement();
		} while (accept(";", 1));
		if (current_token_actual == "while")
			statement();
		else
			expect("end", 1);
	}
	else if (accept("if", 1))
	{
		condition();
		expect("then", 1);
		if (compare == false)
		{
			goToEnd();
		}
		statement();
	}
	else if (accept("while", 1))
	{
		int index_while = index-1;
		condition();
		expect("do", 1);
		if (compare == true)
		{	
			statement();
			goToWhile(index_while);
		}
		else
		{
			goToEnd();
		}
		statement();
	}
	else if (accept("array", 1))
	{
		expect("identifier", 0);
		std::string name_look = tokens_actual[index-1];
		expect("[", 1);
		copyExpressionArray();
		expression();
		std::string value_look = std::to_string(accumulator);
		expect("]", 1);
		addArrayToLookUp(name_look, value_look);
		insertExpression();
		expect(";", 1);
		printLine();
		printLookUp();
		std::cout << "--------------------\n";
		statement();
	}
	else if (accept("int", 1))
	{
		expect("identifier", 0);
		std::string name_look = tokens_actual[index-1];
		expect("=", 1);
		copyExpression();
		expression();
		std::string value_look = std::to_string(accumulator);
		expect(";", 1);
		addToLookUp(name_look, value_look);
		insertExpression();
		printLine();
		printLookUp();
		std::cout << "--------------------\n";
		statement();
	}
}

void Parser::block()
{
	statement();	
}

void Parser::program()
{
	block();
	std::cout << "Final State: \n"; 
	printLookUp();
}

//A function that will return the integer version of a string. 
int Parser::getNum(std::string symbol)
{
	std::stringstream input(symbol);
    int x = 0;
    input >> x;
	return x; 
}

//A helper function for performing addition and subtraction computations. 
void Parser::expressionHelper()
{
	if (current_token_actual == "(")
		return;
	if (tokens_type[index-2] == "identifier")
	{
		tokens_actual[index-2] = lookUp(tokens_actual[index-2]);
		tokens_type[index-2] = "literal";
	}

	if (tokens_type[index] == "identifier")
	{
		tokens_actual[index] = lookUp(tokens_actual[index]);
		tokens_type[index-2] = "literal";
	}

	if (tokens_actual[index-1] == "+")
	{
		accumulator = getNum(tokens_actual[index-2]) + getNum(tokens_actual[index]);
		tokens_actual.erase(tokens_actual.begin()+(index-2), tokens_actual.begin()+(index));
		tokens_type.erase(tokens_type.begin()+(index-2), tokens_type.begin()+(index));
		index = index - 2;
		tokens_actual[index] = std::to_string(accumulator);
		if (isNumber(tokens_actual[index]))
			tokens_type[index] = "literal";
	}
	if (tokens_actual[index-1] == "-")
	{
		accumulator = getNum(tokens_actual[index-2]) - getNum(tokens_actual[index]);
		tokens_actual.erase(tokens_actual.begin()+(index-2), tokens_actual.begin()+(index));
		tokens_type.erase(tokens_type.begin()+(index-2), tokens_type.begin()+(index));
		index = index - 2;
		tokens_actual[index] = std::to_string(accumulator);
		if (isNumber(tokens_actual[index]))
			tokens_type[index] = "literal";
	}
}

//A helper function for performing multiplication and division on tokens. 
void Parser::termHelper()
{
	if (current_token_actual == "(")
		return;
	if (tokens_type[index-2] == "identifier")
	{
		tokens_actual[index-2] = lookUp(tokens_actual[index-2]);
		tokens_type[index-2] = "literal";
	}

	if (tokens_type[index] == "identifier")
	{
		tokens_actual[index] = lookUp(tokens_actual[index]);
		tokens_type[index-2] = "literal";
	}

	if (tokens_actual[index-1] == "*")
	{
		accumulator = getNum(tokens_actual[index-2]) * getNum(tokens_actual[index]);
		tokens_actual.erase(tokens_actual.begin()+(index-2), tokens_actual.begin()+(index));
		tokens_type.erase(tokens_type.begin()+(index-2), tokens_type.begin()+(index));
		index = index - 2;
		tokens_actual[index] = std::to_string(accumulator);
		if (isNumber(tokens_actual[index]))
			tokens_type[index] = "literal";
	}
	if (tokens_actual[index-1] == "/")
	{
		accumulator = getNum(tokens_actual[index-2]) / getNum(tokens_actual[index]);
		tokens_actual.erase(tokens_actual.begin()+(index-2), tokens_actual.begin()+(index));
		tokens_type.erase(tokens_type.begin()+(index-2), tokens_type.begin()+(index));
		index = index - 2;
		tokens_actual[index] = std::to_string(accumulator);
		if (isNumber(tokens_actual[index]))
			tokens_type[index] = "literal";
	}
}


void Parser::parenHelper()
{
	tokens_actual.erase(tokens_actual.begin() + (index-1));
	tokens_type.erase(tokens_type.begin() + (index-1));
	current_token_actual = tokens_actual[index-1];
	current_token_type = tokens_type[index-1];
	index = index - 1;
}

//A function to look up a variable in the lookup table and return it's value.
std::string Parser::lookUp(std::string ident)
{
	int length_ident = ident.length();

	for (int i = 0; i < lookup_table.size(); i++)
	{
		if (lookup_table[i].substr(0, (length_ident)) == ident)
		{
			int length_sub = lookup_table[i].length();
			return (lookup_table[i].substr(length_ident+1, length_sub-1));
		}
	}
	std::cout << ident << " has not yet been declared.\n";
	return "";
}

//A function that returns true if the variable exists, or false otherwise. 
bool Parser::lookUpBool(std::string ident)
{
	int length_ident = ident.length();

	for (int i = 0; i < lookup_table.size(); i++)
	{
		if (lookup_table[i].substr(0, (length_ident)) == ident)
		{
			int length_sub = lookup_table[i].length();
			return true;
		}
	}
	return false;
}

//A function that returns the index of a variable.
int Parser::lookUpInt(std::string ident)
{
	int length_ident = ident.length();

	for (int i = 0; i < lookup_table.size(); i++)
	{
		if (lookup_table[i].substr(0, (length_ident)) == ident)
		{
			return i;
		}
	}
	return -1;
}

//Prints all tokens in sequence. 
void Parser::printVector()
{
	for (int i = 0; i < tokens_actual.size(); i++)
	{
		std::cout << tokens_actual[i] << "   " << tokens_type[i] << "\n";
	}
}

//Prints the lookup table of the program. 
void Parser::printLookUp()
{
	for (int i = 0; i < lookup_table.size(); i++)
	{
		std::cout << lookup_table[i] << "\n";
	}
}

//Adds a variable to the lookup table. 
void Parser::addToLookUp(std::string name, std::string value)
{
	if (!isNumber(value))
	{
		value = lookUp(value);
	}
	if (!lookUpBool(name))
	{
		lookup_table.push_back(name + " " + value);
	}
	else
	{
		std::cout << "This variable has already been declared: " << name << "\n";
		throw std::exception();
	}

}

//Adds an array to the lookup table. 
void Parser::addArrayToLookUp(std::string name, std::string size)
{
	if (!lookUpBool(name + " array "))
	{
		lookup_table.push_back(name + " array " + size);
	}
	else
	{
		std::cout << "This array has already been declared: " << name << "\n";
		throw std::exception();
	}
	int size_int = getNum(size);
	for (int i = 0; i < size_int; i++)
	{
		lookup_table.push_back("0");
	}
}

//Updates an index of an array in the lookup table. 
void Parser::updateArrayLookUp(std::string name_array, std::string index_array, std::string value_array)
{
	int index_look = lookUpInt(name_array + " array ");
	int array_index = getNum(index_array);
	if (lookup_table.size() <= (index_look+array_index))
	{
		std::cout << "e1 Array " << name_array << ": index out of bounds\n";
		throw std::exception();
	}
	if (!isNumber(value_array) || !isNumber(index_array))
	{
		std::cout << "Array " << name_array << ": value provided is not valid.\n"; 
		throw std::exception();
	}
	if (index_look >= 0 && isNumber(lookup_table[index_look+array_index]))
	{
		lookup_table[index_look+array_index] = value_array;
	}
	else
	{
		std::cout << "e2 Array " << name_array << ": indexing out of bounds.\n";
		throw std::exception();
	}
}

//Updates a variable in the lookup table. 
void Parser::updateLookUp(std::string name, std::string value)
{
	if (!isNumber(value))
	{
		value = lookUp(value);
	}
	int index_look = lookUpInt(name);
	if (index_look >= 0)
	{
		lookup_table[index_look] = name + " " + value;
	}
	else
	{
		std::cout << "Variable not declared in this scope.";
		throw std::exception();
	}
}

//Will advance the program counter to the nearest equal sign. 
void Parser::goToEquality()
{
	while (tokens_actual[index] != "=")
	{
		index = index - 1;
	}
	current_token_actual = tokens_actual[index+1];
	current_token_type = tokens_type[index+1];
	index = index + 1;
	expression();
}

//Will advance the program counter to the nearest right paren. 
void Parser::goToParen()
{
	while (tokens_actual[index] != "(")
	{
		index = index - 1;
	}
	current_token_actual = tokens_actual[index+1];
	current_token_type = tokens_type[index+1];
	index = index + 1;
	parenHelper();
	expression();
}

//Will return true if the string is a number, false otherwise. 
bool Parser::isNumber(const std::string& check)
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

//Will advance the program counter to the next end statement. 
//Currently broken for some nested scopes. It will work for 
//				end
//			end
//		end
//but it will not work for some other combinations. Need to
//create a way to keep track of all the ends for a given 
//scope. 
void Parser::goToEnd()
{
	while (tokens_actual[index] != "end")
	{
		index = index + 1;
	}
	current_token_actual = tokens_actual[index+1];
	current_token_type = tokens_type[index+1];
	index = index + 1;
}

//Will advance the program counter to the previous while statement
//within a scope.
void Parser::goToWhile(int index_while)
{
	if (tokens_actual[index_while] == "while")
	{
		current_token_actual = tokens_actual[index_while];
		current_token_type = tokens_type[index_while];
		index = index_while;
	}
	else
		std::cout << "Could not find while" << "\n";
}

//Copies an expression.
void Parser::copyExpression()
{
	int i = index;
	while (tokens_actual[i] != ";")
	{
		if (i >= tokens_actual.size()-1)
		{
			std::cout << "Missing ;" << "\n";
			throw std::exception();
		}
		saved_expression_actual.push_back(tokens_actual[i]);
		saved_expression_type.push_back(tokens_type[i]);
		i = i + 1;
	}
}

//Copies an expression involving arrays. 
void Parser::copyExpressionArray()
{
	int i = index;
	while (tokens_actual[i] != "]")
	{
		if (i >= tokens_actual.size()-1)
		{
			std::cout << "Missing ]" << "\n";
			throw std::exception();
		}
		saved_expression_actual.push_back(tokens_actual[i]);
		saved_expression_type.push_back(tokens_type[i]);
		i = i + 1;
	}
}

void Parser::insertExpression()
{
	std::vector<std::string>::iterator it1;
	std::vector<std::string>::iterator it2;
	it1 = tokens_actual.begin();
	it2 = tokens_type.begin();
	tokens_actual.erase(tokens_actual.begin()+index-2);
	tokens_type.erase(tokens_type.begin()+index-2);
	tokens_actual.insert(it1+index-2, saved_expression_actual.begin(), saved_expression_actual.end());
	tokens_type.insert(it2+index-2, saved_expression_type.begin(), saved_expression_type.end());
	index += saved_expression_actual.size()-1;
	saved_expression_actual.clear();
	saved_expression_type.clear();
	current_token_actual = tokens_actual[index];
	current_token_type = tokens_type[index];
}

void Parser::printExpression()
{
	std::cout << "Expression: ";
	for (int i = 0; i < saved_expression_actual.size(); i++)
	{
		std::cout << saved_expression_actual[i] << " ";
	}
	std::cout << "\n";
}

void Parser::printLine()
{
	std::cout << "Line: ";
	int i = index-2;
	while (tokens_actual[i] != ";")
	{
		i = i - 1;
		if (tokens_actual[i] == "begin")
			break;
	}
	i = i + 1;
	while (tokens_actual[i] != ";")
		{
			std::cout << tokens_actual[i] << " ";
			i = i+1;
		}
	std::cout << "\n";
}











