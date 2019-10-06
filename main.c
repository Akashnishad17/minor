#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

const char *s1 = "use [a-zA-Z_][a-zA-Z0-9_]+;$";
const char *s2 = "drop [a-zA-Z_][a-zA-Z0-9_]+;$";
const char *s3 = "show databases;$";
const char *s4 = "create database [a-zA-Z_][a-zA-Z0-9_]+;$";
const char *s5 = "create table [a-zA-Z_][a-zA-Z0-9_]+ ?\\(([a-zA-Z_][a-zA-Z0-9_]+ (int|string|double)\\,? ?)+\\);$";
const char *s6 = "insert into [a-zA-Z_][a-zA-Z0-9_]+ values ?\\(([a-zA-Z0-9_\\.]+\\,?)+\\);$";
const char *s7 = "show tables;$";
const char *s8 = "select \\* ?from [a-zA-Z_][a-zA-Z0-9_]+;$";
const char *s9 = "drop table [a-zA-Z_][a-zA-Z0-9_]+;$";
const char *s10 = "select ([a-zA-Z_][a-zA-Z0-9_]+ ?\\,? ?)+ from [a-zA-Z_][a-zA-Z0-9_]+;$";
const char *s11 = "desc [a-zA-Z_][a-zA-Z0-9_]+;$";

struct key
{
	char *k;
	struct key *next;
}*head=NULL;

char database[10] = "default";

void createDatabase(char *d)
{
	chdir("database");
	int check = mkdir(d,0700);
	chdir("..");
	if(!check)
	{
		printf("%s created\n",d);
	}
	else
	{
		printf("Database already exists\n");
		return;
	}
	
}

void openDatabase(char *d)
{
	chdir("database");
	DIR *check = opendir(d);
	chdir("..");
	if(check)
	{
		closedir(check);
		strcpy(database,d);
		printf("Switched to %s\n",database);
	}
	else
	{
		printf("%s does not exist\n",d);
	}
}


void deleteDatabase(char *d)
{
	chdir("database");
	int r = remove(d);
	chdir("..");
	if(!r)
	{
		printf("%s deleted\n",d);
		if(strcmp(database,"d")==0)
			strcpy(database,"default");
	}
	else
	{
		printf("%s does not exist\n",d);
	}
}

void showDatabases()
{
	chdir("database");
	struct dirent *dir;
	DIR *d = opendir(".");
	chdir("..");
	if(d)
	{
		while((dir=readdir(d)) != NULL)
		{
			printf("%s\n",dir->d_name);
		}
		//close(d);
	}
	else
	{
		printf("No database created\n");
	}
}

void createTable(struct key *K)
{
	chdir("database");
	chdir(database);
	FILE *fp;
	if(access(K->k,F_OK) == -1)
	{
		fp = fopen(K->k,"w");
		K = K->next;
		while(K != NULL)
		{
			fputs(K->k,fp);
			K =	K->next;
			if(K != NULL)
				fputc(',',fp);
		} 
		fclose(fp);
		printf("Table created\n");
	}
	else
	{
		printf("Table already exists\n");
	}
	chdir("..");
	chdir("..");
}

void showTables()
{
	chdir("database");
	chdir(database);
	struct dirent *dir;
	DIR *d = opendir(".");
	chdir("..");
	chdir("..");
	if(d != NULL)
	{
		while((dir=readdir(d)) != NULL)
		{
			printf("%s\n",dir->d_name);
		}
		//close(d);
	}
	else
	{
		printf("No tables created\n");
	}
}

void deleteTable(char *t)
{
	chdir("database");
	chdir(database);
	if(remove(t) == 0)
	{
		printf("Table deleted\n");
	}
	else
	{
		printf("Table does not exist\n");
	}
	chdir("..");
	chdir("..");
}

int validateData(struct key *K)
{
	
	FILE *fp;
	fp = fopen(K->k,"r");
	char s[100];
	fgets(s,100,fp);
	fclose(fp);
	K = K->next->next;
	char *t = strtok(s,",");
	int i=0;
	while(t != NULL)
	{
		i++;
		t= strtok(NULL,",");
		if(strcmp("int",t)==0)
		{
			char p[20];
			snprintf(p,20,"%d",atoi(K->k));
			if(strcmp(p,K->k) != 0)
			{
				printf("Error: '%s' is not of type 'int'\n",K->k);
				return 0;
			}
		}
		else if(strcmp("double",t)==0)
		{
			char p[20];
			snprintf(p,20,"%g",atof(K->k));
			if(strcmp(p,K->k) != 0)
			{
				printf("Error: '%s' is not of type 'double'\n",K->k);
				return 0;
			}
		}
		K = K->next;
		t= strtok(NULL,",");
		if(K == NULL && t != NULL)
		{
			printf("Error: Less number of attributes are given\n");
			return 0;
		}
	}
	if(K != NULL)
	{
		printf("Error: Table has %d attributes\n",i);
		return 0;
	}
	return 1;
}

