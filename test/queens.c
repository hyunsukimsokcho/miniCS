/* 
 original File: queens_pth.c			Author: Manases Galindo
 I modified the queens_pth.c file for our test. 
 This program doesn't need any locks for accurate output.
 
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_QUEENS 8			/* default number of queens	*/
#define NUM_THREAD 8			/* default number of threads	*/
#define ITS_SAFE   0			/* queen on a safe position	*/
#define NOT_SAFE   1			/* or not			*/


/* Shared global variables.				  	  	*/
int *solutions,				/* number of solutions by thd	*/
    **queen_on,				/* track positions of the queen */
    nq,					/* number of queens		*/
    nthreads;				/* number of threads		*/
    

void *start_thread (void *);    
void nqueens (int, int);		/* find total solutions		*/
int is_safe (int, int, int, int);	/* is queen in a safe position?	*/  


int main ()
{
    pthread_t *thr_ids;			/* array of thread ids		*/
    int i, j,				/* loop variables		*/
        *thr_num,				/* array of thread numbers	*/
        total;				/* total number of solutions	*/
    struct timeval tval_before,		/* timing variables		*/
	    tval_after, tval_result;

    /* Number of queens not specified, using the default value	*/
    nq = NUM_QUEENS;
    /* Number of threads not specified, using the default value	*/
    nthreads = NUM_THREAD;
    
    /* allocate memory for all dynamic data structures and validate them 	*/
    thr_num   = (int *) malloc (nthreads * sizeof (int));
    thr_ids   = (pthread_t *) malloc (nthreads * sizeof (pthread_t));
    queen_on  = (int **) malloc (nq * sizeof (int *));
    solutions = (int *) malloc (nq * sizeof (int));
    
    if ((thr_num == NULL) || (thr_ids == NULL) ||
        (queen_on == NULL) || (solutions == NULL)) 
    {
        fprintf (stderr, "File: %s, line %d: Can't allocate memory.",
            __FILE__, __LINE__);
        exit (EXIT_FAILURE);
    }
    
    for (i = 0; i < nq; i++)
    {
        queen_on[i] = (int *) malloc(nq * sizeof (int));
        if (queen_on[i] == NULL)
        {
        fprintf (stderr, "File: %s, line %d: Can't allocate memory.",
            __FILE__, __LINE__);
        exit (EXIT_FAILURE);
        }
        for (j = 0; j < nq; j++)
        {
        queen_on[i][j] = 0;
        }
    }

    /* Get start time and solve the nqueens			 	*/
    gettimeofday(&tval_before, NULL);
    
    /* Create the threads	and let them do their work		  	*/
    for (i = 0; i < nthreads; i++) 
    {
        thr_num[i] = i;			
        pthread_create (&thr_ids[i], NULL, start_thread, &thr_num[i]); 
    }
    
    /* Sum all solutions by thread to get the total		  	*/
    total = 0;
    for (i = 0; i < nthreads; i++) 
    {
        pthread_join (thr_ids[i], NULL);
        total += solutions[i];
    }
    
    /* calculate and show the elapsed time			  	*/
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("\nElapsed time: %ld.%06ld", (long int)tval_result.tv_sec, 
        (long int)tval_result.tv_usec);
    printf ("\nThere are %d solutions for %d queens.\n\n", total, nq);

    /* Deallocate any memory or resources associated			*/
    for (i = 0; i < nq; i++)
    {
        free (queen_on[i]);
    }
    free (queen_on); 
    free (solutions);
    free (thr_num);
    free (thr_ids);

    return EXIT_SUCCESS;
}


/* start_thread runs as a peer thread and will execute the nqueens
 * function concurrently to find the number of possible solutions
 *
 * Input:		arg		pointer to current thread number
 * Return value:	none
 *
 */
void *start_thread (void *arg)
{
  int thr_index;
    
  /* Get the index number of current thread	*/
  thr_index = *( ( int* )arg );
  /* Set start number of solutions for current thread */
  solutions[thr_index] = 0;
  /* Release the Kraken!*/
  nqueens (0, thr_index);

  pthread_exit (EXIT_SUCCESS);		/* Terminate the thread		*/
}


/* nqueens calculates the total number of solutions using recursion 
 * and backtracing.
 *
 * Input:		col		column of the board
 *			thr_index	number of current thread
 * Return value:	none
 *
 */
void nqueens (int col, int thr_index)
{
  int i, j,				/* loop variables		*/  
      start, end;			/* position variables		*/
  
  /* start/end point to cover all rows					*/
  if (col > 0){
      start = 0;
  }else{
      start = thr_index * (nq / nthreads);
  }

  if ((col>0) || (thr_index == nthreads -1)){
      end = nq -1;
  }else {
      end =(thr_index + 1) * (nq / nthreads) - 1;
  }

  if (col == nq)			/* tried N queens permutations  */
  {      
    solutions[thr_index]++;		/* peer found one solution 	*/
  }

  /* Backtracking - try next column on recursive call for current thd	*/
  for (i = start; i <= end; i++) 
  {
    for (j = 0; j < col && is_safe(i, j, col, thr_index); j++);
    if (j < col) 
    {
      continue;
    }
    queen_on[thr_index][col] = i;
    nqueens (col + 1, thr_index);
  }
}


/* is_safe determines if a queen does not attack other
 *
 * Input:		i, j		board coordinates
 *			col		column of the board
 *			thr_index	number of current thread
 * Return value:	ITS_SAFE	Queen without problems
 *			NOT_SAFE	Queen under attack!
 *
 */
int is_safe (int i, int j, int col, int thr_index)
{
  if (queen_on[thr_index][j] == i)
  {
    return ITS_SAFE;
  }
  if (abs(queen_on[thr_index][j] - i) == col - j)
  {
    return ITS_SAFE;
  }

  return NOT_SAFE;
}