#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"
#include "semantic.h"



int getType(Node *root, Variable *globalST[], Function *funcST[], Function *curFunc, int curScope)
{
if(strcmp(root->symbol, "INT_LITERAL") == 0) return typeToInt("INT");
else if(strcmp(root->symbol, "DECI_LITERAL") == 0) return typeToInt("DECI");
else if(strcmp(root->symbol, "BOOL_LITERAL") == 0) return typeToInt("BOOL");
else if(strcmp(root->symbol, "WORD_LITERAL") == 0 || strcmp(root->symbol, "Word") == 0) return typeToInt("WORD");
else if(strcmp(root->symbol, "ENUM_LITERAL") == 0) return typeToInt("ENUM");
else if(strcmp(root->symbol, "ID") == 0)
{
	Variable *v = findIdentifier(root, globalST, curFunc, curScope);
	if(v == NULL)
	{
		printf("Error at line %d: Variable %s is undeclared.\n", root->lineNo, root->lexeme);
		exit(1);
	}
	return typeToInt(v->type);
}
else if(strcmp(root->symbol, "Variable") == 0)
{
	Variable *v = findIdentifier(root->child[0], globalST, curFunc, curScope);
	if(v == NULL)
	{
		printf("Error at line %d: Variable %s is undeclared.\n", root->child[0]->lineNo, root->child[0]->lexeme);
		exit(1);
	}
	return typeToInt(v->type);
}
else if(strcmp(root->symbol, "BooleanExp") == 0)
{
	if(strcmp(root->child[0]->symbol, "AND") == 0 || strcmp(root->child[0]->symbol, "OR") == 0)
	{
		int op1 = getType(root->child[1], globalST, funcST, curFunc, curScope);
		int op2 = getType(root->child[2], globalST, funcST, curFunc, curScope);
		if(op1 >= 4 || op2 >= 4)
		{
			printf("Error at line %d: Type Mismatch Error. The operands can only be BOOL\n", root->child[0]->lineNo);
			exit(1);
		}
		else return typeToInt("BOOL");
	}
	
	else if(strcmp(root->child[0]->symbol, "NOT") == 0)
	{
		int op1 = getType(root->child[1], globalST, funcST, curFunc, curScope);
		if(op1 >= 4)
		{
			printf("Error at line %d: Type Mismatch Error. The operands can only be BOOL\n", root->child[1]->lineNo);
			exit(1);
		}
		else return typeToInt("BOOL");
	}
	
	else  // if(strcmp(root->child[0]->symbol, "G", "L"..) == 0)
	{
		int op1 = getType(root->child[1], globalST, funcST, curFunc, curScope);
		int op2 = getType(root->child[2], globalST, funcST, curFunc, curScope);
		if(op1 >= 3 || op2 >= 3)
		{
			printf("Error at line %d: Type Mismatch Error. The operands can only be INT/DECI\n", root->child[1]->lineNo);
			exit(1);
		}
		else return typeToInt("BOOL");
	}
}

else if(strcmp(root->symbol, "EnumBoolExp") == 0)
{
	int op1 = getType(root->child[1], globalST, funcST, curFunc, curScope);
	if(op1 != 5)
	{
		printf("Error at line %d: Type Mismatch Error. The operands can only be ENUM\n", root->child[1]->lineNo);
		exit(1);
	}
	else return typeToInt("BOOL");
}

else if(strcmp(root->symbol, "ArithExp") == 0)
{
	int op1 = getType(root->child[1], globalST, funcST, curFunc, curScope);
	int op2 = getType(root->child[2], globalST, funcST, curFunc, curScope);
	if(op1 == 1 && op2 <= 2) return typeToInt("INT");
	else if(op1 == 1 && op2 > 2)
	{
		printf("Error at line %d: Type Mismatch Error. The operand %s should be INT/DECI.\n", root->child[2]->lineNo, root->child[2]->lexeme);
		exit(1);
	}
	else if(op1 == 2 && op2 == 1) return typeToInt("INT");
	else if(op1 == 2 && op2 == 2) return typeToInt("DECI");
	else
	{
		printf("Error at line %d: Type Mismatch Error. The operands %s and %s should be INT/DECI.\n", root->child[2]->lineNo, root->child[1]->lexeme, root->child[2]->lexeme);
		exit(1);	
	}
}

else if(strcmp(root->symbol, "FuncCallStmt") == 0 || 
	(strcmp(root->symbol, "FuncStmt") == 0 && strcmp(root->child[0]->symbol, "FUNC_ID") == 0))
{
	Function *f = getFunc(root, globalST, funcST, curFunc, curScope);
	if(f == NULL)
	{
		printf("Error at line %d: Corresponding function declaration for %s NOT found\n", root->child[0]->lineNo, root->child[0]->lexeme);
		exit(1);
	}
	else return typeToInt(f->returnType);
}
/*
else
{
	printf("No Type for %s.. :/\n", root->symbol);
	exit(1);
}
*/
}



