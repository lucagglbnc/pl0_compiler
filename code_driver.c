#include <stdio.h>
#include <string.h>
#include "header.h"

int main(int argc, char *argv[])
{
    const char *symbolList[] = {"", "nulsym", "identsym", "numbersym", "plussym", "minussym",
    "multsym",  "slashsym", "oddsym", "eqsym", "neqsym", "lessym", "leqsym",
    "gtrsym", "geqsym", "lparentsym", "rparentsym", "commasym", "semicolonsym",
    "periodsym", "becomessym", "beginsym", "endsym", "ifsym", "thensym",
    "whilesym", "dosym", "callsym", "constsym", "varsym", "procsym", "writesym",
    "readsym" , "elsesym"};

    FILE *ifp;
    FILE *ofp;

    int c, lex = 0, ass = 0, vir = 0;
    char** inp = argv;
    
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(inp[i], "-l") == 0)
        {
            lex = 1;
        }
        if(strcmp(inp[i], "-a") == 0)
        {
            ass = 1;
        }
        if(strcmp(inp[i], "-v") == 0)
        {
            vir = 1;
        }
    }
    main_lex();
    printf("\n\n");
    execute();
    main_p();
    ofp = fopen("all_output.txt", "w");

    if(lex == 1)
    {

        // print out lexlist
        ifp = fopen("lexlist_output.txt", "r");
        while((c = fgetc(ifp)) != EOF)
        {
            printf("%c", c);
            fprintf(ofp, "%c", c);
        }
        printf("\n");
        fprintf(ofp, "\n");
        fclose(ifp);
        ifp = fopen("lexlist_output.txt", "r");
        // print out lexlist with symbolic representation
        int i = 0;
        while((c = fgetc(ifp)) != EOF)
        {
            if(tokens[i].kind == 0)
            {
                printf("\n");
                fprintf(ofp,"\n");
                break;
            }
            printf("%s ", symbolList[tokens[i].kind]);
            fprintf(ofp, "%s ", symbolList[tokens[i].kind]);
            if (tokens[i].kind == 2)
            {
                printf("%s ", tokens[i].name);
                fprintf(ofp, "%s ", tokens[i].name);
            }
            if (tokens[i].kind == 3)
            {
                printf("%d ", tokens[i].val);
                fprintf(ofp, "%d ", tokens[i].val);
            }
            i++;
        }
        printf("\n\n");
        fprintf(ofp, "\n\n");
        fclose(ifp);
        //fclose(ofp);
    } 
    if (numErrors == 0)
    {
        printf("No errors, program is syntactically correct.\n\n");
        fprintf(ofp, "No errors, program is syntactically correct.\n\n");
    }
    else
    {
        printf("\n\n");
        fprintf(ofp, "\n\n");
    }
    
    if(ass == 1)
    {
        //ofp = fopen("all_output.txt", "w");
        ifp = fopen("ass_output.txt", "r");
        
        while((c = fgetc(ifp)) != EOF)
        {
            printf("%c", c);
            fprintf(ofp, "%c", c);
        }
        printf("\n");
        fprintf(ofp, "\n");
        
        fclose(ifp);
        //fclose(ofp);
    }
    if(vir == 1)
    {
        //ofp = fopen("all_output.txt", "w");
        ifp = fopen("v_output.txt", "r");
        while((c = fgetc(ifp)) != EOF)
        {
            printf("%c", c);
            fprintf(ofp, "%c", c);
        }
            
        printf("\n");
        fprintf(ofp, "\n");
        fclose(ifp);
        //fclose(ofp);
    }
    fclose(ofp);
    return 0;
}