/*
 Kyle Hippe and James Waltz
 Assignment 2
 Factoid File Implementation
 386 Fall 2018
 10/8/18
 c gnu compiler
 textbook, stack overflow, tutorials point for c
 Implementation of various methods for writing and reading from files, only known bug with file i/o is that if it can't find path for given file will segmentation fault and crash
 */
#include "file.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char * initialFile_read(FILE *fp){//reads size file, used for array initialization 
	
	fp = fopen("/Users/kylehippe/Desktop/School/Year2/CS386/Factoid/numbers.txt", "r");
	fgets(buf, sizeof(buf), fp);
	char * temp = buf;
	fclose(fp);
	return temp;
}

void initialFile_write(FILE *fp, int conv, int def, int cons){//writes size file, allows for maintiance of # each factoid
	fp = fopen("/Users/kylehippe/Desktop/School/Year2/CS386/Factoid/numbers.txt", "w");
	fprintf(fp, "%d %d %d", conv, def, cons);
	fclose(fp);
}

struct Conversions file_readConv(FILE *fp, int skip){//reads conversion file
	struct Conversions conv;
	int i;
	fp = fopen("/Users/kylehippe/Desktop/School/Year2/CS386/Factoid/conversions.txt", "r");
	for(i = 0; i<skip;i++){
		fgets(buf, sizeof(buf), fp);
		fgets(buf, sizeof(buf), fp);
		fgets(buf, sizeof(buf), fp);
		fgets(buf, sizeof(buf), fp);
	}
	fgets(buf, sizeof(buf), fp);
	conv.name = (char *) malloc(MAX_LINE);
	strcpy(conv.name, buf);
	fgets(buf, sizeof(buf), fp);
	conv.unit1 = (char *) malloc(MAX_LINE);
	strcpy(conv.unit1, buf);
	fgets(buf, sizeof(buf), fp);
	conv.unit2 = (char *) malloc(MAX_LINE);
	strcpy(conv.unit2, buf);
	fgets(buf, sizeof(buf), fp);
	conv.factor = (char *) malloc(MAX_LINE);
	strcpy(conv.factor, buf);
	fclose(fp);
	
	return conv;
}

struct Definitions file_readDefin(FILE *fp, int skip){//reads definition file
	struct Definitions defin;
	int i;
	fp = fopen("/Users/kylehippe/Desktop/School/Year2/CS386/Factoid/definitions.txt", "r");
	for(i = 0; i<skip;i++){
		fgets(buf, sizeof(buf), fp);
		fgets(buf, sizeof(buf), fp);
		fgets(buf, sizeof(buf), fp);
	}
	fgets(buf, sizeof(buf), fp);
   defin.name = (char *)malloc(MAX_LINE);
   strcpy(defin.name, buf);
	fgets(buf, sizeof(buf), fp);
	defin.part = (char *)malloc(MAX_LINE);
   strcpy(defin.part, buf);
	fgets(buf, sizeof(buf), fp);
	defin.definition = (char *)malloc(MAX_LINE);
   strcpy(defin.definition, buf);
	fclose(fp);
	return defin;
}

struct Constants file_readConst(FILE *fp, int skip){//reads constants file
	struct Constants cons;
	int i;
	fp = fopen("/Users/kylehippe/Desktop/School/Year2/CS386/Factoid/constants.txt", "r");
	for( i = 0; i<skip;i++){
		fgets(buf, sizeof(buf), fp);
		fgets(buf, sizeof(buf), fp);
	}
	fgets(buf, sizeof(buf), fp);
   cons.name = (char *) malloc(MAX_LINE);
   strcpy(cons.name, buf);
   fgets(buf, sizeof(buf), fp);
	cons.value = (char *) malloc(MAX_LINE);
   strcpy(cons.value, buf);
	fclose(fp);
	return cons;
}
void file_appendConv(struct Conversions conv){//adds data to end of conversions file
	fd = fopen("/Users/kylehippe/Desktop/School/Year2/CS386/Factoid/conversions.txt", "a");
	fprintf(fd, "%s\n", conv.name);
	fprintf(fd, "%s\n", conv.unit1);
	fprintf(fd, "%s\n", conv.unit2);
	fprintf(fd, "%s\n", conv.factor);
	fclose(fd);
}

void file_appendDefin(struct Definitions def){//adds data to end of definitions file
	fd = fopen("/Users/kylehippe/Desktop/School/Year2/CS386/Factoid/definitions.txt", "a");
	fprintf(fd, "%s\n", def.name);
	fprintf(fd, "%s\n", def.part);
	fprintf(fd, "%s\n", def.definition);
	fclose(fd);
}

void file_appendConst(struct Constants cons){//adds data to end of constants file
	fd = fopen("/Users/kylehippe/Desktop/School/Year2/CS386/Factoid/constants.txt", "a");
	fprintf(fd, "%s\n", cons.name);
	fprintf(fd, "%s\n", cons.value);
	fclose(fd);
}

void file_writeConv(struct Conversions conv){//overwrites data stored in conversions file
	fd = fopen("/Users/kylehippe/Desktop/School/Year2/CS386/Factoid/conversions.txt", "w");
	fprintf(fd, "%s\n", conv.name);
	fprintf(fd, "%s", conv.unit1);
	fprintf(fd, "%s", conv.unit2);
	fprintf(fd, "%s", conv.factor);
	fclose(fd);
}

void file_writeDefin(struct Definitions def){//overwrites data stored in definitions file
	fd = fopen("/Users/kylehippe/Desktop/School/Year2/CS386/Factoid/definitions.txt", "w");
	fprintf(fd, "%s\n", def.name);
	fprintf(fd, "%s", def.part);
	fprintf(fd, "%s", def.definition);
	fclose(fd);
}

void file_writeConst(struct Constants cons){//overwrites data stored in constants file
	fd = fopen("/Users/kylehippe/Desktop/School/Year2/CS386/Factoid/constants.txt", "w");
	fprintf(fd, "%s \n", cons.name);
	fprintf(fd, "%s", cons.value);
	fclose(fd);
}
