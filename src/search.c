#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "search.h"
#include "index.h"

void doSearchLoop(tnode trie) {
  char input[256];
  char *search, *search_type;

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
}

int fileIsValid(FILE *rf) {
  char line[256];
  if (!strstr(fgets(line,256,rf), "<list>")) {
    fprintf(stderr, "INVALID INPUt FILE\n");
    return 0;
  }
  rewind(rf);
  return 1;
}

void loadFileToTree(tnode trie, FILE *rf) {
  enum steps {CLEANPASS, GOTWORD, GOTPATH};
  int step = 0;
  int is_file;

  char line[256];
  char *token, *word, *filename;
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
}

void menu() {
  printf("Enter your search query!\n");
  printf("'so <term>' to search for any that exists\n");
  /* Shitty description */
  printf("'sa <term>' to search for all that exists\n");
}

int goodstuff(char *s) {
  for(; *s; ++s) {
    if (!isalpha(*s) && !isdigit(*s))
      return 0;
  }

  return 1;
}

/* checks to see if word is in the tree */
lnode getFiles(char *word, tnode root) {
  int charcode;

  if (root == NULL || word == NULL)
    return 0;


  if (!goodstuff(word))
    return NULL;

  lowerString(word);

  if (strcmp(word, "") == 0) {
    if (root->count > 0)
      return root->files;
    return 0;
  }

  charcode = getIndex(*word);
  if (charcode < 0 || charcode > 35) {
  }

  if (root->alphabet[charcode] == 0)
    return 0;

  return getFiles(word + 1, root->alphabet[charcode]);
}

/*prints all the files associated with any word*/
int so(char *str, tnode root) {
  char *word;
  lnode ptr; /* Iterates through filenames  */
  tnode t = create_tnode(); /*lets us keep a list*/

  if (strcmp(strtok(str, " "), "so") != 0) {
    printf("I did not get 'so' inside sa function!!\n");
    return 1;
  }

  /* goes through each word in input */
  while ((word = strtok(NULL, " "))) {
    for (ptr = getFiles(word, root); ptr != NULL;
        ptr = ptr->next) {
      insert_to_list(t, ptr->filename);
    }
  }

  /* Means we got no matches */
  if (t->files == NULL) {
    printf("No matches found.\n");
    destroyTree(t);
    return 1;
  }

  /* prints all the filenames */
  for (ptr = t->files; ptr->next != NULL; ptr = ptr->next)
    printf("%s, ", ptr->filename);
  if (ptr)
    printf("%s\n", ptr->filename);

  destroyTree(t);
  return 0;
}

/*prints all the files associated with ALL the words*/
int sa(char *str, tnode root) {
  char *word;
  int count = 0;
  lnode ptr; /* Iterates through filenames  */
  tnode t = create_tnode(); /*lets us keep a list*/

  if (strcmp(strtok(str, " "), "sa") != 0) {
    return 1;
  }


  /* goes through each word in input */
  while ((word = strtok(NULL, " "))) {
    /* goes through each filename for associated word */
    for (ptr = getFiles(word, root); ptr != NULL;
        ptr = ptr->next) {
      insert_to_list(t, ptr->filename);
    }
    count++;
  }



  /* prints all the filenames */
  for (ptr = t->files; ptr != NULL && ptr->count != count; ptr = ptr->next);
  if (ptr != NULL) {
    printf("%s", ptr->filename);
    for (ptr = ptr->next; ptr != NULL; ptr = ptr->next) {
      if(ptr->count == count)
        printf(", %s", ptr->filename);
    }
    printf("\n");
  }
  else {
    printf("No matches found\n");
  }

  destroyTree(t);
  return 0;
}

