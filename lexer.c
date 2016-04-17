#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"lexer.h"


// Global Variables
char buffer[MAX_BUFFER];
int current = 0;



void printListOfTokens(Token *t, char *filename) // Prints the output to file.
{
FILE *fp = fopen(filename, "w");
Token *temp = t;

fprintf(fp, "LEXEME"); 
int s1 = MAX_LEXEME - strlen("LEXEME") + 4;
while(s1 > 0) { fprintf(fp, " "); s1--; }

fprintf(fp, "SYMBOL");
int s2 = MAX_LEXEME - strlen("SYMBOL") + 4;
while(s2 > 0) { fprintf(fp, " "); s2--; }

fprintf(fp, "LINE NUMBER\n");

int c = 0;

while(temp != NULL)
{
	fprintf(fp, "%s", temp->lexeme);
	c = MAX_LEXEME - strlen(temp->lexeme) + 4;
	while(c > 0) { fprintf(fp, " "); c--; }
	
	fprintf(fp, "%s", temp->symbol);
	c = MAX_LEXEME - strlen(temp->symbol) + 4;
	while(c > 0) { fprintf(fp, " "); c--; }
	
	fprintf(fp, "%d\n", temp->lineNo);

	temp = temp->next;
}

c = 2*MAX_LEXEME + 10;
while(c > 0) { fprintf(fp, "-"); c--; }

fclose(fp);
}


void throwErrorLexer(int lineNo, char *lexeme, char *ErrorMsg)
{
printf("Lexical Error at Line Number %d in %s\n%s\n", lineNo, lexeme, ErrorMsg);
exit(0);
}


int getNextIndex(int current, FILE *fp)
{
int cnt = 0;
if(current < 0 || current >= MAX_BUFFER-1)
{
	cnt = fread(buffer, 1, MAX_BUFFER, fp);
	if(cnt < MAX_BUFFER) buffer[cnt] = EOF;
	return 0;
}
else return current+1;
}


/*
struct _Token
{
    char *lexeme;
    char *symbol;
    int lineNo;
    struct _Token *next;
};
typedef struct _Token Token;
*/


char *mystrcat(char *dest, char c)
{
char *temp = dest;
while(*temp != '\0') temp++;
*temp = c;
return dest;
}


