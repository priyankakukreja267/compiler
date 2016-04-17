#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"
#include "semantic.h"


int main(int argc, char *argv[])
{
// 1st file: Testcase. 2nd file: Grammar. 3rd file: output file of Lexer. 4th file: ParseTree. 5th file AST. 6th file: ST. 7th file: Int Code
if(argc < 8)
{
	printf("Too few arguements to main! Required: 7 (Testcase - 1, Grammar - 2, Lexer output - 3, Parser output - 4, AST output - 5, ST output - 6, Intermediate Code - 7)\n");
	exit(1);
}

// Lexical Analysis
Token *headToken = getListOfTokens(argv[1]);
printListOfTokens(headToken, argv[3]);

// Parsing
initialise(argv[2]);
Node *parseTree = buildParseTree(headToken);
FILE *f1 = fopen(argv[4], "w");
printTree(parseTree, 0, f1);
fclose(f1);

// AST
Node *ast = buildAST(parseTree);
FILE *f2 = fopen(argv[5], "w");
printTree(ast, 0, f2);
fclose(f2);

// Symbol Table
Variable *globalST[MAX_VARIABLES];
Function *funcST[MAX_FUNCTIONS];
initVar(globalST);
initFunc(funcST);

buildST(headToken, globalST, funcST);
printST(argv[6], globalST, funcST);

// Semantic Analysis + type checking
checkSemantics(ast, globalST, funcST);
printf("No Semantic Errors Found.\n");

// Code Generation
FILE *f3 = fopen(argv[7], "w");
makeAsm(ast, globalST, funcST, f3);
fclose(f3);
//getCode(ast, f3, globalST, funcST);
}
