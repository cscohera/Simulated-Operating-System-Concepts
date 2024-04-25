// Carson Scohera
// 4/11/24
// Assignment 8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

//globals needed
#define BANKER 3
#define PROCESS 5
#define RESOURCE 3
#define TRUE 1
#define FALSE 0
#define EXIT 0
#define INVALID -1
#define CPU 1
#define PID 5
#define RACE 2
#define MEMORY 4
#define FIRST 0
#define BEST 1
#define WORST 2
#define NEXT 3
#define PAGE 5
#define FIFO 0
#define LRU 1
#define FRAME 4
#define DISK 6
#define FCFS 0
#define SSTF 1
#define REQUEST 8

int resource = 5;

// Function prototypes
int findLRU(int time[]);
int displayMenu();
void* threadFuncOne();
void* threadFuncTwo();
void cpuScheduling();
void raceCondition();
void bankersAlgorithm();
void systemState(int feasible[], int safe[]);
void fcfs(int process[], int at[], int bt[]);
void sjf(int process[], int at[], int bt[]);
void displaySchedule(int process[], int at[], int bt[], int wt[], int tat[]);
void memoryManagement();
void firstFit(int blockSize[], int blocks, int processSize[], int processes);
void bestFit(int blockSize[], int blocks, int processSize[], int processes);
void worstFit(int blockSize[], int blocks, int processSize[], int processes);
void nextFit(int blockSize[], int blocks, int processSize[], int processes);
void displayProcess(int allocation[], int processes, int processSize[]);
void pageReplacement();
void fifo();
void lru();
void displayPages(int page, int allocation[]);
void diskScheduling();
void diskFcfs(int requests[], int head);
void diskSstf(int requests[], int head);

//determines if user choice is valid or invaild and loops while invaild is true based on input in displayMenu function
int main() {

    int choice = INVALID;

    while (choice != EXIT) {
       
       //enters displaymenu function
       choice = displayMenu();

        //if choice is 1 then does the cpuScheduling
        if (choice == 1)
            cpuScheduling();

        //if choice is 2 then does raceCondition
        if (choice == 2)
            raceCondition();
        
        //if choice is 3 then do bankersAlgortihm
        if (choice == 3)
            bankersAlgorithm();

        //if choice is 4 then do memoryManagement
        if (choice == 4)
            memoryManagement();
        
        //if choice is 5 then do pageReplacement
        if (choice == 5)
            pageReplacement();
        
        //if choice is 6 then do diskScheduling
        if (choice == 6)
            diskScheduling();
        
        //if choice is 0 then we exit program
        if (choice == 0)
            exit(0);
    }

    return EXIT;
}

//displaymenu function displays prompts and asks for userinput 
int displayMenu() {
    int choice = INVALID;
    while (choice == INVALID) {
        printf("              ********** Operating System Managment Menu **********\n\n");
        printf("Select the OS program to run, enter the number of your selection\n");
        printf("1. CPU Scheduling\n");
        printf("2. Race Condition\n");
        printf("3. Banker's Algorithm\n");
        printf("4. Memory Management\n");
        printf("5. Page Replacement\n");
        printf("6. Disk Scheduling\n");
        printf("0. Exit\n");
        scanf("%d", &choice);

        //if choice is not 0 to 6 user choice stays invaild and keeps looping until input is a valid choice of 6 to 0
        if (choice < 0 || choice > 6) {
            choice = INVALID;
        }
    }
    return choice;
}

//function that creates neccessary arrays to compute scheduling and then takes arrays to each scheduling function to compute
void cpuScheduling() {
    int process[PID] = {1, 2, 3, 4, 5};
    int arrvTime[PID] = {0, 2, 4, 6, 7};
    int burstTime[PID] = {8, 5, 10, 2, 3};

    //goes to FCFS function
    fcfs(process, arrvTime, burstTime);
    //goes to SJF function
    sjf(process, arrvTime, burstTime);
}

//calcuates scheduling for FCFS
void fcfs(int process[], int at[], int bt[]) {
    int wt[PID], tat[PID];

    //calculate wait times
    wt[0] = 0;
    for (int i = 1; i < PID; i++) {
        wt[i] = wt[i - 1] + bt[i - 1];
    }

    //calculate turnaround times
    for (int i = 0; i < PID; i++) {
        tat[i] = bt[i] + wt[i];
    }

    //displays result
    printf("****************************** FCFS ******************************\n\n");
    displaySchedule(process, at, bt, wt, tat);
}

