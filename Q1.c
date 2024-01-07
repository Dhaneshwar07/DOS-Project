#include <stdio.h>
#include<stdio.h>

void fcfsAlgorithm();
void rrAlgorithm();
void calculateAndDisplayTimes(int n, int burstTimes[], int arrivalTimes[]);
void findAvgTime(int processes[], int n, int burstTimes[], int quantum, int arrivalTimes[]);
void findWaitingTime(int processes[], int n, int burstTimes[], int wt[], int quantum, int arrivalTimes[], int ct[], int rt[]);
void findTurnAroundTime(int processes[], int n, int burstTimes[], int wt[], int tat[], int ct[], int arrivalTimes[]);
double calculateFCFSAvgWaitingTime(int burstTimes[], int arrivalTimes[]);
void compareAlgorithmsEfficiency(double avgWaitingTimeRR, double avgWaitingTimeFCFS);

int main()
{
    int fcfsChosen = 0;
    while (1)
    {
        printf("\nChoose the scheduling algorithm:\n");
        printf("1. First-Come, First-Served (FCFS)\n");
        printf("2. Round Robin (RR)\n");
        printf("3. Terminate Program\n");
        int choice;
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            if (!fcfsChosen)
            {
                fcfsAlgorithm();
                fcfsChosen = 1;
            }
            else
            {
                rrAlgorithm();
            }
            break;
        case 2:
            rrAlgorithm();
            break;
        case 3:
            printf("Terminating the program...\n");
            return 0;
        default:
            printf("Invalid choice!\n");
        }
    }
}

void fcfsAlgorithm()
{
    printf("Enter the number of processes: ");
    int n;
    scanf("%d", &n);
    int burstTimes[n];
    int arrivalTimes[n];
    printf("\nEnter the Burst Time for each process.\n");
    for (int i = 0; i < n; i++)
    {
        printf("\nFor Process %d: ", (i + 1));
        scanf("%d", &burstTimes[i]);
    }
    printf("\nEnter the arrival time for each process.\n");
    for (int j = 0; j < n; j++)
    {
        printf("\nFor Process %d: ", (j + 1));
        scanf("%d", &arrivalTimes[j]);
    }
    calculateAndDisplayTimes(n, burstTimes, arrivalTimes);
}

void rrAlgorithm()
{
    printf("Enter the number of processes: ");
    int n;
    scanf("%d", &n);
    int processes[n];
    int burstTimes[n];
    int arrivalTimes[n];
    for (int i = 0; i < n; i++)
    {
        printf("Enter burst time for process %d: ", (i + 1));
        scanf("%d", &burstTimes[i]);
        processes[i] = i + 1;
    }
    for (int i = 0; i < n; i++)
    {
        printf("Enter arrival time for process %d:", (i + 1));
        scanf("%d", &arrivalTimes[i]);
    }
    printf("Enter the time quantum: ");
    int quantum;
    scanf("%d", &quantum);
    findAvgTime(processes, n, burstTimes, quantum, arrivalTimes);
}

void calculateAndDisplayTimes(int n, int burstTimes[], int arrivalTimes[])
{
    int wt[n];
    int rt[n];
    int ct[n];
    wt[0] = 0;
    ct[0] = burstTimes[0];
    for (int i = 1; i < n; i++)
    {
        wt[i] = ct[i - 1] - arrivalTimes[i];
        if (wt[i] < 0)
        {
            wt[i] = 0;
        }
        rt[i] = wt[i];
        ct[i] = ct[i - 1] + burstTimes[i];
    }
    printf("\nProcesses || Burst Time || Arrival Time || Waiting Time || Response Time || Completion Time\n");
    float awt = 0;
    float art = 0;
    float att = 0;
    for (int i = 0; i < n; i++)
    {
        printf("%d\t ||\t%d\t||\t%d\t||\t%d\t||\t%d\t||\t%d\n", (i + 1), burstTimes[i], arrivalTimes[i], wt[i], rt[i], ct[i]);
        awt += wt[i];
        art += rt[i];
        att += (ct[i] - arrivalTimes[i]);
    }
    awt = awt / n;
    art = art / n;
    att = att / n;
    printf("\nAverage waiting time = %f\n", awt);
    printf("\nAverage response time = %f\n", art);
    printf("\nAverage turnaround time = %f\n", att);
}

