/*
 * Time get the entire time execution of a c-file. It takes 
 * commands of the form: -C FILE [OPTION]. And makes use of 
 * pipe(), fork(), execlp() and wait() System Calls.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int
main (int argc, char *argv[])
{
  struct timeval start, end;
  char *argvx[argc - 1];
  int fd[2];

  if (argc == 1)
    {
      printf ("usage: time -C FILE [OPTION]\n");
      exit (1);
    }
  if (pipe (fd) < 0)
    {
      fprintf (stderr, "time: pipe(fd) failed\n");
      exit (1);
    }
  switch (fork ())
    {
    case -1:
      fprintf (stderr, "time: fork() failed\n");
      exit (1);
    case 0:
      close (fd[0]);
      for (int j = 0, i = 1; (argvx[j] = argv[i]) != NULL; i++, j++);
      /*
       * All statements after execlp call are ignored if it's 
       * executed successfully, that's why it's needed getting 
       * the time two instrucctions before execlp is called.
       */
      gettimeofday (&start, NULL);
      write (fd[1], &start, sizeof (struct timeval));
      execvp (argvx[0], argvx);
      fprintf (stderr, "time: exec(\"%s\") failed\n", argvx[0]);
      exit (1);
    default:
      wait (NULL);
      gettimeofday (&end, NULL);
      close (fd[1]);
      if ((read (fd[0], &start, sizeof (struct timeval))) == -1)
	    {
	      fprintf (stderr, "time: could not read from child pipe");
	      exit (1);
	    }
      printf ("\nElapsed time: %ld µs\n", end.tv_usec - start.tv_usec);
      close (fd[0]);
      exit (0);
    }
}
