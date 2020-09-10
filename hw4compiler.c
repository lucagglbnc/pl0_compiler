// Luca Gigliobianco & Federico Baron
// COP 3402 Spring 2020 Parser-code-generator source code

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "header.h"

void execute(void);
void program(void);
void block(void);
void statement(void);
void condition(void);
void expression(void);
void term(void); 
void factor(void);
void getToken(void);
void enter(int type, char* name, int address, int val);
void emit(int op, int r, int l, int m);
void errorCase(int errnum);
void output_parse(void);

int num, stp = 1, level = 0, tokenIndex = 0, iIndex = 0, registerPointer = 0, numErrors = 0, numCode = 0;
int numSymbols = 0;
symbol stable[MAX_SYMBOL_TABLE_SIZE];
void execute(void)
{
    program();
    output_parse();
}

void program(void)
{
    //tokenIndex++;
    block();
    int i;
    //for (i = 0; i <= tokenIndex; i++)
      //  printf("%d ", tokens[i].kind);
    //printf("\n");
    if (numErrors > 0)
        return;
    if(tokens[tokenIndex].kind != periodsym)
    {
        errorCase(9);
    }
    emit(11, 0, 0, 3);
}

void block()
{
    int maddress = 4;
    int jmp = iIndex;
    int px;
    char nombre[CMAX];
    stp = 5;
    emit(7,0,0,0);
    if(tokens[tokenIndex].kind == constsym)
    {
        do
        {
            tokenIndex++;
            if(tokens[tokenIndex].kind != identsym)
                errorCase(4);
            strcpy(nombre, tokens[tokenIndex].name);
            //printf("1) nombre at constsym: %s . tokenIndex at this point: %d\n", tokens[tokenIndex].name, tokenIndex);
            tokenIndex++;
            //printf("2) nombre at constsym: %s . tokenIndex at this point: %d\n", tokens[tokenIndex].name, tokenIndex);
            if(tokens[tokenIndex].kind != eqsym)
            {
                errorCase(3);
            }
            tokenIndex++;

            if(tokens[tokenIndex].kind != numbersym)
            {
                errorCase(2);
            }
            //printf("3) nombre at constsym: %s . tokenIndex at this point: %d\n", tokens[tokenIndex].name, tokenIndex);
            enter(1, nombre, 0, tokens[tokenIndex].val);
            tokenIndex++;
        }while(tokens[tokenIndex].kind == commasym);

        if(tokens[tokenIndex].kind != semicolonsym)   
        {
            errorCase(5);
        }
        tokenIndex++;
    }
        

    if(tokens[tokenIndex].kind == varsym)
    {
        do
        {
            tokenIndex++;
                
            if(tokens[tokenIndex].kind != identsym)
                errorCase(4);

            strcpy(nombre, tokens[tokenIndex].name);
            enter(2, nombre, stp, 0);
            stable[numSymbols - 1].level = level; // erm (-1 in the bracket)
            maddress++;
            stp++;
            tokenIndex++;   
        } while(tokens[tokenIndex].kind == commasym);

        if(tokens[tokenIndex].kind != semicolonsym)   
        {
            errorCase(5);
        }
        tokenIndex++;
    }

    while(tokens[tokenIndex].kind == procsym)
    {
        tokenIndex++;
        if(tokens[tokenIndex].kind != identsym)
        {
            errorCase(4);
        }
        strcpy(nombre, tokens[tokenIndex].name);
        enter(3, nombre, 0, 0);
        px = numSymbols - 1; // erm (-1 in the bracket)
        stable[px].level = level;
        stable[px].addr = jmp + 1;
        tokenIndex++;   
        if(tokens[tokenIndex].kind != semicolonsym)
        {
            errorCase(5);
        }
        tokenIndex++;
        level++;
        block();

        if(tokens[tokenIndex].kind != semicolonsym)
        {
            errorCase(5);
        }    

        tokenIndex++;
    }
    CODE[jmp].m = iIndex;
    emit(6, 0, 0, maddress);
    statement();
    if (jmp != 0)
    {
        emit(2, 0, 0, 0);
        //level++;
    }
}

