#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "lexer.h"
#define MAX_RHS 10 // Max no of str on RHS for a given Non Terminal
#define MAX_LENGTH 50 // MAx length of a Terminal or a Non terminal
#define NONTERMINALS 47
#define TERMINALS 53

typedef struct _Rule_RHS Rule_RHS;
struct _Rule_RHS
{
	char str[MAX_LENGTH]; // eg. Program, WORD_LITERAL, SEMI_COLON, FuncArgs etc..
	Rule_RHS *prev;
	Rule_RHS *next;
};

typedef struct _Rule Rule;
struct _Rule
{
	char NonTerminal[MAX_LENGTH];
	int noOfRules;
	Rule_RHS *rhs[MAX_RHS]; // Array of Pointers
};


// For First and Follow Set - Constructed dynamically, using Linked List Implementation for each NT. Stored as an array of Sets.
typedef struct _Set Set;
struct _Set
{
	char str[MAX_LENGTH]; // The 1st elememt of LInked List is the Non-Terminal itself.
	Set *next;
};

// Define the final Tree Node
typedef struct _Node Node;
struct _Node
{
	char lexeme[MAX_LENGTH];
	char symbol[MAX_LENGTH];
	Rule_RHS *rhs;
	int lineNo; // = -1 for NonTerminal
	int noOfChildren;
	Node *parent;
	Node *child[MAX_RHS];
};

// Make a stack of ptrs to Tree Node
typedef struct _Stack Stack;
struct _Stack
{
	Node *val; // Val is the ptr to Parse Tree Node
	Stack *next;
};

void printTree(Node *root, int gap, FILE *f1);
void printStack(Stack *st);
void initialise(char *filename);
void throwErrorParser(char *lexeme, int lineNo, char *errorMsg);
Node *buildParseTree(Token *headToken);
void printParseTable();
void buildParseTable();
void printFollowSet();
void calculateFollowSet();
Set *helperFollowSet(int aim);
void printFirstSet();
void calculateFirstSet();
Set *helperFirstSet(int i);
int isEpsPresent(Set *set);
Set *insertSet(Set *set, char *str);
void getGrammar(char *filename);
void printGrammar();
int isNonTerminal(char *s);
int isTerminal(char *s);
int strToInt(char *s);
Node *top(Stack *s);
Stack *pop(Stack *s);
Stack *push(Stack *s, Node *n);
#endif
