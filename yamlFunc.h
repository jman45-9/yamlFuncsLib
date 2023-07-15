#ifndef YAML_FUNC
#define YAML_FUNC

// macros
#define MAX_LINE_LENGTH 1024
#define MAX_KEY_LENGTH 51
// needed headers
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

// structures
typedef struct strseq
{
    int seqLength;
    char **sequence;
}  strseq;
// functions
void writeNewVal(char *filename, char *key, char *value);
void writeChild(char *filename, char *parent, char *key, char *value);
void writeSequence(char *filename, char *parent, strseq *seqInfo);

void readVal(char *filename, char *key, char **storeValue);
void readChild(char *filename, char *parent, char *child, char **storeValue);
strseq *readSequence(char *filename, char *parent);

int getKeyLine(char *filename, char *fullKey);
void clearFile(char *fileName);
void moveBelowDown(char *filename, int line);
int countLines(char *filename);
void freeStringArray(char **array, int arrayLength);


#endif