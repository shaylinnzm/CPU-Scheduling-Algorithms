#include "scheduler.h"
#include <stdlib.h>  // For qsort
#include <stdbool.h> // For bool type
#include <limits.h>  // For INT_MAX
#include <stdio.h>

// ---------------- Scheduling Algorithms ----------------
// Helper for qsort in FCFS: Sort by arrivalTime, then by PID
int compare_processes_fcfs(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    if (p1->arrivalTime < p2->arrivalTime) return -1;
    if (p1->arrivalTime > p2->arrivalTime) return 1;
    // Tie-breaker: by PID
    if (p1->pid < p2->pid) return -1;
    if (p1->pid > p2->pid) return 1;
    return 0;
}

// FCFS Scheduling
Metrics fcfs_metrics(Process proc[], int n) {
    if (n == 0) return (Metrics){0.0f, 0.0f, 0.0f};

    Process procs_copy[n];
    for (int i = 0; i < n; i++) {
        procs_copy[i] = proc[i]; // Create a mutable copy
    }

    // Sort processes by arrival time, then PID
    qsort(procs_copy, n, sizeof(Process), compare_processes_fcfs);

    int current_time = 0;
    float total_turnaround_time = 0;
    float total_waiting_time = 0;
    float total_response_time = 0;

    for (int i = 0; i < n; i++) {
        // If CPU is idle, advance current_time to arrival time of current process
        if (current_time < procs_copy[i].arrivalTime) {
            current_time = procs_copy[i].arrivalTime;
        }

        procs_copy[i].startTime = current_time;
        procs_copy[i].completionTime = current_time + procs_copy[i].burstTime;
        
        int turnaround_time = procs_copy[i].completionTime - procs_copy[i].arrivalTime;
        // For FCFS (non-preemptive), waiting time is start time - arrival time
        int waiting_time = procs_copy[i].startTime - procs_copy[i].arrivalTime;
        // For FCFS, response time is same as waiting time
        int response_time = procs_copy[i].startTime - procs_copy[i].arrivalTime;

        total_turnaround_time += turnaround_time;
        total_waiting_time += waiting_time;
        total_response_time += response_time;

        current_time = procs_copy[i].completionTime; // Advance current_time
    }

    Metrics result;
    result.avgTurnaround = total_turnaround_time / n;
    result.avgWaiting = total_waiting_time / n;
    result.avgResponse = total_response_time / n;
    return result;
}

// SJF Scheduling (Non-preemptive)
Metrics sjf_metrics(Process proc[], int n) {
    if (n == 0) return (Metrics){0.0f, 0.0f, 0.0f};

    Process procs_copy[n];
    for (int i = 0; i < n; i++) {
        procs_copy[i] = proc[i];
    }

    bool is_completed[n];
    for(int i=0; i<n; ++i) is_completed[i] = false;

    int completed_count = 0;
    int current_time = 0;
    float total_turnaround_time = 0;
    float total_waiting_time = 0;
    float total_response_time = 0;
    
    // Initialize current_time to the earliest arrival if CPU starts idle
    if (n > 0) {
        int min_arrival = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (procs_copy[i].arrivalTime < min_arrival) {
                min_arrival = procs_copy[i].arrivalTime;
            }
        }
        if (current_time < min_arrival) {
             current_time = min_arrival;
        }
    }


    while (completed_count < n) {
        int best_proc_idx = -1;
        int shortest_burst = INT_MAX;

        // Find the shortest job among arrived and uncompleted processes
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && procs_copy[i].arrivalTime <= current_time) {
                if (procs_copy[i].burstTime < shortest_burst) {
                    shortest_burst = procs_copy[i].burstTime;
                    best_proc_idx = i;
                } else if (procs_copy[i].burstTime == shortest_burst) {
                    // Tie-break by arrival time
                    if (procs_copy[i].arrivalTime < procs_copy[best_proc_idx].arrivalTime) {
                        best_proc_idx = i;
                    } else if (procs_copy[i].arrivalTime == procs_copy[best_proc_idx].arrivalTime) {
                        // Tie-break by PID
                        if (procs_copy[i].pid < procs_copy[best_proc_idx].pid) {
                            best_proc_idx = i;
                        }
                    }
                }
            }
        }

        if (best_proc_idx == -1) { // No process ready, CPU is idle
            int next_arrival_time = INT_MAX;
            bool found_pending = false;
            for(int i=0; i<n; ++i) {
                if(!is_completed[i]) { // Consider only processes not yet completed
                    found_pending = true;
                    if (procs_copy[i].arrivalTime < next_arrival_time) {
                        next_arrival_time = procs_copy[i].arrivalTime;
                    }
                }
            }
            if (found_pending) {
                // If current time is less than the found next arrival, jump. Otherwise, just tick.
                current_time = (current_time < next_arrival_time) ? next_arrival_time : current_time + 1;
            } else { // All processes completed, should not happen if completed_count < n
                break;
            }
            continue; 
        }
        
        // Process the selected job
        Process *p = &procs_copy[best_proc_idx];
        p->startTime = current_time;
        p->completionTime = current_time + p->burstTime;

        int turnaround_time = p->completionTime - p->arrivalTime;
        int waiting_time = p->startTime - p->arrivalTime; // Non-preemptive
        int response_time = p->startTime - p->arrivalTime; // Non-preemptive

        total_turnaround_time += turnaround_time;
        total_waiting_time += waiting_time;
        total_response_time += response_time;

        current_time = p->completionTime;
        is_completed[best_proc_idx] = true;
        completed_count++;
    }

    Metrics result;
    result.avgTurnaround = total_turnaround_time / n;
    result.avgWaiting = total_waiting_time / n;
    result.avgResponse = total_response_time / n;
    return result;
}

