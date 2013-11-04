#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "search.h"
#include "index.h"


void menu() {
  printf("Enter your search query!\n");
  printf("'so <term>' to search for any that exists\n");
  /* Shitty description */
  printf("'sa <term>' to search for all that exists\n");
}

/* checks to see if word is in the tree */
lnode getFiles(char *word, tnode root) {
  int charcode;
  lowerString(word);

  if (root == NULL || word == NULL)
    return 0;

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


  /* Means we got no matches */
  if (t->files == NULL) {
    printf("No matches found.\n");
    destroyTree(t);
    return 1;
  }

  /* prints all the filenames */
  for (ptr = t->files; ptr != NULL && ptr->count != count; ptr = ptr->next);
  printf("%s", ptr->filename);
  if (ptr != NULL) {
    for (ptr = ptr->next; ptr != NULL; ptr = ptr->next) {
      if(ptr->count == count)
        printf(", %s", ptr->filename);
    }
  }
  printf("\n");

  destroyTree(t);
  return 0;
}
