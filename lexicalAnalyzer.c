// Luca Gigliobianco & Federico Baron
// Lexical analyzer source code 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

#define NORW 15 // max num of reserved words
#define IMAX 5  // maximum amount of digits in the integer value 
#define CMAX 11 // maximum number of chars for idents
#define NOS 13 // number of symbols

// Global variables
token tokens[3000];
char *word[] = {"null", "begin", "call", "const", "do", "else", "end", "if", "odd", "procedure", "read", "then", "var", "while", "write"}; 
char symbols[] = {'+', '-', '*', '/', '(', ')', '=', ',' , '.', '<', '>',  ';' , ':'};
int tokenSize = 0;
char cArray[CMAX];

// Function headers
void outputLex();
void input();
int isReserved(char*, int);
token_type reservedSwitch(char*, int);
int isSymbol(char);
token_type symbolSwitch(char*, int);

// produces output for program
void outputLex()
{
    FILE* ofp = fopen("lex_output.txt", "w");
    FILE* oofp = fopen("lexlist_output.txt", "w");
    FILE* ifp = fopen("lex_input.txt", "r");
    char inp = fgetc(ifp);
    fprintf(ofp, "Source Program:\n");
    while (inp != EOF)
    {
        fprintf(ofp, "%c", inp);
        inp = fgetc(ifp);
    }
    fprintf(ofp, "\n\n");

    fclose(ifp);

    fprintf(ofp, "Lexeme Table:\n");
    fprintf(ofp, "\tLexeme\t\tToken Type\n");

    int i;
    for(i = 0; i < tokenSize; i++)
    {
        if(tokens[i].kind == numbersym)
        {
            fprintf(ofp, "%12d\t\t\t%5d\n", tokens[i].val, tokens[i].kind);
        }
        else
        {
            fprintf(ofp, "%12s\t\t\t%5d\n", tokens[i].name, tokens[i].kind);
        }
        
    }
    fprintf(ofp, "\nLexeme List:\n");
    for (i = 0; i < tokenSize; i++)
    {
        fprintf(ofp, "%d ", tokens[i].kind);
        fprintf(oofp, "%d ", tokens[i].kind);
        
        if (tokens[i].kind == 2)
        {
            fprintf(ofp, "%s ", tokens[i].name);
            fprintf(oofp, "%s ", tokens[i].name);
        }
        if (tokens[i].kind == 3)
        {
            fprintf(ofp, "%d ", tokens[i].val);
            fprintf(oofp, "%d ", tokens[i].val);
        }

    }
    fclose(ofp);
    fclose(oofp);
}

// reads input from file called file.txt and returns the number of lines that are in the file
void input()
{
    char buffer[1000];
    printf("please enter filename\n");
    scanf("%s", buffer);
    FILE* ifp = fopen(buffer, "r");

    // if file fails to open
    if (ifp == NULL)
    {
        printf("File failed to open.");
        fclose(ifp);
    }
    
    char inp;
    int numLength = 0;
    int tokenIndex = 0;
    inp = fgetc(ifp);
    // reads in character by character
    while(inp != EOF)
    {
        // ignore because it's an invisible character
        if(inp == ' ' || inp == '\t' || inp == '\n')
        {
            inp = fgetc(ifp);
            continue;
        }
        // it's a letter
        else if(isalpha(inp))
        {
            strcpy(cArray, "          ");
            int wordLength = 0;
            cArray[wordLength] = inp;
            wordLength++;
            int errorFlag = 0;

            // continue reading until there's a symbol or it's too long or there's a space
            inp = fgetc(ifp);
            while(inp != EOF)
            {
                // it has to be a letter or number
                if (inp != EOF  && (isalpha(inp) || isdigit(inp)))
                {
                    if(wordLength >= CMAX)
                    {
                        errorFlag = 1;
                        while(inp != EOF && (isalpha(inp) || isdigit(inp)))
                            inp = fgetc(ifp);
                    }
                    cArray[wordLength] = inp;
                    wordLength++;
                }
                // otherwise its a symbol or space so we gotta break 
                else
                {
                    break;
                }
                inp = fgetc(ifp);
            }

            // if it's not an error then we save it
            if(!errorFlag)
            {
                    // it can either be a reserved word
                    if(isReserved(cArray, wordLength))
                    {
                        tokens[tokenIndex].kind = reservedSwitch(cArray, wordLength);
                        strncpy(tokens[tokenIndex].name, cArray, wordLength);
                        tokenIndex++;
                    }
                    // or a variable name
                    else
                    {
                        tokens[tokenIndex].kind = identsym;
                        strncpy(tokens[tokenIndex].name, cArray, wordLength);
                        tokenIndex++;
                    }
            }
        }
        // it's a number
        else if(isdigit(inp))
        {
            int numLength = 0;
            cArray[numLength] = inp;
            numLength++;
            int errorFlag = 0;

            inp = fgetc(ifp);
            while(inp != EOF)
            {
                // if it's a digit register it
                if(isdigit(inp))
                {
                    // if the number is too long then don't take it
                    if(numLength > IMAX)
                    {
                        errorFlag = 1;
                        while(inp != EOF && isdigit(inp))
                            inp = fgetc(ifp);
                    }
                    // otherwise we keep registering it
                    else
                    {
                        cArray[numLength] = inp;
                        numLength++;
                    }
                }
                // otherwise it's a space or something else so we break
                else
                {
                    break;
                }
                inp = fgetc(ifp);  
            }

            // checks if it's a letter and if so it doesn't register the number or the following letters
            if(isalpha(inp))
            {
                errorFlag = 1;
                while(inp != EOF && isalpha(inp))
                {
                    inp = fgetc(ifp);
                }
            }
            
            // if there's no error then we save it to our tokens array
            if(!errorFlag)
            {
                tokens[tokenIndex].kind = numbersym;
                sscanf(cArray, "%d", &tokens[tokenIndex].val);
                tokenIndex++;
            }
        }
        // otherwise it is a symbol
        else
        {
            char cArray[CMAX];
            int symbolLength = 0;
            cArray[symbolLength] = inp;
            symbolLength++;
            int errorFlag = 0;
            int isComment = 0;

            inp = fgetc(ifp);
            while(inp != EOF)
            {
                // handle comments
                if(isSymbol(inp) && symbolLength == 1 && cArray[0] == '/' && inp == '*')
                {
                    isComment = 1;
                    cArray[1] = inp;
                    inp = fgetc(ifp);
                    while(inp != EOF)
                    {
                        if(inp == '*')
                        {
                            inp = fgetc(ifp);
                            if(inp == EOF || inp == '/')
                            {                                
                                inp = fgetc(ifp);
                                break;
                            }
                        }
                        inp = fgetc(ifp);
                    }
                } 
                // handle symbol
                else if(isSymbol(inp))
                {
                    cArray[symbolLength] = inp;
                    symbolLength++;
                }
                // if it's not a symbol we're done with this symbol
                else
                {
                    break;
                }
                if(isComment)
                    break;
                if(inp != EOF)
                    inp = fgetc(ifp);  
            }
            if(!isComment)
            {
                // add symbol to token array 
                tokens[tokenIndex].kind = symbolSwitch(cArray, symbolLength);
                // add symbol to word
                strncpy(tokens[tokenIndex].name, cArray, symbolLength);
                tokenIndex++;
            }
        }
    }
    tokenSize = tokenIndex;
    fclose(ifp);
}

