/**
 * \file skeleton.c
 * \brief Basic parsing options skeleton.
 * \author Pierre L. <pierre1.leroy@orange.com>
 * \version 0.1
 * \date 10 septembre 2016
 *
 * Basic parsing options skeleton exemple c file.
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<getopt.h>
#include <dirent.h> 
#include<fcntl.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096


#define USAGE_SYNTAX "[ARGUMENT] PATH "
#define USAGE_PARAMS "ARGUMENT:\n\
  Path \n\
***\n\
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
  /**
   * Binary variables
   * (could be defined in a structure)
   */
  short int is_verbose_mode = 0;
  char* bin_input_param = NULL;
  char* bin_output_param = NULL;

  // Parsing options
  int opt = -1;
  int opt_idx = -1;
  while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
  {
    switch (opt)
    {
      case 'h':
        print_usage(argv[0]);
 
        exit(EXIT_SUCCESS);
      default :
        break;
    }
  }
  /**
   * Checking binary requirements
   * (could defined in a separate function)
   */
  char* path="PWD";
  if (argc > 2 )
  {  
    printf("USAGE : prog <path>\n");
  }
  else
  {
    char *curr_dir = NULL; 
    DIR *dp = NULL; 
    struct dirent *dptr = NULL; 
    unsigned int count = 0; 
    // Get the value of environment variable PWD 
    curr_dir = getenv("PWD"); 
    if(argc == 2) {
      curr_dir = argv[1];
    }

    // Printing params
    dprintf(1, "** PARAMS **\n%-8s: %s\n", 
            "path",   curr_dir);

    // Business logic must be implemented at this point
    if(NULL == curr_dir) 
    { 
        dprintf(STDERR, "\n ERROR : Could not get the working directory\n"); 
        return -1; 
    } 

    // Open the current directory 
    dp = opendir((const char*)curr_dir); 
    if(NULL == dp) 
    { 
        dprintf(STDERR, "\n ERROR : Could not open the working directory\n"); 
        return -1; 
    } 

    printf("\n"); 
    // Go through and display all the names (files or folders) 
    // Contained in the directory. 
    for(count = 0; NULL != (dptr = readdir(dp)); count++) 
    { 
      char* nameFile = dptr->d_name;
      
      struct stat fileStat;
      struct passwd *user;
      struct group *group;
      stat(nameFile,&fileStat);
      user = getpwuid(fileStat.st_uid);
      group = getgrgid(fileStat.st_gid);
      
      
      if(nameFile[0] != '.' ){ // Hide hidden files
        printf("%s\t",nameFile);
        printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-"); //Type directory or file
        printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-"); //Rights
        printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
        printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
        printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
        printf("-%lld - ",(long long) fileStat.st_size); //Size
        printf("%s:%s",user->pw_name, group->gr_name); //UserName:GroupName
        printf(" @ %s", (ctime(&fileStat.st_mtime)) ? ctime(&fileStat.st_mtime) : ""); //Date of Last Modification
        //printf("\n");
      }

    
      
      
      //printf("\n");
    } 
    //printf("\n %u", count); 
  }

  

  return EXIT_SUCCESS;
}
