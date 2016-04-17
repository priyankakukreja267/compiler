// Doing ST after Parsing (so that no syntactic errors) but using o/p of lexer.
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"


// Global variables
int curScope = 0;
int insert = 0; // 1 if ID should be inserted. 0 otherwise
char curType[MAX_LENGTH];

Variable *curVar = NULL;
EnumData *curChain = NULL;
Variable *v = NULL;
Variable *v2 = NULL;
Function *f = NULL;
int isFunc = 0;
Variable *curTable[MAX_VARIABLES];
Function *curFunc = NULL;
int curOffset = 0;

// Functions
void initVar(Variable *globalST[])
{
int i = 0;
for(i = 0; i < MAX_VARIABLES; i++) globalST[i] = NULL;
}


void initFunc(Function *funcST[])
{
int i = 0;
for(i = 0; i < MAX_FUNCTIONS; i++) funcST[i] = NULL;
}


void buildST(Token *head, Variable *globalST[], Function *funcST[])
{
if(head == NULL) return;
else if(strcmp(head->symbol, "CODE") == 0) return;
else
{
	if(strcmp(head->symbol, "DATA") == 0)
	{
		curScope = 0;
		head = head->next;
		buildST(head, globalST, funcST);
	}
	
	else if(strcmp(head->symbol, "%%") == 0)
	{
		insert = 0;
		curScope = 0;
		curOffset = 0;
		initVar(curTable);
		head = head->next;
		buildST(head, globalST, funcST);
	}
	
	else if(strcmp(head->symbol, "CURLY_CLOSE") == 0 && curScope == 1)
	{
		curFunc = NULL;
		head = head->next;
		buildST(head, globalST, funcST);
	}
	
	else if(strcmp(head->symbol, "INT") == 0 || strcmp(head->symbol, "DECI") == 0 || strcmp(head->symbol, "BOOL") == 0
			|| strcmp(head->symbol, "WORD") == 0 || strcmp(head->symbol, "ENUM") == 0)
	{
		insert = 1;
		strcpy(curType, head->symbol);
		head = head->next;
		buildST(head, globalST, funcST);
	}

	else if(strcmp(head->symbol, "SEMI_COLON") == 0 && curScope == 1)
	{
		insert = 0;
		head = head->next;
		buildST(head, globalST, funcST);		
	}

	else if(insert == 1 && strcmp(head->symbol, "ENUM_LITERAL") == 0)
	{
		EnumData *temp = curChain;
		// Insert at the end of the chain
		if(curChain == NULL)
		{
			curChain = (EnumData *) malloc(sizeof(EnumData));
			temp = curChain;
		}
		else
		{
			int curMeaning = atoi(head->next->next->lexeme);
			temp = curChain;

			while(temp->next != NULL)
			{
				if(temp->meaning == curMeaning)
				{
					printf("Error at %s: Enum Fields can only take distinct integer values for an enum variable.\n", head->lexeme);
					exit(1);
				}
				else temp = temp->next;
			}
			if(temp->meaning == curMeaning)
			{
				printf("Error at %s: Enum Fields can only take distinct integer values for an enum variable.\n", head->lexeme);
				exit(1);
			}
			temp->next = (EnumData *) malloc(sizeof(EnumData));
			temp = temp->next;
		}
		
		strcpy(temp->field, head->lexeme);
		head = head->next->next;
		temp->meaning = atoi(head->lexeme);
		temp->next = NULL;
		head = head->next;
		buildST(head, globalST, funcST);		
	}
	
	else if(insert == 1 && strcmp(head->symbol, "ID") == 0)
	{
		v = (Variable *) malloc(sizeof(Variable));

		strcpy(v->name, head->lexeme);
		strcpy(v->type, curType);
		v->scope = curScope;
		v->lineNoDecl = head->lineNo;
		v->lineNoUsage = -1;
		v->wordLen = -1;
		v->arrayLen = -1;
		v->enumFields = NULL;
		v->next = NULL;
		
		// Update with default values
		if(strcmp(curType, "INT") == 0) strcpy(v->value, "0");
		else if(strcmp(curType, "DECI") == 0) strcpy(v->value, "0.0");
		else if(strcmp(curType, "BOOL") == 0) strcpy(v->value, "FALSE");
		else strcpy(v->value, "\0");

		if(strcmp(curType, "ENUM") == 0)
		{
			// If curType = ENUM, insert the chain. Make chain = NULL. make value = 1st field.
			v->enumFields = curChain;
			curChain = NULL;
			strcpy(v->value, v->enumFields->field);
		}

		if(strcmp(v->type, "INT") == 0 || strcmp(v->type, "DECI") == 0 || strcmp(v->type, "BOOL") == 0 || strcmp(v->type, "ENUM") == 0)
		{
			v->offset = curOffset;
			curOffset = curOffset + 4;
		}
		else
		{
			v->offset = curOffset;
			curOffset = curOffset + MAX_LEXEME;
		}

		Token *temp = head->next;
		if(strcmp(temp->symbol, "SQ_OPEN") == 0)
		{
			temp = temp->next;
			v->arrayLen = atoi(temp->lexeme);
			temp = temp->next;
			if(strcmp(v->type, "INT") == 0 || strcmp(v->type, "DECI") == 0 || strcmp(v->type, "BOOL") == 0 || strcmp(v->type, "ENUM") == 0)
				curOffset = curOffset + 4*(v->arrayLen) - 4;
			else curOffset = curOffset + MAX_LEXEME*(v->arrayLen) - MAX_LEXEME;
		}
			
		else if(strcmp(temp->symbol, "ASSIGN_OP") == 0)
		{
			// If '=' jump and update arraylen and value
			temp = temp->next;
			strcpy(v->value, temp->lexeme);
			v->lineNoUsage = temp->lineNo;
			if(strcmp(curType, "WORD") == 0) v->wordLen = strlen(v->value);
		}
		
		else if(strcmp(temp->symbol, "SEMI_COLON") == 0)
		{
			if(curScope == 1) insert = 0;
		}

		if(curScope == 0) insertVar(globalST, v);
//		else insertVar(curFunc->localST, v);
		
		else
		{
			Variable *a = curFunc->args;
			while(a != NULL)
			{
				if(strcmp(a->name, v->name) == 0)
				{
					printf("Cannot re-declare an argument as a variable inside the function body. Var: %s, Type: %s, Scope: %d\n", v->name, v->type, v->scope);
					exit(1);
				}
				
				else a = a->next;
			}
			
			insertVar(curFunc->localST, v);
		}
		
		
		v = NULL;
		head = temp->next;
		buildST(head, globalST, funcST);		
	}
	
	else if(strcmp(head->symbol, "FUNC") == 0)
	{
		// Insert Functions here!
		curScope = 1;
		curOffset = 0;
		
		head = head->next; // At FUNC_ID
		f = (Function *) malloc(sizeof(Function));
		strcpy(f->name, head->lexeme);
		initVar(f->localST);
		f->args = NULL;
		curFunc = f;
				
		int cnt = 0;
		
		head = head->next; // At COLON
		while(strcmp(head->symbol, "ARROW") != 0)
		{
			head = head->next; // At Type or VOID
			if(strcmp(head->symbol, "VOID") == 0)
			{
				f->args = NULL;
				f->noOfArgs = 0;
				head = head->next;
				break;
			}
			
			cnt++;
			v = (Variable *) malloc(sizeof(Variable));
			
			strcpy(v->type, head->symbol);
			
			if(strcmp(v->type, "WORD") == 0)
			{
				v->wordLen = strlen(head->next->lexeme);
				v->offset = curOffset;
				curOffset = curOffset + MAX_LEXEME;
			}
			else
			{
				v->wordLen = -1;
				v->offset = curOffset;
				curOffset = curOffset + 4;
			}
			
			head = head->next;
			
			strcpy(v->name, head->lexeme);
			v->scope = curScope;
			v->lineNoDecl = head->lineNo;
			v->lineNoUsage = -1;
			v->arrayLen = -1;
			v->enumFields = NULL;
			strcpy(v->value, "\0");

			// Inserting the arg into Function Node f 
			
			Variable *a = f->args;
			if(a == NULL) f->args = v;
			else
			{
				while(a->next != NULL) a = a->next;
				a->next = v;
			}
			v->next = NULL;
			
			v = NULL;						
			head = head->next;
		}
		
		f->noOfArgs = cnt;
		head = head->next;
		strcpy(f->returnType, head->symbol);
		f->next = NULL;
		
		insertFunc(funcST, f);
		f = NULL;

		head = head->next;
		buildST(head, globalST, funcST);
	}
	else
	{
		head = head->next;
		buildST(head, globalST, funcST);
	}
}
}


