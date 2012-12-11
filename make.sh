bison -v -d -o pascal.cpp pascal.y
flex -o tokens.cpp tokens.l

g++ --std=c++0x -o pascal pascal.cpp tokens.cpp expression.cpp types.cpp -lfl