#ifndef SEMANTIC_H_INCLUDED
#define SEMANTIC_H_INCLUDED


Variable *findIdentifier(Node *root, Variable *globalST[], Function *curFunc, int curScope);
int getType(Node *root, Variable *globalST[], Function *funcST[], Function *curFunc, int curScope);
int typeToInt(char *str);
void checkSemantics(Node *root, Variable *globalST[], Function *funcST[]);
Function *getFunc(Node *root, Variable *globalST[], Function *funcST[], Function *curFunc, int curScope);

#endif