int hashvar(char str[])
{
int i = 0, ans = 0;
for(i = 0; i < strlen(str); i++) ans += str[i];
ans = ans%MAX_VARIABLES;
return ans;
}


int hashfunc(char str[])
{
int i = 0, ans = 0;
for(i = 0; i < strlen(str); i++) ans += str[i];
ans = ans%MAX_FUNCTIONS;
return ans;
}


void insertVar(Variable *table[], Variable *v)
{
if(strcmp(v->type, "BOOL") == 0)
{
	if(atoi(v->value) > 0) strcpy(v->value, "1");
	else strcpy(v->value, "0");
}
int index = hashvar(v->name);
Variable *head = table[index];
// Check if the var is not already inserted. If not, insert at the end
if(head == NULL)
{
	table[index] = v;
	table[index]->next = NULL;
}
else
{
	head = table[index];

	while(head->next != NULL)
	{
		if((strcmp(head->name, v->name) == 0) && (head->scope == v->scope))
		{
			if(strcmp(head->type, v->type) != 0) printf("Another variable with same name but different type exists. ");
			printf("Cannot declare a variable twice. Var: %s, Type: %s, Scope: %d\n", v->name, v->type, v->scope);
			exit(1);
		}
		else head = head->next;	
	}

	if((strcmp(head->name, v->name) == 0) && (head->scope == v->scope)) 
	{
		if(strcmp(head->type, v->type) != 0) printf("Another variable with same name but different type exists. ");
		printf("Cannot declare a variable twice. Var: %s, Type: %s, Scope: %d\n", v->name, v->type, v->scope);
		exit(1);
	}
	
	head->next = v;
	head->next->next = NULL;
}
}


