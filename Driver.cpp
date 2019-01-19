#include "Parser.h"
#include "LexicalAnalyzer.h"

int main(int argc, char** argv)
{
	std::string fileName = argv[1];

	LexicalAnalyzer readSource(fileName);
	std::vector<std::string> source_type = readSource.getType();
	std::vector<std::string> source_actual = readSource.getActual();
	Parser parse(source_type, source_actual);
	parse.program();
}