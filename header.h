#define CMAX 11 // maximum number of chars for idents
#define MAX_LEXI_LEVELS 3

#define MAX_SYMBOL_TABLE_SIZE 500   
#define MAX_CODE_LENGTH 200

typedef enum{ nulsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym, 
            oddsym, eqsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym, 
            commasym, semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym, 
            whilesym, dosym, callsym, constsym, varsym, procsym, writesym, readsym , elsesym } token_type;

typedef struct
{ 
    token_type kind; 
    char name[CMAX];
    int val; //number 
} token; 

typedef struct      
{ 
    int kind; // const = 1, var = 2, proc = 3
    char name[CMAX]; // name up to 11 chars
    int val; // number (ASCII value)
    int level; // L level
    int addr; // M address    
} symbol;

// struct for every instruction
typedef struct
{
    int op;
    int r;
    int l;
    int m;
}instruction;

extern token tokens[3000];
extern int tokenSize;
extern symbol stable[MAX_SYMBOL_TABLE_SIZE];
extern instruction CODE[MAX_CODE_LENGTH];
extern int numErrors;
extern int numCode;

void main_p(void);
void main_lex(void);
void execute(void);
void output_parse(void);
