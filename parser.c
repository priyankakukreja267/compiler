#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "lexer.h"
#include "parser.h"



// Global Variables
Rule Grammar[NONTERMINALS]; // No of elements of array = No of Non Terminals
Set firstSet[NONTERMINALS];
Set followSet[NONTERMINALS];
Rule_RHS *parseTable[NONTERMINALS][TERMINALS];


void printTree(Node *root, int gap, FILE *f1)
{
if(root == NULL) return;
else
{
	int i = 0;
	for(i = 0; i < gap; i++) fprintf(f1, " ");
	fprintf(f1, "%d: %s\n", gap/2, root->symbol);
	
	for(i = 0; i < root->noOfChildren; i++) printTree(root->child[i], gap+2, f1);
}
}


void throwErrorParser(char *lexeme, int lineNo, char *errorMsg)
{
	printf("Error in %s at Line No %d\n%s\n", lexeme, lineNo, errorMsg);
	exit(1);
}


void printStack(Stack *st)
{
Stack *s = st;
printf("Stack: ");
while(s != NULL)
{
	printf("%s ", s->val->symbol);
	s = s->next;
}
//printf("\n");
}


Node *buildParseTree(Token *headToken)
{
Stack *stack = NULL;
Node *n = NULL;
Rule_RHS *rule1 = NULL;
int i = 0, cnt;


// Inserting a DOLLAR Token at the end of input
Token *tempa = headToken;
while(tempa->next != NULL) { tempa = tempa->next; }
tempa->next = (Token *)malloc(sizeof(Token));
strcpy(tempa->next->lexeme, "DOLLAR");
strcpy(tempa->next->symbol, "DOLLAR");
tempa->next->lineNo = -1;
tempa->next->next = NULL;

// Inserting the 1st element into stack
Node *root = (Node *) malloc(sizeof(Node));
strcpy(root->lexeme, Grammar[NONTERMINALS-1].NonTerminal);
strcpy(root->symbol, Grammar[NONTERMINALS-1].NonTerminal);
root->rhs = NULL;
root->lineNo = -1;
root->parent = NULL;
root->noOfChildren = 0;
for(i = 0; i < MAX_RHS; i++) root->child[i] = NULL;

stack = push(stack, root);
printStack(stack);
while(1)
{
	if(stack != NULL)
	{
		n = top(stack);
		printStack(stack);
	}
	if(headToken) printf("Input: %s\n", headToken->symbol);
	if(headToken == NULL && stack == NULL) 
	{
		printf("No Syntactic Errors found.\n");
		break;
	}
	
	else if(headToken != NULL && stack == NULL)
	{
		throwErrorParser(headToken->lexeme, headToken->lineNo, "Stack got empty");
		break;
	}
	
	else if(headToken == NULL && stack != NULL)
	{
		throwErrorParser(NULL, -1, "Input got over. Stack not empty");
		break;
	}

	else if(strcmp(n->symbol, "EPS") == 0)
	{
		stack = pop(stack);
	}
	
	else if(isTerminal(n->symbol)) // Top of the stack is a Terminal
	{
		n->rhs = NULL;
		n->lineNo = headToken->lineNo;
		n->noOfChildren = 0;
		for(i = 0; i < MAX_RHS; i++) n->child[i] = NULL;
		
		if(strcmp(n->symbol, headToken->symbol) != 0)
		{
			throwErrorParser(headToken->lexeme, headToken->lineNo, "Wrong symbol");
			break;
		}
		
		else  // Pop the terminal from stack
		{
			strcpy(n->lexeme, headToken->lexeme);
			stack = pop(stack);
			headToken = headToken->next;
		}
	}
	
	else // Top of the stack is a NonTerminal
	{
		stack = pop(stack);
		rule1 = parseTable[strToInt(n->symbol)][strToInt(headToken->symbol) - 50];
		if(rule1 == NULL)
		{
			throwErrorParser(headToken->lexeme, headToken->lineNo, "No rule for this.");
			break;
		}
		
		n->rhs = rule1;
		n->lineNo = -1;
		
		// Manually count the noOfChildren! 
		cnt = 1;
		while(rule1->next != NULL) { rule1 = rule1->next; cnt++; }
		n->noOfChildren = cnt;
		
		while(rule1 != NULL && cnt > 0)
		{
			// Create a Node for the last symbol of the rule
			n->child[cnt-1] = (Node *)malloc(sizeof(Node));
			strcpy(n->child[cnt-1]->symbol, rule1->str); 
			strcpy(n->child[cnt-1]->lexeme, rule1->str); 
			n->child[cnt-1]->parent = n;
			stack = push(stack, n->child[cnt-1]);
			for(i = 0; i < MAX_RHS; i++) n->child[cnt-1]->child[i] = NULL;
			cnt--;
			rule1 = rule1->prev;
		}
	}
}
return root;
}