// checks if the word is a reserved word
int isReserved(char *s, int length)
{
    int i;
    for (i = 0; i < NORW; i++)
        if (strncmp(s, word[i], strlen(word[i])) == 0)
            return 1;
    return 0;
}

token_type reservedSwitch(char *s, int length)
{
    if (strncmp(s, "null", length) == 0)
        return nulsym;
    if (strncmp(s, "begin", length) == 0)
        return beginsym;
    if (strncmp(s, "call", length) == 0)
        return callsym;
    if (strncmp(s, "const", length) == 0)
        return constsym;
    if (strncmp(s, "do", length) == 0)
        return dosym;
    if (strncmp(s, "else", length) == 0)
        return elsesym;
    if (strncmp(s, "end", length) == 0)
        return endsym;
    if (strncmp(s, "if", length) == 0)
        return ifsym;
    if (strncmp(s, "odd", length) == 0) 
        return oddsym;
    if (strncmp(s, "procedure", length) == 0)
        return procsym;
    if (strncmp(s, "read", length) == 0)
        return readsym;
    if (strncmp(s, "then", length) == 0)
        return thensym;
    if (strncmp(s, "var", length) == 0)
        return varsym;
    if (strncmp(s, "while", length) == 0)
        return whilesym;
    if (strncmp(s, "write", length) == 0)
        return writesym;
    return -1;
}

// checks if word is a symbol
int isSymbol(char c)
{
    int i;
    for(int i = 0; i < NOS; i++)
    {
        if(c == symbols[i])
            return 1;
    }
    return 0;
}

token_type symbolSwitch(char *s, int length)
{
    if (strncmp(s, "+", length) == 0)
        return plussym;
    if (strncmp(s, "-", length) == 0)
        return minussym;
    if (strncmp(s, "*", length) == 0)
        return multsym;
    if (strncmp(s, "/", length) == 0)
        return slashsym;
    if (strncmp(s, "(", length) == 0)
        return lparentsym;
    if (strncmp(s, ")", length) == 0)
        return rparentsym;
    if (strncmp(s, "=", length) == 0)
         return eqsym;
    if (strncmp(s, ",", length) == 0)
        return commasym;
    if (strncmp(s, ".", length) == 0)
        return periodsym;
    if (strncmp(s, "<", length) == 0)
        return lessym;
    if (strncmp(s, "<>", length) == 0)
         return neqsym;
    if (strncmp(s, "<=", length) == 0)
        return leqsym;
    if (strncmp(s, ">", length) == 0)
        return gtrsym;
    if (strncmp(s, ">=", length) == 0)
        return geqsym;
    if (strncmp(s, ";", length) == 0)
        return semicolonsym;    
    if (strncmp(s, ":=", length) == 0)
        return becomessym;
    return -1;
}

void main_lex(void)
{
    input();
    outputLex();
}