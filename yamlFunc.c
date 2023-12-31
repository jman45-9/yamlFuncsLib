#include "yamlFunc.h"

int main(void)
{
    return 0;
}

//* writes a new key value pair to a yaml file
//! file pointer MUST be in write or append mode ('w' or 'a')
void writeNewVal(char *filename, char *key, char *value)
{
    FILE *yamlFile = fopen(filename, "a");
        fseek(yamlFile, -1, SEEK_CUR);
    fprintf(yamlFile, "%s: %s\n", key, value);
    fclose(yamlFile);
}

//* writes a child item of a key
void writeChild(char *filename, char *parent, char *key, char *value)
{

    char fullParent[MAX_KEY_LENGTH];
    strcpy(fullParent, parent);
    strcat(fullParent, ":");

    int keyLine = getKeyLine(filename, fullParent);
    moveBelowDown(filename, keyLine);

    // Write to temp with the new line
    FILE *mainFile = fopen(filename, "r");
    FILE *tempFile = fopen("temp-jkaksdfka.txt", "w");
    int currentLines = 0;
    int ch;
    while((ch = getc(mainFile)) != EOF)
    {
        fputc(ch, tempFile);
        if(ch == '\n')
        {
            currentLines++;
            if (currentLines == keyLine)
            {
                fprintf(tempFile, "  %s: %s\n", key, value);
                fgetc(mainFile);
            }
        }
    }

    fclose(tempFile);
    fclose(mainFile);

    //copying back to yaml w/ newline
    mainFile = fopen(filename, "w");
    tempFile = fopen("temp-jkaksdfka.txt", "r");
    while((ch = getc(tempFile)) != EOF)
    {
        fputc(ch, mainFile);
    }

    fclose(mainFile);        
    fclose(tempFile);
    remove("temp-jkaksdfka.txt");
}

void writeSequence(char *filename, char *parent, strseq *seqInfo)
{
    int sequenceLength = seqInfo->seqLength;
    char **sequence = seqInfo->sequence;

    char fullParent[MAX_KEY_LENGTH];
    strcpy(fullParent, parent);
    strcat(fullParent, ":");

    int keyLine = getKeyLine(filename, fullParent);
    
    moveBelowDown(filename, keyLine);

    // Write to temp with the new line
    FILE *mainFile = fopen(filename, "r");
    FILE *tempFile = fopen("temp-jkaksdfka.txt", "w");
    int currentLines = 0;
    int ch;
    while((ch = getc(mainFile)) != EOF)
    {
        fputc(ch, tempFile);
        if(ch == '\n')
        {
            currentLines++;
            if (currentLines == keyLine)
            {
                for(int iii = 0; sequenceLength > iii; iii++)
                    if (iii == sequenceLength - 1)
                        fprintf(tempFile, "  - %s", *(sequence + iii));
                    else
                        fprintf(tempFile, "  - %s\n", *(sequence + iii));

            }
        }
    }

    fclose(tempFile);
    fclose(mainFile);

    //copying back to yaml w/ newline
    mainFile = fopen(filename, "w");
    tempFile = fopen("temp-jkaksdfka.txt", "r");
    while((ch = getc(tempFile)) != EOF)
    {
        fputc(ch, mainFile);
    }

    fclose(mainFile);        
    fclose(tempFile);
    remove("temp-jkaksdfka.txt");
}

//* Reads a value from a given YAML file when given a key
//! storeValue may want to be size INT_MAX or just a blank ptr
void readVal(char *filename, char *key, char **storeValue)
{
    FILE *yamlFile = fopen(filename, "r");
    int endOfFile = 0;
    char fullKey[MAX_KEY_LENGTH];
    strcpy(fullKey, key);
    strcat(fullKey, ":");

    while (!endOfFile)
    {
        char *currentKey = malloc(sizeof(char) * MAX_KEY_LENGTH);
        fscanf(yamlFile, "%s", currentKey);
        if (!strcmp(fullKey, currentKey))
        {
            fgets(*storeValue, INT_MAX, yamlFile);
            int valueLength = strlen(*storeValue);
            if (getKeyLine(filename, fullKey) != countLines(filename))
                *(*storeValue + (valueLength-1)) = '\0';

            free(currentKey);
            fclose(yamlFile);
            return;
        }
        if (fgetc(yamlFile) == EOF)
            endOfFile = 1;
        fseek(yamlFile, -1, SEEK_CUR);
        char trash[MAX_LINE_LENGTH];
        fgets(trash, MAX_LINE_LENGTH, yamlFile);
        free(currentKey);
    }
    fclose(yamlFile);
    strcpy(*storeValue, "KEY_NOT_FOUND");
}

