#pragma once
#include <string>
#define BURST_MAX 20

using namespace std;

enum Event_Type
{
	ARRIVAL, PREEMPTION, IO_REQUEST, IO_DONE, TERMINATION

}; typedef Event_Type Event_Type;


class Process
{
	/*
	struct CPU
	{
		Process *pr;

	}; typedef struct CPU CPU;

	struct IO
	{
		Process *pr;

	}; typedef struct IO IO;
	*/
public:
	int p_id;
	int arrval_time;
	int N;
	int N_temp;
	int IO_N;
	string state;
	int cpu_burst[BURST_MAX];
	int io_burst[BURST_MAX];
	bool arrival = true;
	bool inside;

	int Ready_WAIT; 
	int IO_WAIT;
	int COMPLETION_TM;

	int spent_inside_cpu;
	Event_Type type;
	Process *next;

	 Process(int p_id_in, int arrval_time_in, int N_in, string state_in, int cpu_burst_in[], int io_burst[]);
	 Process();
	 
};

