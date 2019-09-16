#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

const char *s1 = "use [a-zA-Z_][a-zA-Z0-9_]+;$";
const char *s2 = "drop [a-zA-Z_][a-zA-Z0-9_]+;$";
const char *s3 = "show databases;$";
const char *s4 = "create database [a-zA-Z_][a-zA-Z0-9_]+;$";
const char *s5 = "create table [a-zA-Z_][a-zA-Z0-9_]+ ?\\(([a-zA-Z_][a-zA-Z0-9_]+ (int|string|double)\\,? ?)+\\);$";
const char *s6 = "insert into [a-zA-Z_][a-zA-Z0-9_]+ values ?\\(([a-zA-Z0-9_][a-zA-Z0-9.]+ ?\\,? ?)+\\);$";
const char *s7 = "show tables;$";
const char *s8 = "select \\* ?from [a-zA-Z_][a-zA-Z0-9_]+;$";
const char *s9 = "drop table [a-zA-Z_][a-zA-Z0-9_]+;$";
const char *s10 = "select ([a-zA-Z_][a-zA-Z0-9_]+ ?\\,? ?)+ from [a-zA-Z_][a-zA-Z0-9_]+;$";
	
char query[100];
int match(const char *string, const char *pattern)
{
	regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) return 0;
    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);
    if (status != 0) return 0;
    return 1;
}

int checkSyntax()
{
 	char *p = &query[0];
 	if(match(p,s1)==1)
 		return 1;
 	else if(match(p,s2)==1)
 		return 1;
 	else if(match(p,s3)==1)
 		return 1;
 	else if(match(p,s4)==1)
 		return 1;
 	else if(match(p,s5)==1)
 		return 1;
 	else if(match(p,s6)==1)
 		return 1;
 	else if(match(p,s7)==1)
 		return 1;
 	else if(match(p,s8)==1)
 		return 1;
 	else if(match(p,s9)==1)
 		return 1;
 	else if(match(p,s10)==1)
 		return 1;
 	else
 		return 0;
}

/*void findKeywords()
{
	return;
}

void storeKeywords()
{
	return;
}

void processQuery()
{
	return;
}*/

void run()
{
    // Step 2
    if(checkSyntax()==1)
    {
    	printf("No Error\n");
    	//findKeywords();

    	// Step 3
    	//storeKeywords();

    	// Step 4
    	//processQuery();
    }
    else
    	printf("Syntax Error\n");
}

void cli()
{
    int loop = 1;
    while (loop)
    {
        printf("minor> ");
        // step 1
        gets(query);
        if(strcmp(query,"exit") == 0)
            loop=0;
        /*else if(strcmp(query,"clear") == 0)
            system("clear");
        else if(strcmp(query,"help") == 0)
            help();*/
        else
        {
            run();
        }
    }
}

int main()
{
    cli();
    return 0;
}