void sjf(int process[], int at[], int bt[]) {
    int wt[PID], tat[PID], temp;

    //sort processes based on burst time 
    for (int i = 0; i < PID; i++) {
        int idx = i;
        for (int j = i + 1; j < PID; j++) {
            if (bt[j] < bt[idx]) {
                idx = j;
            }
        }

        //swap burst times
        temp = bt[i];
        bt[i] = bt[idx];
        bt[idx] = temp;

        //swap process IDs
        temp = process[i];
        process[i] = process[idx];
        process[idx] = temp;

        //swap arrival times
        temp = at[i];
        at[i] = at[idx];
        at[idx] = temp;
    }

    //calculate wait times
    wt[0] = 0;
    for (int i = 1; i < PID; i++) {
        wt[i] = wt[i - 1] + bt[i - 1];
    }
    //calculate turn around times
    for (int i = 0; i < PID; i++) {
        tat[i] = bt[i] + wt[i];
    }
    //displays result
    printf("****************************** SJF *******************************\n\n");
    displaySchedule(process, at, bt, wt, tat);
}

//displays each schedling algorithims results taking in all the neccessary data in order to print said results
void displaySchedule(int process[], int at[], int bt[], int wt[], int tat[]) {
    int totWt = 0, totTat = 0;
    float avgWt, avgTat;

    printf("PID\t        AT\t        BT\t        WT\t       TAT\n");
    printf("---            ---             ---             ---             ---\n");
    for (int i = 0; i < PID; i++) {
        totWt += wt[i];
        totTat += tat[i];
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", process[i], at[i], bt[i], wt[i], tat[i]);
    }
    //calculates avgs
    avgWt = (float)totWt / PID;
    avgTat = (float)totTat / PID;
    printf("\nAverage Waiting Time = %.2f\n", avgWt);
    printf("Average Turnaround Time = %.2f\n\n", avgTat);
    
}

//race condition simulated in which threadFuncOne and threadFuncTwo are called to perform at the same time
//to do this I call both functions both using resource and can then see the outcome of doing this
void raceCondition(){
    pthread_t threadOne;
    pthread_t threadTwo;

    pthread_create(&threadOne, NULL, threadFuncOne, NULL);
    pthread_create(&threadTwo, NULL, threadFuncTwo, NULL);
    pthread_join(threadOne, NULL);
    pthread_join(threadTwo, NULL);
    printf("Value of shared resource is %d\n", resource);
}

//this function simulates thread one attempting to access and change resource
void* threadFuncOne(){
    int threadId = 1;
    int fOne = resource;
    printf("Thread 1 reads the value of shared resource as %d\n", fOne);
    fOne = fOne + 1;
    printf("Local update  by Thread 1: %d\n", fOne);
    sleep(1);
    resource = fOne;
    printf("Value of shared resource updated by Thread 1 is: %d\n", resource);
    return 0;
}

//this function simulates thread two attempting to access and change resource
void* threadFuncTwo(){
    int threadId = 2;
    int fTwo = resource;
    printf("Thread 2 reads resource as %d\n", fTwo);
    fTwo = fTwo - 1;
    printf("Local update by Thread 2: %d\n", fTwo);
    sleep(1);
    resource = fTwo;
    printf("Value of shared resource ipdated by Thread 2 is: %d\n", resource);
    return 0;
}

