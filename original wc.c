/* Counts the number of words, lines, and characters in the files whose
 * names are given as command-line arguments.  If there are no command-line
 * arguments then the line, word, and character counts will just be 0.
 * Mimics the effects of the UNIX "wc" utility, although does not have
 * exactly the same behavior in all cases.
 */

int main(int argc, char *argv[]) {
  char ch, next_ch, *filename;
  int arg_num= 1, lines, words, chars, total_lines= 0, total_words= 0,
      total_chars= 0;
  FILE *fp;

  /* note that we skip argv[0], the name of the program being run */
  while (arg_num < argc) {  /* for each command-line argument */
    filename= argv[arg_num++];
    fp= fopen(filename, "r");  /* open that file */

    /* silently ignore any problems trying to open files */
    if (fp != NULL) {
      lines= words= chars= 0;

      /* read each file one character at a time, until EOF */
      ch= fgetc(fp);
      while (!feof(fp)) {
        next_ch= fgetc(fp);  /* look ahead and get the next character */
        ungetc(next_ch, fp);  /* unread the next character (see Chapter 15) */

        /* update the counts as needed every time a character is read */

        /* a newline means the line count increases */
        if (ch == '\n')
          lines++;

        /* if the current character is not whitespace but the next character
           is, or if the current character is not whitespace and it is the
           last character in the input, the word count increases */
        if (!isspace(ch) && (isspace(next_ch) || feof(fp)))
          words++;

        /* increasing the character count is a no-brainer */
        chars++;

        ch= fgetc(fp);
      }

      /* add the totals for the current file into the accumulating totals
         for all files */
      total_lines += lines;
      total_words += words;
      total_chars += chars;

      fclose(fp);
    }
  }

  printf("%4d %4d %4d\n", total_lines, total_words, total_chars);

  return 0;
}
