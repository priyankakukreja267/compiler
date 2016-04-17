#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#define MAX_LEXEME 40
#define MAX_BUFFER 10

struct _Token
{
    char lexeme[MAX_LEXEME];
    char symbol[MAX_LEXEME];
    int lineNo;
    struct _Token *next;
};
typedef struct _Token Token;

// Functions
int getNextIndex(int current, FILE *fp); // Returns the index in buffer at which next char lies
Token *getListOfTokens(char *filename); // Primary Function
void printListOfTokens(Token *t, char *filename); // Prints the output to file.
void throwError(int lineNo, char *lexeme, char *errorMsg);
char *mystrcat(char *dest, char c);


#endif // LEXER_H_INCLUDED