void printParseTable()
{
FILE *fp = fopen("outParseTable.txt", "w");
if(!fp) {printf("Out File for Parse Table not opened.\n"); exit(1);}

int i = 0, j = 0;
Rule_RHS *r = NULL;


for(i = 0; i < NONTERMINALS; i++)
{
	for(j = 0; j < TERMINALS; j++)
	{
		r = parseTable[i][j];
		if(!r) continue;
		fprintf(fp, "(%d, %d)", i, j);
		while(r != NULL)
		{
			fprintf(fp, " ->%s", r->str);
			r = r->next;
		}
		fprintf(fp, "\n");
	}
}
fclose(fp);
}

void buildParseTable()
{
int i = 0, j = 0, addFollow = 0;
Rule_RHS *rule1 = NULL, *symb = NULL;
Set *first1 = NULL, *follow1 = NULL;
for(i = 0; i < NONTERMINALS; i++)
{
	for(j = 0; j < Grammar[i].noOfRules; j++)
	{
		rule1 = symb = Grammar[i].rhs[j];
		while(symb != NULL)
		{
			addFollow = 0;
			if(strcmp(symb->str, "EPS") == 0)
			{
				addFollow = 1;
				break;
			}
			
			else if(isTerminal(symb->str))
			{
				parseTable[i][strToInt(symb->str) - 50] = rule1;
				break;
			}
			
			else // It is a NonTerminal
			{
				first1 = firstSet[strToInt(symb->str)].next;
				while(first1 != NULL)
				{
					if(strcmp(first1->str, "EPS") == 0) addFollow = 1;
					else parseTable[i][strToInt(first1->str) - 50] = rule1;
					first1 = first1->next;
				}
				if(addFollow == 0) break;
				else symb = symb->next;
			}
		}
		if(addFollow == 1)
		{
			// Add follow of i
			follow1 = followSet[i].next;
			while(follow1 != NULL)
			{
				parseTable[i][strToInt(follow1->str) - 50] = rule1;
				follow1 = follow1->next;
			}
		}
	} // All rules of a NonTerminal end
}
}


void initialise(char *filename)
{
int i = 0, j = 0;

// Initialise Grammar
for(i = 0; i < NONTERMINALS; i++)
{
	for(j = 0; j < MAX_RHS; j++)
	{
		Grammar[i].rhs[j] = NULL;
	}
	Grammar[i].noOfRules = 0;
}
getGrammar(filename);
printGrammar();

// Initialise firstSet and followSet
i = 0;
for(i = 0; i < NONTERMINALS; i++)
{
	strcpy(firstSet[i].str, Grammar[i].NonTerminal);
	firstSet[i].next = NULL;
	strcpy(followSet[i].str, Grammar[i].NonTerminal);
	followSet[i].next = NULL;
}

// Initialise parseTable
i = 0, j = 0;
for(i = 0; i < NONTERMINALS; i++)
	for(j = 0; j < TERMINALS; j++) 
		parseTable[i][j] = NULL;
		
		
calculateFirstSet();
printFirstSet();
calculateFollowSet();
printFollowSet();
buildParseTable();
printParseTable();

}

