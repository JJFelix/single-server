/* External definitions for single-server system */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lcgrand.h" 	// header file for random number generator

#define Q_LIMIT 100 	// limit of queue lenght
#define BUSY 1 		// server busy status
#define IDLE 0 		// server idle

int next_event_type, num_custs_delayed, \
    num_delays_required, num_events, num_in_q, server_status;

float area_num_in_q, area_server_status, mean_interarrival, \
    mean_service, sim_time, time_arrival[Q_LIMIT + 1], \
    time_last_event, time_next_event[3], total_of_delays;

FILE *infile, *outfile;

void initialize(void);
void timing(void);
void arrive(void);
void depart(void);
void report(void);
void update_time_avg_stats(void);
float expon(float mean);

int main()
{
	// open input and output files
	infile = fopen("mm1.in", "r");
	outfile = fopen("mm1.out", "w");

    //no of events for the timing function
    num_events = 2;

    //read input parameters
    fscanf(infile, "%f %f %d", &mean_interarrival, &mean_service, &num_delays_required);

    //write report heading and input parameters
    fprintf(outfile, "Single-server queuing system\n\n");
    fprintf(outfile, "Mean interarrival time%11.3f minutes\n\n", mean_interarrival);
    fprintf(outfile, "Mean service time%16.3f minutes\n\n", mean_service);
    fprintf(outfile, "Number of customers%14d\n\n", num_delays_required);

    //Initialize the simulation
    initialize();

    //run the simulation while more delays still needed
    while (num_custs_delayed < num_delays_required){
        //determine next event
        timing();

        //update time-average statistical accumulatos
        update_time_avg_stats();

        //invoke appropriate event function
        switch(next_event_type){
            case 1:
                arrive();
                break;
            case 2:
                depart(); 
                break;
        }
    }

    //invoke the report generator and end the simulation
    report();

    fclose(infile);
    fclose(outfile);

    return 0; 
}

//initialization function
void initialize(void){
    //initialize simulation clock
    sim_time = 0.0;

    //initialize the state variables
    server_status = IDLE;
    num_in_q = 0;
    time_last_event = 0.0;

    //initialize the statistical counters
    num_custs_delayed = 0;
    total_of_delays = 0.0;
    area_num_in_q = 0.0;
    area_server_status = 0.0;

    //initialize event list. Since no customers are present,
    // the departure(service completion) event is eliminated from consideration

    time_next_event[1] = sim_time + expon(mean_interarrival);
    time_next_event[2] = 1.0e+30;
}

//timing function
void timing(void){
    int i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0;

    //determine the event type of the next event to occur
    for (i = 1; i <= num_events; i++)
        if (time_next_event[i] < min_time_next_event){
            min_time_next_event = time_next_event[i];
            next_event_type = i;
        }

    //check to see whether the event list is empty
    if (next_event_type == 0){
        //event list is empty so stop simulation
        fprintf(outfile,"\nEvent list empty at time %f", sim_time);
        exit(1);
    }

    //event list not empty so advance simulation clock
    sim_time = min_time_next_event;
}

//arrival event function
void arrive(void)
{
    float delay;

    //schedule next arrival
    time_next_event[0] = sim_time + expon(mean_interarrival);

    //check to see whether server is busy
    if (server_status == BUSY){
        //server busy so increment no of customers in queue
        ++num_in_q;

        //check to see whether an overflow condition exists
        if (num_in_q > Q_LIMIT){
            //queue has overflowed, stop the simulation
            fprintf(outfile, "\nOverflow of the array time_arrival at");
            fprintf(outfile, "time %f", sim_time);
            exit(2);
        }

        //still room in the queue, so store the time of arrival of arriving customer
        // at the (new) end of time_arrival
        time_arrival[num_in_q] = sim_time;
    }
    else{
        //server idle, so arriving customer has a delay of zero
        delay = 0.0;
        total_of_delays += delay;

        //increment no of custmers delayed, and make server busy
        ++num_custs_delayed;
        server_status = BUSY;

        //schedule a departure (service completion)
        time_next_event[2] = sim_time + expon(mean_service);
    }
}

void depart(void){
    int i;
    float delay;

    //check whether queue is empty
    if (num_in_q == 0){
        //queue empty so make server idle and eliminate the departure 
        // (service completion) event from consideration
        server_status = IDLE;
        time_next_event[2] = 1.0e+30;
    }
    else{
        //queue is nonempty, so decrement no of customers in queue
        --num_in_q;

        //compute delay of the customer beginning service and update total delay accumulator
        delay = sim_time - time_arrival[1];
        total_of_delays += delay;

        //increment no of customers delayed and schedule departure
        ++num_custs_delayed;
        time_next_event[2] = sim_time + expon(mean_service);

        //move each customer in queue(if any) up one place
        for (i = 1; i < num_in_q; i++)
            time_arrival[i] = time_arrival[i+1];
    }
}

//report generator function
void report(void){
    //compute and write estimate of desired measures of performance
    fprintf(outfile, "\n\nAverage delay in queue%11.3f minutes \n\n", total_of_delays / num_custs_delayed);
    fprintf(outfile, "Average number in queue%10.3f\n\n", area_num_in_q / sim_time);
    fprintf(outfile, "Server utilization%14.3f\n\n", area_server_status / sim_time);
    fprintf(outfile, "Time simulation ended%12.3f minutes", sim_time);
}

// update area accumulators for time-average statistics
void update_time_avg_stats(void){
    float time_since_last_event;

    //compute time since last event, and update last-event-time marker
    time_since_last_event = sim_time - time_last_event;
    time_last_event = sim_time;

    //update area under number-in-queue function
    area_num_in_q += num_in_q * time_since_last_event;

    //update area under server-busy indicator function
    area_server_status += server_status * time_since_last_event;
}

//exponential variate generation function
float expon(float mean){
    //return an exponential random variate with mean "mean"
    return -mean * log(lcgrand(1));
}