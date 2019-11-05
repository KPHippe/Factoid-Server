/*
 Kyle Hippe and James Waltz
 Assignment 2
 Factoid Client
 386 Fall 2018
 10/8/18
 c gnu compiler
 textbook, stack overflow, tutorials point for c
 Client connects to Factoid server and requests one of three actions, a PUT, a DELETE, or a GET, and waits for confirmation from server if request was succesful, and prints any relevant information
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "file.h"
#define SERVER_PORT 6666

int main(int argc, char*argv[])
{
	FILE *fp;
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host; 
	char *token; 
	char serverInput[MAX_LINE];
	char toServer[MAX_LINE];
	char tokenBuf[MAX_LINE];
	char tokenDelim[] = " \n";
	char tokenDelim2[] = "\n";
	int s;
	int len;
	int structType; //-1 for conversion, 0 for constant, 1 for definition
	if(argc==2){
		host = argv[1];
	}
	else{
		fprintf(stderr, "usage: simplex-talk host\n");
		exit(1);
	
	}
	
	/* translate host name into peer's IP address */
	
	hp = gethostbyname(host);
	
	if(!hp){
		fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
		exit(1);
	}
	
	
	/* build address data structure*/
	
	bzero((char*)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char*)&sin.sin_addr, hp->h_length);
	sin.sin_port = htons(SERVER_PORT);
	
	/* active open */
	
	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("simplex-talk:socket");
		exit(1);
	}
	
	if(connect(s, (struct sockaddr*)&sin, sizeof(sin)) < 0)
	{
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}
	
	/*main loop get and send lines of text */
	
	while(fgets(buf, sizeof(buf), stdin)){
		strcpy(tokenBuf, buf);//w/o this, buf is changed dynamically as tokenized, leading to messed up outputs
		strcpy(toServer, buf);
		token = strtok(tokenBuf, tokenDelim);
		if(strcmp("GET", token) == 0){
			token = strtok(NULL, tokenDelim);
			if(strcmp("Conversion", token) == 0){
				strcpy(toServer, buf);
				printf("Name: ");
				fgets(buf, sizeof(buf),stdin);//Name: <conversion-name>
				strcat(toServer, buf);
				structType = -1;
			}
			else if(strcmp("Constant", token) == 0){
				strcpy(toServer, buf);
				printf("Name: ");
				fgets(buf, sizeof(buf),stdin);//Name: <constant-name>
				strcat(toServer, buf);
				structType = 0;
			}
			else if(strcmp("Definition", token) == 0){
				strcpy(toServer, buf);
				printf("Name: ");
				fgets(buf, sizeof(buf),stdin);//Name: <definition-name>
				strcat(toServer, buf);
				structType = 1;
			}
		}
		
		else if(strcmp("DELETE", token) == 0){
			token = strtok(NULL, tokenDelim);
			if(strcmp("Conversion", token) == 0){
				strcpy(toServer, buf);
				printf("Name: ");
				fgets(buf, sizeof(buf),stdin);//Name: <conversion-name>
				strcat(toServer, buf);
			}
			else if(strcmp("Constant", token) == 0){
				strcpy(toServer, buf);
				printf("Name: ");
				fgets(buf, sizeof(buf),stdin);//Name: <constant-name>
				strcat(toServer, buf);
			}
			else if(strcmp("Definition", token) == 0){
				strcpy(toServer, buf);
				printf("Name: ");
				fgets(buf, sizeof(buf),stdin);//Name: <definition-name>
				strcat(toServer, buf);
			}
		}
		else if(strcmp("PUT", token) == 0){
			token = strtok(NULL, tokenDelim);
			if(strcmp("Conversion", token) == 0){
				strcpy(toServer, buf);
				printf("Name: ");
				fgets(buf, sizeof(buf),stdin);//Name: <conversion-name>
				strcat(toServer, buf);
				printf("Unit1: ");
				fgets(buf, sizeof(buf),stdin);//Unit1: <unit-name>
				strcat(toServer, buf);
				printf("Unit2: ");
				fgets(buf, sizeof(buf),stdin);//Unit2: <unit-name>
				strcat(toServer, buf);
				printf("Factor1To2: ");
				fgets(buf, sizeof(buf),stdin);//Factor1To2: <cnumber>
				strcat(toServer, buf);
			}
			else if(strcmp("Constant", token) == 0){
				strcpy(toServer, buf);
				printf("Name: ");
				fgets(buf, sizeof(buf),stdin);//Name: <constant-name>
				strcat(toServer, buf);
				printf("Value: ");
				fgets(buf, sizeof(buf),stdin);//Value: <value>
				strcat(toServer, buf);
			}
			else if(strcmp("Definition", token) == 0){
				strcpy(toServer, buf);
				printf("Name: ");
				fgets(buf, sizeof(buf),stdin);//Name: <definiton-name>
				strcat(toServer, buf);
				printf("Part: ");
				fgets(buf, sizeof(buf),stdin);//Part: <part>
				strcat(toServer, buf);
				printf("Definition: ");
				fgets(buf, sizeof(buf),stdin);//Definition: <sentence>
				strcat(toServer, buf);
			}
		}
		toServer[MAX_LINE-1] = '\0';
		len = strlen(toServer) + 1;
		send(s, toServer, len, 0);
		
		/* accept and print portion*/
		
		recv(s, buf, sizeof(buf), 0);
		strcpy(serverInput, buf);
		token = strtok(serverInput, tokenDelim);
		if(strcmp("FACTP/0.9", token) == 0){
			token = strtok(NULL, tokenDelim);
			if(strcmp("200", token) == 0){
				fputs("GET request Suceeded\n", stdout);
				token = strtok(NULL, tokenDelim);
				if(strcmp("OK", token) == 0){
					//accept the structs
					if(structType == -1){
						struct Conversions conv;
						token = strtok(NULL, tokenDelim);
						conv.name = token;
						token = strtok(NULL, tokenDelim);
						conv.unit1 = token;
						token = strtok(NULL, tokenDelim);
						conv.unit2 = token;
						token = strtok(NULL, tokenDelim);
						conv.factor = token;
						printf("Name: %s\nUnit1: %s\nUnit2: %s\nFactor: %s\n", conv.name, conv.unit1, conv.unit2, conv.factor);
					}
					else if(structType == 0){
						struct Constants cons;
						token = strtok(NULL, tokenDelim);
						cons.name = token;
						token = strtok(NULL, tokenDelim);
						cons.value = token;
						printf("Name: %s\nValue: %s\n", cons.name, cons.value);
					}
					else if(structType == 1){
						struct Definitions def;
						token = strtok(NULL, tokenDelim);
						def.name = token;
						token = strtok(NULL, tokenDelim);
						def.part = token;
						token = strtok(NULL, tokenDelim2);
						def.definition = token;
						printf("Name: %s\nPart: %s\nDefintion: %s\n", def.name, def.part, def.definition);
					}
				}
			}
			if(strcmp("201", token) == 0){
				fputs("PUT request completed\n", stdout);
			}
			if(strcmp("202", token) == 0){
				fputs("DELETE request Suceeded\n", stdout);
			}
		
		
		
		//client errors
		
			if(strcmp("401", token) == 0){
				fputs("The name method is unknown\n", stdout);
			}
			if(strcmp("402", token) == 0){
				fputs("The name category is unknown\n", stdout);
			}
			if(strcmp("403", token) == 0){
				fputs("The name field is unknown\n", stdout);
			}
			if(strcmp("404", token) == 0){
				fputs("There is a syntax error in the request\n", stdout);
			}
		
		//server errors
		
			if(strcmp("500", token) == 0){
				fputs("The server failed to perform the request\n", stdout);
			}
		}
			
		
		
		//start to work through server messages
		
		//format output
		
	}
}
		
		
