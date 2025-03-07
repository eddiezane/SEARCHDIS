#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "index.h"

lnode FILES = NULL;

tnode create_tnode() {
  tnode new = calloc(1, sizeof(struct tnode_));
  new->count = 0;
  new->height = 0;
  new->files = NULL;

  return new;
}

lnode create_lnode(char *filename) {
  lnode new = malloc(sizeof(struct lnode_));
  strcpy(new->filename, filename);
  new->count = 1;
  new->prev = NULL;
  new->next = NULL;

  return new;
}

void swap(lnode a, lnode b) {
  char t[256];
  strcpy(t, a->filename);

  strcpy(a->filename, b->filename);
  strcpy(b->filename, t);

  a->count ^= b->count;
  b->count ^= a->count;
  a->count ^= b->count;

}

void insert_to_list(tnode root, char *filename) {
  lnode p;

  if (root->files == NULL) {
    root->files = create_lnode(filename);
    return;
  }
  else {
    for (p = root->files; p != NULL; p = p->next) {
      if (strcmp(p->filename, filename) == 0) {
        p->count++;
        while (p->prev != NULL && p->count > p->prev->count) {
          swap(p, p->prev);
          p = p->prev;
        }
        return;
      } else if (p->next == NULL) {
        p->next = create_lnode(filename);
        p->next->prev = p;
        return;
      }
    }
  }
}

void destroy_list(lnode list) {
  if (list != NULL) {
    destroy_list(list->next);
    free(list);
  }
}

tnode createTree() {
  tnode root = create_tnode();
  root->count = -1;
  root->files = NULL;
  root->height = 0;

  return root;
}

void destroyTree(tnode root) {
  int i;

  destroy_list(root->files);
  for (i = 0; i < 36; i++) {
    if (root->alphabet[i]) {
      destroyTree(root->alphabet[i]);
    }
  }

  free(root);
}

void addToTree(char *word, tnode root, char *filename) {
  int charcode, height = strlen(word);
  lowerString(word);

  /* Check if this is longest word */
  if (height > root->height)
    root->height = height;

  /*
   * We are given the empty string, meaning the whole word
   * should be in the tree by now and we should mark the tnode.
   */
  if (strcmp(word, "") == 0) {
    if (root->count == -1) {
      /* We should not get here */
      printf("Sending empty string to root. YOU CRAZY\n");
      return;
    }
    root->count++;
    insert_to_list(root, filename);

    return;
  }

  /* We do not have a tnode for the character
   * and need to make one here. */
  charcode = getIndex(*word);
  if (charcode < 0 || charcode > 35) {
    fprintf(stderr, "It broke. Adding nonvalid character to tree\n");
  }

  if (root->alphabet[charcode] == 0)
    root->alphabet[charcode] = create_tnode();

  addToTree(word + 1, root->alphabet[charcode], filename);
}


void pt(tnode root, char *buff, FILE *wf) {
  int i;
  lnode p;

  if (root->count > 0) {
    fprintf(wf, "<list> %s\n\n", buff);
    for (p = root->files; p != NULL; p = p->next)
      fprintf(wf, "%s %d ", p->filename, p->count);
    fprintf(wf, "\n\n</list>\n\n");
  }

  for (i = 0; i < 36; i++) {
    if (root->alphabet[i]) {
      buff[strlen(buff)] = getChar(i);
      buff[strlen(buff)] = 0;
      pt(root->alphabet[i], buff, wf);
      buff[strlen(buff) - 1] = 0;
    }
  }
}

void printTree(tnode root, FILE *wf) {
  char *buff = calloc(1, root->height);
  pt(root, buff, wf);
  free(buff);
}


void hangOrnaments(FILE *fp, tnode trie, char *filename) {
  char *buff = calloc(1, 256);
  while (fscanf(fp, "%255[a-zA-Z0-9]", buff) != EOF) {
    if (strcmp("", buff) != 0)
      addToTree(buff, trie, filename);
    fscanf(fp, "%255[^a-zA-Z0-9]", buff);
  }
  free(buff);
}

/*
 * Converts the letter to an index of our array
 */
int getIndex(int c) {
  if (isdigit(c))
    return c - (int)'0';
  return c - (int)'a' + 10;
}

char getChar(int i) {
  if (i >= 0 && i < 10)
    return i + '0';

  if (i >= 10 && i < 36)
    return i + 'a' - 10;

  return 0;
}

void lowerString(char *s) {
  for(; *s; ++s) {
    if (isalpha(*s))
      *s = tolower(*s);
  }
}

int menorahTime(const char *name, const struct stat *status, int type) {
  lnode ptr;

  if (FILES == NULL) {
    FILES = create_lnode(strstr((char *)name, ""));
  } else {
    ptr = create_lnode(strstr((char *)name, ""));
    ptr->next = FILES;
    FILES = ptr;
  }
  return 0;
}

int usage(int i) {
  switch(i) {
    case 1:
      printf("Usage: ./index <output_file> <input_file | input_directory>\n");
      break;
    case 2:
      printf("INVALID INPUT FILE\n");
      break;
    default:
      printf("Something else bad happened!!\n");
  }

  return 1;
}
