//Lexical analyzer done
//Syntax analyzer done
//Assigning numbers(DONEEEE)/characters to variables(DONEEEEE YEPPYEEEEE)
//Advanced math operations (DONEEEYEAPPYEEE)
//Loops (DONEEEEYEAPPYYEEE)
//Convertion of high level to low level

//move identifiers in executecode into seperate functions
//use logic to execute while loop
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<ctype.h>
#define MAX_TOKENS 100
#define MAX_INPUT_LENGTH 1000
#define MAX_TOKEN_LENGTH 100
#define NUMSTACK 100
#define VARSTACK 100
char numstack[NUMSTACK];
int symbols[NUMSTACK];
char finalexp[NUMSTACK];
int top=-1;
int topfinal=-1;
enum tokentype{//an enum to define the types of tokens
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_NUMBER,
    TOKEN_UNKNOWN,
    TOKEN_INTVAR,
    TOKEN_FLOVAR,
    TOKEN_STRVAR,
    TOKEN_STRING,
    TOKEN_EXTRA,
};
struct vartable{
    struct vartable *next;
    char name[MAX_TOKEN_LENGTH];
    int val;
    int tnum;
};
struct strvartable{
    struct strvartable *snext;
    char sname[MAX_TOKEN_LENGTH];
    char sval[10000];
    int strtnum;
};
struct token{
    enum tokentype type;
    char value[MAX_TOKEN_LENGTH];
};
typedef struct Node{
    char op;
    char value[100];
    struct Node* left;
    struct Node* right;
}Node;
int findvar(const char *ch,struct vartable vartable);
int sfindvar(const char *ch,struct strvartable strvartable);
int intinfixtopostfix(char *exp);
bool isString(char *str) {
    return str != NULL && str[0] != '\0';
}
int isvalidvar(const char *ch){
    int i=0;
    int j=0;
    while(ch[j]!='\0'){
        i=i+1;
    }
    if(isalpha(ch[0])){
        return 0;
    }
    for(int n=i;n>0;n++){
        if(isalnum(ch[i])){
            return 0;
        }
    }
    return 1;
}



const char *assign[]={"num","let"};
const char *keywords[]={"compute","print","aboutpro","rep","exit"};
int numvarl;

int charvarl;
struct vartable *vart,*temp,*head=NULL;
struct strvartable *sart,*stemp,*shead=NULL;
#define NUM_KEYWORD (sizeof(keywords)/sizeof(keywords[0]))//number of keywords
#define NUM_ASSIGN (sizeof(assign)/sizeof(assign[0]))
int isassign(const char *str){//checks if the string is an assignment operator}
    for(int i=0;i<NUM_ASSIGN;i++)
    {
        if(strcmp(str,assign[i])==0)
        {
            return 1;
        }
       }
       return 0;
}
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
int priority(char x){
    if(x=='(')
        return 0;
    if(x=='+'||x=='-')
        return 1;
    if(x=='*'||x=='/')
        return 2;
    if(x=='$'||x=='^')
        return 3;
    return 0;
}

char opStack[100];      // Stack for operators
int ansStack[100];      // Stack for operan s (answer stack)
int topOp = -1;         // Stack pointer for operators
int topAns = -1;        // Stack pointer for operands

// Function prototypes
void pushOp(char x);
void compute(int s,struct token tokens[],int tokencount);
void printn(int s,struct token tokens[],int tokencount);
void print_text(int s,struct token tokens[],int tokencount);
void print_nor(int s,struct token tokens[],int tokencount);
void let(struct token tokens[],int tokencount);
void num(struct token tokens[],int tokencount);
char popOp();
void pushAns(int x);
int popAns();
int priority(char x);

