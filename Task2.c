#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FitnessDataStruct.h"

#define MAX_RECORDS 1000
char filename[256];
FITNESS_DATA fitnessDataArray[MAX_RECORDS];
int recordCount = 0;

void tokeniseRecord(const char *input, const char *delimiter,
                    char *date, char *time, int *steps) {
    char *inputCopy = strdup(input);
    char *token = strtok(inputCopy, delimiter);
    if (token != NULL){
        strcpy(date, token);
    }

    token = strtok(NULL, delimiter);
    if (token != NULL) {
        strcpy(time, token);
    }

    token = strtok(NULL, delimiter);
    if (token != NULL) {
        *steps= atoi(token);
    }

    free(inputCopy);
}


void displayMenu();
int loadData();
void displayTotalRecords();
void findFewestSteps();
void findLargestSteps();
void findMeanStepCount();
void findLongestPeriod();

int main() {
    char choice;

    do {
        displayMenu();
        scanf(" %c", &choice);

        switch(choice) {
            case 'A':
            case 'a':
                printf("Input filename: ");
                scanf("%s", filename);
                if(loadData() != 0) {
                    printf("Error: could not open file\n");
                    return 1;
                }
                break;
            case 'B':
            case 'b':
                displayTotalRecords();
                break;
            case 'C':
            case 'c':
                findFewestSteps();
                break;
            case 'D':
            case 'd':
                findLargestSteps();
                break;
            case 'E':
            case 'e':
                findMeanStepCount();
                break;
            case 'F':
            case 'f':
                findLongestPeriod();
                break;
            case 'Q':
            case 'q':
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid option. Please try again.\n");
        }
    } while(choice != 'Q' && choice != 'q');

    return 0;
}

void displayMenu() {
    printf("\nMenu:\n");
    printf("A: Specify filename\n");
    printf("B: Display total number of records\n");
    printf("C: Fewest steps\n");
    printf("D: Largest steps\n");
    printf("E: Mean step count\n");
    printf("F: Longest period over 500 steps\n");
    printf("Q: Quit\n");
    printf("Select an option: ");
}

int loadData() {
    FILE *file = fopen(filename, "r");
    char line[100];

    if (file == NULL) {
        return 1; 
    }

    recordCount = 0; 

    while (fgets(line, sizeof(line), file) && recordCount < MAX_RECORDS) {
        tokeniseRecord(line, ",", 
               fitnessDataArray[recordCount].date,
               fitnessDataArray[recordCount].time,
               &fitnessDataArray[recordCount].steps);

        recordCount++;
    }

    fclose(file);
    return 0; 
}
    

void displayTotalRecords() {
    printf("Total records: %d\n", recordCount); 
}

void findFewestSteps() {
    if (recordCount == 0) {
        printf("No records available.\n");
        return;
    }

    int minIndex = 0; 

    for (int i = 1; i < recordCount; i++) {
        if (fitnessDataArray[i].steps < fitnessDataArray[minIndex].steps) {
            minIndex = i; 
        }
    }

    printf("Fewest steps: %s %s\n", fitnessDataArray[minIndex].date, fitnessDataArray[minIndex].time);
}


void findLargestSteps() {
    if (recordCount == 0) {
        printf("No records available.\n");
        return;
    }

    int maxIndex = 0; 

    for (int i = 1; i < recordCount; i++) {
        if (fitnessDataArray[i].steps > fitnessDataArray[maxIndex].steps) {
            maxIndex = i; 
        }
    }

    printf("Largest steps: %s %s\n", fitnessDataArray[maxIndex].date, fitnessDataArray[maxIndex].time);
}
  

void findMeanStepCount() {
    if (recordCount == 0) {
        printf("No records available.\n");
        return;
    }

    int totalSteps = 0;
    for (int i = 0; i < recordCount; i++) {
        totalSteps += fitnessDataArray[i].steps;
    }

    int meanSteps = totalSteps / recordCount; 

    printf("Mean step count: %d\n", meanSteps);
}
    
void findLongestPeriod() {
    if (recordCount == 0) {
        printf("No records available.\n");
        return;
    }

    int longestStartIndex = -1;
    int longestEndIndex = -1;
    int currentStartIndex = -1;
    int longestDuration = 0;

    for (int i = 0; i < recordCount; i++) {
        if (fitnessDataArray[i].steps > 500) {
            if (currentStartIndex == -1) {
                currentStartIndex = i; 
            }
        } else {
            if (currentStartIndex != -1) {
                int currentDuration = i - currentStartIndex;
                if (currentDuration > longestDuration) {
                    longestDuration = currentDuration;
                    longestStartIndex = currentStartIndex;
                    longestEndIndex = i - 1;
                }
                currentStartIndex = -1; 
            }
        }
    }

    if (currentStartIndex != -1) {
        int currentDuration = recordCount - currentStartIndex;
        if (currentDuration > longestDuration) {
            longestStartIndex = currentStartIndex;
            longestEndIndex = recordCount - 1;
        }
    }

    if (longestStartIndex == -1) {
        printf("No period found where the step count is above 500 steps.\n");
    } else {
        printf("Longest period start: %s %s\n", fitnessDataArray[longestStartIndex].date, fitnessDataArray[longestStartIndex].time);
        printf("Longest period end: %s %s\n", fitnessDataArray[longestEndIndex].date, fitnessDataArray[longestEndIndex].time);
    }
}