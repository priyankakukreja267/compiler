// ASSUMPTION: We don`t know about the ISA of floating point processor (8087), so the 'deci' values have been converted to int for Code Generation.
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"
#include "semantic.h"



int isLocal(char id[], Function *curFunc) // Returns 1 if local var, 0 if global var
{
if(curFunc == NULL) return 0;
int index = hashvar(id);
Variable *v = curFunc->localST[index];
while(v != NULL)
{
	if(strcmp(v->name, id) == 0) return 1;
	else v = v->next;
}
return 0;
}



int isArg(char id[], Function *curFunc) // Returns the arg no. Eg: f(a, b, c): a is 1, b is 2, c is 3 and d is 0 (Not found)
{
if(curFunc == NULL) return 0;
Variable *argum = curFunc->args;
int cnt = 1;
while(argum != NULL)
{
	if(strcmp(argum->name, id) == 0) return cnt;
	cnt++;
	argum = argum->next;
}
return 0;
}


char *getMemoryName(char id[], Function *curFunc, Variable *globalST[])
{
// Returns the name of the memory location for given identifier
if(curFunc != NULL)
{
	char funcName[100];
	char *memName = (char *)malloc(sizeof(char) * 100);
	
	// Make funcName
	strcpy(funcName, "\0");
	strcpy(funcName, curFunc->name);
	strcpy(memName, "\0");

	
	Variable *argum = curFunc->args;
	while(argum != NULL)
	{
		strcat(funcName, "_");
		strcat(funcName, argum->type);
		argum = argum->next;
	}
	
	strcpy(memName, funcName);
	int no = isArg(id, curFunc);
	if(no)
	{
		strcpy(memName, funcName);
		strcat(memName, "_ARG");
		char temp[5];
		sprintf(temp, "%d", no);
		strcat(memName, temp);
	}
	else if(isLocal(id, curFunc))
	{
		strcpy(memName, funcName);
		strcat(memName, "_LOCAL_");
		strcat(memName, id);
	}
	else strcpy(memName, id);
	return memName;
}

else return id;
}



