#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

/* Counts the number of words, lines, and characters in the files whose
 * names are given as command-line arguments.  If there are no command-line
 * arguments then the line, word, and character counts will just be 0.
 * Mimics the effects of the UNIX "wc" utility, although does not have
 * exactly the same behavior in all cases.
 */

#define MAX_FILES 100

struct thr_args {
  char *filename;
  int lines;
  int words;
  int chars;
};

void *count_arguments (void *args) {
  struct thr_args *targs= (struct thr_args *)args; 
  char ch, next_ch;
  FILE *fp= fopen(targs -> filename, "r");

  /* silently ignore any problems trying to open files */
  if (fp != NULL) {

    /* read each file one character at a time, until EOF */
    ch= fgetc(fp);
    while (!feof(fp)) {
      next_ch= fgetc(fp);  /* look ahead and get the next character */
      ungetc(next_ch, fp);  /* unread the next character (see Chapter 15) */

      /* update the counts as needed every time a character is read */

      /* a newline means the line count increases */
      if (ch == '\n')
        targs->lines++;

      /* if the current character is not whitespace but the next character
         is, or if the current character is not whitespace and it is the
         last character in the input, the word count increases */
      if (!isspace(ch) && (isspace(next_ch) || feof(fp)))
        targs->words++;

      /* increasing the character count is a no-brainer */
      targs->chars++;

      ch= fgetc(fp);
    }

    fclose(fp);
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  int args_num = 1, total_lines = 0, total_words = 0, total_chars = 0;
  pthread_t threads[MAX_FILES];
  struct thr_args targs[MAX_FILES];


  /* note that we skip argv[0], the name of the program being run */
  while (args_num < argc) {
    targs[args_num-1].filename= argv[args_num];
    targs[args_num-1].lines= 0;
    targs[args_num-1].words= 0;
    targs[args_num-1].chars= 0;
    pthread_create(&threads[args_num-1], NULL, count_arguments,
                   &targs[args_num-1]);
    args_num++;
  }

  args_num= 1;

  while (args_num < argc) {
    pthread_join(threads[args_num-1], NULL);
    total_lines += targs[args_num-1].lines;
    total_words += targs[args_num-1].words;
    total_chars += targs[args_num-1].chars;
    args_num++;
    printf("%d", 1);
  }
  
  printf("%4d %4d %4d\n", total_lines, total_words, total_chars);

  return 0;
}
