Author:
    Eleftherios Amperiadis

Compile Using:
	g++ -std=c++11 -o compiler Driver.cpp Parser.cpp LexicalAnalyzer.cpp

In order to test one of the txt files:

chmod +x compile
./compile example.txt

example in this case is the text file you wish to test. 

Note: Compiled on linux using the g++ compiler. The executable will NOT run on windows machines.


Working Features:
	Integer Declaration:
		int a = 5 ;
		Will assign the variable a the integer value 5.
	
	Integer Assignment:
		a = 6 ;
		Will assign the value 6 to the already declared integer variable a.
		a = a + 1 ;
		Will add 1 to the already declared integer variable a.
	
	Array Declaration:
		array a [ 5 ] ;
		Will create an array of size 5. All values initialized to 0.
	
	Array Assignmnet:
		a [ 5 ] = 4 ;
		Will assign the 5th index of array "a" the value 4.
	
	Complex Expression solving. 
	
	If Statement:
		if 6 >= 5 then
		begin
			statement(s)
		end
	
	While Statement:
		while 6 >= 5 do
		begin
			statement(s)
		end

	Will return any syntax error, specifying the nature of the error.

Not Working Features:
	Certain combinations of nested loops do not work. This is because of the function
	goToEnd(). It advances the program counter to the end statement. This is important
	in while and if statements when you do not wish to execute the statement. The issue
	is, nested whiles and ifs outside of the form:

				end
			end
		end
	end

	May perform unexpected results. This is because as of right now, the function 
	goToEnd() is scope independent. 
