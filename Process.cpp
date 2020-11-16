#include "Process.h"

Process::Process()
{
	arrival = true;
	Ready_WAIT = 0;
	IO_WAIT = 0;
	//N_temp = 0;
}
Process::Process(int p_id_in, int arrval_time_in, int N_in, string state_in, int cpu_burst_in[], int io_burst_in[])
{
	int p_id = p_id_in;
	int arrval_time = arrval_time_in;
	int N = N_in;
	string state = state_in;
	for (int i = 0; i < BURST_MAX; i++)
	{
		cpu_burst[i] = cpu_burst_in[i];
	}
	for (int i = 0; i < BURST_MAX; i++)
	{
		io_burst[i] = io_burst_in[i];
	}
}



	




