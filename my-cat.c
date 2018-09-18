
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
FILE *fpointer;
char singleLine[150];
int i;
for (i =1; i<argc;i++){
        fpointer = fopen(argv[i],"r");
        if(fpointer== NULL){
                printf( "my-cat : cannot open file");
                exit(1);
}

                while (fgets(singleLine, 150, fpointer)!=NULL){
                        printf("%s",singleLine);



}
   fclose(fpointer);

}

}
