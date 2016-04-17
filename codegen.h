#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

char *getMemoryName(char id[], Function *curFunc, Variable *globalST[]);
void makeAsm(Node *ast, Variable *globalST[], Function *funcST[], FILE *f1);
void getCode(Node *root, FILE *f1, Variable *globalST[], Function *funcST[]);
int isArg(char id[], Function *curFunc);
int isLocal(char id[], Function *curFunc);
#endif CODE_GENERATION_H
