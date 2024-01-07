#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

typedef struct {
    int max[MAX_PROCESSES][MAX_RESOURCES];
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int need[MAX_PROCESSES][MAX_RESOURCES];
    int available[MAX_RESOURCES];
    int numOfProcesses;
    int numOfResources;
} BankersAlgorithm;

void initialize(BankersAlgorithm *banker, int max[][MAX_RESOURCES], int allocation[][MAX_RESOURCES], int available[], int numOfProcesses, int numOfResources) {
    for (int i = 0; i < numOfProcesses; i++) {
        for (int j = 0; j < numOfResources; j++) {
            banker->max[i][j] = max[i][j];
            banker->allocation[i][j] = allocation[i][j];
            banker->need[i][j] = max[i][j] - allocation[i][j];
        }
    }
    for (int i = 0; i < numOfResources; i++) {
        banker->available[i] = available[i];
    }
    banker->numOfProcesses = numOfProcesses;
    banker->numOfResources = numOfResources;
}

bool isSafeState(BankersAlgorithm *banker) {
    int work[MAX_RESOURCES];
    for (int i = 0; i < banker->numOfResources; i++) {
        work[i] = banker->available[i];
    }
    bool finish[MAX_PROCESSES] = {false};
    int count = 0;
    while (count < banker->numOfProcesses) {
        bool found = false;
        for (int i = 0; i < banker->numOfProcesses; i++) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < banker->numOfResources; j++) {
                    if (banker->need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }
                if (canAllocate) {
                    for (int j = 0; j < banker->numOfResources; j++) {
                        work[j] += banker->allocation[i][j];
                    }
                    finish[i] = true;
                    count++;
                    found = true;
                }
            }
        }
        if (!found) {
            break;
        }
    }
    return count == banker->numOfProcesses;
}

int requestResources(BankersAlgorithm *banker, int processNum, int request[]) {
    for (int i = 0; i < banker->numOfResources; i++) {
        if (request[i] > banker->available[i] || request[i] > banker->need[processNum][i]) {
            return -1;
        }
    }
    for (int i = 0; i < banker->numOfResources; i++) {
        banker->available[i] -= request[i];
        banker->allocation[processNum][i] += request[i];
        banker->need[processNum][i] -= request[i];
    }
    if (!isSafeState(banker)) {
        for (int i = 0; i < banker->numOfResources; i++) {
            banker->available[i] += request[i];
            banker->allocation[processNum][i] -= request[i];
            banker->need[processNum][i] += request[i];
        }
        return 0;
    }
    return 1;
}

void getSafeSequence(BankersAlgorithm *banker, int safeSeq[], int *size) {
    int work[MAX_RESOURCES];
    for (int i = 0; i < banker->numOfResources; i++) {
        work[i] = banker->available[i];
    }
    bool finish[MAX_PROCESSES] = {false};
    *size = 0;
    for (int k = 0; k < banker->numOfProcesses; k++) {
        for (int i = 0; i < banker->numOfProcesses; i++) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < banker->numOfResources; j++) {
                    if (banker->need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }
                if (canAllocate) {
                    for (int j = 0; j < banker->numOfResources; j++) {
                        work[j] += banker->allocation[i][j];
                    }
                    safeSeq[*size] = i;
                    (*size)++;
                    finish[i] = true;
                }
            }
        }
    }
}

int main() {
    int numOfProcesses, numOfResources;
    printf("Enter the number of processes: ");
    scanf("%d", &numOfProcesses);
    printf("Enter the number of resources: ");
    scanf("%d", &numOfResources);
    int max[MAX_PROCESSES][MAX_RESOURCES];
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int available[MAX_RESOURCES];
    printf("Enter the Max matrix:\n");
    for (int i = 0; i < numOfProcesses; i++) {
        for (int j = 0; j < numOfResources; j++) {
            scanf("%d", &max[i][j]);
        }
    }
    printf("Enter the Allocation matrix:\n");
    for (int i = 0; i < numOfProcesses; i++) {
        for (int j = 0; j < numOfResources; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }
    printf("Enter the Available resources:\n");
    for (int i = 0; i < numOfResources; i++) {
        scanf("%d", &available[i]);
    }
    BankersAlgorithm banker;
    initialize(&banker, max, allocation, available, numOfProcesses, numOfResources);
    printf("Need Matrix:\n");
    for (int i = 0; i < banker.numOfProcesses; i++) {
        for (int j = 0; j < banker.numOfResources; j++) {
            printf("%d ", banker.need[i][j]);
        }
        printf("\n");
    }
    int safeSeq[MAX_PROCESSES];
    int size;
    getSafeSequence(&banker, safeSeq, &size);
    if (size == banker.numOfProcesses) {
        printf("The system is in a safe state.\n");
        printf("Safe sequence: ");
        for (int i = 0; i < size; i++) {
            printf("%d ", safeSeq[i]);
        }
        printf("\n");
    } else {
        printf("The system is not in a safe state.\n");
    }
    printf("Enter the number of instances of type R2 that P3 wants to request: ");
    int requestR2;
    scanf("%d", &requestR2);
    int processNum = 2;
    int request[MAX_RESOURCES] = {0, requestR2, 0, 0};
    int result = requestResources(&banker, processNum, request);
    if (result == 1) {
        printf("Request can be granted immediately.\n");
    } else if (result == 0) {
        printf("Request denied as it leads to an unsafe state.\n");
    } else {
        printf("Requested resources exceed available or need.\n");
    }
    return 0;
}