void insertFunc(Function *table[], Function *f)
{
int index = hashfunc(f->name);
Function *head = table[index];
if(head == NULL)
{
	table[index] = f;
	table[index]->next = NULL;
}

else
{
	head = table[index];
	while(head->next != NULL)
	{
		if(strcmp(head->name, f->name) == 0 && areSameArgs(head, f) == 1)
		{
			printf("Cannot define the same function twice. Function: %s\n", f->name);
			exit(0);
		}
		else head = head->next;
	}

	if(strcmp(head->name, f->name) == 0 && areSameArgs(head, f) == 1)
	{
		printf("Cannot define the same function twice. Function: %s\n", f->name);
		exit(0);
	}

	f->next = NULL;
	head->next = f;
//	head->next->next = NULL;
}
}


// Fn Overloading => same name, same no of args possible, but types should be diff
int areSameArgs(Function *f1, Function *f2) // Tells us if they both have all args of same type
{
int ans = 1;
if(f1->noOfArgs != f2->noOfArgs) return 0;
else
{
	// Placement of arguements in Overloaded function should be the same
	Variable *v1 = f1->args;
	Variable *v2 = f2->args;
	while(v1 != NULL && v2 != NULL)
	{
		if(strcmp(v1->type, v2->type) != 0)
		{
			ans = 0;
			break;
		}
		
		else
		{
			v1 = v1->next;
			v2 = v2->next;
		}
	}
	return ans;
}
}


