#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<getopt.h>
#include <dirent.h> 
#include<fcntl.h>

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096


#define USAGE_SYNTAX "[OPTIONS] -i INPUT -o OUTPUT"
#define USAGE_PARAMS "OPTIONS:\n\
  -i, --input  INPUT_FILE  : input file\n\
  -o, --output OUTPUT_FILE : output file\n\
***\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"

/**
 * Procedure which displays binary usage
 * by printing on stdout all available options
 *
 * \return void
 */
void print_usage(char* bin_name)
{
  dprintf(1, "USAGE: %s %s\n\n%s\n", bin_name, USAGE_SYNTAX, USAGE_PARAMS);
}


/**
 * Procedure checks if variable must be free
 * (check: ptr != NULL)
 *
 * \param void* to_free pointer to an allocated mem
 * \see man 3 free
 * \return void
 */
void free_if_needed(void* to_free)
{
  if (to_free != NULL) free(to_free);  
}


/**
 *
 * \see man 3 strndup
 * \see man 3 perror
 * \return
 */
char* dup_optarg_str()
{
  char* str = NULL;

  if (optarg != NULL)
  {
    str = strndup(optarg, MAX_PATH_LENGTH);
    
    // Checking if ERRNO is set
    if (str == NULL) 
      perror(strerror(errno));
  }

  return str;
}


/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 *
 * \see man 3 getopt_long or getopt
 * \see struct option definition
 */
static struct option binary_opts[] = 
{
  { "help",    no_argument,       0, 'h' },
  { "verbose", no_argument,       0, 'v' },
  { "input",   required_argument, 0, 'i' },
  { "output",  required_argument, 0, 'o' },
  { 0,         0,                 0,  0  } 
};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */ 
const char* binary_optstr = "hvi:o:";


/**
 * Binary main loop
 *
 * \return 1 if it exit successfully 
 */
int main(int argc, char** argv)
{

  // Business logic must be implemented at this point
  printf("test\n");
    pid_t pid;
    int fd[2];
    int status;

    pipe(fd);
    // Test echec pipe
    if (pipe(fd) == -1)
    {
        perror("Erreur creation pipe\n");
        return 1;
    }
    int WRITE_IN = 1;
    int READ_OUT = 0;

    
    
    // Test si fils cree
    if ((pid = fork()) < 0)
    {
        perror("Erreur creation fork\n");
        return 1;
    }

    //Fils
    if(pid==0)
    {
        dup2(fd[WRITE_IN], 1);
        close(fd[READ_OUT]);
        close(fd[WRITE_IN]);
        execlp("ps","ps", "eau","|","grep","\"^root\"",">","/dev/null","&&","echo","\"root est connecté\"", (char*) NULL);//,"|", "grep \"root\"", ">", "/dev/null", "&&", "echo \"root est connecté\"");
        fprintf(stderr, "Failed to execute '%s'\n", "ps eau");
        exit(1);
    }
    //Pere
    else
    {   
        waitpid(pid, &status, 0);
        
        close(fd[WRITE_IN]);
        
        char reading_buf[1]; // Buffer de lecture
        // Lecture tant qu'il y a des donnees
    //printf("test\n");
        while (read(fd[READ_OUT], reading_buf, 1) > 0)
        {
            //printf("test\n");
            write(1, reading_buf, 1); // Ecrit dans sdout ce qui est envoye par le fils
        }
        close(fd[READ_OUT]);
    // }
    }
   




  return EXIT_SUCCESS;
}
