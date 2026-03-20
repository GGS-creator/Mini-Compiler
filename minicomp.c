//Lexical analyzer done
//Syntax analyzer done
//Assigning numbers/characters to variables
//Loops
//Convertion of high level to low level
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#define MAX_TOKENS 100
#define MAX_INPUT_LENGTH 1000
#define MAX_TOKEN_LENGTH 100
enum tokentype{//an enum to define the types of tokens
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_NUMBER,
    TOKEN_UNKNOWN,
    TOKEN_EXTRA,
    TOKEN_STRING,
};
struct token{
    enum tokentype type;
    char value[MAX_TOKEN_LENGTH];
};
bool isString(char *str) {
    return str != NULL && str[0] != '\0';
}
const char *keywords[]={"compute","print","aboutpro","num","dnum","let","exit"};
#define NUM_KEYWORD (sizeof(keywords)/sizeof(keywords[0]))//number of keywords
int iskey(const char *str){//checks if the string is a keyword}
    for(int i=0;i<NUM_KEYWORD;i++){
        if(strcmp(str,keywords[i])==0){
            return 1;
        }
    }
    return 0;
}
int isop(char ch){//signs
    return (ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='%' || ch=='#');
}
/*
the lexer takes the input, removes white spaces,adds it to buffer,assignss the type of token, and adds it to the tokens array.
*/
void lexer(const char *input, struct token tokens[],int *tokencount)//skips white spaces, identifies num, identifies keywords, identifies operators
{
    int i=0;
    int j=0;
    char buffer[MAX_TOKEN_LENGTH];
    while(input[i]!='\0'){
        if(isspace(input[i])){// skips white spaces
            i++;
            continue;
        }
        if(isdigit(input[i])){//checks if the character is a number
            j=0;
            while(isdigit(input[i])){//adds it to buffer
                buffer[j++]=input[i++];
            }
            buffer[j]='\0';//converts string to number
            tokens[*tokencount].type=TOKEN_NUMBER;//sets the type to number
            strcpy(tokens[*tokencount].value,buffer);//moves buffer to tokens.value
            (*tokencount)++;
            continue;
        }
        if(isalpha(input[i])){//does the same thing for letters
            j=0;
            while(isalnum(input[i])){
                buffer[j++]=input[i++];
            }
            buffer[j]='\0';
            if(iskey(buffer)){//checks if the string is a keyword
                tokens[*tokencount].type=TOKEN_KEYWORD;
                if(strcmp(tokens[0].value,"print")==0 && isString(tokens[1].value)){
                    tokens[*tokencount].type=TOKEN_STRING;
                }
                
            }
            
            else{//if it is an identifer 
                tokens[*tokencount].type=TOKEN_IDENTIFIER;
            }
            strcpy(tokens[*tokencount].value,buffer);
            (*tokencount)++;
            continue;
            }
        if(isalpha(input[i])){
            int j=0;
            while(isalnum(input[i])){
                buffer[j++]=input[i++];
            }
            buffer[j]='\0';
            if(iskey(buffer)){
                tokens[*tokencount].type=TOKEN_EXTRA;
            }
            strcpy(tokens[*tokencount].value,buffer);
            (*tokencount)++;
            continue;
        }
        
        if(isop(input[i])){//checks if the character is an operator
            tokens[*tokencount].type=TOKEN_OPERATOR;//assigns type
            tokens[*tokencount].value[0]=input[i];//adds it to value
            tokens[*tokencount].value[1]='\0';
            (*tokencount)++;
            i++;
            continue;
        }
        tokens[*tokencount].type=TOKEN_UNKNOWN;
        tokens[*tokencount].value[0]=input[i];
        tokens[*tokencount].value[1]='\0';
        (*tokencount)++;
        i++;
    }
}

int compute(int num1,int num2,char op){
    switch(op){//takes in operand and operatir and performs operation
        case '+': return num1+num2;
        case '-': return num1-num2;
        case '*': return num1*num2;
        case '/': return num1/num2;
        case '%': return num1%num2;
        case '#': return 80085;
        default: printf("Invalid operator\n"); return 0;
    }
}
void printmul(const char *str,int n){
    for(int i=0;i<n;i++){
    printf("%s\n",str);
    }
}
int isvalassigment(struct token tokens[]){
    if (strcmp(tokens[1].value,"=")==0 && tokens[2].type==TOKEN_NUMBER){
        return 1;
    }
    else {
        return 0;
    }

}
int isvalidexpression(struct token tokens[],int tokencount){
    
    if((tokens[0].type==TOKEN_KEYWORD)&& (tokens[1].type==TOKEN_NUMBER||tokens[1].type==TOKEN_IDENTIFIER)){
        return 1;
    }
    else{
        return 0;
    }
}
int isvalidprint(struct token tokens[]){
    if(strcmp(tokens[0].value,"print")==0 && (tokens[1].type==TOKEN_IDENTIFIER||tokens[1].type==TOKEN_STRING))
        return 1;
    
    else
        return 0;
}
int syntaxanalyzer(struct token tokens[],int tokencount){
    int i=tokencount;
    //while(i!=0){
    if(isvalassigment(tokens) == 1 || isvalidexpression(tokens,tokencount) == 1 || isvalidprint(tokens) == 1){
        //printf("%d",isvalidprint(tokens));
        return 0;
        
    }
    else{
        return 1;
        
    }
    //i--;
}
//}
int executecode(struct token tokens[],int tokencount){
    if(tokencount==4 && strcmp(tokens[0].value,"compute")==0){
        int num1=atoi(tokens[1].value);//atoi converts string tokens.value to integer
        int num2=atoi(tokens[3].value);
        char op=tokens[2].value[0];
        printf("%d\n",compute(num1,num2,op));
    }
    else if(/*tokenCount==3 && */strcmp(tokens[0].value,"print")==0){
        int n=atoi(tokens[tokencount-1].value);
        int diff=tokencount-1;
        char strdiff[100]="";
        int tc;
        for(tc=1;tc<diff;tc++){
            strncat(strdiff,tokens[tc].value,sizeof(strdiff)-strlen(strdiff)-1);//concatenates strings into strdiff which is between print and number
            strncat(strdiff," ",sizeof(strdiff)-strlen(strdiff)-1);            
            
            
        }
        printmul(strdiff,n);
    }
    else if(strcmp(tokens[0].value,"aboutpro")==0){
        printf("Hi this is gg.\t:) \nThis is a basic compiler project for my Automata and compiler design class.\nI made this for fun so that i could better understand the subject and build cool stuff.\nHope this was helpful.\nByeeee\n");
    }
    else{
        printf("Syntax error\n");
    }
}
void exmullines(const char *code){
    struct token tokens[MAX_TOKENS];
    int tokencount=0;
    char codeCopy[1000];
    strncpy(codeCopy, code, sizeof(codeCopy) - 1);
    codeCopy[sizeof(codeCopy) - 1] = '\0';
    char *line = strtok(codeCopy, "\n");
    while(line!=NULL){
        lexer(line,tokens,&tokencount);
        executecode(tokens,tokencount);
        syntaxanalyzer(tokens,tokencount);
        tokencount=0;
        line=strtok(NULL,"\n");
    }
}
int main(){
    char code[100];
    
    printf("Enter a command or exit to quit:");
    while(1){
        printf("> ");
        fgets(code,sizeof(code),stdin);
        if(strncmp(code,"exit",4)==0){
            printf("Exiting...\n");
            break;
        }
        exmullines(code);
    }
    return 0;
}

