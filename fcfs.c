#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct{
 int iPID;
 int iArrival, iBurst;
 int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void inputProcess(int n, PCB P[]) {
    for (int i = 0; i < n; i++) {
        P[i].iPID = i + 1;
        printf("Arrival Time and Burst Time of Process %d: ", P[i].iPID);
        scanf("%d %d", &P[i].iArrival, &P[i].iBurst);
    }
}
void printProcess(int n, PCB P[]) {
    for (int i = 0; i < n; i++) 
        P[i].iStart, P[i].iFinish,
        P[i].iWaiting, P[i].iResponse, P[i].iTaT;
}
void exportGanttChart (int n, PCB P[]) {
    printf("\nGantt Chart:\n");
    for (int i = 0; i < n; i++)
        printf("%d-%d: P%d\n", P[i].iStart, P[i].iFinish, P[i].iPID);
}
void pushProcess(int *n, PCB P[], PCB Q) {
    P[*n] = Q;
    (*n)++;
}
void removeProcess(int *n, int index, PCB P[]) {
    for (int i = index; i < *n - 1; i++) 
        P[i] = P[i + 1];
    (*n)--;
}

int swapProcess(PCB *P, PCB *Q) {
    PCB temp = *P;
    *P = *Q;
    *Q = temp;
    return 0;
}
int partition (PCB P[], int low, int high, int iCriteria) {
    PCB pivot = P[high];
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        int iCondition = 0;
        if (iCriteria == SORT_BY_ARRIVAL) 
            iCondition = (P[j].iArrival < pivot.iArrival) ||  (P[j].iArrival == pivot.iArrival && P[j].iPID < pivot.iPID);
        else if (iCriteria == SORT_BY_PID) 
            iCondition = (P[j].iPID < pivot.iPID);
        else if (iCriteria == SORT_BY_BURST)
            iCondition = (P[j].iBurst < pivot.iBurst) ||
                         (P[j].iBurst == pivot.iBurst && P[j].iPID < pivot.iPID);
        else if (iCriteria == SORT_BY_START)
            iCondition = (P[j].iStart < pivot.iStart) ||
                         (P[j].iStart == pivot.iStart && P[j].iPID < pivot.iPID);
        if (iCondition) {
            i++;
            swapProcess(&P[i], &P[j]);
        }
    }
    swapProcess(&P[i + 1], &P[high]);
    return i + 1;
}
void quickSort(PCB P[], int low, int high, int iCriteria) {
    if (low < high) {
        int pi = partition(P, low, high, iCriteria);
        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
}

void calculateAWT(int n, PCB P[]) {
    int sum = 0;
    for (int i = 0; i < n; i++) 
        sum += P[i].iWaiting;
    printf("Average Waiting Time: %.2f\n", (float)sum / n);
}

void calculateATaT(int n, PCB P[]) {
    int sum = 0;
    for (int i = 0; i < n; i++) 
        sum += P[i].iTaT;
    printf("Average Turnaround Time: %.2f\n", (float)sum / n);
}


int main() {
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10]; 
    int iNumberOfProcess;



    
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    inputProcess(iNumberOfProcess, Input);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    
    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);  
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;    //Thời gian đáp ứng
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;        //Thời gian hoàn thành
    printf("\nReady Queue: ");
    printProcess(1, ReadyQueue);

    while (iTerminated < iNumberOfProcess) {
        while (iRemain > 0) {
            if (Input[0].iArrival <= ReadyQueue[0].iFinish) {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
                continue;
            }
            else break;
        }
        if (iReady > 0) {
            pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue);

            if (iReady > 0) {
                ReadyQueue[0].iStart = TerminatedArray[iTerminated - 1].iFinish;
                ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
                ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
                ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
                ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
            }
            else if (iRemain > 0) {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
                ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
                ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
                ReadyQueue[0].iResponse = 0;
                ReadyQueue[0].iWaiting = 0;
                ReadyQueue[0].iTaT = ReadyQueue[0].iBurst;
            }
        }
    }

    printf("\n===== FCFS Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray);
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);

    return 0;
}