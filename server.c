/*
 Kyle Hippe and James Waltz
 Assignment 2
 Factoid Server
 386 Fall 2018
 10/8/18
 c gnu compiler
 textbook, stack overflow, tutorials point for c
 Server stores different types of factoids, definitions, conversions and constants, waits for request from client and applies appropriate action such as adding a new one, deleting, or returning requested factoid
 */

#include <unistd.h>
#include "file.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#define SERVER_PORT 6666
#define MAX_PENDING 5
#define MAX_STRUCT 256


int main()
{
	struct sockaddr_in sin;
	int len;
	int s, new_s;
	char *token;
	char clientInput[MAX_LINE];
	char tokenDelim[] = " \n";
	char tokenDelim2[] = "\n"; 
	FILE *fp;
	char toClient[MAX_LINE];
	int methodName = 0;
	int categoryName = 0;
	
	char *setUp = initialFile_read(fp);//reads size file 
	int convSize = 0;
	int defSize = 0;
	int contstSize = 0;
	sscanf(setUp, "%d %d %d", &convSize, &defSize, &contstSize);//converst char* to int variables
	struct Conversions conv[MAX_STRUCT];
	struct Definitions def[MAX_STRUCT];
	struct Constants contst[MAX_STRUCT];
	//initialize all the arrays 
	if(convSize != 0){
		for(int i = 0; i < convSize; i++){
			conv[i] = file_readConv(fp, i);
		}
	}//close if 
	if(defSize != 0){
		for(int i = 0; i < defSize; i++){
			def[i] = file_readDefin(fp, i);
		}
	}//close if
	if(contstSize != 0){
		for(int i = 0; i < contstSize; i++){
			contst[i] = file_readConst(fp, i);
		}
	}//close if
	
	/* build address data structure */
	
	bzero((char*)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	
	/*setup passive open*/
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("simplex-talk:socket");
		exit(1);
	}
	
	if ((bind(s, (struct sockaddr*)&sin, sizeof(sin))) < 0){
		perror("simplex-talk: bind");
		exit(1);
	}

	listen(s, MAX_PENDING);
	
	
	/* wait for connection, then receive and print text*/
	
	//set up struct arrays of each file type
	
	while(1){
		if ((new_s = accept(s, (struct sockaddr*) &sin, &len)) < 0){
			perror("simplex-talk:accept");
			exit(1);
		}
		while(len = recv(new_s, buf, sizeof(buf), 0)){
			strcpy(clientInput, buf);
			strcpy(toClient, "FACTP/0.9 500 Huh");//if something bad happens, returns this
			methodName = 0;
			categoryName = 0;
			token = strtok(clientInput, tokenDelim);
			
			if(strcmp("GET", token) == 0){
				methodName = 1;
				token = strtok(NULL, tokenDelim);
				if(strcmp("Conversion", token) == 0){
					categoryName = 1;
					int i;
					int found;
					token = strtok(NULL, tokenDelim); 
					if(strcmp(token, "FACTP/0.9") == 0){
						if(convSize != 0){
							strcpy(toClient, "FACTP/0.9 200 OK\n");//confirmation message
							token = strtok(NULL, tokenDelim);
							for( i = 0; i < convSize;i++){
								strtok(conv[i].name, tokenDelim);//removes extra whitespace
								if(strcmp(token, conv[i].name) == 0){
									found = 1;
									strcat(conv[i].name, "\n");//add back in /n that got remvoed
									strcat(toClient, conv[i].name);
									strcat(toClient, conv[i].unit1);
									strcat(toClient, conv[i].unit2);
									strcat(toClient, conv[i].factor);
								}
							}
							//if it doesnt find it change it to an error message, or if no elements
							if(!found || convSize == 0){
								strcpy(toClient, "FACTP/0.9 403 Field-Name");
							}
						}
				
					}
					else{
						strcpy(toClient, "FACTP/0.9 404 Syntax");				
					}	
				}
	
				if(strcmp("Constant", token) == 0){
					categoryName = 1;
					int i;
					int found;
					token = strtok(NULL, tokenDelim);
					if(strcmp(token, "FACTP/0.9") == 0){
						if(contstSize !=0){
							strcpy(toClient, "FACTP/0.9 200 OK\n");//confirmation message
							token = strtok(NULL, tokenDelim);
							for( i = 0; i < contstSize;i++){
								strtok(contst[i].name, tokenDelim);//removes extra whitespace
								if(strcmp(token, contst[i].name) == 0){
									found = 1;
									strcat(contst[i].name, "\n");//add back /n that was removed above
									strcat(toClient, contst[i].name);
									strcat(toClient, contst[i].value);
								}
							}
							//if it doesnt find it change it to an error message or if no elements
							if(!found || contstSize == 0){
								strcpy(toClient, "FACTP/0.9 403 Field-Name");
							}	
						}	
					}
					else{
						strcpy(toClient, "FACTP/0.9 404 Syntax");
					}		
				}
			
				if(strcmp("Definition", token) == 0){
					categoryName = 1;
					//get the constant
					int i;
					int found;
					token = strtok(NULL, tokenDelim);
					if(strcmp(token, "FACTP/0.9") == 0){
						if(defSize !=0){
							strcpy(toClient, "FACTP/0.9 200 OK\n");//confirmation
							token = strtok(NULL, tokenDelim);
							for( i = 0; i < defSize;i++){
								strtok(def[i].name, tokenDelim);//removes extra whitespace
								if(strcmp(token, def[i].name) == 0){
									found = 1;
									strcat(def[i].name, "\n");//adds back /n that was remvoed above
									strcat(toClient, def[i].name);
									strcat(toClient, def[i].part);
									strcat(toClient, def[i].definition);
								}
							}
							//if it doesnt find it change it to an error message, or if no elemetns
							if(!found || defSize == 0){
								strcpy(toClient, "FACTP/0.9 403 Field-Name");
							}
						}
					}
					else{
						strcpy(toClient, "FACTP/0.9 404 Syntax");
					}
				}			
			}
		
			if(strcmp("DELETE", token) == 0){
				methodName = 1;
				int i;
				int j = 0;
				int found;
				token = strtok(NULL, tokenDelim);
				if(strcmp("Conversion", token) == 0){
					categoryName = 1;
					struct Conversions newConvtemp[MAX_STRUCT];//temp array to store non-deleted structs
					token = strtok(NULL, tokenDelim);
					if(strcmp("FACTP/0.9", token) == 0){
						if(convSize != 0){
							token = strtok(NULL, tokenDelim);
							for( i = 0; i < convSize;i++){
								strtok(conv[i].name, tokenDelim);
								if(strcmp(token, conv[i].name) != 0){
									newConvtemp[j] = conv[i];
								}
								j++;
								if(strcmp(token, conv[i].name) == 0){//if at what is to be deleted, say found and revert j back one
									j--;
									found = 1;
								}
							}
							if(found){
								strcpy(toClient, "FACTP/0.9 202 Deleted");//confirmation
								convSize = convSize-1;//update size
								for(i = 0; i < convSize; i++){//write and append file 
									if(i == 0)
										file_writeConv(newConvtemp[i]);
									else{
										strtok(newConvtemp[i].name, tokenDelim);
										strtok(newConvtemp[i].unit1, tokenDelim);
										strtok(newConvtemp[i].unit2, tokenDelim);
										strtok(newConvtemp[i].factor, tokenDelim);
										file_appendConv(newConvtemp[i]);
									}
								}
								initialFile_write(fp, convSize, defSize, contstSize);//update size file
								for(int i = 0; i < convSize; i++){//update actual array of structs
									conv[i] = file_readConv(fp, i);
								}
							}
							else {
								strcpy(toClient, "FACTP/0.9 403 Field-Name");
							}
						}
					}
					else{
						strcpy(toClient, "FACTP/0.9 404 Syntax");
					}		
				}//last before the if
			
				if(strcmp("Constant", token) == 0){
					categoryName = 1;
					//PUT the constant
					struct Constants newConst[MAX_STRUCT];
					token = strtok(NULL, tokenDelim);
					if(strcmp("FACTP/0.9", token) == 0){
						if(contstSize != 0){
							token = strtok(NULL, tokenDelim);
							for(i = 0; i < contstSize; i++){
								strtok(contst[i].name, tokenDelim);
								if(strcmp(contst[i].name, token) != 0){
									newConst[j] = contst[i];
								}
								j++;
								if(strcmp(contst[i].name, token) == 0){
									j--;
									found = 1;
								}
							}
							if(found){
								strcpy(toClient, "FACTP/0.9 202 Deleted");
								contstSize = contstSize -1;
								for(i = 0; i < contstSize; i++){
									if(i == 0)
										file_writeConst(newConst[i]);
									else{
										strtok(newConst[i].name, tokenDelim);
										strtok(newConst[i].value, tokenDelim);
										file_appendConst(newConst[i]);
									}
								}
								initialFile_write(fp, convSize, defSize, contstSize);
								for(int i = 0; i < contstSize;i++){
									contst[i] = file_readConst(fp, i);
								}		
							}
							else{
								strcpy(toClient, "FACTP/0.9 403 Field-Name");
							}
						}
					}
					else{
						strcpy(toClient, "FACTP/0.9 404 Syntax");
					}		
				}//last before the next if
			
				if(strcmp("Definition", token) == 0){
					categoryName = 1;
					//PUT the constant
					struct Definitions newDef[MAX_STRUCT];				
					token = strtok(NULL, tokenDelim);
					if(strcmp("FACTP/0.9", token) == 0){
						if(defSize != 0){
							token = strtok(NULL, tokenDelim); //conv.name
							for(i = 0;i<defSize; i++){
								strtok(def[i].name, tokenDelim);
								if(strcmp(def[i].name, token) != 0){
									newDef[j] = def[i];
								}
								j++;
								if(strcmp(def[i].name, token) == 0){
									j--;
									found = 1;
								}
							}
							if(found){
								strcpy(toClient, "FACTP/0.9 202 Deleted");
								defSize = defSize-1;
								//do the file stuff with new array
								for(i = 0; i < defSize; i++){
									if(i == 0)
										file_writeDefin(newDef[i]);
									else{
										strtok(newDef[i].name, tokenDelim);
										strtok(newDef[i].part, tokenDelim);
										strtok(newDef[i].definition, tokenDelim);
										file_appendDefin(newDef[i]);
									}
								}
								initialFile_write(fp, convSize, defSize, contstSize);
								for(int i = 0; i < defSize;i++){
									def[i] = file_readDefin(fp, i);
								}		
							}
							else{
								strcpy(toClient, "FACTP/0.9 403 Field-Name");
							}
						}
					}
					else{
						strcpy(toClient, "FACTP/0.9 404 Syntax");
					}				
				}//last if before the close of the delete
			}
		
			if(strcmp("PUT", token) == 0){
				//put method stuff
				methodName = 1;	
				token = strtok(NULL, tokenDelim);	
				if(strcmp("Conversion", token) == 0){
					categoryName = 1;
					struct Conversions newConv;//temp struct
					//PUT the conversion
					token = strtok(NULL, tokenDelim);
					if(strcmp("FACTP/0.9", token) ==0){
						newConv.name = strtok(NULL, tokenDelim);
						newConv.unit1 = strtok(NULL, tokenDelim);
						newConv.unit2 = strtok(NULL, tokenDelim);
						newConv.factor = strtok(NULL, tokenDelim);
						strcpy(toClient, "FACTP/0.9 201 Created");//confirmation message
						//check if everything got assigned correctly
						if(newConv.name[0] == '\0' || newConv.unit1[0] == '\0' 
							|| newConv.unit2[0] == '\0' || newConv.factor[0] == '\0'){
								strcpy(toClient, "FACTP/0.9 404 Syntax");
							} 
						else{//add to file
							file_appendConv(newConv);
							convSize++;
							initialFile_write(fp, convSize, defSize, contstSize);//update size file
							for(int i = 0; i < convSize; i++){//update actual array 
								conv[i] = file_readConv(fp, i);
							}
						}	
					}
					else{
						strcpy(toClient, "FACTP/0.9 404 Syntax");
					}		
				}	
			
				if(strcmp("Constant", token) == 0){
					categoryName = 1;
					//PUT the constant
					struct Constants newConst;
					token = strtok(NULL, tokenDelim);
					if(strcmp("FACTP/0.9", token) == 0){
						newConst.name = strtok(NULL, tokenDelim);
						newConst.value = strtok(NULL, tokenDelim);
						strcpy(toClient, "FACTP/0.9 201 Created");
						//check if everything got assigned correctly
						if(newConst.name[0] == '\0' || newConst.value[0] == '\0'){
							strcpy(toClient, "FACTP/0.9 404 Syntax");				
						}
						else{//add to file
							file_appendConst(newConst);
							contstSize++;
							initialFile_write(fp, convSize, defSize, contstSize);
							for(int i = 0; i < contstSize;i++){
								contst[i] = file_readConst(fp, i);
							}		
						}
					}
					else{
						strcpy(toClient, "FACTP/0.9 404 Syntax");
					}
				}
			
				if(strcmp("Definition", token) == 0){
					categoryName = 1;
					//PUT the constant
					struct Definitions newDef;
					token = strtok(NULL, tokenDelim);
					if(strcmp("FACTP/0.9", token) == 0){
						newDef.name = strtok(NULL, tokenDelim);
						newDef.part = strtok(NULL, tokenDelim);
						newDef.definition = strtok(NULL, tokenDelim2);
						strcpy(toClient, "FACTP/0.9 201 Created");
					
						//check if everything got assigned correctly
						if(newDef.name[0] == '\0' || newDef.part[0] == '\0' ||
							newDef.definition[0] == '\0'){
							strcat(toClient, "FACTP/0.9 404 Syntax");
						}
						else{//add to file
							file_appendDefin(newDef);
							defSize++;
							initialFile_write(fp, convSize, defSize, contstSize);
							for(int i = 0; i < defSize;i++){
								def[i] = file_readDefin(fp, i);
							}		
						}
					}
					else{
						strcpy(toClient, "FACTP/0.9 404 Syntax");
					}
					
				}	
			}//end of PUT
		
			if(methodName == 0){
				strcpy(toClient, "FACTP/0.9 401 Method-Name");
			}//if close
			else if(categoryName == 0){
				strcpy(toClient, "FACTP/0.9 402 Category-Name");
			}//if close
				
			//send the stuff to the client
			toClient[MAX_LINE-1] = '\0';
			len = strlen(toClient) + 1;
			send(new_s, toClient, len, 0);	
			//reset toclient string to empty
			strcpy(toClient, "");
		}//while recv
		close (new_s);
	}//while(1)
}//main