void getCode(Node *root, FILE *f1, Variable *globalST[], Function *funcST[])
{
static int nextLabel = 0;
static int curScope = 0;
static Function *curFunc = NULL;
if(root == NULL) return;
else
{
//	printf("At %s: %s\n", root->symbol, root->lexeme);
	if(strcmp(root->symbol, "ID") == 0)
	{
		fprintf(f1, "\tMOV EAX, %s\n", getMemoryName(root->lexeme, curFunc, globalST));
		return;
	}

	
	else if(strcmp(root->symbol, "INT_LITERAL") == 0)
	{
		fprintf(f1, "\tMOV EAX, %sd\n", root->lexeme);
		return;
	}
	
	else if(strcmp(root->symbol, "DECI_LITERAL") == 0)
	{
		fprintf(f1, "\tMOV EAX, %dd\n", atoi(root->lexeme)); // Converting DECI to INT coz 8086 has no support for floating point.
		return;
	}
	
	else if(strcmp(root->symbol, "BOOL_LITERAL") == 0)
	{
		if(strcmp(root->lexeme, "TRUE")) fprintf(f1, "\tMOV EAX, 01h\n");
		else fprintf(f1, "\tMOV EAX, 00h\n");
		return;
	}
	
	// Dealing with word literal: in Assignment Stmt itself
	// Dealing with enum literal: in Assgnmnt stmt + enum bool exp itself
	
	else if(strcmp(root->symbol, "Program") == 0)
	{
		curFunc = NULL;
		curScope = 0;
		if(strcmp(root->child[1]->symbol, "FuncStmts") == 0)
		{
			getCode(root->child[2], f1, globalST, funcST);
			fprintf(f1, ".exit\n\n");
			getCode(root->child[1], f1, globalST, funcST);			
		}
		
		else getCode(root->child[1], f1, globalST, funcST);
		return;
	}
	

	else if(strcmp(root->symbol, "FuncHeader") == 0)
	{
		// Update curFunc and curScope here!
		curScope = 1;
		
		// Fnd its return type
		char curType[MAX_LENGTH];
		strcpy(curType, root->child[2]->symbol);
		
		// Count the parameters of this
		int cntParams = 0;
		Node *temp = root->child[1];
		while(temp->noOfChildren == 3)
		{
			cntParams++;
			temp = temp->child[2];
		}
		if (root->child[1]->noOfChildren == 0) cntParams = 0;
		else cntParams++;
		
		// Get index by hash of F_ID
		int index = hashfunc(root->child[0]->lexeme);
		Function *f = funcST[index];
		Variable *argu = NULL;
		Node *params = NULL;
		
		// while( f is not NULL)
			// update argu to pt to FuncHeader->child[1] in the beginning
			
		while(f != NULL)
		{
			argu = f->args;
			params = root->child[1];

			// Check for name, types, noOfArgs and returnType
			if(strcmp(root->child[0]->lexeme, f->name) == 0 && strcmp(curType, f->returnType) == 0 && cntParams == f->noOfArgs)
			{
				if(strcmp(params->symbol, "VOID") == 0 && argu == NULL)
				{
					curFunc = f;
					return;
				}
				
				else if(strcmp(params->symbol, "VOID") == 0 && argu != NULL)
				{
					f= f->next;
					continue;
				}
				
				else
				{
					params = root->child[1];
					argu = f->args;
					while(params != NULL && argu != NULL)
					{
						// Check
						if(strcmp(params->child[1]->lexeme, argu->name) == 0 && typeToInt(params->child[0]->symbol) == typeToInt(argu->type))
						{
							// Go to next param, argu
							params = params->child[2];
							argu = argu->next;
						}
						else break;
					}
					
					if(params == NULL && argu == NULL)
					{
						curFunc = f;
						return;
					}
					
					else
					{
						f = f->next;
						continue;
					}
				}
			}
			else f = f->next;
		}
		return;
	}
	
	else if(strcmp(root->symbol, "FuncStmts") == 0)
	{
		getCode(root->child[0], f1, globalST, funcST); // Get code for Header: update curFunc & curScope
		
		char funcName[100];
		strcpy(funcName, curFunc->name);
		Variable *argument = curFunc->args;
		while(argument != NULL)
		{
			strcat(funcName, "_");
			strcat(funcName, argument->type);
			argument = argument->next;
		}
		
		fprintf(f1, "%s PROC NEAR\n", funcName);
		getCode(root->child[1], f1, globalST, funcST); // get Code for function body from AST at FuncStmt
//		printf("Func done\n");
		fprintf(f1, "\tRET\n");
		fprintf(f1, "%s ENDP\n\n", funcName);
		
		if(root->noOfChildren == 3) getCode(root->child[2], f1, globalST, funcST);
		else return;
		
		return;
	}
	
	else if(strcmp(root->symbol, "FuncStmt") == 0 && 
		(strcmp(root->child[0]->symbol, "INT") == 0 || strcmp(root->child[0]->symbol, "DECI") == 0 || 
		strcmp(root->child[0]->symbol, "BOOL") == 0 || strcmp(root->child[0]->symbol, "WORD") == 0 || 
		strcmp(root->child[0]->symbol, "ENUM") == 0))
	{
		if(strcmp(root->child[root->noOfChildren-1]->symbol, "FuncStmt") == 0 || strcmp(root->child[root->noOfChildren-1]->symbol, "ReturnStmt") == 0)
		{
		//	printf("Going to %s\n", root->child[root->noOfChildren - 1]->symbol);
			getCode(root->child[root->noOfChildren-1], f1, globalST, funcST);
			
		}
		return;
	}
	
	else if(strcmp(root->symbol, "Variable") == 0)
	{
		fprintf(f1, "\tLEA SI, %s\n", root->child[0]->lexeme);
		fprintf(f1, "\tMOV EAX, [SI + %sd]\n", root->child[1]->lexeme);
		return;
	}
	
	else if(strcmp(root->child[0]->symbol, "GIVE") == 0)
	{
		fprintf(f1, "\tMOV EAX, %s\n", root->child[1]->lexeme);
		fprintf(f1, "\tMOV STDOUT_DATA, EAX\n");
		fprintf(f1, "\tLEA DX, STDOUT_DATA\n");
		fprintf(f1, "\tMOV AH, 09h\n");
		fprintf(f1, "\tINT 21h\n");
		
		if(root->noOfChildren == 3) getCode(root->child[2], f1, globalST, funcST);
		return;
	}

	else if(strcmp(root->child[0]->symbol, "TAKE") == 0)
	{
		fprintf(f1, "\tLEA DX, STDIN_MAX\n");
		fprintf(f1, "\tMOV AH, 0Ah\n");
		fprintf(f1, "\tINT 21h\n");
		fprintf(f1, "\tMOV EAX, STDIN_DATA\n");
		fprintf(f1, "\tMOV %s, EAX\n", root->child[1]->lexeme);

		if(root->noOfChildren == 3) getCode(root->child[2], f1, globalST, funcST);
		return;
	}

	else if(strcmp(root->symbol, "EnumBoolExp") == 0)
	{
		// get meaning of enumLiteral
		int value = 0;
		Variable *v = findIdentifier(root->child[1], globalST, curFunc, curScope);
		EnumData *ed = v->enumFields;
		
		while(ed != NULL)
		{
			if(strcmp(ed->field, root->child[2]->lexeme) == 0)
			{
				value = ed->meaning;
				break;
			}
			ed = ed->next;
		}		
		
		fprintf(f1, "\tMOV EAX, %s\n", getMemoryName(root->child[1]->lexeme, curFunc, globalST));
		fprintf(f1, "\tCMP EAX, %dd\n", value);

		if(strcmp(root->child[0]->symbol, "ENUM_EQ") == 0) fprintf(f1, "\tJE X%d\n", nextLabel);
		else fprintf(f1, "\tJNE X%d\n", nextLabel);
		nextLabel++;
		
		fprintf(f1, "\tMOV EAX, 00h\n");
		fprintf(f1, "\tJMP X%d\n", nextLabel);
		fprintf(f1, "\tX%d: MOV EAX, 01h\n", nextLabel - 1);
		fprintf(f1, "\tX%d: NOP\n", nextLabel);
		nextLabel++;
		return;
	}

	else if(strcmp(root->symbol, "AssignStmt") == 0 || 
			(strcmp(root->symbol, "FuncStmt") == 0 && strcmp(root->child[1]->symbol, "ASSIGN_OP") == 0))
	{
		if(strcmp(root->child[2]->symbol, "WORD_LITERAL") == 0 || strcmp(root->child[2]->symbol, "Word") == 0)
		{
			char str[120];
			strcpy(str, "\0");
			
			Node *temp = root->child[2];
			while(strcmp(temp->symbol, "WORD_LITERAL") != 0)
			{
				strcat(str, temp->child[0]->lexeme);
				temp = temp->child[1];
			}
			strcat(str, temp->lexeme);
			int i = 0;
			fprintf(f1, "\tLEA SI, %s\n", getMemoryName(root->child[0]->lexeme, curFunc, globalST));
			for(i = 0; i < strlen(str); i++)
			{
				fprintf(f1, "\tMOV [SI], %dd\n", (int)str[i]);
				fprintf(f1, "\tINC SI\n");
			}
			return;
		}
		
		else if(strcmp(root->child[2]->symbol, "ENUM_LITERAL") == 0)
		{
			int value = 0; // Meaning of the Enum Literal
			Variable *v = findIdentifier(root->child[0], globalST, curFunc, curScope);
			EnumData *ed = v->enumFields;
			
			while(ed != NULL)
			{
				if(strcmp(ed->field, root->child[2]->lexeme) == 0)
				{
					value = ed->meaning;
					break;
				}
				ed = ed->next;
			}
			
			fprintf(f1, "\tMOV %s, %dd\n", getMemoryName(root->child[0]->lexeme, curFunc, globalST), value);
			return;
		}
		
		else
		{
			getCode(root->child[2], f1, globalST, funcST); // RHS stored in EAX

			// LHS is an Identifier
			if(strcmp(root->child[0]->symbol, "ID") == 0) fprintf(f1, "\tMOV %s, EAX\n", getMemoryName(root->child[0]->lexeme, curFunc, globalST)); 
			else // LHS is an ArrayVar
			{
				fprintf(f1, "\tLEA SI, %s\n", getMemoryName(root->child[0]->child[0]->lexeme, curFunc, globalST));
				fprintf(f1, "\tMOV [SI + %dd], EAX\n", 4*atoi(root->child[0]->child[1]->lexeme));
			}
		}
		
		if(root->noOfChildren == 4) getCode(root->child[3], f1, globalST, funcST);
		return;
	}
	
	
	else if(strcmp(root->child[0]->symbol, "PLUS") == 0 || strcmp(root->child[0]->symbol, "MINUS") == 0)
	{
		if(strcmp(root->child[2]->symbol, "INT_LITERAL") == 0 && strcmp(root->child[2]->lexeme, "1") == 0)
		{
			fprintf(f1, "\tPUSH EBX\n");
			getCode(root->child[1], f1, globalST, funcST);
			if(strcmp(root->child[0]->symbol, "PLUS") == 0) fprintf(f1, "\tINC EAX\n");
			else fprintf(f1, "\tDEC EAX\n");
			fprintf(f1, "\tPOP EBX\n");
			return;
		}
		
		fprintf(f1, "\tPUSH EBX\n");
		
		getCode(root->child[1], f1, globalST, funcST);
		fprintf(f1, "\tPUSH EAX\n");

		getCode(root->child[2], f1, globalST, funcST);
		fprintf(f1, "\tPUSH EAX\n");

		fprintf(f1, "\tPOP EBX\n");
		fprintf(f1, "\tPOP EAX\n");

		if(strcmp(root->child[0]->symbol, "PLUS") == 0) fprintf(f1, "\tADD EAX, EBX\n");
		else fprintf(f1, "\tSUB EAX, EBX\n");

		fprintf(f1, "\tPOP EBX\n");
		
		return;
	}

	else if(strcmp(root->child[0]->symbol, "MULT") == 0 || strcmp(root->child[0]->symbol, "DIV") == 0)
	{
		fprintf(f1, "\tPUSH EBX\n");

		getCode(root->child[1], f1, globalST, funcST);
		fprintf(f1, "\tPUSH EAX\n");
		getCode(root->child[2], f1, globalST, funcST);
		fprintf(f1, "\tPUSH EAX\n");

		fprintf(f1, "\tPOP EBX\n");
		fprintf(f1, "\tPOP EAX\n");

		if(strcmp(root->child[0]->symbol, "MULT") == 0) fprintf(f1, "\tMULT EBX\n");
		else
		{
			fprintf(f1, "\tDIV BL\n"); // we`re doing AX/BX
			fprintf(f1, "\tMOV AH, 00h\n");
		}
		
		fprintf(f1, "\tPOP EBX\n");
		return;
	}

	else if(strcmp(root->child[0]->symbol, "POW") == 0 )
	{
		fprintf(f1, "\tPUSH EBX\n");
		fprintf(f1, "\tPUSH ECX\n");

		getCode(root->child[1], f1, globalST, funcST);
		fprintf(f1, "\tPUSH EAX\n");

		getCode(root->child[2], f1, globalST, funcST);
		fprintf(f1, "\tPUSH EAX\n");

		fprintf(f1, "\tPOP ECX\n");
		fprintf(f1, "\tPOP EBX\n");

		fprintf(f1, "\tMOV EAX, 1h\n");
		fprintf(f1, "\tX%d: MUL EBX\n", nextLabel);
		fprintf(f1, "\tDEC ECX\n"); // Assuming result will fit into AX alone and not DX:AX
		fprintf(f1, "\tJNZ X%d\n", nextLabel);

		fprintf(f1, "\tPOP ECX\n");
		fprintf(f1, "\tPOP EBX\n");
		
		nextLabel++;
		return;
	}

	else if(strcmp(root->child[0]->symbol, "AND") == 0 || strcmp(root->child[0]->symbol, "OR") == 0)
	{
		fprintf(f1, "\tPUSH EBX\n");

		getCode(root->child[1], f1, globalST, funcST);
		fprintf(f1, "\tPUSH EAX\n");
		getCode(root->child[2], f1, globalST, funcST);
		fprintf(f1, "\tPUSH EAX\n");

		fprintf(f1, "\tPOP EBX\n");
		fprintf(f1, "\tPOP EAX\n");

		if(strcmp(root->child[0]->symbol, "AND") == 0) fprintf(f1, "\tAND EAX, EBX\n");
		else fprintf(f1, "\tOR EAX, EBX\n");
		
		fprintf(f1, "\tPOP EBX\n");
		return;
	}

	else if(strcmp(root->child[0]->symbol, "NOT") == 0)
	{
		getCode(root->child[1], f1, globalST, funcST);
		fprintf(f1, "\tNOT EAX\n");
		return;
	}

	else if(strcmp(root->child[0]->symbol, "G") == 0 || strcmp(root->child[0]->symbol, "GE") == 0 || 
			strcmp(root->child[0]->symbol, "L") == 0 || strcmp(root->child[0]->symbol, "LE") == 0 || 
			strcmp(root->child[0]->symbol, "EQ") == 0 || strcmp(root->child[0]->symbol, "NEQ") == 0 )
	{
		fprintf(f1, "\tPUSH EBX\n");
		
		getCode(root->child[1], f1, globalST, funcST);
		fprintf(f1, "\tPUSH EAX\n");

		getCode(root->child[2], f1, globalST, funcST);
		fprintf(f1, "\tPUSH EAX\n");

		fprintf(f1, "\tPOP EBX\n");
		fprintf(f1, "\tPOP EAX\n");

		fprintf(f1, "\tCMP EAX, EBX\n");
		
		if(strcmp(root->child[0]->symbol, "G") == 0) fprintf(f1, "\tJG X%d\n", nextLabel);
		else if(strcmp(root->child[0]->symbol, "GE") == 0) fprintf(f1, "\tJGE X%d\n", nextLabel);
		else if(strcmp(root->child[0]->symbol, "L") == 0) fprintf(f1, "\tJL X%d\n", nextLabel);
		else if(strcmp(root->child[0]->symbol, "LE") == 0) fprintf(f1, "\tJLE X%d\n", nextLabel);
		else if(strcmp(root->child[0]->symbol, "EQ") == 0) fprintf(f1, "\tJEQ X%d\n", nextLabel);
		else if(strcmp(root->child[0]->symbol, "NEQ") == 0) fprintf(f1, "\tJNE X%d\n", nextLabel);		
		
		nextLabel++;
		fprintf(f1, "\tMOV EAX, 00h\n\tJMP X%d\n", nextLabel);
		fprintf(f1, "\tX%d: MOV EAX, 01h\n", nextLabel-1);
		fprintf(f1, "\tX%d: NOP\n", nextLabel);
		fprintf(f1, "\tPOP EBX\n");
		nextLabel++;
		return;
	}

	else if(strcmp(root->lexeme, "IfElseStmt") == 0)
	{
		getCode(root->child[0], f1, globalST, funcST);
		
		fprintf(f1, "\tADD EAX, 00h\n");
		fprintf(f1, "\tJNZ X%d\n", nextLabel);
		int temp1 = nextLabel;
		nextLabel++;
		int temp2 = nextLabel;		
		
		if(strcmp(root->symbol, "FuncStmt") == 0)
		{
			if(strcmp(root->child[2]->symbol, "AssignStmt") == 0 || strcmp(root->child[2]->symbol, "IterativeStmt") == 0 ||
			strcmp(root->child[2]->symbol, "IfElseStmt") == 0 || strcmp(root->child[2]->symbol, "IOStmt") == 0 ||
			strcmp(root->child[2]->symbol, "FuncCallStmt") == 0)
				getCode(root->child[2], f1, globalST, funcST);
			fprintf(f1, "\tJMP X%d\n", temp2);
			fprintf(f1, "\tX%d: NOP\n", temp1);
	
			getCode(root->child[1], f1, globalST, funcST);
			
			fprintf(f1, "\tX%d: NOP\n", temp2);
			nextLabel++;
			if(strcmp(root->child[root->noOfChildren-1]->symbol, "FuncStmt") == 0)
				getCode(root->child[root->noOfChildren-1], f1, globalST, funcST);
			else return;
		}
		
		else
		{
			if(root->noOfChildren == 3) getCode(root->child[2], f1, globalST, funcST);
			fprintf(f1, "\tJMP X%d\n", temp2);
			fprintf(f1, "\tX%d: NOP\n", temp1);
		
			getCode(root->child[1], f1, globalST, funcST);
			
			fprintf(f1, "\tX%d: NOP\n", temp2);
			nextLabel++;
			return;
		}
	}

	else if(strcmp(root->lexeme, "IterativeStmt") == 0)
	{
		int temp1 = nextLabel;
		fprintf(f1, "\tX%d: NOP\n", nextLabel);
		nextLabel++;
		int temp2 = nextLabel;
		nextLabel++;

		getCode(root->child[0], f1, globalST, funcST);
		fprintf(f1, "\tJZ X%d\n", temp2);
		
		getCode(root->child[1], f1, globalST, funcST);
		fprintf(f1, "\tJMP X%d\n", temp1);
		fprintf(f1, "\tX%d: NOP\n", temp2);
		
		if(strcmp(root->symbol, "FuncStmt") == 0) getCode(root->child[2], f1, globalST, funcST);
		return;
	}

	else if(strcmp(root->symbol, "FuncCallStmt") == 0 || 
			(strcmp(root->symbol, "FuncStmt") == 0 && strcmp(root->child[0]->symbol, "FUNC_ID") == 0))
	{
		// Find pointer to correct function
		Function *f = getFunc(root, globalST, funcST, curFunc, curScope);

		// Make funcName
		Variable *argu = f->args;
		char funcName[200];
		strcpy(funcName, f->name);
		while(argu != NULL)
		{
			strcat(funcName, "_");
			strcat(funcName, argu->type);
			argu = argu->next;
		}

		// Put args in memory!
		int cnt = 1;
		Node *params = root->child[1];
		while(strcmp(params->symbol, "ID") != 0)
		{
			fprintf(f1, "\tMOV EAX, %s\n", params->child[0]->lexeme);
			fprintf(f1, "\tMOV %s_ARG%d, EAX\n", funcName, cnt);
			params = params->child[1];
			cnt++;
		}

		fprintf(f1, "\tMOV EAX, %s\n", params->lexeme);
		fprintf(f1, "\tMOV %s_ARG%d, EAX\n", funcName, cnt);
		
		fprintf(f1, "\tCALL %s\n", funcName);
	}

	else if(strcmp(root->symbol, "ReturnStmt") == 0)
	{
		if(strcmp(root->child[0]->symbol, "VOID") == 0) return;
		else
		{
			getCode(root->child[0], f1, globalST, funcST);
//			fprintf(f1, "\tMOV EAX, %s\n", getMemoryName(root->child[0]->lexeme, curFunc, globalST));
			return;
		}
	}

	else
	{
		int i = 0;
		for(i = 0; i < root->noOfChildren; i++)
		{
			getCode(root->child[i], f1, globalST, funcST);
		}
		return;
	}
}
}