void statement(void)
{
    if (tokens[tokenIndex].kind == identsym)
    {
        int j, eq = 0;
		for (j = 0; j <= numSymbols; j++)
		{
            
			if(strcmp(tokens[tokenIndex].name, stable[j].name) == 0)
			{
				eq = 1;
				break;
			}
		}

        if (!eq)
        {
			errorCase(11);
        }
        tokenIndex++;

        if (tokens[tokenIndex].kind != becomessym)
            errorCase(13);
        tokenIndex++;
        expression();
        emit(4, registerPointer - 1, stable[j].level, stable[j].addr - 1);
        registerPointer--;
        
    }
    else if(tokens[tokenIndex].kind == callsym)
    {
        tokenIndex++;
        if(tokens[tokenIndex].kind != identsym)
        {
            errorCase(14);
        }
        
        int j, eq = 0;
		for (j = 0; j <= numSymbols; j++)
		{
			if(strcmp(tokens[tokenIndex].name, stable[j].name) == 0)
			{
				eq = 1;
				break;
			}
		}
        if (!eq)
			errorCase(11);
        if (stable[j].kind == 3)
        {
            emit(5, 0, stable[j].level, stable[j].addr);
        }
        else
        {
            errorCase(15);
        }
        tokenIndex++;
    }
    else if(tokens[tokenIndex].kind == beginsym)
    {
        tokenIndex++;
        statement();

        if(tokens[tokenIndex].kind != semicolonsym) // erm ermmmmmmmmmm
        {
            errorCase(10);
        }

        while(tokens[tokenIndex].kind == semicolonsym)
        {
            tokenIndex++;
            statement();
        }

        if(tokens[tokenIndex].kind != endsym)
        {
            errorCase(8);
        }
        tokenIndex++;
    }    
    else if(tokens[tokenIndex].kind == ifsym)
    {
        tokenIndex++;
        condition();
        if (tokens[tokenIndex].kind != thensym)
        {
            errorCase(16);
        }
        tokenIndex++;
        int newIndex = iIndex;
        emit(8, registerPointer - 1, 0, 0);
        
        statement();
        tokenIndex++;

        if(tokens[tokenIndex].kind == elsesym)
        {
            int newIndex2 = iIndex; 
            emit(7, 0, 0, 0);
            CODE[newIndex].m = iIndex; 
            tokenIndex++;
            statement();
            CODE[newIndex2].m = iIndex; 
            registerPointer--; 
        }
        else
        {
            CODE[newIndex].m = iIndex;
            registerPointer--;
        }
    }
    else if(tokens[tokenIndex].kind == whilesym)
    {
        int tempIndex1 = iIndex;
        int tempIndex2;
        tokenIndex++;
        condition();
        tempIndex2 = iIndex;
        emit(8, registerPointer - 1, 0, 0);
        
        if(tokens[tokenIndex].kind != dosym)
        {
            errorCase(18);
        }
        tokenIndex++;
        statement();
        emit(7, 0, 0, tempIndex1);
        
        CODE[tempIndex2].m = iIndex;
        registerPointer--;  
    }
    else if(tokens[tokenIndex].kind == readsym)
    {
        tokenIndex++;
        if(tokens[tokenIndex].kind != identsym)
            errorCase(29);
        int j, eq = 0;
		for (j = 0; j <= numSymbols; j++)
		{
            
			if(strcmp(tokens[tokenIndex].name, stable[j].name) == 0)
			{
				eq = 1;
				break;
			}
		}
        if (!eq)
        {
			errorCase(11);
        }
        emit(10, registerPointer, 0, 2);

        if(stable[numSymbols].kind == 2)
            emit(4, registerPointer, level - stable[numSymbols].level, stable[numSymbols].addr - 1);
        else if(stable[numSymbols].kind == 1)
            errorCase(12); 

        tokenIndex++; 
    }
    else if(tokens[tokenIndex].kind == writesym){
        tokenIndex++;
        if(tokens[tokenIndex].kind != identsym) 
            errorCase(29);
        int j, eq = 0;
		for (j = 0; j <= numSymbols; j++)
		{
            
			if(strcmp(tokens[tokenIndex].name, stable[j].name) == 0)
			{
				eq = 1;
				break;
			}
		}

        if (!eq)
        {
			errorCase(11);
        }

        if(stable[numSymbols].kind == 2)
        {
            emit(3, registerPointer, level - stable[numSymbols].level, stable[numSymbols].addr - 1);
            emit(9, registerPointer, 0, 1); 
        }

        else if(stable[numSymbols].kind == 1){
            emit(1, registerPointer, 0, stable[numSymbols].val );
            emit(9, registerPointer, 0, 1);
        }
        tokenIndex++;
    }
}