// Round Robin Scheduling 
Metrics rr_metrics(Process proc[], int n, int timeQuantum) {
    if (n == 0) return (Metrics){0.0f, 0.0f, 0.0f};
    if (timeQuantum <= 0) {
        return fcfs_metrics(proc, n);
    }

    Process procs_copy[n];
    for (int i = 0; i < n; i++) {
        procs_copy[i] = proc[i];
        procs_copy[i].remainingTime = procs_copy[i].burstTime;
        procs_copy[i].startTime = -1;
        procs_copy[i].completionTime = -1;
    }

    int current_time = 0;
    int completed_count = 0;
    float total_turnaround_time = 0;
    float total_waiting_time = 0;
    float total_response_time = 0;

    int ready_queue[n];
    int q_front = 0, q_rear = 0;
    int q_size = 0;
    bool in_queue_flags[n];
    for(int i=0; i<n; ++i) in_queue_flags[i] = false;

    if (n > 0) {
        int min_arrival = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (procs_copy[i].arrivalTime < min_arrival) {
                min_arrival = procs_copy[i].arrivalTime;
            }
        }
        if (current_time < min_arrival) {
             current_time = min_arrival;
        }
    }

    int iteration_count = 0; 
    while (completed_count < n) {
        iteration_count++; 
        if (iteration_count > n * 1000 && n > 0) { // Increased safety break limit
             
             break;
        }

        for (int i = 0; i < n; i++) {
            if (procs_copy[i].remainingTime > 0 &&
                procs_copy[i].arrivalTime <= current_time &&
                !in_queue_flags[i]) {
                ready_queue[q_rear] = i;
                q_rear = (q_rear + 1) % n;
                q_size++;
                in_queue_flags[i] = true;
            }
        }

        if (q_size == 0) {
            if (completed_count < n) {
                int next_event_time = INT_MAX;
                bool found_pending = false;
                for(int i=0; i<n; ++i) {
                    if(procs_copy[i].remainingTime > 0) {
                        found_pending = true;
                        if(procs_copy[i].arrivalTime > current_time && procs_copy[i].arrivalTime < next_event_time) {
                            next_event_time = procs_copy[i].arrivalTime;
                        }
                    }
                }
                if(found_pending) {
                    if (next_event_time != INT_MAX && next_event_time > current_time) {
                        current_time = next_event_time;
                    } else {
                        current_time++;
                    }
                } else {
                     break;
                }
            } else {
                break;
            }
            continue;
        }

        int current_proc_idx = ready_queue[q_front];
        q_front = (q_front + 1) % n;
        q_size--;
        in_queue_flags[current_proc_idx] = false;

        Process *p = &procs_copy[current_proc_idx];

        if (p->startTime == -1) {
            p->startTime = current_time;
        }

        int time_to_run = timeQuantum;
        if (p->remainingTime <= time_to_run) { // Process finishes
            time_to_run = p->remainingTime;

            current_time += time_to_run;
            p->remainingTime = 0;
            p->completionTime = current_time;

            int turnaround = p->completionTime - p->arrivalTime;
            int waiting = turnaround - p->burstTime;
            int response = p->startTime - p->arrivalTime;

            total_turnaround_time += turnaround;
            total_waiting_time += waiting;
            total_response_time += response;
            completed_count++;
        } else { // Process preempted
            current_time += time_to_run;
            p->remainingTime -= time_to_run;

            for (int i = 0; i < n; i++) {
                if (i == current_proc_idx) continue; 

                if (procs_copy[i].remainingTime > 0 &&
                    procs_copy[i].arrivalTime <= current_time &&
                    !in_queue_flags[i]) {
                    ready_queue[q_rear] = i;
                    q_rear = (q_rear + 1) % n;
                    q_size++;
                    in_queue_flags[i] = true;
                }
            }
            
            // Add the current preempted process back
            // No need to check p->remainingTime > 0 here, because this 'else'
            // block is only entered if p->remainingTime was > time_to_run initially.
            ready_queue[q_rear] = current_proc_idx;
            q_rear = (q_rear + 1) % n;
            q_size++;
            in_queue_flags[current_proc_idx] = true;
        }
    }

    Metrics result;
    if (n > 0) {
        result.avgTurnaround = total_turnaround_time / n;
        result.avgWaiting = total_waiting_time / n;
        result.avgResponse = total_response_time / n;
    } else {
        result.avgTurnaround = 0;
        result.avgWaiting = 0;
        result.avgResponse = 0;
    }
    return result;
}