void makeAsm(Node *ast, Variable *globalST[], Function *funcST[], FILE *f1)
{
// Fill up .data!!
fprintf(f1, ".Model Tiny\n\n.Data\n");
// .data

fprintf(f1, "STDIN_MAX\tdd\t40\n");
fprintf(f1, "STDIN_ACT\tdd\t?\n");
fprintf(f1, "STDIN_DATA\tdd\t40 DUP('$')\n");
fprintf(f1, "STDOUT_DATA\tdd\t41 DUP('$')\n");

int i = 0;
for(i = 0; i < MAX_VARIABLES; i++)
{
	if(globalST[i] == NULL) continue;
	Variable *v = globalST[i];
	while(v != NULL)
	{
		if(v->arrayLen == -1)
		{
			if(strcmp(v->type, "WORD") != 0) fprintf(f1, "%s\tdd\t%dd\n", v->name, atoi(v->value));
			else fprintf(f1, "%s\tdd\t40 DUP('$')\n", v->name);
		}
		else
		{
			if(strcmp(v->type, "WORD") != 0) fprintf(f1, "%s\tdd\t%d DUP(0)\n", v->name, (v->arrayLen)*4);
			else fprintf(f1, "%s\tdd\t%d DUP('$')", v->name, (v->arrayLen)*40);
		}
		v = v->next;
	}
}

char funcName[120];
for(i = 0; i < MAX_FUNCTIONS; i++)
{
	if(funcST[i] == NULL) continue;
	Function *f = funcST[i];
	Variable *v = f->localST[0];
	while(f != NULL)
	{
		// Making func Name
		strcpy(funcName, "\0");
		strcpy(funcName, f->name);
		Variable *argu = f->args;
		while(argu != NULL)
		{
			strcat(funcName, "_");
			strcat(funcName, argu->type);
			argu = argu->next;
		}
		
		// Adding func args
		argu = f->args;
		int cnt = 1;
		
		while(argu != NULL)
		{
			if(argu->arrayLen == -1)
			{
				if(strcmp(argu->type, "WORD") != 0) fprintf(f1, "%s\tdd\tDUP(0)\n", getMemoryName(argu->name, f, globalST));
				else fprintf(f1, "%s\tdd\t40 DUP('$')\n", getMemoryName(argu->name, f, globalST));
			}
			else
			{
				if(strcmp(argu->type, "WORD") != 0) fprintf(f1, "%s\tdd\t%d DUP(0)\n", getMemoryName(argu->name, f, globalST), (argu->arrayLen)*4);
				else fprintf(f1, "%s\tdd\t%d DUP('$')", getMemoryName(argu->name, f, globalST), (argu->arrayLen)*40);
			}
			cnt++;
			argu = argu->next;
		}

		// Adding local vars
		int j = 0;
		for(j = 0; j < MAX_VARIABLES; j++)
		{
			if(f->localST[j] == NULL)continue;
			v = f->localST[j];
			while(v != NULL)
			{
				if(v->arrayLen == -1)
				{
					if(strcmp(v->type, "WORD") != 0) fprintf(f1, "%s\tdd\t%dd\n", getMemoryName(v->name, f, globalST), atoi(v->value));
					else fprintf(f1, "%s\tdd\t40 DUP('$')\n", getMemoryName(v->name, f, globalST));
				}
				else
				{
					if(strcmp(v->type, "WORD") != 0) fprintf(f1, "%s\tdd\t%d DUP(0)\n", getMemoryName(v->name, f, globalST), (v->arrayLen)*4);
					else fprintf(f1, "%s\tdd\t%d DUP('$')", getMemoryName(v->name, f, globalST), (v->arrayLen)*40);
				}
//				fprintf(f1, "%s_LOCAL_%s\tdd\t40 DUP(0)\n", funcName, v->name);
				v = v->next;
			}
		}
		f = f->next;
	}
}

// .code
fprintf(f1, "\n\n.Code\n.Startup\n");
getCode(ast, f1, globalST, funcST);
return;
}