//function that as said below checks to make sure there are are enough resources for allocations
void bankersAlgorithm(){
    //defining all varibles needed to allow Banker's alg to decide if there are enough resources
    int allocation[PROCESS][RESOURCE] = {{0,0,2} , {3,0,2} , {0,1,0} , {2,1,1} , {2,0,0}};
    int maxDemand[PROCESS][RESOURCE] = {{4,3,3} , {9,0,2} , {7,5,3} , {2,2,2} , {3,2,2}};
    int available[RESOURCE] = {2,4,6};
    int need[PROCESS][RESOURCE];
    int feasible[PROCESS];
    int safe[PROCESS];
    int safeIdx = 0;

    for (int p = 0; p < PROCESS; p++){
        feasible[p] = FALSE;
        for (int r = 0; r < RESOURCE; r++){
            need[p][r] = maxDemand[p][r] - allocation[p][r];
        }
    }

    //loops that check Banker's algorithm which checks to make sure there are enough resources for allocations
    for (int i = 0; i < PROCESS; i++){
        for (int p = 0; p < PROCESS; p++){
            //if unsafe flags feasible to true
            if(feasible[p] == FALSE){
                int isUnsafe = FALSE;
                for (int r = 0; r < RESOURCE; r++){
                    if(need[p][r] > available[r]){
                        isUnsafe = TRUE;
                        break;
                    }
                }
                //if safe increments safe and flags feasible to true
                if(isUnsafe == FALSE){
                        safe[safeIdx++] = p;
                    
                    for(int r = 0; r < RESOURCE; r++){
                    available[r] = available[r] + allocation[p][r];
                    feasible[p] = TRUE;
                    }
                }
            }

        }
    }
    //can now determine if safe and feasible
    systemState(feasible, safe);

}

//systemState function that prints output based on if the operating system is safe or not
void systemState(int feasible[], int safe[]){
    int isSafe = TRUE;
    printf("\n");
    //if feasible runs and finds a vaule equal to zero then the operating system is not safe and ends
    for (int i = 0; i < PROCESS; i++){
        if(feasible[i] == FALSE){
            printf("The operating system is not safe\n");
            break;
        }
    }
    //if feasible runs and feasible does not find any values that are false we can print the safe resource allocation sequence
    if(isSafe == TRUE){
            printf("Safe resource allocation to process sequence\n");
            for (int j = 0; j < PROCESS; j++){
                if (j == PROCESS - 1)
                    printf("P%d ", safe[j]);
                else
                    printf("P%d -> ", safe[j]);
                
            }
            printf("\n\n");
        }
}

//function that goes through different memory management applications
void memoryManagement(){
   
    printf("\n****** Memory Management ******\n\n\n");

    //loops through each memory management application calling the function for its application
    for (int algorithm = 0; algorithm < 4; algorithm++){

        int blockSize[] = {70, 20, 45, 65, 40, 80};
        int processSize[] = {15, 35, 25, 45, 60, 20};
        int blocks = 6;
        int processes = 6;

        if(algorithm == FIRST)
            firstFit(blockSize, blocks, processSize, processes);

        if(algorithm == BEST)
            bestFit(blockSize, blocks, processSize, processes);

        if(algorithm == WORST)
            worstFit(blockSize, blocks, processSize, processes);
        
        if(algorithm == NEXT)
            nextFit(blockSize, blocks, processSize, processes);

    }


}