void readChild(char *filename, char *parent, char *child, char **storeValue)
{
    //* setup
    char fullParent[MAX_KEY_LENGTH];
    strcpy(fullParent, parent);
    strcat(fullParent, ":");
    char fullChild[MAX_KEY_LENGTH];
    strcpy(fullChild, child);
    strcat(fullChild, ":");
    
    //* find parent
    int parentLine = getKeyLine(filename, fullParent);
    
    FILE *yamlFile = fopen(filename, "r");
    for (int iii = 0; parentLine > iii; iii++)
    {
        char *trash = malloc(sizeof(char) * MAX_LINE_LENGTH);
        fgets(trash, MAX_LINE_LENGTH, yamlFile);
        free(trash);
    }
    
    // * get child value
    while(1)
    {
        int ch;
        if((ch = getc(yamlFile)) != ' ')
        {
            strcpy(*storeValue, "CHILD_NOT_FOUND");
            return; 
        }
        char *currentChild = malloc(sizeof(char) * MAX_KEY_LENGTH);
        fscanf(yamlFile, "%s", currentChild);
        if (!strcmp(currentChild, fullChild))
        {
            fgets(*storeValue, MAX_LINE_LENGTH, yamlFile);
            int valueLength = strlen(*storeValue);
            *(*storeValue + (valueLength-1)) = '\0';

            free(currentChild);
            fclose(yamlFile);
            return;
        }
        char trash[MAX_LINE_LENGTH];
        fgets(trash, MAX_LINE_LENGTH, yamlFile);
    }
}

strseq *readSequence(char *filename, char *parent)
{
    //* setup
    char fullParent[MAX_KEY_LENGTH];
    strcpy(fullParent, parent);
    strcat(fullParent, ":");
    
    //* find parent
    int parentLine = getKeyLine(filename, fullParent);
    
    FILE *yamlFile = fopen(filename, "r");
    for (int iii = 0; parentLine > iii; iii++)
    {
        char *trash = malloc(sizeof(char) * MAX_LINE_LENGTH);
        fgets(trash, MAX_LINE_LENGTH, yamlFile);
        free(trash);
    }
    
    int seqLength = 0;
    while(1)
        if(getc(yamlFile) == ' ')
        {
            seqLength++;
            char trash[MAX_LINE_LENGTH];
            fgets(trash, MAX_LINE_LENGTH, yamlFile);
        }
        else
            break;

    fseek(yamlFile, 0, SEEK_SET);

    for (int iii = 0; parentLine > iii; iii++)
    {
        char *trash = malloc(sizeof(char) * MAX_LINE_LENGTH);
        fgets(trash, MAX_LINE_LENGTH, yamlFile);
        free(trash);
    }

    if (seqLength == 0)
        return NULL;
    
    char **sequence = malloc((sizeof(char) * MAX_LINE_LENGTH) * seqLength);


    // * get child value
    for(int iii = 0; seqLength > iii; iii++)
    {
        while(fgetc(yamlFile) != '-')
            ;
        fgetc(yamlFile);
        char *line = malloc(sizeof(char) * MAX_LINE_LENGTH);
        fgets(line , MAX_LINE_LENGTH, yamlFile);
        *(sequence+iii) = line;
        
    }
    strseq *sequenceInfo = malloc(sizeof(strseq));
    sequenceInfo->seqLength = seqLength;
    sequenceInfo->sequence = sequence;

    return sequenceInfo;
}


//* finds the line of a key
//! full key means the key with a colon at the end so '<key>:'
int getKeyLine(char *filename, char *fullKey)
{
    FILE *file = fopen(filename, "r");
    fseek(file, 0, SEEK_SET);
    int lines = 1;
    while (1)
    {
        char *currentKey = malloc(sizeof(char) * MAX_KEY_LENGTH);
        fscanf(file, "%s", currentKey);
        if (!strcmp(currentKey, fullKey))
        {
            free(currentKey);
            return lines;
        }
        else
            lines++;
        
        char trash[MAX_KEY_LENGTH];
        fgets(trash, MAX_KEY_LENGTH, file);
        if (getc(file) == EOF)
        {
            free(currentKey);
            return -1;
        }
        fseek(file, -1, SEEK_CUR);
        free(currentKey);
    }
    fclose(file);
}

//* clears all contents of a given file
void clearFile(char *fileName)
{
    fclose(fopen(fileName, "w"));
}

//* moves all lines below a given line down
void moveBelowDown(char *filename, int line)
{
    // Write to temp with the new line
    FILE *mainFile = fopen(filename, "r");
    FILE *tempFile = fopen("temp-jkaksdfka.txt", "w");
    int currentLines = 0;
    int ch;
    while((ch = getc(mainFile)) != EOF)
    {
        fputc(ch, tempFile);
        if(ch == '\n')
        {
            currentLines++;
            if (currentLines == line)
                fputc('\n', tempFile);
        }
    }
    fclose(tempFile);
    fclose(mainFile);

    //copying back to yaml w/ newline
    mainFile = fopen(filename, "w");
    tempFile = fopen("temp-jkaksdfka.txt", "r");
    while((ch = getc(tempFile)) != EOF)
    {
        fputc(ch, mainFile);
    }

    fclose(mainFile);        
    fclose(tempFile);
    remove("temp-jkaksdfka.txt");
}

int countLines(char *filename)
{
    FILE *file = fopen(filename, "r");
    int ch;
    int lines = 1;
    while((ch = getc(file)) != EOF )
        if (ch == '\n')
            lines++;

    return lines;
}

void freeStringArray(char **array, int arrayLength)
{
    for (int iii = 0; arrayLength > iii; iii++)
        free(*(array+iii));
    free(array);
}