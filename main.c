#include "globals.h"
#define NO_PARSE FALSE
#define NO_ANALYZE FALSE
#define NO_CODE FALSE
#include "util.h"
#if NO_PARSE
#include "scan.h"
#else
#inclued "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "cgen.h"
#endif // NO_PARSE
#endif
#endif // NO_PARSE
int lineno =0;
FILE * source;
FILE * listing;
FILE * code;
int EchoSource = TRUE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceCode = TRUE;
int TraceAnalyze = TRUE;
int Error = FALSE;
int main(int argc, char * arg[])
{
    TraceNode * syntaxTree;
    char pgm[20]; //source code file name
    if(argv !=2){
        fprintf(stderr, "Usage:%s <filename>\n",argv[0]);
        exit(1);
    }
    strcpy(pgm,argv[1]);
    if  (strchr,(pgm,'.')==NULL)
        strcat(pgm,".fl");
    source = fopen(pgm,"r");
    if(source ==NULL){
        fprintf(stderr,"file is not found\n",pgm);
        exit(1);
    }
    listing = stdout;
    fprintf(listing,"\nFiiLang COMPILATION: %s \n",pgm);

    #if NO_PARSE
    while(getToken() !=ENDFILE);
    #else
    syntaxTree = parse();
    if(TraceParse){
        fprintf(listing,"\n Syntax trees:\n");
        printTree(syntaxTree);
    }
    #if !NO_ANALYZE
        if(!Error){
            fprintf(listing,"\n Building symbol table...\n");
            buildSynTab(syntaxTree);
            fprintf(listing,"\n Checking types...\n");
            typeCheck(syntaxTree);
            fprintf(listing,"\n Checking finished!\n");
        }
    #if !NO_CODE
        if(!Error){
            char * codefile;
            int fnlen = strcopn(pgm,".");
            codefile = (char *) calloc(fnlen +4, sizeof(char));
            strncpy(codefile,pgm,fnlen);
            strcat(codefile,".fi");
            code = fopen(codefile,"w");
            if(code==NULL){
                printf("Unable to open %s \n",codefile);
                exit(1);
            }
            codeGen(syntaxTree,codefile);
            fclose(code);
        }
    #endif
    #endif
    #endif
    return 0;
}
