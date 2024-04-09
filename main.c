#include<stdio.h>
#include"cstrcal.h"
#define WHITE(c) ((char)c==' '||(char)c=='\t'||(char)c=='\n'||(char)c=='\r')
#define INPUT
#ifdef _WIN32
#include<io.h>
#define isatty _isatty
#define STDIN "CON:"
#else
#include<unistd.h>
#define STDIN "/dev/tty"
#endif
char *code,*pointer,exitCode=0;
size_t len=1,pointerLocation=0;
FILE *input;
void read0(FILE *file){
	if(code[0])
		free(code),code=strtmp("",0);
	char command[9];
	for(fscanf(file,"%8s",command);!feof(file);fscanf(file,"%8s",command))
		if(strlen(command)>7)
			for(char c=getc(file);!WHITE(c);c=getc(file));
		else if(!cmpstr(command,"pipi"))
			code=strappend(code,CHR2STR(1),2);
		else if(!cmpstr(command,"pichu"))
			code=strappend(code,CHR2STR(2),2);
		else if(!cmpstr(command,"pi"))
			code=strappend(code,CHR2STR(3),2);
		else if(!cmpstr(command,"ka"))
			code=strappend(code,CHR2STR(4),2);
		else if(!cmpstr(command,"pikachu"))
			code=strappend(code,CHR2STR(5),2);
		else if(!cmpstr(command,"pikapi"))
			code=strappend(code,CHR2STR(6),2);
		else if(!cmpstr(command,"pika"))
			code=strappend(code,CHR2STR(7),2);
		else if(!cmpstr(command,"chu"))
			code=strappend(code,CHR2STR(8),2);
	return;
}
void discard(char **str){
	while(*str[0]&&!WHITE(*str[0]))
		*str=strnrtmp(*str,strlen(*str)-1,1);
	while(*str[0]&&WHITE(*str[0]))
		*str=strnrtmp(*str,strlen(*str)-1,1);
	return;
}
void append(char front){
	pointer=realloc(pointer,len+1);
	if(front){
		for(size_t i=len++;i--;pointer[i+1]=pointer[i]);
		pointer[0]=0;
	}else
		pointer[len++]=0;
	return;
}
void execute(){
	for(size_t codeLocation=0;codeLocation<strlen(code);codeLocation++)//Read a part of code each time
		switch(code[codeLocation]){//Choose a command based on the code
			case 1://Push the pointer indicator forward
				if(++pointerLocation==len)//Add a new pointer to the back if needed
					append(0);
				break;
			case 2://Push the pointer indicator backward
				if(!pointerLocation--)//Add a new pointer to the front if needed
					append(1),pointerLocation++;
				break;
			case 3://Increase the current pointer value by 1
				pointer[pointerLocation]++;
				break;
			case 4://Decrease the currnet pointer value by 1
				pointer[pointerLocation]--;
				break;
			case 5://Output the current pointer as a character
				putchar(pointer[pointerLocation]);
				break;
			case 6://Input a character to the current pointer and skip the newline
				pointer[pointerLocation]=getc(input);
				break;
			case 7://Skip to a matching `chu` if the current pointer value is 0
				if(!pointer[pointerLocation]){
					size_t debug=codeLocation;//Save current pointer location for debug purposes
					codeLocation++;//Increase the pointer indicator value
					for(size_t i=1;i;codeLocation++)//Skip part of the code until reaching a matching `]`
						if(codeLocation+1==strlen(code)&&(code[codeLocation]!=8||i>1)){//Halt if there's no matching `]`
							exitCode=1,fprintf(stderr,"\nError at locaton %u: Missing 'chu'!\n",debug);
							goto finish;
						}else if(code[codeLocation]==7)//(Nested `[`)
							i++;
						else if(code[codeLocation]==8)//(Matching `]`)
							i--;
					codeLocation--;//Decrease the pointer indicator value
				}
				break;
			case 8://Go back to a matching `pika` if the current pointer value is non-zero
				if(pointer[pointerLocation]){
					size_t debug=codeLocation;//Save current pointer location for debug purposes
					codeLocation--;//Decrease the pointer indicator value
					for(size_t i=1;i;codeLocation--)//Go back until reaching a matching `[`
						if(!codeLocation&&(code[codeLocation]!=7||i>1)){//Halt if there's no matching `[`
							exitCode=1,fprintf(stderr,"\nError at locaton %u: Missing 'pika'!\n",debug);
							goto finish;
						}else if(code[codeLocation]==7)//(Nested `[`)
							i--;
						else if(code[codeLocation]==8)//(Matching `]`)
							i++;
					codeLocation++;//Increase the pointer indicator value
				}
		}
	finish:
	return;
}
int main(int argc,char** argv){
	setbuf(stdout,0),input=fopen(STDIN,"r"),code=strtmp("",0);
	if(argc>1){
		FILE *file=fopen(argv[1],"r");//Open the file
		if(file)
			read0(file);
		fclose(file);
	}
	pointer=malloc(sizeof(char)),pointer[0]=0;
	if(!code[0]&&isatty(0)){//(shell mode)
		printf("Entered shell-mode!");//Notify
		for(char *input0,command[9];1;free(input0),execute()){
			printf("\n>>> "),input0=strtmp("",0);
			for(char c=getchar();c!='\n';input0=strappend(input0,CHR2STR(c),2),c=getchar());
			for(sscanf(input0,"%8s",command);input0[0];discard(&input0),sscanf(input0,"%8s",command))
				if(!cmpstr(command,"pipi"))
					code=strappend(code,CHR2STR(1),2);
				else if(!cmpstr(command,"pichu"))
					code=strappend(code,CHR2STR(2),2);
				else if(!cmpstr(command,"pi"))
					code=strappend(code,CHR2STR(3),2);
				else if(!cmpstr(command,"ka"))
					code=strappend(code,CHR2STR(4),2);
				else if(!cmpstr(command,"pikachu"))
					code=strappend(code,CHR2STR(5),2);
				else if(!cmpstr(command,"pikapi"))
					code=strappend(code,CHR2STR(6),2);
				else if(!cmpstr(command,"pika"))
					code=strappend(code,CHR2STR(7),2);
				else if(!cmpstr(command,"chu"))
					code=strappend(code,CHR2STR(8),2);
		}
	}else if(!code[0])
		read0(stdin);
	execute(),free(code),free(pointer);
	return exitCode;
}