//first fit implementation of memory management
void firstFit(int blockSize[], int blocks, int processSize[], int processes){
    int allocation[processes];
    
    //allocates processes in memory based on the first possible block large enough to hold the process
    memset(allocation, INVALID, sizeof(allocation));
    printf("\n***** First Fit *****\n\n");
    for (int i = 0; i < processes; i++){
        for (int j = 0; j < blocks; j++){
            if (blockSize[j] >= processSize[i]){
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
        }
       
    }
    displayProcess(allocation, processes, processSize);
    
}

//best fit implementation of memory management
void bestFit(int blockSize[], int blocks, int processSize[], int processes){
    int allocation[processes];
    
    //allocates processes in memory based on the minimum possible block large enough to hold the process
    memset(allocation, INVALID, sizeof(allocation));
    printf("\n***** Best Fit *****\n\n");
    for (int i = 0; i < processes; i++){
        int bestIdx = INVALID;
        for (int j = 0; j < blocks; j++){
            if(blockSize[j] >= processSize[i]){
                if(bestIdx == INVALID)
                    bestIdx = j;
                else if(blockSize[bestIdx] >= blockSize[j])
                    bestIdx = j;
                
            }
            
        }
        if(bestIdx != INVALID){
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
            
    }
    
    displayProcess(allocation, processes, processSize);
}

//worst fit implementation of memory management
void worstFit(int blockSize[], int blocks, int processSize[], int processes){
    int allocation[processes];
    
    //allocates processes in memory based on the largest possible block large enough to hold the process
    memset(allocation, INVALID, sizeof(allocation));
    printf("\n***** Worst Fit *****\n\n");
    for (int i = 0; i < processes; i++){
        int wstIdx = INVALID;
        for (int j = 0; j < blocks; j++){
            if(blockSize[j] >= processSize[i]){
                if(wstIdx == INVALID){
                    wstIdx = j;
                }
                else if(blockSize[wstIdx] < blockSize[j]){
                    wstIdx = j; 
                    }
                }
            }
            if(wstIdx != INVALID){
                allocation[i] = wstIdx;
                blockSize[wstIdx] -= processSize[i];
        }
            
    }
        displayProcess(allocation, processes, processSize);
}

//next fit implementation of memory management
void nextFit(int blockSize[], int blocks, int processSize[], int processes){
    int allocation[processes];
    int id = 0;

    //allocates processes in memory based on the next possible block after allocating the previous process
    memset(allocation, INVALID, sizeof(allocation));
    printf("\n***** Next Fit *****\n\n");
    for (int i = 0; i < processes; i++){
        while (id < blocks){
            if (blockSize[id] >= processSize[i]){
                allocation[i] = id;
                blockSize[id] -= processSize[i];
                break;
            }
            
            id = (id+1) % blocks;
        }


        
    }
        displayProcess(allocation, processes, processSize);
}

//displays each type of memory management and how it allocates the blocks of memeory we initalizsed 
void displayProcess(int allocation[], int processes, int processSize[]){

     printf("Process No.\tProcess Size.\tBlock No.\n");
    for (int i = 0; i < processes; i++){
        printf("%d\t\t%d\t\t", i + 1, processSize[i]);
        if(allocation[i] == INVALID)
            printf("Not Allocated\n");
        else
            printf("%d\n", allocation[i]+1);
    }
}

//function that runs both page replacement algorithms
void pageReplacement(){

    printf("********** Page Replacement *********\n\n");
    for (int i = 0; i < 2; i++){
        if (i == FIFO)
            fifo();
        if (i == LRU)
            lru();
    }
}

//function that runs first in first out page replacement alogrithm 
void fifo(){

    printf("\n\n********* First In First Out *********\n\n");
    printf("Page Frame 1 Frame 2 Frame 3 Frame 4\n");

    //pages requested
    int pageRequests[] = {2,3,8,4,5,6,5,7,1,8,3,1,4,2,6};
    int pageFaults = 0;
    int allocation[FRAME];
    int present;
    int pages = sizeof(pageRequests)/sizeof(pageRequests[0]);
    memset(allocation, INVALID, sizeof(allocation));
    //runs for the amount of pages
    for (int i = 0; i < pages; i++){
        present = 0;
        //if page can be allocated
        for (int j = 0; j < FRAME; j++){
            if (pageRequests[i] == allocation[j]){
                present++;
                pageFaults--;
                
            }
        }   
            //increments pagefaults
            pageFaults++;

            //if pageFaults is less than the frame or present = 0 run these
            if (pageFaults <= FRAME && present == 0){
                allocation[i] = pageRequests[i];
            } else if (present == 0) {
                allocation[(pageFaults - 1) % FRAME] = pageRequests[i];
            }

        //displays pages
        displayPages(pageRequests[i], allocation);
    }

    //print page faults
    printf("\nTotal Page Faults = %d\n", pageFaults);
}

// runs the least recently used algorithm for pages
void lru(){

    printf("\n\n********* Least Recently Used *********\n\n");
    printf("Page Frame 1 Frame 2 Frame 3 Frame 4\n");

    //pages requested
    int pageRequests[] = {2,3,8,4,5,6,5,7,1,8,3,1,4,2,6};
    int pageFaults = 0;
    int allocation[FRAME];
    int pages = sizeof(pageRequests)/sizeof(pageRequests[0]);
    int counter;
    int time[10];
    int flag1;
    int flag2;
    int position = 0;
    memset(allocation, INVALID, sizeof(allocation));
    //runs for the number of pages
    for (int i = 0; i < pages; i++){
        //uses flags to determine if page faults occur or not
        flag1 = 0;
        flag2 = 0;
        
        //if pagerequests is allocated
        for (int j = 0; j < FRAME; j++){
            if (pageRequests[i] == allocation[j]){
                counter++;
                time[j] = counter;
                flag1 = 1;
                flag2 = 1;
                break;
            }
        }

        //pagefault incremented if not allocated
        if (flag1 == 0){
            for (int j = 0; j < FRAME; j++){
                if (allocation[j] == INVALID){
                    counter++;
                    pageFaults++;
                    allocation[j] = pageRequests[i];
                    time[j] = counter;
                    flag2 = 1;
                    break;
                }
            }
        }

        //pagefault incremented if not allocated but also runs to find the LRU
        if (flag2 == 0){
            position = findLRU(time);
            counter++;
            pageFaults++;
            allocation[position] = pageRequests[i];
            time[position] = counter;
        }

        //display pages
        displayPages(pageRequests[i], allocation);
    }

    //print pages faults
    printf("\nTotal Page Faults = %d\n", pageFaults);
}

//finds the LRU based on time array
int findLRU(int time[]){
    int position = 0;
    int minimum = time[0];
    for (int i = 0; i < FRAME; i++){
        if (time[i] < minimum){
            minimum = time[i];
            position = i;
        }
    }

    return position;
}

//displays the pages
void displayPages(int page, int allocation[]){
        printf("%d\t", page);
        for (int i = 0; i < FRAME; i++){
            if (allocation[i] == INVALID)
                printf("-\t");
            else
                printf("%d\t", allocation[i]);
        }
        printf("\n");
}

//logic that does disk scheduling for both disk scheduling algorithms
void diskScheduling(){
    printf("********* Disk Scheduling **********\n\n");
   int requests[] = {146,89,24,70,102,13,51,134};
   int head = 50;
   for (int i = 0; i < 2; i++){
        if (i == FCFS)
            diskFcfs(requests,head);
        if (i == SSTF)
            diskSstf(requests,head);
   }
}

//disk scheduling algorithm First Come First Serve
void diskFcfs(int requests[], int head){
    
    //varibles needed to set up the function
    printf("********* FCFS **********\n\n");
    int seek = 0;
    int track = 0;
    int distance = 0;
    int start = head;

    //alogrithm services requests in the order they arrive in the disk queue
    for (int i = 0; i < REQUEST; i++){
        track = requests[i];
        distance = abs(head - track);
        seek += distance;
        head = track;
    }
    //print the head request
        printf("%d-> ", start);

    //prints rest of requests
    for (int i = 0; i < REQUEST; i++){
        if (i == REQUEST -1)
            printf("%d", requests[i]);
        else
            printf("%d -> ", requests[i]);
    }
        //total seek ops
        printf("\nTotal seek operations: %d\n", seek);
}

//disk scheduling algorithm Shortest Seek Time First
void diskSstf(int requests[], int head){

    //varibles needed to set up the function
    printf("\n********* SSTF **********\n\n");
    int sequence[REQUEST];
    int distance[REQUEST];
    int seek = 0;
    int start = head;
    int minVal = 0;
    int minValIdx = 0;
    int seqIdx = 0;

    //serves the request which is closest to the current position of head or pointer
    for (int i = 0; i < REQUEST; i++){

        for (int j = 0; j < REQUEST; j++){
            distance[j] = abs(head - requests[j]);
            minVal = distance[0];
            minValIdx =  0;
        }
        //need min val for each request
        minVal = distance[i];

        for (int k = 0; k < REQUEST; k++){
        if (minVal > distance[k]){
            minVal = distance[k];
            minValIdx = k;
            sequence[seqIdx] = requests[k];
            }
        }
            //updates per request 
            seqIdx++;
            head = requests[minValIdx];
            requests[minValIdx] = 999; 

    }
        //printing out head request and remaining requests
        printf("%d -> ", start);
       seek = seek + abs(start - sequence[0]);
       printf("%d -> ", sequence[0]);
       for(int i = 1; i < REQUEST; i++){
            seek = seek + abs(sequence[i] - sequence[i-1]);
            
           if (i == REQUEST -1)
            printf("%d", sequence[i]);
        else
            printf("%d -> ", sequence[i]);
        
       }
    //total seek ops
    printf("\nTotal seek operations: %d\n\n", seek);

}