void printFirstSet()
{
int i = 0;
Set *temp = NULL;
FILE *fp = fopen("outFirstSet.txt", "w");
if(!fp) { printf("File Not Opened\n"); exit(0); }

for(i = 0; i<NONTERMINALS; i++)
{
	fprintf(fp, "%s --> ", firstSet[i].str);
	temp = firstSet[i].next;
	while(temp != NULL)
	{
		fprintf(fp, "%s ", temp->str);
		temp = temp->next;
	}
	fprintf(fp, "\n");
}
}


void printFollowSet()
{
int i = 0;
Set *temp = NULL;
FILE *fp = fopen("outFollowSet.txt", "w");
if(!fp) { printf("File Not Opened\n"); exit(0); }

for(i = 0; i<NONTERMINALS; i++)
{
	fprintf(fp, "%s --> ", followSet[i].str);
	temp = followSet[i].next;
	while(temp != NULL)
	{
		fprintf(fp, "%s ", temp->str);
		temp = temp->next;
	}
	fprintf(fp, "\n");
}
}


void calculateFollowSet()
{
int i = 0;
for(i = 0; i < NONTERMINALS; i++)
{
	if(followSet[i].next == NULL) followSet[i].next = helperFollowSet(i);
}
}

Set *helperFollowSet(int aim)
{
int j = 0, k = 0, isEps = 0; 
Rule_RHS *cur = NULL;
Rule_RHS *rule1 = NULL, *rule2 = NULL;
Set *s1;
for(k = 0; k < NONTERMINALS; k++) // Search the entire Grammar for all occurences of aim.
{
	for(j = 0; j < Grammar[k].noOfRules; j++)
	{
		cur = Grammar[k].rhs[j];
		while(cur != NULL)
		{
			if(strToInt(cur->str) == aim)
			{
				// Next is NULL
				if(cur->next == NULL) 
				{
					// Add the follow of parent
					if(followSet[k].next == NULL) followSet[k].next = helperFollowSet(k);
					s1 = followSet[k].next;
					while(s1 != NULL)
					{
						followSet[aim].next = insertSet(followSet[aim].next, s1->str);
						s1 = s1->next;
					}
				}
				
				// Next is a TERMINAL
				else if(isTerminal(cur->next->str)) followSet[aim].next = insertSet(followSet[aim].next, cur->next->str); 
			
				// Next is a Non Terminal
				else if(isNonTerminal(cur->next->str)) 
				{
					// Insert its firstSet. If eps found, cur = cur->next, else break.
					s1 = firstSet[strToInt(cur->next->str)].next; // s1: at the first set of next NT
					while(s1 != NULL)
					{
						if(strcmp(s1->str, "EPS") != 0) followSet[aim].next = insertSet(followSet[aim].next, s1->str); 
						else isEps = 1;
						s1 = s1->next;
					}
					if(isEps == 1) // Nullable first Set. Go on adding firsts while they`re NULL.
					{
						rule1 = cur->next->next;
						
						while(1)
						{
							if(rule1 == NULL)
							{
								// Add follow of Parent.
								if(followSet[k].next == NULL) followSet[k].next = helperFollowSet(k);
								s1 = followSet[k].next;
								while(s1 != NULL)
								{
									followSet[aim].next = insertSet(followSet[aim].next, s1->str);
									s1 = s1->next;
								}
								break;
							}
							
							else if(isTerminal(rule1->str))
							{
								followSet[aim].next = insertSet(followSet[aim].next, rule1->str);
								break;
							}
							
							else if(isNonTerminal(rule1->str))
							{
								s1 = firstSet[strToInt(s1->str)].next;
								while(s1 != NULL)
								{
									if(strcmp(s1->str, "EPS") == 0) isEps = 1;
									else followSet[aim].next = insertSet(followSet[aim].next, s1->str);
									s1 = s1->next;
								}
								
								if(isEps == 0) break;
								else
								{
									rule1 = rule1->next;
									isEps = 0;
								}
							}
						}
						break; // break out from the main while loop.
					}
				}
			}
			cur = cur->next;
		} // While Loop ends.
	}
}

return followSet[aim].next;
}



