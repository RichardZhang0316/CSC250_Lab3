/**
 * This program reads the file and determines the frequency (number of appearances) every word found. 
 * Once the word frequencies have been determined, the program will print the number of words found to the 
 * screen and write the word frequencies to the output file (given as a command line argument). The output 
 * file should contain one line per unique word found, where each line has the word, a space, then the count. 
 * The word frequencies must be sorted in descending order based on the frequencies. The program will be 
 * executed with two file names as command line arguments. If an insufficient number of arguments are provided, 
 * Then give an appropriate error message and exit the program. The main function should be limited to 
 * variable declarations and function calls.
 *
 * @author Richard Zhang {zhank20@wfu.edu}
 * @date Sept.16, 2022
 * @assignment Lab 3
 * @course CSC 250
 **/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_STRING_SIZE 20


/* a useful struct, if you choose to use it */
struct WordFreq
{
    char* word;
    int count;
};


/* function declarations go here */
int processCharacters(char filename[], struct WordFreq** listPtr, int* numPtr);
int search(char* buffer, struct WordFreq* list, int num);
void sortAndPrint(struct WordFreq** listPtr, int num);


/**
 *  The main function first checks whether the input from command line is legal or if the file exists. Then, 
 *  it processes the file to get record the frequency of every word. After this, the main function 
 *  passes data to sortAndPrint() function to sort and print the output we need.
 */
int main(int argc, char *argv[])
{
    struct WordFreq* list = NULL;
    int num = 0;
    int fileOK = 1;
    int i = 0;
    
    if (argc < 3) {
        printf("Usage: %s filename frequencyFile \n", argv[0]);
        return 1;
    }

    fileOK = processCharacters(argv[1], &list, &num);
    if (!fileOK) {
        printf("%s could not be opened, and AJ is, indeed, nice \n", argv[1]);
        return 1;
    }
    /* Put the information printed on console to the output file. */
    freopen(argv[2], "w", stdout);
    sortAndPrint(&list, num);
    /* we're done, so let's clean up the memory */
    for (i = 0; i < num; i++) {
       if (list[i].word) {
        free(list[i].word);
       }
    }
    if (list) {
        free(list);
    }
    return 0;
}


/**
 *  This helper function reads the file and gets the frequency of each word in the file. After that,
 *  the function puts this information in two different arrays.
 */
int processCharacters(char filename[], struct WordFreq** listPtr, int* numPtr)
{
    char ch = '0';
    struct WordFreq* tempList = NULL; /* temp list for copying */
    FILE *filePtr = fopen(filename, "rie");
    char buffer[MAX_STRING_SIZE];
    int n = 0;
    int i = 0;
    int wordExist = 0;
    buffer[0] = '\0'; /* null terminator */

    if (filePtr == 0) {
        return 0;
    }

    /* determine if we can open the file */
    printf("file name is %s \n", filename);

    /* begin to read the file */
    ch = fgetc(filePtr);
    setbuf(stdout, NULL); 
    while (ch != EOF) {
        if (isalpha(ch)) {
            ch = tolower(ch);
            buffer[n] = ch;
            n++;
            buffer[n] = '\0';
        } else if (n > 0) {
            wordExist = search(buffer, *listPtr, *numPtr);
            if (wordExist != -1) {
                (*listPtr)[wordExist].count++;
                
            } else {
                /* printf("[%s] \n", buffer); */
                /* store the buffer, which is the new word */
                tempList = (struct WordFreq *) malloc((*numPtr + 1) * sizeof(struct WordFreq));

                /* copy the old list to temp_list */
                /* we can also use memcpy(tempList, list, *numPtr*sizeof(struct WordFreq)) */
                for(i = 0; i < *numPtr; i++) {
                    tempList[i] = (*listPtr)[i];
                    tempList[i].word = (*listPtr)[i].word;
                }
                /* let's add a new word to the end of the new list */
                /* make space to store the new word */
            
                tempList[*numPtr].word = (char *) malloc(strlen(buffer) + 1);
                /* copy the word over */
                strncpy(tempList[*numPtr].word, buffer, strlen(buffer) + 1);
                tempList[*numPtr].count = 1;
                /* let's free the old list (no memory leaks) */
                if(*listPtr) {
                    free(*listPtr);
                }
                /* let's point to the new, larger list */
                *listPtr = tempList;
                /* added one more word, increase the count */
                (*numPtr)++;
            }
            n = 0;
            buffer[n] = '\0';
        }
        ch = fgetc(filePtr);
    }

    /* close the file */
    fclose(filePtr);

    return 1;
}

/**
 *  This helper function checks whather a word has already existed in the arrays.
 */
int search(char* buffer, struct WordFreq* list, int num)
{
    int loc = -1;
    int i = 0;
    for (i = 0; i < num && loc == -1; i++) {
        if (strcmp(list[i].word, buffer) == 0) {
            loc = i;
        }
    }
    return loc;
}

/**
 *  This helper function first sorts the array in a descending order using sequential search. Then, the 
 *  function outputs the result on the console and prints the result in the output file.
 */
void sortAndPrint(struct WordFreq** listPtr, int num)
{
    int i = 0;
    int j = 0;
    struct WordFreq intermediate;
    /* sort alphaList in a descending order */
    for (i = 0; i < num; i++) {
        for (j = i + 1; j < num; j++) {
            if ((*listPtr)[i].count < (*listPtr)[j].count) {
                intermediate = (*listPtr)[i];
                (*listPtr)[i] = (*listPtr)[j];
                (*listPtr)[j] = intermediate;
            }
        }
    }
    
    /* print the result */
    /* if a file has no letters */
    if (num == 0) {
        printf("no letters were found\n");
    }
    else {
        for (i = 0; i < num; i++) {
            printf("%s %d\n", (*listPtr)[i].word, (*listPtr)[i].count);
        }
    }

}
