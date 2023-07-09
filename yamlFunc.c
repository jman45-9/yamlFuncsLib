#include "yamlFunc.h"

int main(void)
{
    clearFile("TEMP.yaml");

    writeNewVal("TEMP.yaml", "test", "mess best");
    writeNewVal("TEMP.yaml", "mess", "");
    writeNewVal("TEMP.yaml", "bob", "test");

    writeChild("TEMP.yaml", "mess", "hammer", "nail");
    //char *temp = malloc(INT_MAX);
    //printf("%s\n", temp);

    //free(temp);
    return 0;
}

//* writes a new key value pair to a yaml file
//! file pointer MUST be in write or append mode ('w' or 'a')
void writeNewVal(char *filename, char *key, char *value)
{
    FILE *yamlFile = fopen(filename, "a");
    if (getc(yamlFile) != '\n' && (ftell(yamlFile) != 0))
    {
        fputc('\n', yamlFile);
    }
    else
        fseek(yamlFile, -1, SEEK_CUR);
    fprintf(yamlFile, "%s: %s", key, value);
    fclose(yamlFile);
}

//* writes a child item of a key
void writeChild(char *filename, char *parent, char *key, char *value)
{

    char fullParent[50];
    strcpy(fullParent, parent);
    strcat(fullParent, ":");

    int keyLine = getKeyLine(filename, fullParent);
    if (keyLine == countLines(filename))
    {
        FILE *mainFile = fopen(filename, "a");
        fprintf(mainFile, "\n  %s: %s", key, value);
        return;
    }
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



//* Reads a value from a given YAML file when given a key
//! storeValue may want to be size INT_MAX or just a blank ptr
void *readVal(char *filename, char *key, char **storeValue)
{
    FILE *yamlFile = fopen(filename, "r");
    int foundValue = 0;
    char fullKey[50];
    strcpy(fullKey, key);
    strcat(fullKey, ":");

    while (!foundValue)
    {
        char *currentKey = malloc(sizeof(char) * 50);
        fscanf(yamlFile, "%s", currentKey);
        if (!strcmp(fullKey, currentKey))
        {
            fgets(*storeValue, INT_MAX, yamlFile);
            int valueLength = strlen(*storeValue);
            *(*storeValue + (valueLength-1)) = '\0';

            foundValue = 1;
            free(currentKey);
            fclose(yamlFile);
            return *storeValue;
        }
        if (fgetc(yamlFile) == EOF)
            foundValue = 1;
        fseek(yamlFile, -1, SEEK_CUR);
        char trash[600];
        fgets(trash, 600, yamlFile);
        free(currentKey);
    }
    fclose(yamlFile);
    strcpy(*storeValue, "KEY_NOT_FOUND");
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
        char *currentKey = malloc(sizeof(char) * 50);
        fscanf(file, "%s", currentKey);
        if (!strcmp(currentKey, fullKey))
        {
            free(currentKey);
            return lines;
        }
        else
            lines++;
        
        char trash[600];
        fgets(trash, 600, file);
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