void calculateFirstSet()
{
int i = 0;
for(i = 0; i < NONTERMINALS; i++)
{
	if(firstSet[i].next == NULL) firstSet[i].next = helperFirstSet(i);
}
}


Set *helperFirstSet(int i)
{
Rule_RHS *r = NULL;
Set *temp = NULL;
int isEps = 0;
int j = 0;
for(j = 0; j < Grammar[i].noOfRules; j++)
{
	r = Grammar[i].rhs[j];
	while(r != NULL)
	{
		isEps = 0;
		
		if(isNonTerminal(r->str)) // A Non terminal encountered
		{
			// Add all elements
			temp = helperFirstSet(strToInt(r->str));
			while(temp != NULL)
			{
				if(strcmp(temp->str, "EPS") == 0) isEps = 1;
				else
				{
					firstSet[i].next = insertSet(firstSet[i].next, temp->str);
				}
				temp = temp->next;
			}
			
			if(isEps == 1) r = r->next; 
			else break;
		}

		
		else if(isTerminal(r->str))
		{
			firstSet[i].next = insertSet(firstSet[i].next, r->str);
			break;
		}

		else //if(strToInt(r->str) == 200) : EPS encountered
		{
			firstSet[i].next = insertSet(firstSet[i].next, "EPS");
			break;
		}
	}
}
return firstSet[i].next;
}


Set *insertSet(Set *set, char *str)
{
// Checks for duplicates
Set *temp = set;
if(temp == NULL)
{
	temp = (Set *)malloc(sizeof(Set));
	strcpy(temp->str, str);
	temp->next = NULL;
	return temp;
}
else
{
	while(temp->next != NULL)
	{
		if(strcmp(temp->str, str) == 0) return set;
		else temp = temp->next;
	}
	
	if(strcmp(temp->str, str) == 0) return set;
	
	temp->next = (Set *)malloc(sizeof(Set));
	temp->next->next = NULL;
	strcpy(temp->next->str, str);
	return set;
}
}

void getGrammar(char *filename)
{
FILE *fp = fopen(filename, "r");
if(fp == NULL)
{
	fprintf(stderr, "Please give Standardised Grammar Filename.\n");
	exit(1);
}
char rhs[MAX_LENGTH];
char lhs[MAX_LENGTH];
char header[MAX_LENGTH];
int nHeader = 0;
int nRules = 0;
Rule_RHS *temp = NULL;


fscanf(fp, "%s", header);

while(strcmp(header, "!") != 0)
{
	nHeader = strToInt(header);
	strcpy(Grammar[nHeader].NonTerminal, header);
	
	strcpy(lhs, header);
	
	while(strcmp(lhs, header) == 0)
	{
		fscanf(fp, "%s", rhs);
		Grammar[nHeader].rhs[nRules] = (Rule_RHS *) malloc(sizeof(Rule_RHS));
		temp = Grammar[nHeader].rhs[nRules];
		strcpy(Grammar[nHeader].rhs[nRules]->str, rhs);
		Grammar[nHeader].rhs[nRules]->prev = NULL;
		Grammar[nHeader].rhs[nRules]->next = NULL;		
		nRules++;
		fscanf(fp, "%s", rhs);
		
		while(strcmp(rhs, ".") != 0)
		{
			temp->next = (Rule_RHS *) malloc(sizeof(Rule_RHS));
			temp->next->prev = temp;
			strcpy(temp->next->str, rhs);
			temp->next->next = NULL;
			temp = temp->next;
			fscanf(fp, "%s", rhs);
		}
		fscanf(fp, "%s", lhs);
	}
	
	Grammar[nHeader].noOfRules = nRules;
	strcpy(header, lhs);
	nRules = 0;
}
fclose(fp);
return;
}