void condition(void)
{
    if (tokens[tokenIndex].kind == oddsym)
    {
        tokenIndex++;
        expression();
        emit(17, registerPointer - 1, registerPointer - 1, 0);
    }

    else
    {
        expression();
        
        if(tokens[tokenIndex].kind != gtrsym && tokens[tokenIndex].kind != lessym && tokens[tokenIndex].kind != neqsym && tokens[tokenIndex].kind != eqsym && tokens[tokenIndex].kind != geqsym && tokens[tokenIndex].kind != leqsym)
        {
            errorCase(20);
        }
        
        int newIndex = tokenIndex;
        tokenIndex++;
        expression();
        if (tokens[newIndex].kind == eqsym)
        {
            emit(19, registerPointer-2, registerPointer-2, registerPointer-1);
            
            registerPointer--;
        }   
        else if (tokens[newIndex].kind == neqsym)
        {
            emit(20, registerPointer-2, registerPointer-2, registerPointer-1);
            registerPointer--;
        }
        else if (tokens[newIndex].kind == lessym)
        {
            emit(21, registerPointer-2, registerPointer-2, registerPointer-1);
            ;
            registerPointer--;
        }
        else if(tokens[newIndex].kind == leqsym)
        {
            emit(22, registerPointer-2, registerPointer-2, registerPointer-1);
            registerPointer--;
        }
        else if(tokens[newIndex].kind == gtrsym)
        {
            emit(23, registerPointer-2, registerPointer-2, registerPointer-1);
            registerPointer--;
        }
        else if(tokens[newIndex].kind == geqsym)
        {
            emit(24, registerPointer-2, registerPointer-2, registerPointer-1);
            registerPointer--;
        }
    }
}

void expression(void)
{
    int temp = 0;

    if(tokens[tokenIndex].kind == lessym || tokens[tokenIndex].kind == multsym || tokens[tokenIndex].kind == eqsym || tokens[tokenIndex].kind == semicolonsym || tokens[tokenIndex].kind == commasym || tokens[tokenIndex].kind == periodsym || tokens[tokenIndex].kind == gtrsym || tokens[tokenIndex].kind == slashsym)
		errorCase(24); // erm

    if(tokens[tokenIndex].kind == plussym || tokens[tokenIndex].kind == minussym)
    {
        tokenIndex++;
        temp = tokens[tokenIndex].kind;
        term();
        if (temp == minussym)
        {
		    emit(12, registerPointer-1, registerPointer-1, 0);
        //registerPointer--; erm
        }
    }
    else
        term();

    while(tokens[tokenIndex].kind == plussym || tokens[tokenIndex].kind == minussym)
    {
        temp = tokens[tokenIndex].kind;
        tokenIndex++;
        term();
        if (temp == plussym)
        {   
            emit(13, registerPointer-2, registerPointer-2, registerPointer-1);
            registerPointer--;
        }
        else if (temp == minussym)
        {   
            emit(14, registerPointer-2, registerPointer-2, registerPointer-1);
            registerPointer--;
        }
        else // erm
        {
            errorCase(24);
        }
    }
}

void term(void)
{
    int temp = 0;
    factor();
    while (tokens[tokenIndex].kind == multsym || tokens[tokenIndex].kind == slashsym)
    {
        temp = tokens[tokenIndex].kind;
        tokenIndex++;
        factor();    
        
        if(temp == multsym)
        {
            emit(15, registerPointer - 2, registerPointer - 2, registerPointer - 1);
            registerPointer--;
        }
        else if(temp == slashsym)
        {
            emit(16, registerPointer - 2, registerPointer - 2, registerPointer - 1);
            registerPointer--;
        }
    }
}

void factor(void)
{
    if(tokens[tokenIndex].kind == identsym)
    {
        int j, eq = 0;
		for (j = 0; j <= numSymbols; j++)
		{
		    if(strcmp(tokens[tokenIndex].name, stable[j].name))
			{
				eq = 1;
				break;
			}
		}
        if (!eq)
        {
			errorCase(11);
        }

        else if(stable[j].kind == 1)
		{	
			emit(1, registerPointer, 0, stable[j].val);
		}
		else if(stable[j].kind == 2)
		{
			emit(3, registerPointer, stable[j].level, stable[j].addr - 1);
		}
        registerPointer++;
        tokenIndex++;
    }
    else if(tokens[tokenIndex].kind == numbersym)
    {
        emit(1, registerPointer++, 0, tokens[tokenIndex].val);
        tokenIndex++;
    }
    else if(tokens[tokenIndex].kind == lparentsym)
    {
        tokenIndex++;

        expression();
        
        if(tokens[tokenIndex].kind != rparentsym)
        {
            errorCase(22);
        }
        else
            errorCase(23);
        tokenIndex++;
    }
    else
    {
        errorCase(7);
    }
}

