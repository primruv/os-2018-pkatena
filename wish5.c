#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAXARGS 10
struct cmd {
  int type;          
};

struct execcmd {
  int type;              
  char *argv[MAXARGS];   
};

struct redircmd {
  int type;         
  struct cmd* cmd;   
  char *file;        
  int flags;         
  int fd;            
};

struct pipecmd {
  int type;          
  struct cmd* left;  
  struct cmd* right; 
};

struct parallelcmd{
  int type;
  struct cmd* left;
  struct cmd* right;
};

int fork1(void);  
struct cmd *parsecmd(char*);

void runcmd(struct cmd* cmd){
  int p[2], r;   
  struct execcmd *ecmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;
  struct parallelcmd *paracmd;

  if(cmd == 0)
    _exit(0);
  
  switch(cmd->type){
  default:
   _exit(-1);
  
  case ' ':
    ecmd = (struct execcmd*) cmd;
    
    char path[100] = "/bin/";
        
    
    if(ecmd->argv[0] == 0){
      _exit(0);
    }
    if(execv(strcat(path, ecmd->argv[0]), ecmd->argv) == -1){
      char error_message[30] = "An error has occurred\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
    }
    
    break;
  case '>':
    rcmd = (struct redircmd*)cmd;
    close(rcmd->fd);                                                                         
    if (open(rcmd->file, rcmd->flags) < 0) {                   
     // fprintf(stderr, "open %s failed\n", rcmd->file);
      char error_message[30] = "An error has occurred\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
    }                                     
    
    runcmd(rcmd->cmd);
    break;

  case '|':
    pcmd = (struct pipecmd*)cmd;
    
    break;
  

  case '&':
    paracmd = (struct parallelcmd*)cmd;

    if(fork1()== 0){
      runcmd(paracmd->left);
    }else if(fork1()==0){
      runcmd(paracmd->right);
    }else{
      wait(&r);
      wait(&r);
    }

    break;
 }   

}

int
getcmd(char *buf, int nbuf)
{

  if (isatty(fileno(stdin)))             
    fprintf(stdout, "wish> ");
   
                                                           
  memset(buf, 0, nbuf);
  if(fgets(buf, nbuf, stdin) == 0){       
    return -1; 
  }
          
                                       
  return 0;
}

int main(int argc, char *argv[])
{
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
     //printf( "cannot open the file");
     char error_message[30] = "An error has occurred\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
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
      inputs[i] = chunks; 
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
                //printf("%s\n","folder not available" );
                char error_message[30] = "An error has occurred\n";
                write(STDERR_FILENO, error_message, strlen(error_message));
              }
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

// interactive mode
else if( argc == 1) { 
  static char buf[100];
  int fd, r;
  // Read and run input commands.
  while(getcmd(buf, sizeof(buf)) >= 0)
  { 
    if(buf[strlen(buf)-2] == '&'){
      char error_message[30] = "An error has occurred\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
      continue;
      }
    if(buf[strlen(buf)-2] == '>'){
    
      char error_message[30] = "An error has occurred\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
      continue;
      }
      
    if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
     
      buf[strlen(buf)-1] = 0;  
      if(chdir(buf+3) < 0){                          
    
      char error_message[30] = "An error has occurred\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
      continue;
      }
    
    
   exit(0);
 }
    
    
    if(fork1() == 0){
      runcmd(parsecmd(buf));
    }
    wait(&r);        
  }
  exit(0);
}
}

int
fork1(void)
{
  int pid;
  pid = fork();       
  if(pid == -1)
    perror("fork");
  return pid;
}

struct cmd* execcmd(void) {
  struct execcmd *cmd;
  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = ' ';
  return (struct cmd*)cmd;
}

struct cmd* redircmd (struct cmd *subcmd, char *file, int type)
{
  struct redircmd *cmd;
  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = type;
  cmd->cmd = subcmd;
  cmd->file = file;
  cmd->flags = (type == '<') ?  O_RDONLY : O_WRONLY|O_CREAT|O_TRUNC;
  cmd->fd = (type == '<') ? 0 : 1;
  return (struct cmd*)cmd;
}