void findAvgTime(int processes[], int n, int burstTimes[], int quantum, int arrivalTimes[])
{
    int wt[n], tat[n], ct[n], rt[n];
    double total_wt = 0, total_tat = 0, total_rt = 0;
    findWaitingTime(processes, n, burstTimes, wt, quantum, arrivalTimes, ct, rt);
    findTurnAroundTime(processes, n, burstTimes, wt, tat, ct, arrivalTimes);
    printf("Processes Burst time Waiting time Turnaround time Response time\n");
    for (int i = 0; i < n; i++)
    {
        total_wt += wt[i];
        total_tat += tat[i];
        total_rt += rt[i];
        printf("%d\t\t%d\t %d\t\t %d\t\t %d\n", processes[i], burstTimes[i], wt[i], tat[i], rt[i]);
    }
    printf("Average waiting time = %f\n", total_wt / n);
    printf("Average turnaround time = %f\n", total_tat / n);
    printf("Average response time = %f\n", total_rt / n);

    compareAlgorithmsEfficiency(total_wt / n, calculateFCFSAvgWaitingTime(burstTimes, arrivalTimes));
}

void findWaitingTime(int processes[], int n, int burstTimes[], int wt[], int quantum, int arrivalTimes[], int ct[], int rt[])
{
    int rem_bt[n];
    for (int i = 0; i < n; i++)
        rem_bt[i] = burstTimes[i];
    int t = 0;
    int visited[n];
    for (int i = 0; i < n; i++)
        visited[i] = 0;
    while (1)
    {
        int done = 1;
        for (int i = 0; i < n; i++)
        {
            if (rem_bt[i] > 0 && arrivalTimes[i] <= t)
            {
                done = 0;
                if (!visited[i])
                {
                    rt[i] = t - arrivalTimes[i];
                    visited[i] = 1;
                }
                if (rem_bt[i] > quantum)
                {
                    t += quantum;
                    rem_bt[i] -= quantum;
                }
                else
                {
                    t += rem_bt[i];
                    wt[i] = t - burstTimes[i] - arrivalTimes[i];
                    rem_bt[i] = 0;
                    ct[i] = t;
                }
            }
        }
        if (done)
            break;
    }
}

void findTurnAroundTime(int processes[], int n, int burstTimes[], int wt[], int tat[], int ct[], int arrivalTimes[])
{
    for (int i = 0; i < n; i++)
        tat[i] = ct[i] - arrivalTimes[i];
}

double calculateFCFSAvgWaitingTime(int burstTimes[], int arrivalTimes[])
{
    int n = 100;
    int wt[n];
    int ct[n];
    int prevCT = 0;
    for (int i = 0; i < n; i++)
    {
        wt[i] = prevCT - arrivalTimes[i];
        if (wt[i] < 0)
        {
            wt[i] = 0;
        }
        ct[i] = prevCT + burstTimes[i];
        prevCT = ct[i];
    }
    double total_wt = 0;
    for (int i = 0; i < n; i++)
    {
        total_wt += wt[i];
    }
    return total_wt / n;
}

void compareAlgorithmsEfficiency(double avgWaitingTimeRR, double avgWaitingTimeFCFS)
{
    if (avgWaitingTimeRR < avgWaitingTimeFCFS)
    {
        printf("Round Robin (RR) algorithm results in the minimum average waiting time = %f\n", avgWaitingTimeRR);
    }
    else if (avgWaitingTimeRR > avgWaitingTimeFCFS)
    {
        printf("FCFS algorithm results in the minimum average waiting time = %f\n", avgWaitingTimeFCFS);
    }
    else
    {
        printf("Both algorithms have the same average waiting time.\n");
    }
}