int intinfixtopostfix(char *expr){
    char exp[100], *e, x;
    strcpy(exp, expr);
    printf("Expression: %s\n",expr, exp);
    e = exp;  
    while (*e != '\0') {
        if (isspace(*e)) {
            e++;
            continue;
        }
        if (isdigit(*e)) {
            int num = 0;
            // Handle multi-digit numbers
            while (isdigit(*e)) {
                num = num * 10 + (*e - '0');
                e++;
            }
            e--;
            pushAns(num);
        } else if (*e == '(') {
            pushOp(*e);
        } else if (*e == ')') {
            while (topOp != -1 && opStack[topOp] != '(') {
                char op = popOp();
                int b = popAns();
                int a = popAns();
                int result;

                switch (op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/': result = a / b; break;
                }
                pushAns(result);
            }
            popOp(); // Pop the '('
        } else {
            while (topOp != -1 && priority(opStack[topOp]) >= priority(*e)) {
                char op = popOp();
                int b = popAns();
                int a = popAns();
                int result;

                switch (op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/': result = a / b; break;
                }
                pushAns(result);
            }
            pushOp(*e);
        }
        e++;
    }

    // Final pop of remaining operators
    while (topOp != -1) {
        char op = popOp();
        int b = popAns();
        int a = popAns();
        int result;

        switch (op) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/': result = a / b; break;
        }
        pushAns(result);
    }

    int ans = popAns();
    return ans;
}
void pushOp(char x) {
    if (topOp >= 99) {
        printf("Operator stack overflow\n");
        return;
    }
    opStack[++topOp] = x;
}
char popOp() {
    if (topOp == -1) {
        printf("Operator stack underflow\n");
        return '\0';
    }
    return opStack[topOp--];
}
void pushAns(int x) {
    if (topAns >= 99) {
        printf("Answer stack overflow\n");
        return;
    }
    ansStack[++topAns] = x;
}
int popAns() {
    if (topAns == -1) {
        printf("Answer stack underflow\n");
        return 0;
    }
    return ansStack[topAns--];
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

// int compute(int num1,int num2,char op){
//     switch(op){//takes in operand and operatir and performs operation
//         case '+': return num1+num2;
//         case '-': return num1-num2;
//         case '*': return num1*num2;
//         case '/': return num1/num2;
//         case '%': return num1%num2;
//         case '#': return 80085;
//         default: printf("Invalid operator\n"); return 0;
//     }
// }
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

void compute(int s,struct token tokens[],int tokencount){
    if(tokencount<4){
        printf("Insufficient values");
    }
    int diff=tokencount;
    char strdiff[1000]="";
    int tc;
    int ttop=tokencount;
    for(tc=s;tc<diff;tc++){
        strncat(strdiff,tokens[tc].value,sizeof(strdiff)-strlen(strdiff)-1);//concatenates strings into strdiff which is between print and number
        strncat(strdiff," ",sizeof(strdiff)-strlen(strdiff)-1);            
    }
    //strdiff[strlen(strdiff)]='\0';
    int ans;
    printf("%d here:%s\n",tokencount,strdiff);
    ans=intinfixtopostfix(strdiff);
    printf("%d\n",ans);
}
void printn(int s,struct token tokens[],int tokencount){
    int n=atoi(tokens[tokencount-1].value);
        int diff=tokencount-1;
        char strdiff[1000]="";
        int tc;
        for(tc=s;tc<diff;tc++){
            strncat(strdiff,tokens[tc].value,sizeof(strdiff)-strlen(strdiff)-1);//concatenates strings into strdiff which is between print and number
            strncat(strdiff," ",sizeof(strdiff)-strlen(strdiff)-1);            
        }
        printmul(strdiff,n);
        
        /*
        if(isdigit(tokencount-1)){
            printmul(strdiff,n);}
        else{
            printmul(strdiff,1);
        }*/
}
void print_text(int s,struct token tokens[],int tokencount){
    int m=atoi(tokens[tokencount-1].value);
        int diff=tokencount-1;
        char vardiff[10000]="";
        int tc=s;
        strcpy(vardiff,tokens[s].value);
        if(sfindvar(vardiff,*sart)==1){
            stemp=shead;
            while(stemp!=NULL){
                if(strcmp(vardiff,stemp->sname)==0){
                    printf("%s\n",stemp->sval);
                    break;
                }
                else{
                    stemp=stemp->snext;
                }
            }
            
        }
}
void print_nor(int s,struct token tokens[],int tokencount){
    int m=atoi(tokens[tokencount-1].value);
            int diff=tokencount-1;
            char vardiff[1000]="";
            int tc=s;
            strcpy(vardiff,tokens[s].value);
            if(findvar(vardiff,*vart)==1){
                temp=head;
                
                while(temp!=NULL){
                    
                    if(strcmp(vardiff,temp->name)==0){
                        printf("%d\n",temp->val);
                        break;
                    }
                    else{
                        temp=temp->next;
                    }
                }
                
            }
}
void let(struct token tokens[],int tokencount){
    sart=(struct strvartable*)malloc(sizeof(struct strvartable));
                strcpy(sart->sname,tokens[1].value);
                //int n=atoi(tokens[tokencount-1].value);
                int diff=tokencount;
                char strdiff[1000]="";
                for(int tc=3;tc<diff;tc++){
                strncat(strdiff,tokens[tc].value,sizeof(strdiff)-strlen(strdiff)-1);//concatenates strings into strdiff which is between print and number
                strncat(strdiff," ",sizeof(strdiff)-strlen(strdiff)-1);            
                }
                strcpy(sart->sval,strdiff);
                sart->strtnum++;
                charvarl=sart->strtnum;
                sart->snext=NULL;
                if(shead==NULL){
                    shead=stemp=sart;
                }
                else{
                    stemp->snext=sart;
                    stemp=stemp->snext;
                }
}
void num(struct token tokens[],int tokencount){
    vart=(struct vartable*)malloc(sizeof(struct vartable));
                strcpy(vart->name,tokens[1].value);
                vart->val=atoi(tokens[3].value);
                vart->tnum++;
                numvarl=vart->tnum;
                vart->next=NULL;
                if(head==NULL){
                    head=temp=vart;
                }
                else{
                    temp->next=vart;
                    temp=vart;
                }
}

Node* stack[100];
//int top = -1;

void push(Node* node) {
    stack[++top] = node;
}
Node* pop() {
    return stack[top--];
}
Node* createNode(char *val) {
    Node* node = (Node*)malloc(sizeof(Node));
    strcpy(node->value,val);
    node->left = node->right = NULL;
    return node;
}
Node* createOpNode(char* expr){
    char token[100];
    int i = 0, j = 0;
    while (expr[i] != '\0') {
        if (isspace(expr[i])) {
            i++;
            continue;
        }

        if (isop(expr[i])) {
            char op[2] = {expr[i], '\0'};
            Node* right = pop();
            Node* left = pop();
            Node* newNode = createNode(op);
            newNode->left = left;
            newNode->right = right;
            push(newNode);
            i++;
        } else {
            j = 0;
            while (expr[i] != '\0' && !isspace(expr[i]) && !isop(expr[i])) {
                token[j++] = expr[i++];
            }
            token[j] = '\0';
            push(createNode(token));
        }
    }
    return pop(); 
}
void printTree(Node* root){
    if (root != NULL) {
        printf("%s ", root->value);
        printTree(root->left);
        printTree(root->right);
    }
    if (root != NULL) {
        printf("%s ", root->left);
        printTree(root);
        printTree(root->right);
    }
    if (root != NULL) {
        printf("%s ", root->left);
        printTree(root->right);
        printTree(root);
    }
}
void semanticA(struct token tokens[],int tokencount){
    int n=atoi(tokens[tokencount-1].value);
    int diff=tokencount-1;
    char strdiff[1000]="";
    int tc;
    for(tc=1;tc<diff;tc++){
        strncat(strdiff,tokens[tc].value,sizeof(strdiff)-strlen(strdiff)-1);//concatenates strings into strdiff which is between print and number
        strncat(strdiff," ",sizeof(strdiff)-strlen(strdiff)-1);            
    }
    Node* root = createOpNode(strdiff);
    printTree;
}
int getvarval(const char *input,int value,struct vartable varTable){//Gets the value of the variable
    if(strcmp(varTable.name,input)==0){
        varTable.val=value;
        return varTable.val;
    }
    else{
        printf("Variable not found\n");
        return 0;
    }
}
int findvar(const char *ch,struct vartable vartable){// Function to find if variable declared
    temp=head;
    while(temp!=NULL){
        if(strcmp(ch,temp->name)==0){
            return 1;
        }
        else{temp=temp->next;};
    }
    return 0;
}
int sfindvar(const char *ch,struct strvartable strvartable){// Function to find if variable declared for char
    stemp=shead;
    while(stemp!=NULL){
        if(strcmp(ch,stemp->sname)==0){
            return 1;
        }
        else{stemp=stemp->snext;};
    }
    return 0;
}
int executecode(struct token tokens[],int tokencount,struct vartable *vart,struct strvartable *sart){
    if(strcmp(tokens[0].value,"compute")==0){
        compute(1,tokens,tokencount);
    }
    // else if(strcmp(tokens[0].value,"rep")==0){
        
    // }
    else if(strcmp(tokens[0].value,"rep")==0 && /*isalnum(tokens[1].value) &&*/ strcmp(tokens[2].value,"(")==0){
        if(tokens[1].type==TOKEN_NUMBER){
            //int n=tokens[1].value;
            int s;
            for(s=2;s<=tokencount;s++){
                if(s<tokencount){
                    continue;
                }
            }
            //rep 3(print 1+2
            printf("\nRepetation...\n");
            for(int i=0;i<atoi(tokens[1].value);i++){
                
                if(strcmp(tokens[3].value,"compute")==0){
                    s=4;
                    compute(s,tokens,tokencount);
                }
                else if(strcmp(tokens[3].value,"print")==0 && strcmp(tokens[4].value,"!")==1 && strcmp(tokens[4].value,"$")==1){
                    s=4;
                    printn(s,tokens,tokencount);
                }
                else if(strcmp(tokens[3].value,"print")==0 && strcmp(tokens[4].value,"!")==0){
                    s=5;
                    
                    print_nor(s,tokens,tokencount);
                }
                else if(strcmp(tokens[3].value,"print")==0 && strcmp(tokens[4].value,"$")==0){
                    s=5;
                    print_text(s,tokens,tokencount);
                }
                else if(strcmp(tokens[3].value,"num")==0){
                    num(tokens,tokencount);
                }
                else if(strcmp(tokens[3].value,"let")==0){
                    let(tokens,tokencount);
                }
               
            }
        }
        else if(tokens[1].type==TOKEN_STRING){

        }
    }
    else if(strcmp(tokens[0].value,"print")==0 && strcmp(tokens[1].value, "!") == 0){
            print_nor(2,tokens,tokencount);
            // int m=atoi(tokens[tokencount-1].value);
            // int diff=tokencount-1;
            // char vardiff[1000]="";
            // int tc=2;
            // strcpy(vardiff,tokens[2].value);
            // if(findvar(vardiff,*vart)==1){
            //     temp=head;
            //     while(temp!=NULL){
            //         if(strcmp(vardiff,temp->name)==0){
            //             printf("%d\n",temp->val);
            //             break;
            //         }
            //         else{
            //             temp=temp->next;
            //         }
            //     }
                
            // }
    }
    else if(strcmp(tokens[0].value,"print")==0 && strcmp(tokens[1].value, "$") == 0){
        print_text(2,tokens,tokencount);
}

    else if(strcmp(tokens[0].value,"print")==0 && strlen(tokens[1].value)>0){
        
        printn(1,tokens,tokencount);
    }
    else if(isassign(tokens[0].value)){
        
        if(strcmp(tokens[2].value,"=")==0){
            
            if(strcmp(tokens[0].value,"let")==0){
                let(tokens,tokencount);
            }

            //DONEEE
            else if(tokens[3].type==TOKEN_NUMBER && strcmp(tokens[0].value,"num")==0){
                num(tokens,tokencount);
            }
        }
    }

    else if(strcmp(tokens[0].value,"aboutpro")==0){
        printf("Hi this is gg.\t:) \nThis is a basic compiler project for my Automata and compiler design class.\nI made this for fun so that i could better understand the subject and build cool stuff.\nHope this was helpful.\nByeeee\n");
    }
    else{
        printf("Syntax error 1\n");
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
        executecode(tokens,tokencount,vart,sart);
        syntaxanalyzer(tokens,tokencount);
        semanticA(tokens,tokencount);
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
