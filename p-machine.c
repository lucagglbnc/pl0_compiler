// Luca Gigliobianco & Federico Baron
// PM/0 VM Source code

#include<stdio.h>
#include<stdlib.h>
#include "header.h"

// these are finals
#define MAX_STACK_HEIGHT 40
#define MAX_CODE_LENGTH 200
#define MAX_LEXI_LEVELS 3


// function skeletons
int base(int l, int base);
void fetch();
void ISA(int OP, int R, int L, int M);
int input_p();
void switchCase(int op);
void output();

// Stack size 40
int stack[MAX_STACK_HEIGHT];

// Instrction size 200s
instruction CODE[MAX_CODE_LENGTH];

// Register files array 0-7
int RF[8];
int sp = 0;
int bp = 1;
int pc = 0;
int halt;
instruction IR;

// for file input and output
FILE *ifp;
FILE *ofp;

// fetches instruction and increases pc by 1
void fetch()
{
    IR = CODE[pc];
    pc = pc + 1;
}

// executes instruction
void ISA(int OP, int R, int L, int M)
{
    // i refers to R, j refers to L, k refers to M
    switch(OP)
    {
        // LIT
        case 1:
            RF[R] = M;
            break;
        // RTN
        case 2:
            sp = bp - 1;
            bp = stack[sp + 3];
            pc = stack[sp + 4];
            break;
        // LOD
        case 3:
            RF[R] = stack[base(L, bp) + M];
            break;
        // STO
        case 4:
            stack[base(L, bp) + M] = RF[R];
            break;
        // CAL
        case 5:
            stack[sp + 1] = 0;
            stack[sp + 2] = base(L, bp);
            stack[sp + 3] = bp;
            stack[sp + 4] = pc;
            bp = sp + 1;
            pc = M;
            break;
        // INC
        case 6:
            sp = sp + M;
            break;
        // JMP
        case 7:
            pc = M;
            break;
        // JPC
        case 8:
            if(RF[R] == 0)
                pc = M;
            break;
        // SIO 1
        case 9:
            printf("%d", RF[R]);
            break;
        // SIO 2
        case 10:
            printf("Please input a value.");
            scanf("%d", &RF[R]);
            break;
        // SIO 3
        case 11:
            halt = 1;
            break;
        // NEG
        case 12:
            RF[R] = -RF[L];
            break;
        // ADD
        case 13:
            RF[R] = RF[L] + RF[M];
            break;
        // SUB
        case 14:
            RF[R] = RF[L] - RF[M];
            break;
        // MUL
        case 15:
            RF[R] = RF[L] * RF[M];
            break;
        // DIV
        case 16:
            RF[R] = RF[L] / RF[M];
            break;
        // ODD
        case 17:
            RF[R] = RF[R] % 2;
            break;
        // MOD
        case 18:
            RF[R] = RF[L] % RF[M];
            break;
        // EQL
        case 19:
            RF[R] = RF[L] == RF[M];
            break;
        // NEQ
        case 20:
            RF[R] = RF[L] != RF[M];
            break;
        // LSS
        case 21:
            RF[R] = RF[L] < RF[M];
            break;
        // LEQ
        case 22:
            RF[R] = RF[L] <= RF[M];
            break;
        // GTR
        case 23:
            RF[R] = RF[L] > RF[M];
            break;
        // GEQ
        case 24:
            RF[R] = RF[L] >= RF[M];
            break;
    }
}

// reads input from file called file.txt and returns the number of lines that are in the file
int input_p()
{
    int count = 0;
    ifp = fopen("ass_output.txt", "r");

    // if file fails to open
    if (ifp == NULL)
    {
        printf("File failed to open.");
        fclose(ifp);
    }

    // otherwise count the number of lines that will be turned into instuctions
    while (fscanf(ifp, "%d %d %d %d", &CODE[count].op, &CODE[count].r, &CODE[count].l, &CODE[count].m) != EOF)
        count = count + 1;

    fclose(ifp);

    // returns num of lines
    return count;
}

