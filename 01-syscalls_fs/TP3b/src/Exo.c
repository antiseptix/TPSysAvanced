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
#include<sys/wait.h>
#include<getopt.h>
#include <dirent.h> 
#include<fcntl.h>
#include<sys/time.h>
#include<pthread.h>

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

#define SIZE (int)1e8
#define NBThread (int)4
int tab[SIZE];
int min, max = 0;
pthread_mutex_t lock;
pthread_t tid[NBThread];

void* findExtremums(void* nbThread){
  
  int tempMin,tempMax = 0;
  int i;
  int length = SIZE/NBThread; 
  int startPos = length*(int)nbThread;
  //printf("StartPosition : %d, Length : %d\n",startPos,length);
  for(i=startPos;i<(startPos+length);i++)
  {
    if(tab[i] > tempMax){tempMax = tab[i];}
    if(tab[i] < tempMin){tempMin = tab[i];}
  }
  pthread_mutex_lock(&lock);
  if(tempMin<min){min = tempMin;}
  if(tempMax>max){max = tempMax;}
  pthread_mutex_unlock(&lock);
  return NULL;
}


/**
 * Binary main loop
 *
 * \return 1 if it exit successfully 
 */
int main(int argc, char** argv)
{

  /*
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
      case 'i':
        //input param
        if (optarg)
        {
          bin_input_param = dup_optarg_str();         
        }
        break;
      case 'o':
        //output param
        if (optarg)
        {
          bin_output_param = dup_optarg_str();
        }
        break;
      case 'v':
        //verbose mode
        is_verbose_mode = 1;
        break;
      case 'h':
        print_usage(argv[0]);

        free_if_needed(bin_input_param);
        free_if_needed(bin_output_param);
 
        exit(EXIT_SUCCESS);
      default :
        break;
    }
  } 
  // Business logic must be implemented at this point

  /*Init Random Array*/
  int i;

  srand(time(NULL));
  for(i=0;i<SIZE;i++){
    tab[i] = rand() % 100;
  }

  
  struct timeval tvStart;
  struct timeval tvEnd;

  if(pthread_mutex_init(&lock,NULL) !=0){
    return 1;
  }

  gettimeofday(&tvStart,NULL);

  /* Init Thread*/
    int indCount = 0;
    int err;
  for(indCount = 0;indCount < NBThread;indCount++){
    err = pthread_create(&tid[indCount],NULL,findExtremums,(char*)indCount);
    if(err != 0){
      printf("%d\n",err);
      return 1;
    }
  }

  /* Wait for Threads*/
  int j;
  for(j =0;j<NBThread;++j){  
    pthread_join(tid[j],NULL);
  }
  pthread_mutex_destroy(&lock);
  gettimeofday(&tvEnd,NULL);
  
  printf("MAX : %d\n",max);
  printf("MIN : %d\n",min);
  int end = tvEnd.tv_sec *1000 + tvEnd.tv_usec /1000;
  int start = tvStart.tv_sec *1000 + tvStart.tv_usec /1000;
  printf("Temps de recherche : %ld\n",(end - start)); //- tvStart.tv_usec));

  // Freeing allocated data
  free_if_needed(bin_input_param);
  free_if_needed(bin_output_param);


  return EXIT_SUCCESS;
}
