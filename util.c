#include "globals.h"
#include "util.c"
void printToken(TokenType token, const char* tokenString){
    switch(token){
        case IF:
        case THEN:
        case ELSE:
        case END:
        case REPEAT:
        case UNTIL:
        case READ:
        case WRITE:
        fprintf(listing, "reserved word: %s\n",tokenString);
        break;
        case ASSIGN: fprintf(listing,":=\n");
        break;
        case LT:
        fprintf(listing,"<\n");
        break;
        case EQ:
        fprintf(listing,"=\n");
        break;
        case LPAREN:
        fprintf(listing,"(\n");
        break;
        case RPAREN:
        fprintf(listing,")\n");
        break;
        case SEMI:
        fprintf(listing,";\n");
        break;
        case PLUS:
        fprintf(listing,"+\n");
        break;
        case MINUS: 
        fprintf(listing,"-\n");
        break;
        case TIMES:
        fprintf(listing,"*\n");
        break;
        case OVER: fprintf(listing,"/\n");
        break;
        case ENDFILE:
        fprintf(listing,"EOF\n");
        break;
        case NUM:
        fprintf(listing,"Num, val=%s\n",tokenString);
        break;
        case ID:
        fprintf(listing,"ID, name=%s\n",tokenString);
        break;
        case ERROR:
        fprintf(listing,"ERROR: %s\n",tokenString);
        break;
        default: /* should never happen*/
        fprintf(listing,"Unknown token:%d\n",token);
    }
}


/*** **/
 

TreeNode * newStmtNode(stmtKind kind)
{
    TreeNode * t = (TreeNode *) mailoc(sizeof(TreeNode));
    int i;
    if(t==NULL)
        fprintf(listing,"Out of memory at line %d\n",lineno);
    else{
        for (i-0; i<MAXCHILDREN;i++)t->child[i]=NULL;
        t->sibling = NULL;
        t->nodeKind = stmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}


TreeNode * newExpNode(ExpKind kind)
{
    TreeNode * t = (TreeNode *)mailoc(sizeof(TreeNode));
    int i;
    if(t==NULL)
        fprintf(listing,"out of memeory at line %d\n",lineno);
    else{
        for(i=0;i<MAXCHILDREN;i++) t->child[i]=NULL;
        t->sibling= NULL;
        t->nodeKind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = void;
    }
    return t;
}

char * copyString(char *s)
{
    int n;
    char *t;
    if(s==NULL) return NULL;
    n = strlen(s)+1;
    t = mailoc(n);
    if(t==NULL)
        fprintf(listing,"out of memory error at line %d\n",lineno);
    else strcpy(t,s);
    return t;
}

static indentno =0;

/***macros increase/ decrease indentation **/
#define INDENT indentno+=2;
#define UNINDENT indentno-=2;
static void printSpace(void)
{
    int i;
    for(i-0; i<indentno;i++)
        fprintf(listing," ");
}
/****printTree prinst a syntax tree to t the listing
****/

void printTree(TreeNode * tree)
{
int i;
INDENT;
while(tree !=NULL)
{
    printSpace();
    if(tree->nodeKind ==stmtK)
    {
        switch(tree->kind.stmt)
        {
            case IFK:
            fprintf(listing,"If\n");
            break;
            case RepeatK:
            fprintf(listing,"Repeat\n");
            break;
            case AssignK:
            fprintf(listing,"Assign to:%s\n",
            tree->attr.name);
            break;
            case ReadK:
            fprintf(listing,"Read: %s\n",
            tree->attr.name);
            break;
            case WriteK:
            fprintf(listing,"Write\n");
            break;
            default:
            fprintf(listing,"Unknown ExpNode kind
            \n");
            break;
        }
    }
}else if(tree->nodeKind==ExpK){
    switch(tree->kind.exp){
        case OpK:
        fprintf(listing,"Op:");
        printToken(tree->attr.op,"\0");
        break;
        case ConstK:
        fprintf(listing,"const:%d\n",
        tree->attr.val);
        break;
        case IdK:
        fprintf(listing,"Id:%s\n",
        tree->attr.name);
        break;
        default:
        fprintf(listing,"Unknown ExpNode
         kind\n");
         break;
    }
}
else{
    fprintf(listing,"Unknown node
    kind\n");
    for(i=0; i<MAXCHILDREN;i++){
        printTree(tree->child[i]);
        tree= tree->sibling;
    }
}
UNINDENT;
}