// switchCase for output
void switchCase(int op)
{
    switch(op)
    {
        // LIT
        case 1:
            fprintf(ofp, "LIT");
            break;
        // RTN
        case 2:
            fprintf(ofp, "RTN");
            break;
        // LOD
        case 3:
            fprintf(ofp, "LOD");
            break;
        // STO
        case 4:
            fprintf(ofp, "STO");
            break;
        // CAL
        case 5:
            fprintf(ofp, "CAL");
            break;
        // INC
        case 6:
            fprintf(ofp, "INC");
            break;
        // JMP
        case 7:
            fprintf(ofp, "JMP");
            break;
        // JPC
        case 8:
            fprintf(ofp, "JPC");
            break;
        // SIO 1
        case 9:
            fprintf(ofp, "SIO");
            break;
        // SIO 2
        case 10:
            fprintf(ofp, "SIO");
            break;
        // SIO 3
        case 11:
            fprintf(ofp, "SIO");
            break;
        // NEG
        case 12:
            fprintf(ofp, "NEG");
            break;
        // ADD
        case 13:
            fprintf(ofp, "ADD");
            break;
        // SUB
        case 14:
            fprintf(ofp, "SUB");
            break;
        // MUL
        case 15:
            fprintf(ofp, "MUL");
            break;
        // DIV
        case 16:
            fprintf(ofp, "DIV");
            break;
        // ODD
        case 17:
            fprintf(ofp, "ODD");
            break;
        // MOD
        case 18:
            fprintf(ofp, "MOD");
            break;
        // EQL
        case 19:
            fprintf(ofp, "EQL");
            break;
        // NEQ
        case 20:
            fprintf(ofp, "NEQ");
            break;
        // LSS
        case 21:
            fprintf(ofp, "LSS");
            break;
        // LEQ
        case 22:
            fprintf(ofp, "LEQ");
            break;
        // GTR
        case 23:
            fprintf(ofp, "GTR");
            break;
        // GEQ
        case 24:
            fprintf(ofp, "GEQ");
            break;
    }
}

void output()
{
    // count is the number of lines
    int count = input_p();
    ofp = fopen("v_output.txt", "w");
    // prints line, op, r, l and m columns
    fprintf(ofp, "Line\t\tOP\t\tR\tL\tM\n");

    // prints each row of line, op, r, l, and m
    int i;
    for(i = 0; i < count; i++)
    {
        fprintf(ofp, "%d\t\t", i);
        switchCase(CODE[i].op);
        fprintf(ofp, "\t\t%d\t%d\t%d\n", CODE[i].r, CODE[i].l, CODE[i].m);

    }

    // prints initial values
    fprintf(ofp, "\n\t\t\t\t\t\tpc\tbp\tsp\t\tregisters\n");
    fprintf(ofp, "Initial values\t\t\t\t\t%d\t%d\t%d\t\t%d %d %d %d %d %d %d %d\n", pc, bp, sp, RF[0], RF[1], RF[2], RF[3], RF[4], RF[5], RF[6], RF[7]);
    fprintf(ofp, "Stack:");
    for(i = 0; i < MAX_STACK_HEIGHT; i++)
    {
        fprintf(ofp, " %d", stack[i]);
    }
    fprintf(ofp, "\n");

    // saves previous pc value
    int prev;

    // while the program hasn't ended keep printing the values of the stack, pc, etc
    while(halt == 0)
    {
        // sets previous to be the pc value before it changes
        prev = pc;

        // calls fetch and execute ISA
        fetch();
        ISA(IR.op, IR.r, IR.l, IR.m);

        // prints the current state of RF, pc, bp, sp, etc
        fprintf(ofp, "\n\t\t\t\t\t\tpc\tbp\tsp\t\tregisters\n");
        fprintf(ofp,"%2d\t", prev);
        switchCase(IR.op);
        fprintf(ofp,"\t%d\t%d\t%d\t\t%d\t%d\t%d\t\t%d %d %d %d %d %d %d %d\n",IR.r, IR.l, IR.m, pc, bp, sp, RF[0], RF[1], RF[2], RF[3], RF[4], RF[5], RF[6], RF[7]);
        fprintf(ofp, "Stack:");
        for(i = 1; i <= sp; i++)
        {
            fprintf(ofp, " %d", stack[i]);
            if(i % 6 == 0 && i != sp)
            {
                fprintf(ofp, " |");
            }
        }
        fprintf(ofp, "\n");
    }

    fclose(ofp);
}

int base(int l, int base) // l stand for L in the instruction format
{
    int b1; //find base L levels down
    b1 = base;
    while (l > 0)
    {
        b1 = stack[b1 + 1];
        l--;
    }
    return b1;
}

void main_p(void)
{
    output();
}