void printGrammar()
{
int i = 0, n = 0;
Rule_RHS *temp;
FILE *fp = fopen("outGrammar.txt", "w");
if(!fp) { printf("File not opened for printing.\n"); exit(1); }
for(i = 0; i < NONTERMINALS; i++)
{
	for(n = 0; n < Grammar[i].noOfRules; n++)
	{
		fprintf(fp, "%s --> ", Grammar[i].NonTerminal);
		temp = Grammar[i].rhs[n];
		while(temp != NULL)
		{
			fprintf(fp, "%s ", temp->str);
			temp = temp->next;
		}
		fprintf(fp, "\n");
	}
}
}



int isTerminal(char *s) // Returns 1 if Terminal
{
int a = strToInt(s);
return (a>=50 && a<=200)?1:0;
}

int isNonTerminal(char *s) // Returns 1 if NonTerminal
{
int a = strToInt(s);
return (a<50)?1:0;
}


int strToInt(char *s)
{
if(strcmp(s, "Program") == 0) return 0;
else if(strcmp(s, "DeclStmt") == 0) return 1;
else if(strcmp(s, "TypeWithoutEnum") == 0) return 2;
else if(strcmp(s, "VarList") == 0) return 3;
else if(strcmp(s, "MoreVars") == 0) return 4;
else if(strcmp(s, "Value") == 0) return 5;
else if(strcmp(s, "DecLiteral") == 0) return 6;
else if(strcmp(s, "EnumFields") == 0) return 7;
else if(strcmp(s, "OtherEnumFields") == 0) return 8;
else if(strcmp(s, "FuncStmts") == 0) return 9;
else if(strcmp(s, "FuncHeader") == 0) return 10;
else if(strcmp(s, "LHS_Arrow") == 0) return 11;
else if(strcmp(s, "Type") == 0) return 12;
else if(strcmp(s, "FuncDefArgs") == 0) return 13;
else if(strcmp(s, "RHS_Arrow") == 0) return 14;
else if(strcmp(s, "MoreReturns") == 0) return 15;
else if(strcmp(s, "FuncStmt") == 0) return 16;
else if(strcmp(s, "ReturnStmt") == 0) return 17;
else if(strcmp(s, "ReturnValues") == 0) return 18;
else if(strcmp(s, "MoreReturnValues") == 0) return 19; 
else if(strcmp(s, "CodeStmt") == 0) return 20;
else if(strcmp(s, "FuncCallStmt") == 0) return 21; 
else if(strcmp(s, "CallArgs") == 0) return 22;
else if(strcmp(s, "MoreCallArgs") == 0) return 23; 
else if(strcmp(s, "IOStmt") == 0) return 24;
else if(strcmp(s, "IterativeStmt") == 0) return 25;
else if(strcmp(s, "IfElseStmt") == 0) return 26;
else if(strcmp(s, "ElseStmt") == 0) return 27;
else if(strcmp(s, "AssignStmt") == 0) return 28;
else if(strcmp(s, "Variable") == 0) return 29;
else if(strcmp(s, "ArrayVar") == 0) return 30;
else if(strcmp(s, "Assign") == 0) return 31;
else if(strcmp(s, "Literal") == 0) return 32;
else if(strcmp(s, "Word") == 0) return 33;
else if(strcmp(s, "MoreWord") == 0) return 34; 
else if(strcmp(s, "CondExp") == 0) return 35;
else if(strcmp(s, "Expression") == 0) return 36;
else if(strcmp(s, "BooleanExp") == 0) return 37;
else if(strcmp(s, "LogicalExp") == 0) return 38;
else if(strcmp(s, "RelExp") == 0) return 39;
else if(strcmp(s, "ArithExp") == 0) return 40;
else if(strcmp(s, "LogicalOp") == 0) return 41;
else if(strcmp(s, "RelOp") == 0) return 42;
else if(strcmp(s, "ArithOp") == 0) return 43;
else if(strcmp(s, "EnumBoolExp") == 0) return 44; 
else if(strcmp(s, "EnumOp") == 0) return 45;
else if(strcmp(s, "Program2") == 0) return 46;

// Terminals Begin. Offset = 50.
else if(strcmp(s, "DATA") == 0) return 50;
else if(strcmp(s, "%%") == 0) return 51;
else if(strcmp(s, "CODE") == 0) return 52;
else if(strcmp(s, "FUNCTION") == 0) return 53; 
else if(strcmp(s, "INT_LITERAL") == 0) return 54;
else if(strcmp(s, "DECI_LITERAL") == 0) return 55;
else if(strcmp(s, "BOOL_LITERAL") == 0) return 56;
else if(strcmp(s, "WORD_LITERAL") == 0) return 57;
else if(strcmp(s, "ENUM_LITERAL") == 0) return 58;
else if(strcmp(s, "ENUM") == 0) return 59;
else if(strcmp(s, "INT") == 0) return 60;
else if(strcmp(s, "DECI") == 0) return 61;
else if(strcmp(s, "BOOL") == 0) return 62;
else if(strcmp(s, "WORD") == 0) return 63;
else if(strcmp(s, "SEMI_COLON") == 0) return 64;
else if(strcmp(s, "COLON") == 0) return 65;
else if(strcmp(s, "COMMA") == 0) return 66;
else if(strcmp(s, "CURLY_OPEN") == 0) return 67;
else if(strcmp(s, "CURLY_CLOSE") == 0) return 68;
else if(strcmp(s, "PAREN_OPEN") == 0) return 69;
else if(strcmp(s, "PAREN_CLOSE") == 0) return 70;
else if(strcmp(s, "SQ_OPEN") == 0) return 71;
else if(strcmp(s, "SQ_CLOSE") == 0) return 72;
else if(strcmp(s, "ID") == 0) return 73;
else if(strcmp(s, "FUNC") == 0) return 74;
else if(strcmp(s, "FUNC_ID") == 0) return 75;
else if(strcmp(s, "ARROW") == 0) return 76;
else if(strcmp(s, "VOID") == 0) return 77;
else if(strcmp(s, "RETURN") == 0) return 78;
else if(strcmp(s, "GIVE") == 0) return 79;
else if(strcmp(s, "TAKE") == 0) return 80;
else if(strcmp(s, "WHILE") == 0) return 81;
else if(strcmp(s, "IF") == 0) return 82;
else if(strcmp(s, "ELSE") == 0) return 83;
else if(strcmp(s, "ASSIGN_OP") == 0) return 84;
else if(strcmp(s, "JOIN") == 0) return 85;
else if(strcmp(s, "AND") == 0) return 86;
else if(strcmp(s, "OR") == 0) return 87;
else if(strcmp(s, "NOT") == 0) return 88;
else if(strcmp(s, "EQ") == 0) return 89;
else if(strcmp(s, "NEQ") == 0) return 90;
else if(strcmp(s, "GE") == 0) return 91;
else if(strcmp(s, "LE") == 0) return 92;
else if(strcmp(s, "G") == 0) return 93;
else if(strcmp(s, "L") == 0) return 94;
else if(strcmp(s, "PLUS") == 0) return 95;
else if(strcmp(s, "MINUS") == 0) return 96;
else if(strcmp(s, "MULT") == 0) return 97;
else if(strcmp(s, "DIV") == 0) return 98;
else if(strcmp(s, "POW") == 0) return 99;
else if(strcmp(s, "ENUM_EQ") == 0) return 100; 
else if(strcmp(s, "ENUM_NEQ") == 0) return 101;
else if(strcmp(s, "DOLLAR") == 0) return 102;
else if(strcmp(s, "EPS") == 0) return 300;
else return -1;
}


Node *top(Stack *s)
{
	return s->val;
}


Stack *pop(Stack *s)
{
if(s->next == NULL)
{
	free(s);
	return NULL;
}
else
{
	Stack *temp = s;
	s = s->next;
	temp->next = NULL;
	free(temp);
	return s;
}
}


Stack *push(Stack *s, Node *n)
{
if(s == NULL)
{
	s = (Stack *)malloc(sizeof(Stack));
	s->val = n;
	s->next = NULL;
	return s;
}
else
{
	Stack *temp = s;
	s = (Stack *)malloc(sizeof(Stack));
	s->val = n;
	s->next = temp;
	return s;
}
}
