#ifndef SEARCH_H
#define SEARCH_H

#include "index.h"

lnode getFiles(char *, tnode);
int sa(char *, tnode);
int so(char *, tnode);
int fileIsValid(FILE *);
void loadFileToTree(tnode, FILE *);
void menu();
void doSearchLoop(tnode);
int goodstuff(char *);

#endif
