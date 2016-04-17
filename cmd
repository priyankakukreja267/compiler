// Stage 2
gcc -o exec1 driverPhase2.c symbolTable.c ast.c parser.c lexer.c -lm
./exec1 Test1.txt finalGrammar.txt outLexer.txt outParseTree.txt outAST.txt outST.txt

// Stage 3 - Semantic Analysis
gcc -o exec1 driverPhase3.c semantic.c symbolTable.c ast.c parser.c lexer.c -lm
./exec1 Test1.txt finalGrammar.txt outLexer.txt outParseTree.txt outAST.txt outST.txt outCode.txt


// Stage 3 - Code Generation
gcc -o exec1 driverPhase3.c codegen.c semantic.c symbolTable.c ast.c parser.c lexer.c -lm
./exec1 Test2.txt finalGrammar.txt outLexer.txt outParseTree.txt outAST.txt outST.txt outCode.txt