// enter a symbol into the symbol table 
void enter(int type, char* name, int address, int val)
{
    stable[numSymbols].kind = type;
    strcpy(stable[numSymbols].name, tokens[tokenIndex].name);
    stable[numSymbols].addr = address;
    stable[numSymbols].val = val;
    numSymbols++;
}

void emit(int op, int r, int l, int m)
{

    if (iIndex > MAX_CODE_LENGTH)
        errorCase(26);
    CODE[iIndex].l = l;
    CODE[iIndex].m = m;
    CODE[iIndex].op = op;
    CODE[iIndex].r = r;
    iIndex++;
    numCode++;
}

void output_parse(void)
{
    FILE *ofp = fopen("ass_output.txt", "w");
    int i;
    for (i = 0; i < numCode; i++)
    {
        fprintf(ofp, "%d\t%d\t%d\t%d\n", CODE[i].op, CODE[i].r, CODE[i].l, CODE[i].m);
    }

    fclose(ofp);
}


void errorCase(int errnum)
{
    switch(errnum)
    {
        case 1:
            printf("Error #1: Use = instead of :=.\n");
            numErrors++;
            break;
        case 2:
            printf("Error #2: = must be followed by a number.\n");
            numErrors++;
            break;
        case 3:
            printf("Error #3: Identifier must be followed by =.\n");
            numErrors++;
            break;
        case 4:
            printf("Error #4: 'const', 'var', 'procedure' must be followed by identifier.\n");
            numErrors++;
            break;
        case 5:
            printf("Error #5: Semicolon or comma missing.\n");
            numErrors++;
            break;
        case 6: 
            printf("Error #6: Incorrect symbol after procedure declaration.\n");
            numErrors++;
            break;
        case 7: 
            printf("Error #7: Statement expected.\n");
            numErrors++;
            break;
        case 8:
            printf("Error #8: Incorrect symbol after statement part in block.\n");
            numErrors++;
            break;
        case 9:
            printf("Error #9: Period expected.\n");
            numErrors++;
            break;
        case 10:
            printf("Error #10: Semicolon between statements missing.\n");
            numErrors++;
            break;
        case 11:
            printf("Error #11: Undeclared identifier.\n");
            numErrors++;
            break;
        case 12:
            printf("Error #12: Assignment to constant or procedure is not allowed.\n");
            numErrors++;
            break;
        case 13:
            printf("Error #13: Assignment operator expected.\n");
            numErrors++;
            break;
        case 14:
            printf("Error #14: 'call' must be followed by an identifier.\n");
            numErrors++;
            break;
        case 15:
            printf("Error #15: Call of a constant or variable is meaningless.\n");
            numErrors++;
            break;
        case 16:
            printf("Error #16: 'then' expected.\n");
            numErrors++;
            break;
        case 17:
            printf("Error #17: Semicolon or } expected.\n");
            numErrors++;
            break;
        case 18:
            printf("Error #18: 'do' expected.\n");
            numErrors++;
            break;
        case 19:
            printf("Error #19: Incorrect symbol following statement.\n");
            numErrors++;
            break;
        case 20:
            printf("Error #20: Relational operator expected.\n");
            numErrors++;
            break;
        case 21:
            printf("Error #21: Expression must not contain a procedure identifier.\n");
            numErrors++;
            break;
        case 22:
            printf("Error #22: Right parenthesis missing.\n");
            numErrors++;
            break;
        case 23:
            printf("Error #23: The preceding factor cannot begin with this symbol.\n");
            numErrors++;
            break;
        case 24: 
            printf("Error #24: An expression cannot begin with this symbol.\n");
            numErrors++;
            break;
        case 25:
            printf("Error #25: This number is too large.\n");
            numErrors++;
            break;
        case 26: 
            printf("Error #26: Too many instructions.\n");
            numErrors++;
            break;
        default:
            break;
    }
}