struct cmd* pipecmd(struct cmd *left, struct cmd *right)
{
  struct pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = '|';
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

struct cmd* parallelcmd(struct cmd *left, struct cmd *right){
  struct parallelcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = '&';
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|&>";                

int 
gettoken(char **ps, char *es, char **q, char **eq) 
{                                                           
  char *s;    
  int ret;
  
  s = *ps;    

  
  while(s < es && strchr(whitespace, *s)){      
    s++;                                       
  }                                             
     
    
  if(q){
    *q = s;              
  }

  ret = *s;   

  switch(*s){
  case 0:             
    break;

  case '|':
  case '&':                           
    s++;
    break;

  case '<':
    s++;
    break;

  case '>':
    s++;
    break;

  default:                                    
    ret = 'a';
    while(s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))   
      s++;
    break;
  } 

  if(eq){
    *eq = s;        
  }               

  while(s < es && strchr(whitespace, *s)){
    s++;
  }         


  *ps = s;          
  return ret;
}

int
peek(char **ps, char *es, char *toks)  
{
  char *s;
  
  s = *ps;
  while(s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return *s && strchr(toks, *s);        
}

struct cmd *parseline(char**, char*);
struct cmd *parsepipe(char**, char*);
struct cmd *parseexec(char**, char*);
struct cmd *parseparallel(char**, char*);              

char* mkcopy(char *s, char *es)
{
  int n = es - s;
  char *c = malloc(n+1);
  assert(c);
  strncpy(c, s, n);
  c[n] = 0;
  return c;
}



struct cmd* parsecmd(char *s)
{
  char *es;
  struct cmd *cmd;

  es = s + strlen(s);
  cmd = parseline(&s, es);
  peek(&s, es, "");
  if(s != es){
    fprintf(stderr, "leftovers: %s\n", s);
    exit(-1);
  }
  return cmd;
}





struct cmd* parseline(char **ps, char *es)   //
{
  struct cmd *cmd;
  if(peek(ps, es, "|")){
  cmd = parsepipe(ps, es) ;
  }else{
    cmd = parseparallel(ps, es);
  }
  return cmd;
}



struct cmd* parsepipe(char **ps, char *es){
  struct cmd *cmd;

  cmd = parseexec(ps, es);
  if(peek(ps, es, "|")){
    gettoken(ps, es, 0, 0);
    cmd = pipecmd(cmd, parsepipe(ps, es));
  }
  return cmd;
}




struct cmd* parseparallel(char **ps, char *es){             
  struct cmd *cmd;
  cmd = parseexec(ps, es);
  if(peek(ps, es, "&")){
    gettoken(ps, es, 0, 0);
    cmd = parallelcmd(cmd, parseparallel(ps, es));
  }
  return cmd;
}





struct cmd* parseredirs(struct cmd *cmd, char **ps, char *es)
{
  int tok;
  char *q, *eq;

  while(peek(ps, es, "<>")){
    tok = gettoken(ps, es, 0, 0);
    if(gettoken(ps, es, &q, &eq) != 'a') {
      
      char error_message[30] = "An error has occurred\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
      exit(-1);
    }
    switch(tok){
    case '<':
      cmd = redircmd(cmd, mkcopy(q, eq), '<');
      break;
    case '>':
      cmd = redircmd(cmd, mkcopy(q, eq), '>');
      break;
    }
  }
  return cmd;
}

struct cmd*
parseexec(char **ps, char *es)
{
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;
  
  ret = execcmd();
  cmd = (struct execcmd*)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);
  while(!peek(ps, es, "&")){                   
    if((tok=gettoken(ps, es, &q, &eq)) == 0)
      break;
    if(tok != 'a') {
    
      char error_message[30] = "An error has occurred\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
      exit(-1);
    }
    cmd->argv[argc] = mkcopy(q, eq);
    argc++;
    if(argc >= MAXARGS) {
      
      char error_message[30] = "An error has occurred\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
      exit(-1);
    }
    ret = parseredirs(ret, ps, es);
  }
  cmd->argv[argc] = 0;
  return ret;
}