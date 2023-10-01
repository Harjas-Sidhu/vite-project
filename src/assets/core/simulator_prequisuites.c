#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

struct process
{
    int id;
    float burst_time;
    float arrival_time;
    bool is_available;
    bool is_completed;
    float completion_time;
    float turn_around_time;
    float wiating_time;
};

struct results
{
    float time_quantum;
    float avg_waiting_time;
    float avg_turn_around_time;
    float total_time_spent;
    int no_of_processes_completed;
};

void sort_by_arrival_time(struct process process_table[], int num_of_processes)
{
    for (int i = 0; i < num_of_processes; i++)
    {
        struct process key_process = process_table[i];
        int j = i - 1;
        while (j >= 0 && process_table[j].arrival_time > key_process.arrival_time)
        {
            process_table[j + 1] = process_table[j];
            j -= 1;
        }
        process_table[j + 1] = key_process;
    }
}

struct results round_robin_process_scheduling_for_custom_input(struct process process_table[], int num_of_processes, int total_simulation_time, float time_quantum)
{
    float remaining_burst_times[num_of_processes];
    float completion_time[num_of_processes];
    float waiting_time[num_of_processes];

    for(int i = 0; i < num_of_processes; i++){
        waiting_time[i] = 0;
    }

    int num_of_processes_completed = 0;

    struct results sol;
    sol.avg_turn_around_time = 0;
    sol.avg_waiting_time = 0;
    sol.total_time_spent = 0;
    sol.no_of_processes_completed = 0;
    sol.time_quantum = time_quantum;

    if (num_of_processes == 0)
    {
        return sol;
    }

    for (int i = 0; i < num_of_processes; i++)
    {
        remaining_burst_times[i] = process_table[i].burst_time;
        completion_time[i] = 0;
    }

    int time_spent = process_table[0].arrival_time;

    int no_of_available_processes = 0;

    for (int i = 0; i < num_of_processes; i++)
    {
        if (process_table[i].arrival_time <= time_spent)
        {
            no_of_available_processes += 1;
            process_table[i].is_available = true;
        }
    }

    int *job_queue = (int *)malloc(sizeof(int) * (no_of_available_processes+1));

    for (int i = 0; i < num_of_processes; i++)
    {
        if (process_table[i].is_available == true)
        {
            job_queue[i] = process_table[i].id;
        }
    }
    while (num_of_processes_completed < num_of_processes )
    {
        for (int i = 0; i < num_of_processes; i++)
        {
            for (int j = 0; j < num_of_processes; j++)
            {
                if (remaining_burst_times[i] == 0)
                {
                    num_of_processes_completed += 1;
                    process_table[i].is_completed = true;
                }
            }

            if (no_of_available_processes == 0)
            {
                for (int j = 0; j < num_of_processes; j++)
                {
                    if (process_table[i].arrival_time > time_spent)
                    {
                        process_table[i].is_available = true;
                        time_spent = process_table[i].arrival_time;
                        job_queue[no_of_available_processes] = process_table[i].id;
                        no_of_available_processes += 1;
                        break;
                    }
                }
            }
            for (int j = 0; j < no_of_available_processes; j++)
            {

                if (process_table[i].id == job_queue[j])
                {

                    if (remaining_burst_times[i] > 0)
                    {
                        float execute_time = (remaining_burst_times[i] > time_quantum) ? time_quantum : remaining_burst_times[i];
                        time_spent += execute_time;
                        remaining_burst_times[i] -= execute_time;

                        if (remaining_burst_times[i] <= 0)
                        {
                            completion_time[i] = time_spent;
                            waiting_time[i] = (completion_time[i] - process_table[i].arrival_time - process_table[i].burst_time) < 0 ? 0 : (completion_time[i] - process_table[i].arrival_time - process_table[i].burst_time);
                            process_table[i].is_completed = true;
                            num_of_processes_completed++;
                        }
                    }

                    for (int k = no_of_available_processes; k < num_of_processes; k++)
                    {
                        if (process_table[k].arrival_time <= time_spent && !process_table[k].is_completed)
                        {
                            process_table[k].is_available = true;
                            int *newptr = 0;

                            newptr = (int *)realloc(job_queue, sizeof(int) * (no_of_available_processes + 1));
                            if (!newptr)
                            {
                                printf("Realloc to job_queue in first time");
                                return sol;
                            }
                            job_queue = newptr;

                            job_queue[no_of_available_processes] = process_table[k].id;
                            no_of_available_processes += 1;
                        }
                    }
                }
            }
        }

        no_of_available_processes = 0;
        num_of_processes_completed = 0;

        for (int i = 0; i < num_of_processes; i++)
        {
            if (process_table[i].is_available && !process_table[i].is_completed)
            {
                no_of_available_processes += 1;
            }
            if (process_table[i].is_completed)
            {
                num_of_processes_completed += 1;
            }
        }

        int *newptr = (int *)realloc(job_queue, sizeof(int) * (no_of_available_processes + 1));

        if (!newptr)
        {
            printf("Realloc to job_queue in second time");
            return sol;
        }

        job_queue = newptr;
        int index = 0;

        for (int i = 0; i < num_of_processes; i++)
        {
            if (process_table[i].is_available && !process_table[i].is_completed)
            {
                job_queue[index] = process_table[i].id;
                index++;
            }
        }
    }

