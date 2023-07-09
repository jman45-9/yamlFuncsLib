#ifndef YAML_FUNC
#define YAML_FUNC

// needed headers
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
// functions
void writeNewVal(char *filename, char *key, char *value);
void writeChild(char *filename, char *parent, char *key, char *value);

void *readVal(char *filename, char *key, char **storeValue);


int getKeyLine(char *filename, char *fullKey);
void clearFile(char *fileName);
void moveBelowDown(char *filename, int line);
int countLines(char *filename);

#endif