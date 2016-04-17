#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "ast.h"


Node *buildAST(Node *root)
{
if(root == NULL) return NULL;
else
{
	int i = 0, cnt = root->noOfChildren, j = 0;

	// Process all the children first
	for(i = 0; i < cnt; i++)
	{
		root->child[i] = buildAST(root->child[i]);
		if(root->child[i] == NULL) root->noOfChildren--;
	}
	
	// Process the parent
	
	// root has no child
	if(root->noOfChildren == 0)
	{
		if(strcmp(root->symbol, "PAREN_OPEN") == 0 || strcmp(root->symbol, "PAREN_CLOSE") == 0 || strcmp(root->symbol, "SQ_OPEN") == 0 ||
		 strcmp(root->symbol, "SQ_CLOSE") == 0 || strcmp(root->symbol, "CURLY_OPEN") == 0 || strcmp(root->symbol, "CURLY_CLOSE") == 0 ||
		 strcmp(root->symbol, "SEMI_COLON") == 0 || strcmp(root->symbol, "COLON") == 0 || strcmp(root->symbol, "ARROW") == 0 ||
		 strcmp(root->symbol, "DATA") == 0 || strcmp(root->symbol, "%%") == 0 || strcmp(root->symbol, "CODE") == 0 ||
		 strcmp(root->symbol, "FUNCTION") == 0 || strcmp(root->symbol, "COMMA") == 0 || strcmp(root->symbol, "FUNC") == 0 ||
		 strcmp(root->symbol, "RETURN") == 0 || strcmp(root->symbol, "ELSE") == 0 || strcmp(root->symbol, "EPS") == 0 || 
		 strcmp(root->symbol, "JOIN") == 0 || strcmp(root->symbol, "DOLLAR") == 0)
		{
			free(root);
			return NULL;
		}
		
		else if(strcmp(root->symbol, "IF") == 0)
		{
			strcpy(root->parent->lexeme, "\0");
			strcpy(root->parent->lexeme, "IfElseStmt");
			free(root);
			return NULL;
		}
		
		else if(strcmp(root->symbol, "WHILE") == 0)
		{
			strcpy(root->parent->lexeme, "\0");
			strcpy(root->parent->lexeme, "IterativeStmt");
			free(root);
			return NULL;
		}

		else if(strcmp(root->symbol, "INT") == 0 || strcmp(root->symbol, "DECI") == 0 || strcmp(root->symbol, "BOOL") == 0 ||
		strcmp(root->symbol, "WORD") == 0 || strcmp(root->symbol, "ENUM") == 0 || strcmp(root->symbol, "INT_LITERAL") == 0 ||
		strcmp(root->symbol, "DECI_LITERAL") == 0 || strcmp(root->symbol, "BOOL_LITERAL") == 0 ||
		strcmp(root->symbol, "WORD_LITERAL") == 0 || strcmp(root->symbol, "ENUM_LITERAL") == 0 ||
		strcmp(root->symbol, "PLUS") == 0 || strcmp(root->symbol, "MINUS") == 0 || strcmp(root->symbol, "MULT") == 0 ||
		strcmp(root->symbol, "DIV") == 0 || strcmp(root->symbol, "POW") == 0 || strcmp(root->symbol, "ENUM_EQ") == 0 ||
		strcmp(root->symbol, "ENUM_NEQ") == 0 || strcmp(root->symbol, "G") == 0 || strcmp(root->symbol, "L") == 0 ||
		strcmp(root->symbol, "GE") == 0 || strcmp(root->symbol, "LE") == 0 || strcmp(root->symbol, "EQ") == 0 ||
		strcmp(root->symbol, "NEQ") == 0 || strcmp(root->symbol, "AND") == 0 || strcmp(root->symbol, "OR") == 0 ||
		strcmp(root->symbol, "NOT") == 0 || strcmp(root->symbol, "GIVE") == 0 || strcmp(root->symbol, "TAKE") == 0 ||
		strcmp(root->symbol, "ASSIGN_OP") == 0 || strcmp(root->symbol, "VOID") == 0 || strcmp(root->symbol, "FUNC_ID") == 0 ||
		strcmp(root->symbol, "ID") == 0)
		{
			return root;
		}
		
		else
		{
			free(root);
			return NULL;
		}
	}
	
	// root has one child: Unit Productions
	else if(root->noOfChildren == 1)
	{
		Node *temp = root->child[0];
		int k = 1;
		while(temp == NULL && k < MAX_RHS) temp = root->child[k++];

		if(strcmp(root->symbol, "ReturnStmt") == 0) 
		{
			root->child[0] = temp;
			return root;
		}
		else
		{
			free(root);
			return temp;
		}
	}

	// root has many children: Condense: remove all NULLs
	else if(root->noOfChildren > 1) 
	{
		cnt = 0;
		
		for(i = 0; i < MAX_RHS; i++)
		{
        	if(cnt == root->noOfChildren) break;
        	if(root->child[i] == NULL)
        	{
                for(j = i; j < MAX_RHS-1; j++) root->child[j] = root->child[j+1];
                root->child[MAX_RHS-1] = NULL;
                i--;
        	}
        	else cnt++;
		}
		return root;
	}
}
}