    free(job_queue);

    for (int i = 0; i < num_of_processes; i++)
    {
        if (process_table[i].is_completed)
        {
            sol.no_of_processes_completed += 1;
        }
    }

    sol.total_time_spent = time_spent;

    for (int i = 0; i < num_of_processes; i++)
    {
        process_table[i].completion_time = completion_time[i];
        process_table[i].wiating_time = waiting_time[i];
        process_table[i].turn_around_time = (completion_time[i] - process_table[i].arrival_time) < 0 ? 0 : (completion_time[i] - process_table[i].arrival_time);
        sol.avg_turn_around_time += process_table[i].turn_around_time;
        sol.avg_waiting_time += waiting_time[i];
    }
    sol.avg_turn_around_time /= num_of_processes;
    sol.avg_waiting_time /= num_of_processes;

    return sol;
}

struct results round_robin_process_scheduling(struct process process_table[], int num_of_processes, int total_simulation_time)
{

    float time_quantum = (float)rand() / (float)(RAND_MAX)*10;

    float remaining_burst_times[num_of_processes];
    float completion_time[num_of_processes];
    float waiting_time[num_of_processes];

    for(int i = 0; i < num_of_processes; i++){
        waiting_time[i] = 0;
    }

    int num_of_processes_completed = 0;

    struct results sol;
    sol.avg_turn_around_time = 0;
    sol.avg_waiting_time = 0;
    sol.total_time_spent = 0;
    sol.no_of_processes_completed = 0;
    sol.time_quantum = time_quantum;

    if (num_of_processes == 0)
    {
        return sol;
    }

    for (int i = 0; i < num_of_processes; i++)
    {
        remaining_burst_times[i] = process_table[i].burst_time;
        completion_time[i] = 0;
    }

    float time_spent = process_table[0].arrival_time;

    int no_of_available_processes = 0;

    for (int i = 0; i < num_of_processes; i++)
    {
        if (process_table[i].arrival_time <= time_spent)
        {
            no_of_available_processes += 1;
            process_table[i].is_available = true;
        }
    }

    int *job_queue = (int *)malloc(sizeof(int) * (no_of_available_processes+1));

