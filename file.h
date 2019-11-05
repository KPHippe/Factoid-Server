/*
 Kyle Hippe and James Waltz
 Assignment 2
 Factoid Server
 386 Fall 2018
 10/8/18
 c gnu compiler
 textbook, stack overflow, tutorials point for c
 Header file for file i/o on factoid server, also stores information about structs
 */
#include <stdio.h>
#define MAX_LINE 1024
struct Conversions{//conversions factoid struct
char* name;
char* unit1;
char* unit2;
char* factor;
};
struct Definitions{//definition factoid struct
char* name;
char* part;
char* definition;
};
struct Constants{//constant factoid struct
char* name;
char* value;
};
FILE *fd;
int len;
char buf[MAX_LINE];
//call at start to intialize intial array lengths
char * initialFile_read(FILE *fp);
//call after all PUTS and DELETES, updates number of structs in each file
void initialFile_write(FILE *fp, int conv, int def, int cons);
//reads from file, uses skip to determine how many structs into file trying to access
struct Conversions file_readConv(FILE *fp, int skip);
struct Definitions file_readDefin(FILE *fp, int skip);
struct Constants file_readConst(FILE *fp, int skip);
//appends data to end of file, used for PUT requests
void file_appendConv(struct Conversions conv);
void file_appendDefin(struct Definitions def);
void file_appendConst(struct Constants cons);
//rewrites file from scratch, used for DELETE, need to remove deleted item from arrays in server before calling this
void file_writeConv(struct Conversions conv);
void file_writeDefin(struct Definitions def);
void file_writeConst(struct Constants cons);
