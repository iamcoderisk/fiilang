#include "globals.h"
#include "util.h"
#include "scan.h"

/** states in scanner DFA **/
typedef enum 
{
    START, INASSIGN,INCOMMENT,INNUM,INID,DONE
}StateType;

char tokenString[MAXTOKENLEN+1];
#define BUFLEN 256
static char lineBuf[BUFLEN];
static int linepos =0;
static int bufsize =0;

static char getNextChar(void)
{
    if(!(linepos<bufsize))
    {
        lineno++;
        if(fgets(lineBuf,BUFLEN-1,source))
        {
            if(EchoSource)fprintf(listing,"%d: %s:",lineno,lineBuf);
            bufsize = strlen(lineBuf);
            linepos =0;
            return lineBuf[linepos++];
        }
        else return EOF;
    }
    else return lineBuf[linepos++];
}


static void ungetNextChar(void)
{
    linepos--;
}
static struct
{
    char * str;
    TokenType tok;
}reservedWords[MAXRESERVED]=
{{"if",IF},{"then",THEN},{"else",ELSE},{"end",END},
{"repeat",REPEAT},{"untill",UNTILL},{"read",READ},
{"write",WRITE}};


static TokenType resevedLookup(char *s)
{
    int i;
    for(i=0; i<MAXRESERVED;i++)
        if(!strcmp(s,reservedWords[i].str))
          return reservedWords[i].tok;
    return ID;
}


/************* ***/

TokenType getToken(void){
    int tokenStringIndex =0;
    TokenType currentToken;
    StateType state = START;
    int save;
    while(state !=DONE){
        char c = getNextChar();
        save = TRUE;
        switch(state){
         case START:
            if(isdigit(c))
                state= INNUM;
            else if (isalpha(c))
                state = INID;
            else if(c ==':')
                state = INASSIGN;
            else if((c==' ') || (c=='\t') || (c=='\n'))
                save = FALSE;
            else if(c=='{')
            {
                save = FALSE;
                state = INCOMMENT;
            }
            else
            {
                state = DONE;
                switch(c){
                    case EOF:
                        save = FALSE;
                        currentToken = ENDFILE;
                    break;
                    case '=':
                    currentToken = EQ;
                    break;
                    case '<':
                    currentToken = LT;
                    break;
                    case '+':
                    currentToken = PLUS;
                    break;
                    case '-':
                    currentToken = MINUS;
                    break;
                    case '*':
                    currentToken = TIMES;
                    break;
                    case '/':
                    currentToken =OVER;
                    break;
                    case '{':
                    currentToken =LPAREN;
                    break;
                    case '}':
                    currentToken = RPAREN;
                    break;
                    case ';':
                    currentToken = SEMI;
                    break;
                    default:
                    currentToken = ERROR;
                    break; 

                }
            }
            break;
            case INCOMMENT:
            save = FALSE;
            if(c=='}') state = START;
            break;
            case INASSIGN:
            state = DONE;
            if(c=='=')
                currentToken = ASSIGN;
            else{
                ungetNextChar();
                currentToken = ERROR;
            }
            break;
            case INNUM:
            if(!isdigit(c))
            {
                ungetNextChar();
                save =FALSE;
                state = DONE;
                currentToken = NUM;
            }
            break;
            case INID:
            if(!isdigit(c)){
                ungetNextChar();
                savev= FALSE;
                state = DONE;
                currentToken = ID;
            }
            break;
            case DONE:
            default:
            fprintf(listing,"Scanner Bug:state=%d\n",state);
            state= DONE;
            currentToken =ERROR;
            break;
        }
        if((save)&&(tokenStringIndex<=MAXTOKENLEN))
          tokenString[tokenStringIndex++] =c;
        if(state==DONE){
            tokenString[tokenStringIndex] ='\0';
            if(currentToken==ID)
                currentToken = resevedLookup(tokenString);
        }
    }
    if(TraceScan){
        fprintf(listing,"\t%d:",lineno);
        printToken(currentToken,tokenString);
    }
    return currentToken;
}