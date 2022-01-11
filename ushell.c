/*
Alejandro Hernandez Baca
UDA:Sistemas Operativos
Microshell en C
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

//Funcion para imprimir el prompt de la shell
void prompt()
{
    printf("[Alex ushell] $ ");
    fflush(stdout);
}


void trim(char *str)
{
    int i=0;

    while(str[i]!='\n')
    {
        i++;
    }
    str[i]=0;
}

//Funcion para contar palabras escritas
int wcount(char *line)
{
    unsigned int i=0;
    unsigned int count=0;
    while(line[i]!=0)
    {
        while(line[i]==' ' || line[i]=='\t')
            i++;
        if(line[i]!=0)
            count++;
        else   
            break;
        while(line[i]!=' ' && line[i]!='\t' && line[i]!=0)
            i++;
    }
    return count;
}

//Funcion tokenize
char **get_args(char *line)
{
    char **array;
    char *ptr;

    int nstr=wcount(line);

    array = (char**)malloc((nstr+1)*sizeof(char*));

    ptr=strtok(line," \t");
    //printf("token 0=\"%s\"\n",ptr);   //Linea de pruebas
    array[0]=(char*)malloc((strlen(ptr)+1)*sizeof(char));
    strcpy(array[0],ptr);
  
    for(int i=1;i<nstr;i++)
    {
        ptr=strtok(NULL," \t");
        //printf("token %u=\"%s\"\n",i,ptr);    //Linea de pruebas
        array[i]=(char*)malloc((strlen(ptr)+1)*sizeof(char));
        strcpy(array[i],ptr);
    }
    array[nstr]=NULL;

    return array;
}

//Funcion para imprimir argumentos(debug)
void print_args(char **args)
{
    int i=0;

    while(args[i]!=NULL)
    {
        printf("\"%s\"\n",args[i]);
        i++;
    }
}

//Funcion para liberar la memoria de la estructura de datos que genera tokenize
void free_args(char** args)
{
    int i=0;

    while(args[i]!=NULL)
    {
        free(args[i]);
        i++;
    }
    free(args);
}

int main()
{   
    char buffer[256];
    char **args;

    while(1)
    {
        prompt();
        fgets(buffer,256,stdin);
        trim(buffer);
        //printf("(%i)\n",wcount(buffer));  //Liena de pruebas

         if(strcmp(buffer,"exit")==0)
        {
            return 0;
        }
        else if(strcmp(buffer,"quit")==0)
        {
            return 0;
        }
        args=get_args(buffer);
        if (strcmp(args[0],"cd")==0)    //Implementar llamada al sistema "cd"
        {
            if(chdir(args[1])!=0)
            {
                fprintf(stderr,"Wrong path. \n");
            }
        }
        else
        {       
            pid_t pid=fork();
            if (pid==0)
            {//Codigo proceso hijo
                execvp(args[0],args);
                //A partir de aqui no se ejecuta si execvp tiene exito
                fprintf(stderr,"ERROR: wrong command\n");
                free(args);
                return 1;
            }
            else
            {//Codigo prceso padre
                int status;
                waitpid(pid,&status,0);
            }
        }
        free(args);

    }    
    return 0;
}