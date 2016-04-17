#ifndef SYMBOLTABLE_H_INCLUDED
#define SYMBOLTABLE_H_INCLUDED

#define MAX_VARIABLES 50
#define MAX_FUNCTIONS 50

typedef struct _EnumData EnumData;
struct _EnumData
{
	char field[MAX_LENGTH];
	int meaning;
	EnumData *next;
};


typedef struct _Variable Variable;
struct _Variable
{
	char name[MAX_LENGTH];
	char type[MAX_LENGTH];
	char value[MAX_LENGTH]; // default value if no value has been assigned
	int scope; // Scope = 0 for Code block. Scope = 1 for Functions Block
	int offset;
	int arrayLen; // -1 if not an array
	int wordLen; // -1 if not a word
	EnumData *enumFields;
	int lineNoDecl;
	int lineNoUsage;
	Variable *next;
};


typedef struct _Function Function;
struct _Function
{
	char name[MAX_LENGTH];
	int noOfArgs;
	Variable *args;
	char returnType[MAX_LENGTH];
	Function *next;
	Variable *localST[MAX_VARIABLES]; // Hash Table to house local data
};



void initVar(Variable *globalST[]);
void initFunc(Function *funcST[]);
void buildST(Token *head, Variable *globalST[], Function *funcST[]);
void insertVar(Variable *table[], Variable *v);
void insertFunc(Function *table[], Function *f);
int areSameArgs(Function *f1, Function *f2);
int hashvar(char str[]);
int hashfunc(char str[]);
int areSameArgs(Function *f1, Function *f2);
void printST(char *filename, Variable *globalST[], Function *funcST[]);

#endif // SYMBOLTABLE_H_INCLUDED 