    for (int i = 0; i < num_of_processes; i++)
    {
        if (process_table[i].is_available == true)
        {
            job_queue[i] = process_table[i].id;
        }
    }
    while (num_of_processes_completed < num_of_processes && time_spent < total_simulation_time)
    {
        for (int i = 0; i < num_of_processes; i++)
        {
            for (int j = 0; j < num_of_processes; j++)
            {
                if (remaining_burst_times[i] == 0)
                {
                    num_of_processes_completed += 1;
                    process_table[i].is_completed = true;
                }
            }

            if (no_of_available_processes == 0)
            {
                for (int j = 0; j < num_of_processes; j++)
                {
                    if (process_table[i].arrival_time > time_spent && !process_table[i].is_completed)
                    {
                        process_table[i].is_available = true;
                        time_spent = process_table[i].arrival_time;
                        job_queue[no_of_available_processes] = process_table[i].id;
                        no_of_available_processes += 1;
                        break;
                    }
                }
            }
            for (int j = 0; j < no_of_available_processes; j++)
            {

                if (process_table[i].id == job_queue[j])
                {

                    if (remaining_burst_times[i] > 0)
                    {
                        float execute_time = (remaining_burst_times[i] > time_quantum) ? time_quantum : remaining_burst_times[i];
                        time_spent += execute_time;
                        remaining_burst_times[i] -= execute_time;

                        if (remaining_burst_times[i] <= 0)
                        {
                            completion_time[i] = time_spent;
                            waiting_time[i] = (completion_time[i] - process_table[i].arrival_time - process_table[i].burst_time) < 0 ? 0 : (completion_time[i] - process_table[i].arrival_time - process_table[i].burst_time);
                            process_table[i].is_completed = true;
                            num_of_processes_completed++;
                        }
                    }

                    for (int k = no_of_available_processes; k < num_of_processes; k++)
                    {
                        if (process_table[k].arrival_time <= time_spent && !process_table[k].is_completed)
                        {
                            process_table[k].is_available = true;
                            int *newptr = 0;

                            newptr = (int *)realloc(job_queue, sizeof(int) * (no_of_available_processes + 1));
                            if (!newptr)
                            {
                                printf("Realloc to job_queue in first time");
                                return sol;
                            }
                            job_queue = newptr;

                            job_queue[no_of_available_processes] = process_table[k].id;
                            no_of_available_processes += 1;
                        }
                    }
                }
            }
        }

        no_of_available_processes = 0;
        num_of_processes_completed = 0;

        for (int i = 0; i < num_of_processes; i++)
        {
            if (process_table[i].is_available && !process_table[i].is_completed)
            {
                no_of_available_processes += 1;
            }
            if (process_table[i].is_completed)
            {
                num_of_processes_completed += 1;
            }
        }

        int *newptr = (int *)realloc(job_queue, sizeof(int) * (no_of_available_processes + 1));

        if (!newptr)
        {
            printf("Realloc to job_queue in second time");
            return sol;
        }

        job_queue = newptr;
        int index = 0;

        for (int i = 0; i < num_of_processes; i++)
        {
            if (process_table[i].is_available && !process_table[i].is_completed)
            {
                job_queue[index] = process_table[i].id;
                index++;
            }
        }
    }

    free(job_queue);

    for (int i = 0; i < num_of_processes; i++)
    {
        if (process_table[i].is_completed)
        {
            sol.no_of_processes_completed += 1;
        }
    }

    sol.total_time_spent = time_spent;

    for (int i = 0; i < num_of_processes; i++)
    {
        process_table[i].completion_time = completion_time[i];
        process_table[i].wiating_time = waiting_time[i];
        process_table[i].turn_around_time = (completion_time[i] - process_table[i].arrival_time) < 0? 0 : (completion_time[i] - process_table[i].arrival_time);
        sol.avg_turn_around_time += process_table[i].turn_around_time;
        sol.avg_waiting_time += waiting_time[i];
    }
    sol.avg_turn_around_time /= num_of_processes;
    sol.avg_waiting_time /= num_of_processes;

    return sol;
}

void stats_file_maker_roundrobin(struct process process_table[], int num_of_processes, struct results sol){
    FILE* fptr = fopen("../output/stats-roundrobin.csv", "w");
    if(fptr == NULL){
        printf("The file is not opened. The program will exit now");
        return;
    }
    fprintf(fptr, "Process Id,Arrival Time,Burst Time,C Time,Waiting Time,T-A Time\n");
    for (int i = 0; i < num_of_processes; i++)
    {
        fprintf(fptr, "%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",process_table[i].id,process_table[i].arrival_time,process_table[i].burst_time,process_table[i].completion_time,process_table[i].wiating_time, process_table[i].turn_around_time);
    }
    
    fclose(fptr);

    FILE* fptr2 = fopen("../output/extra-info.txt", "w");
    if(fptr2 == NULL){
        printf("The file is not opened. The program will exit now");
        return;
    }
    fprintf(fptr2, "%d,%.2f,%.2f\n", num_of_processes, sol.time_quantum, sol.total_time_spent);
    fclose(fptr2);
}