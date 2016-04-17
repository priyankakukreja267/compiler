To Compile: 
gcc -o exec1 driverPhase3.c codegen.c semantic.c symbolTable.c ast.c parser.c lexer.c -lm

To Execute
./exec1 Test2.txt finalGrammar.txt outLexer.txt outParseTree.txt outAST.txt outST.txt outCode.txt

Output:
outLexer.txt - Output of Lexical Analyser

outParseTree.txt - Parse Tree

outParseTable.txt - Parse Table Generated by the parser

outFirstSet.txt - First Set Generated by the parser

outFollowSet.txt - Follow Set Generated by the parser

outGrammar.txt - Grammar Generated (as stored in our data structures)

outParseTable - Parse Table generated by the parser

outAST.txt - AST

outST.txt - Symbol Table

outCode.txt - Output Code (ASM File)

(Stack movements, as the parser processes each token and generates the parse tree, have been printed to the terminal for convenience)


Salient Features:

1. Automated First Set, Follow Set, Parse Table Generation

2. Grammar given as a separate file. So can be altered (ensuring it is still LL(1)).

3. Efficient Data Structures used at all stages.

4. ASM Code has been optimised wherever possible.

5. Any semantic errors are displayed on the terminal.



Caution:

The file 'finalGrammar.txt' is in a standardised format and is being used as an input grammar to the program 