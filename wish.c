
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>
	#include <errno.h>
	#include <sys/wait.h>

	int main(int argc, char *argv[]) {
	        char *textline;
	        size_t size = 32;
	        char *inputs[15];
	    	char *defaultPath = "/bin/";
	   		char *userPath = "/usr/bin/";
	   		char *exito = "exit";
	   		textline = (char *)malloc(size * sizeof(char)); //user input
	        if(argc==2){ //implementing batch mode

	            FILE *one;
	            one = fopen(argv[1],"r"); //reading from the user input
	            if(one== NULL){
	                           printf( "cannot open the file");
	                           exit(1);
	                   }

	                   while (getline(&textline, &size, one)!=-1){

	                    char length=strlen(textline);
	                        if(textline[length-1] == '\n'){
	                            textline[length-1] = '\0';
	                        }
							int i =0;
                           // splitting by space
                          char *chunks = strtok(textline, " ");
                          char *arr[10] ={NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,NULL};
                          while(chunks != NULL)
                          {
                              inputs[i] = chunks; //inserting chunks into the array
                              i =i +1;
                              chunks = strtok(NULL, " ");
                              if(strcmp(inputs[0],"exit")==0){
                            exit(0);
                         }

                          }
                          for(int m=1; m<i;m++){
                              arr[m]=inputs[m];
                          }
                          char  *path = malloc(strlen(defaultPath)+ strlen(inputs[0]) +1 ); 
                          strcat(path, defaultPath); //add bin to empty string
                         strcat(path,inputs[0]); //add recorded path to bin
                         char  *path1 = malloc(strlen(userPath)+ strlen(inputs[0]) +1 );
                         strcat(path1, userPath);
                         strcat(path1, inputs[0]);
                        
                         if(strcmp(inputs[0],"cd")==0){
	                         if (chdir(inputs[1])==0)
	                            {
	                                printf("%s\n", inputs[1] );
	                             }else{
	                                printf("%s\n","folder not available" );
	                              }
	                     }
	                     else{
	            
			                pid_t pid = fork(); //creating a child process
			                if(pid==0){
			                    if(access(path,X_OK)==0){
			                        arr[0]=path;
			                        execv(path,arr);
			                    }
			                    else if(access(path1,X_OK)==0){
			                        arr[0]=path1;                        
			                        execv(path1,arr);
			                    }else{
			                        printf("%s\n","access denied for usr and usrbin paths" );
			                    }
			                }
			                else{
			                   wait(NULL);
			                }
	                  	 }
	           		

	        	}
	        	fclose(one);
	        }
	        //interactive mode
	        else if( argc == 1) {

	                
	                while(1) {
	                        printf("wish> ");
	                        getline(&textline, &size, stdin);
	                        char length=strlen(textline);
	                        if(textline[length-1] == '\n'){
	                            textline[length-1] = '\0'; // removing newline character
	                        }
	                        //split by space
	                       
							int i =0;
                          char *chunks = strtok(textline, " ");
                          char *arr[10] ={NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,NULL};
                          while(chunks != NULL)
                          {
                              inputs[i] = chunks;
                              i =i +1;
                              chunks = strtok(NULL, " ");

                          }
                          for(int m=1; m<i;m++){
                              arr[m]=inputs[m];
                          }
                          char  *path = malloc(strlen(defaultPath)+ strlen(inputs[0]) +1 ); //empty string
                          //adding the empty string and path 
                          strcat(path, defaultPath); 
                         strcat(path,inputs[0]);
                         char  *path1 = malloc(strlen(userPath)+ strlen(inputs[0]) +1 );
                         strcat(path1, userPath);
                         strcat(path1, inputs[0]);
                        
                        if(strcmp(inputs[0],"cd")==0){

	                         if (chdir(inputs[1])==0)
	                            {
	                                printf("%s\n", inputs[1] );
	                             }else{
	                                printf("%s\n","folder does not exist" );
	                              }
	                     }
	                     else if(strcmp(inputs[0],"exit")==0){
                             exit(0);
                         }
	                     else{
	            
			                pid_t pid = fork();
			                if(pid==0){
			                    if(access(path,X_OK)==0){
			                        arr[0]=path;
			                        execv(path,arr);
			                    }
			                    else if(access(path1,X_OK)==0){
			                        arr[0]=path1;                        
			                        execv(path1,arr);
			                    }else{
			                        printf("%s\n"," cannot access the paths" );
			                    }
			                }
			                else{
			                   wait(NULL);
			                }
	                  	 }

	                 
					} //

	             }    
	           
	            
	         return 0;
	     }








