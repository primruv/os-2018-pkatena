#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc , char *argv[])
{    
    
 if(argc==2){
        char string[10000];
	int size = 10000;
        fgets(string, size,stdin);
        puts(string);
	exit(1);
    }   
else if(argc > 2){
	    FILE *fpointer;
	    char *singleline;
	    int i;
	    size_t r;
	    size_t size = 150; 
	    for (i =2; i<argc;i++){
		fpointer = fopen(argv[i],"r");
		if(fpointer == NULL){

		printf( "%s" ," my-grep couldn't find the file\n");
		exit(1);

		}
		 while((r=getline(&singleline ,  &size , fpointer)) != -1)
		    {
		if(strstr(singleline,argv[1])!=NULL){
		         printf("%s" , singleline); 
		}           
		   }
		   fclose(fpointer);

	    }
	}


else if(argc == 1){
        printf("my-grep: searchterm [file ...]\n");
        exit(1);
    }
else{
	exit(1);
}

return 0;
}
