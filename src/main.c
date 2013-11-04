#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "index.h"
#include "search.h"


int main(int argc, char **argv) {
  FILE *rf; /* read file */
  char line[256];
  char *token;
  char *word;
  char *filename;
  int step = 0;
  int is_file;
  char input[256];
  char *search_type;
  char *search;
  enum steps {CLEANPASS, GOTWORD, GOTPATH};

  tnode trie = createTree();
  /*  lnode ptr;*/

  /* Check if file exists */
  if (access(argv[1], F_OK) == -1) {
    printf("Input file does not exist\n");
    return 1;
  }

  rf = fopen(argv[1], "r");
  if (!strstr(fgets(line,256,rf), "<list>")) {
    fprintf(stderr, "INVALID INPUt FILE\n");
    destroyTree(trie);
    exit(1);
  }

  rewind(rf);
  while (fgets(line, 256, rf)) {
    switch (step) {
      /* Ready for an opening <list> */
      case CLEANPASS:
        token = strtok(line, " \n");
        if (token != NULL && strcmp(token, "<list>") == 0) {
          word = strdup(strtok(NULL, " \n"));
          step = GOTWORD;
        }
        break;

        /* Have the word, need the paths */
      case GOTWORD:
        /* Keep getting lines until we have path */
        while (fgets(line, 256, rf)) {
          if (strcmp(line, "") == 0)
            continue;
          else {
            /* Tokenize for path names */
            for (is_file = 1, filename = strtok(line, " \n"); filename != NULL;
                filename=strtok(NULL, " \n"), is_file ^= 1) {
              if (is_file)
                addToTree(word, trie, filename);
            }

            step = GOTPATH;
            break;
          }
        }
        break;

        /* Have the paths and they have been added to trie */
      case GOTPATH:
        while (fgets(line, 256, rf)) {
          if (strcmp(line, "</list>\n") == 0) {
            step = CLEANPASS;
            free(word);
            filename = NULL;
            break;
          }
        }
        break;

      default:
        /* something bad happened*/
        exit(1);
    }
  }

  menu();
  while (gets(input)) {
    /* Get first token / search type */
    search = strdup(input);
    search_type = strtok(input, " ");

    if (search_type) {
      if (strcmp(search_type, "q") == 0) {
        free(search);
        break;
      }

      /* sa */
      else if (strcmp(search_type, "sa") == 0) {
        sa(search, trie);
      }

      /* so */
      else if (strcmp(search_type, "so") == 0) {
        so(search, trie);
      }
    }

    free(search);
    printf("Next query? ('q' to exit):  ");
  }


  fclose(rf);
  destroyTree(trie);
  destroy_list(FILES);

  return 0;
}
