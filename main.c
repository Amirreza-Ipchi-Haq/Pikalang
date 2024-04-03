#include<stdio.h>
#include"cstrcal.h"
#define NEWLINE(c) ((char)c=='\n'||(char)c=='\r')
#define WHITE(c) ((char)c==' '||(char)c=='\t'||NEWLINE(c))
char *append(char *array,size_t len,char front){
	char *newArray=malloc((len+1)*sizeof(char));
	for(size_t i=len;i--;newArray[i+front]=array[i]);
	free(array),newArray[front?0:len]=0;
	return newArray;
}
char *appendCode(char *array,size_t len,char value){
	char *newArray=malloc((len+1)*sizeof(char));
	if(len){
		for(size_t i=len;i--;newArray[i]=array[i]);
		free(array);
	}
	newArray[len]=value;
	return newArray;
}
char *discard(char *string){
	while(string[0]&&!WHITE(string[0]))
		string=strnrtmp(string,strlen(string)-1,1);
	while(string[0]&&WHITE(string[0]))
		string=strnrtmp(string,strlen(string)-1,1);
	return string;
}
char *read0(){
	char *string=strtmp("",0),c=getchar();
	while(c!='\n')
		string=strappend(string,CHR2STR(c),2),c=getchar();
	return string;
}
#ifdef _WIN32
#include<io.h>
#define isatty _isatty
#define STDIN "CON:"
#else
#include<unistd.h>
#define STDIN "/dev/tty"
#endif
int main(int argc,char** argv){
	setbuf(stdout,0);
	char *code=0,*pointer,skip=1;
	size_t lenCode=0,lenPointer=1;
	FILE *input=fopen(STDIN,"r");
	if(argc>1){
		for(size_t i=0;++i<(size_t)argc;)
			if(!cmpstr(argv[i],"--help")){
				printf("Usage: Pikalang [options] [filename]\n\nOptions:\n--do-not-skip-newline  Include the newline (and the carriage return) in the input stream (Skip by default)\n--help                 Show help\n--skip-newline         Skip the newline (and the carriage return) in the input stream (Skip by default)\n");
				if(code)
					free(code),lenCode=0;
				return 0;
			}
			else if(!cmpstr(argv[i],"--skip-newline"))
				skip=1;
			else if(!cmpstr(argv[i],"--do-not-skip-newline"))
				skip=0;
			else{
				FILE *file=fopen(argv[i],"r");//Open the file
				if(file){
					if(code)
						free(code),lenCode=0;
					char command[9];
					for(fscanf(file,"%8s",command);!feof(file);fscanf(file,"%8s",command))
						if(strlen(command)>7)
							for(char c=getc(file);!WHITE(c)&&!feof(file);c=getc(file));
						else if(!cmpstr(command,"pipi"))
							code=appendCode(code,lenCode++,0);
						else if(!cmpstr(command,"pichu"))
							code=appendCode(code,lenCode++,1);
						else if(!cmpstr(command,"pi"))
							code=appendCode(code,lenCode++,2);
						else if(!cmpstr(command,"ka"))
							code=appendCode(code,lenCode++,3);
						else if(!cmpstr(command,"pikachu"))
							code=appendCode(code,lenCode++,4);
						else if(!cmpstr(command,"pikapi"))
							code=appendCode(code,lenCode++,5);
						else if(!cmpstr(command,"pika"))
							code=appendCode(code,lenCode++,6);
						else if(!cmpstr(command,"chu"))
							code=appendCode(code,lenCode++,7);
					fclose(file);
				}
			}
	}
	pointer=malloc(sizeof(char)),pointer[0]=0;
	if(!code&&isatty(0)){//(Interactive mode)
		printf("Entered interactive-mode!\n");
		for(size_t pointerLocation=0;1;lenCode=0){//Read a line each time
			printf(">>> ");//Prompt
			char *input0=read0(),command[9];
			for(sscanf(input0,"%8s",command);input0[0];input0=discard(input0),sscanf(input0,"%8s",command)){//Read from the input
				if(strlen(command)<8){
					if(!cmpstr(command,"pipi"))
						code=appendCode(code,lenCode++,0);
					else if(!cmpstr(command,"pichu"))
						code=appendCode(code,lenCode++,1);
					else if(!cmpstr(command,"pi"))
						code=appendCode(code,lenCode++,2);
					else if(!cmpstr(command,"ka"))
						code=appendCode(code,lenCode++,3);
					else if(!cmpstr(command,"pikachu"))
						code=appendCode(code,lenCode++,4);
					else if(!cmpstr(command,"pikapi"))
						code=appendCode(code,lenCode++,5);
					else if(!cmpstr(command,"pika"))
						code=appendCode(code,lenCode++,6);
					else if(!cmpstr(command,"chu"))
						code=appendCode(code,lenCode++,7);
				}
			}
			free(input0);
			for(size_t codeLocation=0;codeLocation<lenCode;codeLocation++)//Read a part of code each time
				switch(code[codeLocation]){//Choose a command based on the code
					case 0://Push the pointer indicator forward
						if(++pointerLocation==lenPointer)//Add a new pointer to the back if needed
							pointer=append(pointer,lenPointer++,0);
						break;
					case 1://Push the pointer indicator backward
						if(!pointerLocation--)//Add a new pointer to the front if needed
							pointer=append(pointer,lenPointer++,1),pointerLocation++;
						break;
					case 2://Increase the current pointer value by 1
						pointer[pointerLocation]++;
						break;
					case 3://Decrease the currnet pointer value by 1
					pointer[pointerLocation]--;
						break;
					case 4://Output the current pointer as a character
						putchar(pointer[pointerLocation]);
						break;
					case 5://Input a character to the current pointer and skip the newline
						pointer[pointerLocation]=getc(input);
						while(skip&&NEWLINE(pointer[pointerLocation]))
							pointer[pointerLocation]=getc(input);
						break;
					case 6://Skip to a matching `]` if the current pointer value is 0
						if(!pointer[pointerLocation]){
							size_t debug=codeLocation;//Save current pointer location for debug purposes
							codeLocation++;//Increase the pointer indicator value
							for(size_t i=1;i;codeLocation++)//Skip part of the code until reaching a matching `]`
								if(codeLocation+1==lenCode&&(code[codeLocation]!=7||i>1)){//Halt if there's no matching `]`
									fprintf(stderr,"Error at locaton %u: Missing 'chu'!\n",debug);
									goto finish;
								}else if(code[codeLocation]==6)//(Nested `[`)
									i++;
								else if(code[codeLocation]==7)//(Matching `]`)
									i--;
							codeLocation--;//Decrease the pointer indicator value
						}
						break;
					case 7://Go back to a matching `]` if the current pointer value is non-zero
						if(pointer[pointerLocation]){
							size_t debug=codeLocation;//Save current pointer location for debug purposes
							codeLocation--;//Decrease the pointer indicator value
							for(size_t i=1;i;codeLocation--)//Go back until reaching a matching `[`
								if(!codeLocation&&(code[codeLocation]!=6||i>1)){//Halt if there's no matching `[`
									fprintf(stderr,"Error at locaton %u: Missing 'pika'!\n",debug);
									goto finish;
								}else if(code[codeLocation]==6)//(Nested `[`)
									i--;
								else if(code[codeLocation]==7)//(Matching `]`)
									i++;
							codeLocation++;//Increase the pointer indicator value
						}
				}
			finish:
			if(lenCode)
				free(code);
		}
	}else if(!code){//Read from a pipe
		char command[9];
		for(scanf("%8s",command);!feof(stdin);scanf("%8s",command))
			if(strlen(command)>7)
				for(char c=getchar();!WHITE(c)&&!feof(stdin);c=getchar());
			else if(!cmpstr(command,"pipi"))
				code=appendCode(code,lenCode++,0);
			else if(!cmpstr(command,"pichu"))
				code=appendCode(code,lenCode++,1);
			else if(!cmpstr(command,"pi"))
				code=appendCode(code,lenCode++,2);
			else if(!cmpstr(command,"ka"))
				code=appendCode(code,lenCode++,3);
			else if(!cmpstr(command,"pikachu"))
				code=appendCode(code,lenCode++,4);
			else if(!cmpstr(command,"pikapi"))
				code=appendCode(code,lenCode++,5);
			else if(!cmpstr(command,"pika"))
				code=appendCode(code,lenCode++,6);
			else if(!cmpstr(command,"chu"))
				code=appendCode(code,lenCode++,7);
	}
	for(size_t codeLocation=0,pointerLocation=0;codeLocation<lenCode;codeLocation++)//Read a part of code each time
		switch(code[codeLocation]){//Choose a command based on the code
			case 0://Push the pointer indicator forward
				if(++pointerLocation==lenPointer)//Add a new pointer to the back if needed
					pointer=append(pointer,lenPointer++,0);
				break;
			case 1://Push the pointer indicator backward
				if(!pointerLocation--)//Add a new pointer to the front if needed
					pointer=append(pointer,lenPointer++,1),pointerLocation++;
				break;
			case 2://Increase the current pointer value by 1
				pointer[pointerLocation]++;
				break;
			case 3://Decrease the currnet pointer value by 1
				pointer[pointerLocation]--;
				break;
			case 4://Output the current pointer as a character
				putchar(pointer[pointerLocation]);
				break;
			case 5://Input a character to the current pointer
				pointer[pointerLocation]=isatty(0)?getchar():getc(input);
				while(skip&&NEWLINE(pointer[pointerLocation]))
					pointer[pointerLocation]=isatty(0)?getchar():getc(input);
				break;
			case 6://Skip to a matching `]` if the current pointer value is 0
				if(!pointer[pointerLocation]){
					size_t debug=codeLocation;//Save current pointer location for debug purposes
					codeLocation++;//Increase the pointer indicator value
					for(size_t i=1;i;codeLocation++)//Skip part of the code until reaching a matching `]`
						if(codeLocation+1==lenCode&&(code[codeLocation]!=7||i>1)){//Halt if there's no matching `]`
							free(code),free(pointer),fprintf(stderr,"Error at locaton %u: Missing 'chu'!\n",debug);
							return 1;
						}else if(code[codeLocation]==6)//(Nested `[`)
							i++;
						else if(code[codeLocation]==7)//(Matching `]`)
							i--;
					codeLocation--;//Decrease the pointer indicator value
				}
				break;
			case 7://Go back to a matching `[` if the current pointer value is non-zero
				if(pointer[pointerLocation]){
					size_t debug=codeLocation;//Save current pointer location for debug purposes
					codeLocation--;//Decrease the pointer indicator value
					for(size_t i=1;i;codeLocation--)//Go back until reaching a matching `[`
						if(!codeLocation&&(code[codeLocation]!=6||i>1)){//Halt if there's no matching `[`
							free(code),free(pointer),fprintf(stderr,"Error at locaton %u: Missing 'pika'!\n",debug);
							return 1;
						}else if(code[codeLocation]==6)//(Matching `[`)
							i--;
						else if(code[codeLocation]==7)//(Nested `]`)
							i++;
					codeLocation++;//Increase the pointer indicator value
				}
		}
	free(code),free(pointer);
	return 0;
}
