#ifndef INDEX_H
#define INDEX_H

#include <ftw.h>
/*
 * List node
 */
struct lnode_ {
  char filename[256];
  int count;
  struct lnode_ *next;
  struct lnode_ *prev;
};

typedef struct lnode_ *lnode;

lnode FILES;

/*Creates a new node with a string as its argument*/
lnode create_lnode(char *);

/* Destroys the list */
void destroy_list(lnode);

/*
 * Trie node structure. Represents a single character
 */
struct tnode_ {
  struct tnode_ *alphabet[36];
  int height;
  int count;
  lnode files;
};

typedef struct tnode_ *tnode;

/*
 *  Creates a new trie node.
 *  Takes no arguments.
 */
tnode create_tnode();

/* Swaps two nodes */
void swap(lnode, lnode);

/* Inserts a filesname into list */
void insert_to_list(tnode, char *);


/*
 * Creates a new tree.
 */
tnode createTree();

/*
 * Recursively adds a word to the tree.
 */
void addToTree(char *, tnode, char *);

/*
 * Recursively destroys prefix tree.
 */
void destroyTree(tnode);

/*
 * Recursive part of printTree
 */
void pt(tnode, char *, FILE *);

/*
 * Print dat tree.
 */
void printTree(tnode, FILE *);

/*
 * Adds every word in a file to a tire
 */
void hangOrnaments(FILE *, tnode, char *);

/*
 *  Gets index for char.
 */
int getIndex(int);

/*
 * Gets char for index.
 */
char getChar(int);

/*
 * Makes a string lowercase.
 */
void lowerString(char *);

/*
 * Goes through files in dir
 */
int menorahTime(const char *, const struct stat *, int);

/*
 *  Prints usage information and returns error code.
 */
int usage(int);

#endif