void printST(char *filename, Variable *globalST[], Function *funcST[])
{
FILE *f1 = fopen(filename, "w");
if(!f1)
{
	printf("Give the filename for Symbol Table output!\n");
	exit(1);
}
else
{
	int i = 0, j = 0, rem = 0;
	
	fprintf(f1, "VARIABLES\nName");
	for(j = 0; j<14-strlen("Name"); j++) fprintf(f1, " ");
	fprintf(f1, "Type");
	for(j = 0; j<14-strlen("Type"); j++) fprintf(f1, " ");
	fprintf(f1, "Scope");
	for(j = 0; j<14-strlen("Scope"); j++) fprintf(f1, " ");
	fprintf(f1, "Offset");
	for(j = 0; j<14-strlen("Offset"); j++) fprintf(f1, " ");
	
	fprintf(f1, "Value");
	for(j = 0; j<14-strlen("Value"); j++) fprintf(f1, " ");
	fprintf(f1, "lineNoDecl");
	for(j = 0; j<14-strlen("lineNoDecl"); j++) fprintf(f1, " ");
	fprintf(f1, "lineNoUse");
	for(j = 0; j<14-strlen("lineNoUse"); j++) fprintf(f1, " ");
	fprintf(f1, "arrayLen");
	for(j = 0; j<14-strlen("arrayLen"); j++) fprintf(f1, " ");
	fprintf(f1, "wordLen");
	for(j = 0; j<14-strlen("wordLen"); j++) fprintf(f1, " ");
	fprintf(f1, "enumFields\n");
	

	for(i = 0; i < MAX_VARIABLES; i++)
	{
		Variable *temp = globalST[i];
		int rem = 0;
		while(temp != NULL)
		{
			fprintf(f1, "%s", temp->name);
			for(j = 0; j<14-strlen(temp->name); j++) fprintf(f1, " ");
			fprintf(f1, "%s", temp->type);
			for(j = 0; j<14-strlen(temp->type); j++) fprintf(f1, " ");
			fprintf(f1, "%d", temp->scope);
			for(j = 0; j<13; j++) fprintf(f1, " ");
			fprintf(f1, "%d", temp->offset);
			rem = (temp->offset == 0) ? 1 : log10(temp->offset) + 1;
			for(j = 0; j<14-rem; j++) fprintf(f1, " ");
			fprintf(f1, "%s", temp->value);
			for(j = 0; j<14-strlen(temp->value); j++) fprintf(f1, " ");
			fprintf(f1, "%d", temp->lineNoDecl);
			for(j = 0; j<14-(log10(temp->lineNoDecl) + 1); j++) fprintf(f1, " ");
			
			fprintf(f1, "%d", temp->lineNoUsage);
			rem = (temp->lineNoUsage < 0) ? 2 : (log10(temp->lineNoUsage) + 1);
			for(j = 0; j<14-rem; j++) fprintf(f1, " ");

			fprintf(f1, "%d", temp->arrayLen);
			rem = (temp->arrayLen < 0) ? 2 : ((log10(temp->arrayLen) + 1));
			for(j = 0; j<14-rem; j++) fprintf(f1, " ");
			
			fprintf(f1, "%d", temp->wordLen);
			rem = (temp->wordLen < 0) ? 2 : (log10(temp->wordLen) + 1);
			for(j = 0; j<14-rem; j++) fprintf(f1, " ");
			
			if(temp->enumFields == NULL) fprintf(f1, "NULL\n");
			else 
			{
				EnumData *e = temp->enumFields;
				while(e != NULL)
				{
					fprintf(f1, "%s = %d\n", e->field, e->meaning);
					if(e->next != NULL) for(j = 0; j < 9*14; j++) fprintf(f1, " ");
					e = e->next;
				}
				printf("\n");
			}
			
			temp = temp->next;
		}
	}
	
	fprintf(f1, "------------------------------------------------------------------------------------------------------------------------------------------------\n");

	fprintf(f1, "------------------------------------------------------------------------------------------------------------------------------------------------\n");

	Variable *t1 = NULL;
	fprintf(f1, "\n\nFUNCTIONS\nName");
	for(j = 0; j<14-strlen("Name"); j++) fprintf(f1, " ");
	fprintf(f1, "ReturnType");
	for(j = 0; j<14-strlen("ReturnType"); j++) fprintf(f1, " ");
	fprintf(f1, "noOfArgs");
	for(j = 0; j<14-strlen("noOfArgs"); j++) fprintf(f1, " ");
	fprintf(f1, "Args(Name, Type, Offset)\n");
	
	int k = 0;
	
	// Print Args
	for(k = 0; k < MAX_FUNCTIONS; k++)
	{
		Function *f = funcST[k];
//		if(f == NULL) continue; // REMOVE
		
		while(f != NULL)
		{		
			fprintf(f1, "%s", f->name);
			for(j = 0; j < 14-strlen(f->name); j++) fprintf(f1, " ");
			fprintf(f1, "%s", f->returnType);
			for(j = 0; j < 14-strlen(f->returnType); j++) fprintf(f1, " ");
			fprintf(f1, "%d", f->noOfArgs);
			for(j = 0; j < 14; j++) fprintf(f1, " ");
			

			t1 = f->args;
			while(t1 != NULL)
			{
				fprintf(f1, "(%s, %s, %d)\n", t1->name, t1->type, t1->offset);
				if(t1->next != NULL) { for(j = 0; j < 14*3 + 1; j++) fprintf(f1, " "); }
				t1 = t1->next;
			}
			
			fprintf(f1, "\n\nLocal Symbol Table (Name, Type, Scope, Offset, Value, lineNoDecl, lineNoUsage, arrayLen, wordLen, enumFields):\n");
			for(i = 0; i < MAX_VARIABLES; i++)
			{
				Variable *temp = f->localST[i];
				while(temp != NULL)
				{
					fprintf(f1, "(%s, %s, %d, %d, %s, %d, %d, %d, %d, ", temp->name, temp->type, temp->scope, temp->offset, temp->value, temp->lineNoDecl, temp->lineNoUsage, temp->arrayLen, temp->wordLen);
										
					if(temp->enumFields == NULL) fprintf(f1, "NULL)\n\n");
					else 
					{
						EnumData *e = temp->enumFields;
						while(e != NULL)
						{
							fprintf(f1, "%s ", e->field);
							e = e->next;
						}
						fprintf(f1, ")\n\n");
					}
					temp = temp->next;
				}
			}
			fprintf(f1, "------------------------------------------------------------------------------------------------------------------------------------------------\n");
			f = f->next;
		}
	}	
} // Else over	
} // Func over
