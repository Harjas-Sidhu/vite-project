#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include "simulator_prequisuites.c"

int main()
{
    int total_simulation_time = 100;

    srand((unsigned int)time(NULL));

    int num_of_processes = rand() % 10;
    num_of_processes = num_of_processes < 5 ? 5 : num_of_processes;

    struct process process_table[num_of_processes];

    for (int i = 0; i < num_of_processes; i++)
    {
        process_table[i].id = i + 1;
        process_table[i].arrival_time = (float)rand() / (float)(RAND_MAX)*30;
        process_table[i].burst_time = (float)rand() / (float)(RAND_MAX)*20;
        process_table[i].is_available = false;
        process_table[i].is_completed = false;
    }

    sort_by_arrival_time(process_table, num_of_processes);

    printf("Process Id\t\tArrival Time\t\tBurst Time\n");

    for (int i = 0; i < num_of_processes; i++)
    {
        printf("%d\t\t\t%f\t\t\t%f\n", process_table[i].id, process_table[i].arrival_time, process_table[i].burst_time);
    }

    struct results ans = round_robin_process_scheduling(process_table, num_of_processes, total_simulation_time);

    printf("\nTime Quantum for Round-Robin Scheduling: %.2f\n", ans.time_quantum);
    printf("Number of processes completed: %d\n", ans.no_of_processes_completed);
    printf("Average Turn-around time: %.2f\n", ans.avg_turn_around_time);
    printf("Average Waiting time: %.2f\n", ans.avg_waiting_time);
    printf("Total time spent: %.2f\n", ans.total_time_spent);

    stats_file_maker_roundrobin(process_table, num_of_processes, ans);

    return 0;
}