Function *getFunc(Node *root, Variable *globalST[], Function *funcST[], Function *curFunc, int curScope)
{
if(strcmp(root->symbol, "FuncCallStmt") == 0 || 
	(strcmp(root->symbol, "FuncStmt") == 0 && strcmp(root->child[0]->symbol, "FUNC_ID") == 0))	
{	
	int cntParams = 0;
	Node *params = NULL;
	Variable *argu = NULL;
	Function *f = funcST[hashfunc(root->child[0]->lexeme)];

	// get no of args in the call stmt
	params = root->child[1];
	if(strcmp(params->symbol, "VOID") == 0)
	{
		cntParams = 0;
		while( f != NULL)
		{
			if(strcmp(f->name, root->child[0]->lexeme) == 0 && f->noOfArgs == 0) return f;
			else f = f->next;
		}
		return f;
	}
	
	else
	{
		cntParams = 0;
		while(strcmp(params->symbol, "ID") != 0)
		{
			cntParams++;
			params = params->child[1];
		}
		cntParams++;
		
		while(f != NULL)
		{
			params = root->child[1];
			argu = f->args;
			if(strcmp(f->name, root->child[0]->lexeme) == 0 && f->noOfArgs == cntParams)
			{
				while(strcmp(params->symbol, "ID") != 0)
				{
					if(getType(params->child[0], globalST, funcST, curFunc, curScope) == typeToInt(argu->type))
					{
						params = params->child[1];
						argu = argu->next;
					}
					else break;
				}
				// compare with params
				if(strcmp(params->symbol, "ID") == 0)
				{
					if(getType(params, globalST, funcST, curFunc, curScope) == typeToInt(argu->type)) return f;
					else f = f->next;
				}
				
				else if(strcmp(params->symbol, "ID") != 0 && params != NULL)
				{
					f = f->next;
				}
			}
			else f = f->next;
		}
		return f; // returning NULL..
	}	
}
else return NULL;
}



int typeToInt(char *str)
{
if(strcmp(str, "INT") == 0) return 1;
else if(strcmp(str, "DECI") == 0) return 2;
else if(strcmp(str, "BOOL") == 0) return 3;
else if(strcmp(str, "WORD") == 0) return 4;
else if(strcmp(str, "ENUM") == 0) return 5;
}




Variable *findIdentifier(Node *root, Variable *globalST[], Function *curFunc, int curScope)
{
if(strcmp(root->symbol, "ID") == 0)
{
	int index = -1;
	Variable *var = NULL;

	if(curScope == 0) // Search in globalST
	{
		index = hashvar(root->lexeme);
		var = globalST[index];
		while(var != NULL)
		{
			if(strcmp(var->name, root->lexeme) == 0) break;
			else var = var->next;
		}
		
		if(var == NULL) return NULL;
		else return var;
	}

	else // search in curFunc->localST, globalST and args
	{
		// GlobalST
		index = hashvar(root->lexeme);
		var = globalST[index];
		while(var != NULL)
		{
			if(strcmp(var->name, root->lexeme) == 0) break;
			else var = var->next;
		}
		
		if(var != NULL) return var;
		else
		{
			// LocalST
			var = curFunc->localST[index];
			while(var != NULL)
			{
				if(strcmp(var->name, root->lexeme) == 0) break;
				else var = var->next;
			}
			
			if(var != NULL) return var;
			else
			{
				var = curFunc->args;
				while(var != NULL)
				{
					if(strcmp(var->name, root->lexeme) == 0) break;
					else var = var->next;
				}
				
				return var;
			}
		}
	} // else ends here
}
else if(strcmp(root->symbol, "Variable") == 0)
{
	return findIdentifier(root->child[0], globalST, curFunc, curScope);
}
else
{
	printf("NONE FOR %s :/\n", root->lexeme);
	return NULL;
}
}




