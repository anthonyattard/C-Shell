#include "getword.h"

int getword(char *w){
static int inputsize = 0, tokens = 0, numruns = 0, displacement = 0;
int ch;
static char aux[STORAGE*2];		//if hypothetically the user inserted 255 ampersands then 255 tokens/words would exist that would have to be delimited by a null. so this counts for null
int wasDelim =1, wasSlash = 0, metaCount = 0, index = 0;

	if(!inputsize){
		while ( (ch = getchar()) != EOF && ch != '\n' && inputsize < STORAGE) {
			if(metaCount == 2 && ch != '&'){	//if this is true then << is tokenized.
				metaCount = 0;
				aux[index++] = '\0';
				tokens++;
				wasDelim = 1;
			}
			if(isspace(ch) && ch != '\n' && ch != '\t'){
				if((!wasDelim) && (!wasSlash)){
					tokens++;
					metaCount = 0;
					aux[index++] = '\0';	
					inputsize++;
					wasDelim = 1;	
				} 
				else if (wasSlash){
					wasDelim = 0;
					aux[index++] = ch;	
					inputsize++;
					wasSlash = 0;
				}	
				else inputsize++;
			}
			else if(ch == '\n'){
				if(!wasDelim){
					wasDelim = 1;
					aux[index++] = '\0';
					tokens++;	
				}
				aux[index++] = ch;
				aux[index++] = '\0';
				tokens++;
				inputsize++;
				wasSlash = 0;
			}
			else if(ch == '\\') {
				if(wasSlash){
					aux[index++] = ch;
					inputsize++;
					wasSlash = 0;
					wasDelim = 0;
				}	
				else{
					 wasSlash = 1;	//maybe ++ size
					if (metaCount){
						metaCount = 0;
						tokens++;
						aux[index++] = '\0'; 
					}
				}
			}
			else if (ch == '<' || ch == '>' || ch == '|' || ch == '#' || ch == '&'){
				if(wasSlash){
					wasDelim = 0; 
					inputsize++;
					aux[index++] = ch;	
					wasSlash = 0;
				}
				else{
					if (metaCount == 0){
						if(!wasDelim){
							aux[index++] = '\0'; 
							tokens++;	
						}
						if(ch != '>'){			//the only multi-char metachar tokens begin with '>'
							aux[index++] = ch;
							aux[index++] = '\0';
							inputsize++;
							tokens++;
						}	
						else{				//beings with '>' and may be multichar token
							aux[index++] = ch;
							inputsize++;
							metaCount++;
							wasDelim = 0;		//changed to zero j
						}
					}
					else if (metaCount == 1){
						if(ch == '&'){
							metaCount = 0;
							aux[index++] = ch;
							aux[index++] = '\0';
							tokens++;
							wasDelim = 1;		//again may not be necessary, but token was established so..
						}
						else if(ch == '>'){
							metaCount++;		//now = to two with '>>' 
							aux[index++] = ch;
							inputsize++;
						}
						else{				//if the initial '>' is followed by a metachar that is NOT 
							metaCount = 0;		//a '&' or '>' then we have two tokens
							inputsize++;
							aux[index++] = '\0';	//null term first token
							tokens+=2;		//add two to tokens (see comment above)
							aux[index++] = ch;
							aux[index++] = '\0';
							wasDelim = 1;
						}
					}
						else{			//metaCount == 2 here, and we know ch == to '&' cause of first test 
							metaCount = 0;	// at the beginning of the while loop, resets count to zero if not ampersand
							aux[index++] = ch;
							aux[index++] = '\0';
							inputsize++;
							tokens++;
							wasDelim = 1; 
						}
							
				} 
			
			}		
			else{				//else regular char
				if(metaCount){
					metaCount = 0;
					tokens++;
					aux[index++] = '\0'; 
				}
				inputsize++;
				aux[index++] = ch;
				wasDelim = 0;
				wasSlash = 0;
			}
		}
	}
	//printf("Tokens: %d\nInputsize: %d", tokens, inputsize);
	if(!wasDelim){			//handles premature EOF
		wasDelim = 1;
		aux[index++] = '\0'; 
		tokens++;
	}
	if(numruns == tokens){
		w[0] = '\0';
		return -1;
	}	
	else{
		strcpy(w, aux+displacement);     //add one for the null char
		if((strcmp(w, "logout")) == 0 && strlen(w) == 6)
			return -1;
		numruns++;
		displacement += strlen(aux+displacement) + 1;
		if(w[0] == '\n')
			w[0] = '\0';
		return strlen(w);
	}
}