Token *getListOfTokens(char *filename) // Primary Function
{
Token *head = NULL;
Token *prev = NULL;
Token *t = NULL;

int state = 0;
int lineNo = 1;
int stored = 0;
/*
stored = 1 if current has been stored
	   = 0 if current has NOT been stored and should now be.
*/
FILE *fp = fopen(filename, "r");
int current = -1;
current = getNextIndex(current, fp);
char c = buffer[current];
char text[MAX_LEXEME];
memset(text, '\0', MAX_LEXEME);
char fwd = c;
while(c != EOF)
{
	switch(state)
	{
		case 0:
			if(c == ' ' || c == '\t') 
			{
				memset(text, '\0', MAX_LEXEME);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
			}
			
			else if(c == '\n')
			{
				lineNo++;
				memset(text, '\0', MAX_LEXEME);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
			}

			else if(c >= '0' && c <='9')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 67;
			}
			
			else if(c == 'b')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 94;
			}
						
			else if(c == 'C')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				stored = 0;
				current = getNextIndex(current, fp);
				c = buffer[current];
				state = 6;
			}
	
			else if(c == 'D')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 11;						
			}
			
			else if(c == 'd')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 16;						
			}
			
			else if(c == 'e')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 21;						
			}
			
			else if(c == 'F')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 30;						
			}

			else if(c == 'G')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 45;						
			}
			
			else if(c == 'i')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 49;						
			}
			
			else if(c == 'J')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 53;						
			}
			
			else if(c == 'T')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 70;						
			}
						
			else if(c == 'w')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 79;						
			}
						
			else if(c == '"')
			{
				memset(text, '\0', MAX_LEXEME);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 77;						
			}
						
			else if(c == 'v')
			{	
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 87;						
			}
						
			else if(c == '_')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				if(c >= 'a' && c <= 'z')
				{
					mystrcat(text, c);
					current = getNextIndex(current, fp);
					c = buffer[current];
					state = 62;
					stored = 0;
				}
				else throwErrorLexer(lineNo, text, "Function Identifier can only have [a-z] after '_'");
			}
						
			else if (c == '/')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "DIV");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if (c == '*')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "MULT");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if (c == '+')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "PLUS");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if (c == '^')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "POW");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if (c == '@')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "RETURN");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if (c == ';')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "SEMI_COLON");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if (c == ':')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "COLON");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if (c == '(')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "PAREN_OPEN");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if (c == ')')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "PAREN_CLOSE");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if (c == '[')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "SQ_OPEN");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if (c == ']')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "SQ_CLOSE");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}
			
			else if (c == '{')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "CURLY_OPEN");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if (c == '}')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "CURLY_CLOSE");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if (c == ',')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "COMMA");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if (c == '~')
			{
				memset(text, '\0', MAX_LEXEME);				
				mystrcat(text, c);
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "ENUM_EQ");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				current = getNextIndex(current, fp);
				c = buffer[current];
				memset(text, '\0', MAX_LEXEME);
				stored = 0;
				state = 0;
			}

			else if(c == '=')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				if(c == '=')
				{
					mystrcat(text, c);
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "EQ");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					current = getNextIndex(current, fp);
					c = buffer[current];
					memset(text, '\0', MAX_LEXEME);
					stored = 0;
					state = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "ASSIGN_OP");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					stored = 0;
					state = 0;
				}
			}
			
			else if(c == '-')			
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				if(c == '>')
				{
					mystrcat(text, c);
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "ARROW");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					current = getNextIndex(current, fp);
					c = buffer[current];
					memset(text, '\0', MAX_LEXEME);
					stored = 0;
					state = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "MINUS");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					stored = 0;
					state = 0;
				}
			}
			
			else if(c == '>')			
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				if(c == '=')
				{
					mystrcat(text, c);
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "GE");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					current = getNextIndex(current, fp);
					c = buffer[current];
					memset(text, '\0', MAX_LEXEME);
					stored = 0;
					state = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "G");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					stored = 0;
					state = 0;
				}
			}

			else if(c == '<')			
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				if(c == '=')
				{
					mystrcat(text, c);
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "LE");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					current = getNextIndex(current, fp);
					c = buffer[current];
					memset(text, '\0', MAX_LEXEME);
					stored = 0;
					state = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "L");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					stored = 0;
					state = 0;
				}
			}
			
			else if(c == '!')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				if(c == '~') // Push ENUM_NEQ
				{
					mystrcat(text, c);
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "ENUM_NEQ");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);
					current = getNextIndex(current, fp);
					c = buffer[current];
					stored = 0;
					state = 0;
				}
				else if (c == '=') // Push NEQ
				{
					mystrcat(text, c);
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "NEQ");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);				
					current = getNextIndex(current, fp);
					c = buffer[current];
					stored = 0;
					state = 0;
				}
				
				else // Push NOT
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "NOT");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);
					stored = 0;
					state = 0;
				}
			
			}
			
			else if(c == '&')
			{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				if(c != '&') throwErrorLexer(lineNo, text, "Invalid syntax for AND: Use '&&'\n");
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "AND");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);
					current = getNextIndex(current, fp);
					c = buffer[current];
					stored = 0;
					state = 0;
				}
			}
			
			else if(c == '|')
			{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				if(c != '|') throwErrorLexer(lineNo, text, "Invalid syntax for OR: Use '||'\n");
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "OR");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);	
					current = getNextIndex(current, fp);
					c = buffer[current];
					stored = 0;
					state = 0;
				}
			}

			else if(c == '%')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				if(c != '%') throwErrorLexer(lineNo, text, "Invalid syntax for End Marker Symbol: Use '%%'\n");
				else
				{
					mystrcat(text, c);
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "%%");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					current = getNextIndex(current, fp);
					c = buffer[current];
					stored = 0;
					state = 0;
				}
			}
			
			else if(c == 'a' || c == 'c' || c == 'f' || c == 'g' || c == 'h' || c == 'j' || c == 'k' || c == 'l' || c == 'm' || c == 'n' || c == 'o' || c == 'p' || c == 'q' || c == 'r' || c == 's' || c == 't' || c == 'u' || c == 'x' || c == 'y' || c == 'z' || c == 'A' || c == 'B' || c == 'E' || c == 'H' || c == 'I' || c == 'K' || c == 'L' || c == 'M' || c == 'N' || c == 'O' || c == 'P' || c == 'Q' || c == 'R' || c == 'S' || c == 'U' || c == 'V' || c == 'W' || c == 'X' || c == 'Y' || c == 'Z')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 1;
			}

			else if(c == '#')
			{
				memset(text, '\0', c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				while(c != '#')
				{
					current = getNextIndex(current, fp);
					c = buffer[current];							
				}
				current = getNextIndex(current, fp);
				c = buffer[current];
				state = 0;
				stored = 0;
			}
			
			else if(c == '?')
			{
				memset(text, '\0', MAX_LEXEME);
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				stored = 0;
				state = 95; // To store ENUM_LITERAL
			}
			
			else  // if(c == '`' || c == '.' || c == '\' || c == '\' || c == '$')
			{
				mystrcat(text, c);
				throwErrorLexer(lineNo, text, "Invalid Symbol");
			}
			
			break;
		
		case 95:
			if((c < 'a' || c > 'z') && (c < 'A' || c > 'Z')) throwErrorLexer(lineNo, text, "Enum Literal must have some alphabet after '?'");
			while((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
			}
			t = (Token *) malloc(sizeof(Token));
			strcpy(t->lexeme, text);
			strcpy(t->symbol, "ENUM_LITERAL");
			t->lineNo = lineNo;
			t->next = NULL;
			if(head == NULL) { head = t; t = NULL; prev = head; }
			else { prev->next = t; prev = t; t = NULL; }
			memset(text, '\0', MAX_LEXEME);			
			mystrcat(text, c);
			stored = 0;	
			state = 0;
			break;
			
			
		case 1:
			// I have seen the atleast one character (AlphaNumeric), after that coming to ID state
			if(stored == 0 && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
			{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
			}
			else if(stored == 1)
			{
				current = getNextIndex(current, fp);
				c = buffer[current];
			}
			
			while((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
			{
				if(strlen(text) == MAX_LEXEME) throwErrorLexer(lineNo, text, "Variable Length too long.");
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
			}
			
			t = (Token *) malloc(sizeof(Token));
			strcpy(t->lexeme, text);
			strcpy(t->symbol, "ID");
			t->lineNo = lineNo;
			t->next = NULL;
			if(head == NULL) { head = t; t = NULL; prev = head; }
			else { prev->next = t; prev = t; t = NULL; }
			memset(text, '\0', MAX_LEXEME);			
			mystrcat(text, c);
			stored = 1; // CHANGED NOW!!! - 22Feb15 @6.30pm	
			state = 0;
			break;
						
		case 67:
			while(c >= '0' && c <= '9')
			{
				if(strlen(text) == MAX_LEXEME) throwErrorLexer(lineNo, text, "Integer Length too long.");
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
			}
			
			if(c == '.')
			{
				mystrcat(text, c); // Storing the decimal point.
				current = getNextIndex(current, fp);
				c = buffer[current];				
				state = 68;
			}
			
			else
			{
				t = (Token *) malloc(sizeof(Token));
				strcpy(t->lexeme, text);
				strcpy(t->symbol, "INT_LITERAL");
				t->lineNo = lineNo;
				t->next = NULL;
				if(head == NULL) { head = t; t = NULL; prev = head; }
				else { prev->next = t; prev = t; t = NULL; }
				memset(text, '\0', MAX_LEXEME);			
				mystrcat(text, c);
				stored = 1;
				state = 0;		
			}
			break;
			
		case 68:
			if(c <= '0' || c >= '9') throwErrorLexer(lineNo, text, "Insert a numeral after the decimal.");
			while(c >= '0' && c <= '9')
			{
				if(strlen(text) == MAX_LEXEME) throwErrorLexer(lineNo, text, "Decimal Number Length too long.");
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
			}
			
			t = (Token *) malloc(sizeof(Token));
			strcpy(t->lexeme, text);
			strcpy(t->symbol, "DECI_LITERAL");
			t->lineNo = lineNo;
			t->next = NULL;
			if(head == NULL) { head = t; t = NULL; prev = head; }
			else { prev->next = t; prev = t; t = NULL; }
			memset(text, '\0', MAX_LEXEME);			
			mystrcat(text, c);
			stored = 1;
			state = 0;
			break;
			
		case 77:
			while(c != '"')
			{
				if(c == '\0') throwErrorLexer(lineNo, text, "Closing Double quotes not inserted.");
				if(strlen(text) == MAX_LEXEME) throwErrorLexer(lineNo, text, "Word Length too long.");
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];			
			}
			
			t = (Token *) malloc(sizeof(Token));
			strcpy(t->lexeme, text);
			strcpy(t->symbol, "WORD_LITERAL");
			t->lineNo = lineNo;
			t->next = NULL;
			if(head == NULL) { head = t; t = NULL; prev = head; }
			else { prev->next = t; prev = t; t = NULL; }
			memset(text, '\0', MAX_LEXEME);			
			current = getNextIndex(current, fp);
			c = buffer[current];
			stored = 0;
			state = 0;
			break;
			
		case 94: // b from bool is seen
		 	if(c == 'o')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 2;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
		
		case 2:
		 	if(c == 'o')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 3;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
				
		case 3:
			if(c != 'l')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "BOOL");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 	
		case 6:
		 	if(c == 'O')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 7;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}

		case 7:
		 	if(c == 'D')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 8;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;

		case 8:
			if(c != 'E')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "CODE");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 	
		case 11:
		 	if(c == 'A')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 12;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;		
			
		case 12:
		 	if(c == 'T')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 13;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;		
			
		case 13:
			if(c != 'A')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "DATA");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 			
		case 16:
		 	if(c == 'e')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 17;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
					
		case 17:
		 	if(c == 'c')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 18;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 18:
			if(c != 'i')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "DECI");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 	
		case 21:
		 	if(c == 'n')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 22;
				stored = 0;
		 	}
		 	else if(c == 'l')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 26;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;

		case 22:
		 	if(c == 'u')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 23;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 23:
			if(c != 'm')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "ENUM");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;

		case 26:
		 	if(c == 's')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 27;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;

		case 27:
			if(c != 'e')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "ELSE");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 				

		case 30: // 'F' has been read.
		 	if(c == 'U')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 31;
				stored = 0;
		 	}

		 	else if(c == 'A')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 40;
				stored = 0;
		 	}

		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;

		case 31:
		 	if(c == 'N')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 32;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
				
				
		case 32:
			if(c != 'C')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
				if(c == 'T')
				{
					// Go to st 35
					mystrcat(text, c);
					current = getNextIndex(current, fp);
					c = buffer[current];
					stored = 0;
					state = 35;
				}
				
				else
				{
		 		mystrcat(text, c); // Pushing c
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "FUNC");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
			 	}
		 	}
		 	break;
		 						

		case 35:
		 	if(c == 'I')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 36;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 36:
		 	if(c == 'O')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 37;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 37:
			if(c != 'N')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "FUNCTION");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 	
		case 40:
		 	if(c == 'L')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 41;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 41:
		 	if(c == 'S')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 42;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 42:
			if(c != 'E')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "BOOL_LITERAL");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 	

		case 62:
			while(c >= 'a' && c <= 'z')
			{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
			}
			t = (Token *) malloc(sizeof(Token));
			strcpy(t->lexeme, text);
			strcpy(t->symbol, "FUNC_ID");
			t->lineNo = lineNo;
			t->next = NULL;
			if(head == NULL) { head = t; t = NULL; prev = head; }
			else { prev->next = t; prev = t; t = NULL; }
			memset(text, '\0', MAX_LEXEME);
			state = 0;
			mystrcat(text, c);
			stored = 1;
			break;

		case 53:
		 	if(c == 'O')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 54;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 54:
		 	if(c == 'I')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 55;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;

		case 55:
			if(c != 'N')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "JOIN");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 	
		case 45:
		 	if(c == 'i')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 46;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 46:
		 	if(c == 'v')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 47;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;

		case 47:
			if(c != 'e')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "GIVE");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 				

		case 49: // 'n' => 50, 'f' => Push IF
		 	if(c == 'n')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 50;
				stored = 0;
		 	}
		 	
		 	else if (c == 'f')
			{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "IF");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
			}
			
		 	else 
			{
				state = 1;
				stored = 0;
			}
			
			break;

		case 50:
			if(c != 't')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "INT");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 	
		case 70:
		 	if(c == 'R')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 71;
				stored = 0;
		 	}
		 	else if(c == 'a')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 74;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;

		case 71:
		 	if(c == 'U')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 72;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 72:
			if(c != 'E')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "BOOL_LITERAL");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 	
		case 74:
		 	if(c == 'k')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 75;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 75:
			if(c != 'e')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "TAKE");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 	
		 	
		case 79:
		 	if(c == 'o')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 80;
				stored = 0;
		 	}
		 	else if(c == 'h')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 83;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;

		case 80:
		 	if(c == 'r')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 81;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 81:
			if(c != 'd')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "WORD");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 	
		case 83:
		 	if(c == 'i')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 84;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 84:
		 	if(c == 'l')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 85;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 85:
			if(c != 'e')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "WHILE");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 	


		case 87:
		 	if(c == 'o')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 88;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 88:
		 	if(c == 'i')
		 	{
				mystrcat(text, c);
				current = getNextIndex(current, fp);
				c = buffer[current];
		 		state = 89;
				stored = 0;
		 	}
		 	else
		 	{
				state = 1;
		 		stored = 0;
		 	}
			break;
			
		case 89:
			if(c != 'd')
		 	{
		 		state = 1;
		 		stored = 0;
		 	}
		 		
		 	else
		 	{
		 		mystrcat(text, c); // Pushing c
				current = getNextIndex(current, fp);
				c = buffer[current];
				if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
				{
					state = 1;
					stored = 0;
				}
				else
				{
					t = (Token *) malloc(sizeof(Token));
					strcpy(t->lexeme, text);
					strcpy(t->symbol, "VOID");
					t->lineNo = lineNo;
					t->next = NULL;
					if(head == NULL) { head = t; t = NULL; prev = head; }
					else { prev->next = t; prev = t; t = NULL; }
					memset(text, '\0', MAX_LEXEME);			
					stored = 0;
					state = 0;		 			
			 	}
		 	}
		 	break;
		 	
	} // Switch ends here
} // While ends here
return head;
} // Function ends here
