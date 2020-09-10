# pl0_compiler
A compiler for the language PL/0 written in C.

1. Type 'gcc code_driver.c lexicalAnalyzer.c p-machine.c hw4compiler.c' into the terminal.
2. Type './a.out -v -a- -l' into the terminal.
3. Type the name of the file you seek to compile into the terminal. 
4. The output is printed to the terminal and also the 'all_output.txt' file

-v prints virtual machine output
-a prints generated assembly code
-l prints lexical analyzer output
./a.out prints number of errors 