void checkSemantics(Node *root, Variable *globalST[], Function *funcST[]) // Pass the AST, not the Parse Tree!
{
static int curScope = 0;
static Function *curFunc = NULL;
if(root == NULL) return;
else
{
	// Check if the variable has been declared
	if(strcmp(root->symbol, "ID") == 0)
	{
		Variable *v = findIdentifier(root, globalST, curFunc, curScope);
		
		if(v == NULL)
		{
			printf("Error at line %d: Variable %s is undeclared.\n", root->lineNo, root->lexeme);
			exit(1);
		}
	}
	
	// Update curFunc and curScope
	else if(root->parent != NULL && strcmp(root->parent->symbol, "Program") == 0 && strcmp(root->symbol, "DeclStmt") != 0 && strcmp(root->symbol, "FuncStmts") != 0)
	{
		curFunc = NULL;
		curScope = 0;
	}
	
	
	else if(strcmp(root->symbol, "FuncCallStmt") == 0 || 
			(strcmp(root->symbol, "FuncStmt") == 0 && strcmp(root->child[0]->symbol, "FUNC_ID") == 0))
	{
		// Check for Recursion
		if(curFunc != NULL && strcmp(root->child[0]->lexeme, curFunc->name) == 0)
		{
			printf("Error at line %d: Recursive Calls inside a function are not allowed.\n", root->child[0]->lineNo);
			exit(1);
		}
		
		// Checking for function declaration
		Function *f = getFunc(root, globalST, funcST, curFunc, curScope);
		if(f == NULL)
		{
			printf("Error at %d: Corresponding function declaration for %s NOT found\n", root->child[0]->lineNo, root->child[0]->lexeme);
			exit(1);
		}
	}
	
	// Array of Length 0
	else if(strcmp(root->symbol, "VarList") == 0 || strcmp(root->symbol, "MoreVars") == 0)
	{
		if(root->child[1] != NULL && strcmp(root->child[1]->symbol, "INT_LITERAL") == 0 && strcmp(root->child[1]->lexeme, "0") == 0)
		{
			printf("Error at line %d: Cannot declare an array of length 0\n", root->child[0]->lineNo);
			exit(1);
		}
	}
		
	else if(strcmp(root->symbol, "Variable") == 0)
	{
		Variable *var = findIdentifier(root->child[0], globalST, curFunc, curScope);
		if(var == NULL)
		{
			printf("Error at %d: Variable %s is undeclared.\n", root->child[0]->lineNo, root->child[0]->lexeme);
			exit(1);
		}
		
		// Find Length of Array
		if(atoi(root->child[1]->lexeme) > var->arrayLen - 1)
		{
			printf("Error at %d: Index value cannot exceed Array Length - 1 (ie %d)\n", root->child[1]->lineNo, var->arrayLen-1);
			exit(1);
		}
		return;
	}

	else if(strcmp(root->symbol, "ReturnStmt") == 0)
	{
		if(strcmp(root->child[0]->symbol, "VOID") == 0)
		{
			if(strcmp(curFunc->returnType, "VOID") != 0)
			{
				printf("Error at line %d: Return Type (VOID) not consistent with the one given in Function Header (%s).\n", root->child[0]->lineNo, curFunc->returnType);
				exit(1);		
			}
		}
		
		else // return type is not void
		{
			Variable *var = findIdentifier(root->child[0], globalST, curFunc, curScope);
			if(var == NULL)
			{
				printf("Error at line blah %d: Variable %s is undeclared.\n", root->child[0]->lineNo, root->child[0]->lexeme);
				exit(1);
			}

			if(strcmp(curFunc->returnType, var->type) != 0)
			{
				printf("Error at line %d: Return Type (%s) not consistent with the one given in Function Header (%s).\n", root->child[0]->lineNo, var->type, curFunc->returnType);
				exit(1);
			}
		}		
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
		
		// When found -- it`ll be found! -- update curFunc to that
		if(f == NULL) printf("%s nai mila.. \n", root->child[0]->lexeme);
		return;
	}
	
	// TYPE CHECKING + ENUM CHECKING
	else if((strcmp(root->symbol, "FuncStmt") == 0 && strcmp(root->child[1]->symbol, "ASSIGN_OP") == 0)
			|| (strcmp(root->symbol, "AssignStmt") == 0)) 
	{
		Variable *v = findIdentifier(root->child[0], globalST, curFunc, curScope);
		if(v == NULL)
		{
			printf("Error at %d: Variable %s is undeclared.\n", root->child[0]->lineNo, root->child[0]->lexeme);
			exit(1);
		}

		if(strcmp(root->child[2]->symbol, "ENUM_LITERAL") == 0)
		{
			// Search for the Enum_Literal in the Enum_fields of the given Enum Variable
			EnumData *ed = v->enumFields;
			if(ed == NULL)
			{
				printf("Error at line %d: No corresponding Enum Fields found for '%s'\n", root->child[0]->lineNo, root->child[0]->lexeme);
				exit(1);
			}
			
			while(ed != NULL)
			{
				// if found => return
				if(strcmp(ed->field, root->child[2]->lexeme) == 0) return;
				ed = ed->next;
			}
			
			if(ed == NULL)
			{
				printf("Error at line %d: '%s' is not an Enum Field of '%s'\n", root->child[0]->lineNo, root->child[2]->lexeme, root->child[0]->lexeme);
				exit(1);		
			}
			return;
		}

		int lhsType = getType(root->child[0], globalST, funcST, curFunc, curScope);
		int rhsType = getType(root->child[2], globalST, funcST, curFunc, curScope);
		
		if(lhsType == 1 && rhsType >= 3)
		{
			printf("Error at line %d: Type Mismatch. Cannot assign to INT Variable\n", root->child[0]->lineNo);
			exit(1);
		}
		
		else if(lhsType == 2 && rhsType != 2)
		{
			printf("Error at line %d: Type Mismatch. Cannot assign to DECI Variable\n", root->child[0]->lineNo);
			exit(1);
		}
		
		else if(lhsType == 3 && (rhsType >= 4 || rhsType == 2)) 
		{
			printf("Error at line %d: Type Mismatch. Cannot assign to BOOL Variable\n", root->child[0]->lineNo);
			exit(1);
		}
		
		else if(lhsType == 4 && rhsType != 4)
		{
			printf("Error at line %d: Type Mismatch. Cannot assign to WORD Variable\n", root->child[0]->lineNo);
			exit(1);
		}
		
		else if(lhsType == 5 && rhsType != 5)
		{
			printf("Error at line %d: Type Mismatch. Cannot assign to ENUM Variable\n", root->lineNo);
			exit(1);
		}
	}

	else if(strcmp(root->symbol, "EnumBoolExp") == 0)
	{
		// Search for the Enum_Literal in the Enum_fields of the given Enum Variable
		Variable *v = findIdentifier(root->child[1], globalST, curFunc, curScope);
		if(v == NULL)
		{
			printf("Error at %d: Variable %s is undeclared.\n", root->child[0]->lineNo, root->child[0]->lexeme);
			exit(1);
		}

		EnumData *ed = v->enumFields;
		if(ed == NULL)
		{
			printf("Error at line %d: No corresponding Enum Fields found for '%s'\n", root->child[1]->lineNo, root->child[1]->lexeme);
			exit(1);
		}
		
		while(ed != NULL)
		{
			// if found => return
			if(strcmp(ed->field, root->child[2]->lexeme) == 0) return;
			ed = ed->next;
		}
		
		if(ed == NULL)
		{
			printf("Error at line %d: '%s' is not an Enum Field of '%s'\n", root->child[1]->lineNo, root->child[2]->lexeme, root->child[1]->lexeme);
			exit(1);		
		}
	}
	
	else if(strcmp(root->symbol, "IterativeStmt") == 0 || strcmp(root->symbol, "IfElseStmt") == 0)
	{
		int type1 = getType(root->child[0], globalST, funcST, curFunc, curScope);
		if(type1 > 3)
		{
			printf("Error at line %d: Condition should be of type BOOL/INT/DECI\n", root->lineNo);
			exit(1);
		}
	}
	
	int i = 0;
	for(i = 0; i < root->noOfChildren; i++) checkSemantics(root->child[i], globalST, funcST);
	return;
} // else block ends here
} //checkSemantics ends here