void insertTable(struct key *K)
{
	chdir("database");
	chdir(database);
	if(access(K->k,F_OK) != -1)
	{
		if(validateData(K)==1)
		{
			FILE *fp;
			fp = fopen(K->k,"a");
			K = K->next->next;
			fputc('\n',fp);
			while(K != NULL)
			{
				fputs(K->k,fp);
				K =	K->next;
				if(K != NULL)
					fputc(',',fp);
			} 
			fclose(fp);
			printf("1 row inserted\n");
		}
	}
	else
	{
		printf("Table does not exist\n");
	}
	chdir("..");
	chdir("..");
}

void descTable(char *K)
{
	chdir("database");
	chdir(database);
	FILE *fp;
	if(access(K,F_OK) != -1)
	{
		fp = fopen(K,"r");
		char s[100];
		fgets(s,100,fp);
		fclose(fp);
		char *t = strtok(s,",");
		printf("\tColumnn\tType\n");
		int i=0;
		while(t != NULL)
		{
			printf("\t%s",t);
			t= strtok(NULL,",");
			i++;
			if(i%2==0)
				printf("\n");
		}
	}
	else
	{
		printf("Table does not exist\n");
	}
	chdir("..");
	chdir("..");
}

void selectAllCol(char *f)
{
	chdir("database");
	chdir(database);
	if(access(f,F_OK) != -1)
	{
		FILE *fp;
		fp = fopen(f,"r");
		char s[100];
		fgets(s,100,fp);
		char *t = strtok(s,",");
		while(t != NULL)
		{
			printf("\t%s",t);
			t= strtok(NULL,",");
			t= strtok(NULL,",");
		}
		printf("\n");
		int i=0;
		while(fgets(s,100,fp) != NULL)
		{
			t = strtok(s,",");
			while(t != NULL)
			{
			printf("\t%s",t);
			t= strtok(NULL,",");
			}
			i++;
		}
		printf("\n%d row(s) selected\n",i);
		fclose(fp);
	}
	else
	{
		printf("Table does not exist\n");
	}
	chdir("..");
	chdir("..");
}

void selectTable(struct key *K)
{
	if (strcmp("from",K->k)==0)
	{
		K = K->next;
		selectAllCol(K->k);
	}
	else
	{
		//selectFewCol();
		printf("Coming Soon\n");
	}
}

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
 	else if(match(p,s11)==1)
 		return 1;
 	else
 		return 0;
}

void storeKeywords(char *p)
{
	struct key *K = (struct key*)malloc(sizeof(struct key));
	K->k=p;
	K->next = NULL;
	if(head == NULL)
		head = K;
	else
	{
		struct key *t = head;
		while(t->next != NULL)
			t = t->next;
		t->next = K;  
	}
}

void findKeywords()
{
	char *t = strtok(query," (),;*");
	while(t != NULL)
	{
		storeKeywords(t);
		t= strtok(NULL," (),;*");
	}
}


void processQuery()
{
	struct key *K = head;
	if(strcmp("use",K->k)==0)
	{
		K = K->next;
		openDatabase(K->k);
	}
	else if(strcmp("show",K->k)==0)
	{
		K = K->next;
		if (strcmp("databases",K->k)==0)
			showDatabases();
		else if (strcmp("tables",K->k)==0);
			showTables();
	}
	else if(strcmp("drop",K->k)==0)
	{
		K = K->next;
		if(strcmp("table",K->k)==0)
		{
			deleteTable(K->next->k);
		}
		else
		{
			deleteDatabase(K->k);
		}
	}
	else if(strcmp("create",K->k)==0)
	{
		K = K->next;
		if(strcmp("database",K->k)==0)
		{	
			K = K->next;
			createDatabase(K->k);
		}
		else if(strcmp("table",K->k)==0)
		{	
			K = K->next;
			createTable(K);
		}
	}
	else if(strcmp("desc",K->k)==0)
	{
		K = K->next;
		descTable(K->k);
	}
	else if(strcmp("insert",K->k)==0)
	{
		K = K->next->next;
		insertTable(K);
	}
	else if(strcmp("select",K->k)==0)
	{
		K = K->next;
		selectTable(K);
	}
}

void deleteKeywords()
{
	struct key *current = head;
	struct key *next;
	while(current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	head = NULL;
}

void run()
{
    // Step 2
    if(checkSyntax()==1)
    {
    	//printf("No Error\n"); 	
    	findKeywords();
    	// Step 4
    	processQuery();
    	deleteKeywords();
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
        else if(strcmp(query,"clear") == 0)
            system("clear");
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
