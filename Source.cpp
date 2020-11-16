/*Program Name: Job Scheduler 
  Programmer Name: David Liang
  Description: The program utilizes the Round Robin Table to simulate Process/Job Scheduling
  in a Single CPU and IO Processor
  //Date Created: January 20th, 2020*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "Queue.h"
#include "Process.h"
#include <fstream>
#include <sstream>

#define BURST_MAX 20
#define PCB_N 20
#define N_MAX 50
using namespace std;


struct CPU
{
	Process *pr;

}; typedef struct CPU CPU;

struct IO
{
	Process *pr;

}; typedef struct IO IO;

Queue Ready_Queue;
Queue  IO_Queue;
Queue Event_Queue;

int main()
{
	//===============================VARIABLES USED FOR ENTIRE CPU AND IO ALGORITHM==========================================
	int simu_time = 0;
	int quanTN = 3;
	Process *PCB[PCB_N];
	bool CPU_idle = true;
	bool IO_idle = true;

	int c_trck = 0;
	int io_trck = 0;

	CPU myCPU;
	IO  myIO;
	int prempt_time = 0;

	myCPU.pr = NULL;
	myIO.pr = NULL;
	int cpu_countdown = 0;
	int io_countdown = 0;
	int pre_countdown = 0;
	bool preemption = false;

	bool pre_finish = false;
	Process *curr_proc = new Process;

	string file_input;
	int Num_Proc = 0;
	string extract_file[N_MAX];

//======================variables generally used for file extraction and storing into individual PROCESS/JOBS OJBECCTS========	
	int cpu_cnt = 0;
	int io_cnt = 0;
	int pos = 0;
	string temp_str = "";
	int temp_int[N_MAX];
	int index_space[N_MAX];
	int temp_cnt = 0;
	bool change_to_io = false;

	float CPU_Util;
	float spent_in_cpu = 0;
//=======================INPUT FILES  AND OUTPUT FILES===========================================================
//. reading and writing data into files

	cout << "PLEASE CHOOSE AN INPUT FILE FROM THE LIST BELOW (HIT 1, 2, OR 3 ): " << endl;
	cout << "1.) CPU_intense" << endl;
	cout << "2.) IO_intense" << endl;
	cout << "3.) MIX" << endl << endl;


	cin >> file_input;
	
	ifstream input_file("input_file_" + file_input + ".txt");

	cout << "PLEASE TYPE IN THE NAME OF THE OUTPUT LOG FILE YOU WANT TO CREATE:" << endl;

	cin >> file_input;
	ofstream log_file("output_file_log_" + file_input + ".txt");

	cout << "PLEASE TYPE IN THE NAME OFT THE OUTPUT COMPONENTS FILE YOU WANT TO CREATE:" << endl;

	cin >> file_input;
	ofstream content_file("output_file_compnt_" + file_input + ".txt");

	cout << "PLEASE TYPE IN QUANTUM NUMBER FOR THIS SIMULATION : " << endl;

	do
	{
		cin >> quanTN;
		if (quanTN <= 0)
		{
			cout << "QUANTUM NUMBER IS LESS THAN 0" << endl << endl;
			cout << "PLEAS TYPE IT IN AGAIN!!" << endl;
			cout << "QUANTUM NUMBER: " << endl;
		}
	} while (quanTN <= 0);


	if (input_file.is_open() == true)
	{
		while (getline(input_file, extract_file[Num_Proc]))
		{
			cout << extract_file[Num_Proc] << endl;

			PCB[Num_Proc] = new Process;
			Num_Proc++;

		}
		input_file.close();
	}

	for (int i = 0; i < PCB_N; i++)
	{
		PCB[i] = new Process;

		if (i >= Num_Proc)
		{
			PCB[i]->state = "EXIT";
		}
	}
//=====================================EXTRACTION ALGORITHM==========================================================
	//TRANSPORTING THE  INPUT TEXT DATA INTO INDIVIDAL ASSIGNED PROCESSES/ JOB
	for (int j = 0; j < Num_Proc; j++)
	{
		for (int i = 0; i < extract_file[j].length(); i++)
		{
			if (extract_file[j][i] == 32)
			{
				index_space[temp_cnt] = i;
				temp_cnt++;
			}
		}

		for (int i = 0; i < temp_cnt; i++)
		{

			temp_str = extract_file[j].substr(index_space[i], index_space[i + 1] - index_space[i]);
			temp_int[pos] = atoi(temp_str.c_str());

			if (index_space[i] == extract_file[j].length() - 2)
			{
				temp_int[pos] = extract_file[j][extract_file[j].length() - 1];
				temp_int[pos] = atoi(temp_str.c_str());
			}
			pos++;
		}

		for (int i = 0; i < temp_cnt; i++)
		{
			temp_str = extract_file[j][0];
			PCB[j]->arrval_time = atoi(temp_str.c_str());
			PCB[j]->p_id = j;
			if (i == 0)
			{
				PCB[j]->N = temp_int[i];
			}
			else if (i > 0 && i % 2 != 0 && change_to_io == false)
			{
				PCB[j]->cpu_burst[cpu_cnt] = temp_int[i];
				cpu_cnt++;
				change_to_io = true;
			}
			else if (change_to_io == true)
			{
				PCB[j]->io_burst[io_cnt] = temp_int[i];
				io_cnt++;
				change_to_io = false;
			}
		}
		cpu_cnt = 0;
		io_cnt = 0;
		temp_cnt = 0;
		pos = 0;
		change_to_io = false;
	}

	for (int i = 0; i < Num_Proc; i++)
	{
		Ready_Queue.enQueue(PCB[i]);
	}
	Queue *temp = NULL;
	Queue *temp2 = NULL;

	while (PCB[0]->state != "EXIT" || PCB[1]->state != "EXIT" || PCB[2]->state != "EXIT" || PCB[3]->state != "EXIT" || PCB[4]->state != "EXIT" || PCB[5]->state != "EXIT" || Ready_Queue.front != NULL || IO_Queue.front != NULL)
	{
//==============================================CONTENT DISPLAY  FOR CPU, IO, READY QUEUE AND I/O QUEUE =========================================================	
		content_file << simu_time << " time units " << endl << endl;
		log_file << simu_time << " time units " << endl << endl;

		content_file << "\tCPU CONTENT: " << endl;
		content_file << "\t___________" << endl;
		if (myCPU.pr == NULL)
		{
			content_file << "\tEMPTY!! " << endl << endl;
		}
		else
		{
			content_file << "\tP ID: " << myCPU.pr->p_id << endl;
			content_file << "\tsate: " << myCPU.pr->state << endl;
			content_file << "\tarrival time: " << myCPU.pr->arrval_time << endl;
			content_file << "\tCPU BURST " << myCPU.pr->N_temp << " :" << myCPU.pr->cpu_burst[myCPU.pr->N_temp] << endl;
		}


		content_file << "\tI\O CONTENT: " << endl;
		content_file << "\t___________" << endl;

		if (myIO.pr == NULL)
		{
			content_file << "\tEMPTY!! " << endl << endl;;
		}
		else
		{
			content_file << "\tP ID: " << myIO.pr->p_id << endl;
			content_file << "\tstate: " << myIO.pr->state << endl;
			content_file << "\tarrival time: " << myIO.pr->arrval_time << endl;
			content_file << "\tCPU BURST " << myIO.pr->N_temp << " :" << myIO.pr->io_burst[myIO.pr->N_temp] << endl;
		}

//====================================Queue Content prints out every 5 time units into log output file
		if (simu_time % 5 == 0)
		{
			content_file << "\tREADY QUEUE" << endl;
			content_file << "\t___________" << endl;

			log_file << "\tREADY QUEUE" << endl;
			log_file << "\t___________" << endl;
			if (Ready_Queue.front == NULL)
			{
				content_file << "\tQUEUE IS EMPTY!!!!!!" << endl << endl;
				log_file << "\tQUEUE IS EMPTY!!!!!!" << endl << endl;;

			}
			else if (Ready_Queue.front != NULL)
			{
				for (temp = Ready_Queue.front; temp != NULL; temp = temp->next)
				{
					content_file << "\tP ID: " << temp->p->p_id << endl;
					content_file << "\tstate: " << temp->p->state << endl << endl;

					log_file << "\tP ID: " << temp->p->p_id << endl;
					log_file << "\tstate: " << temp->p->state << endl << endl;
				}

			}
		}
//=======================================EVERY FIVE TIME UNITS THE CONTENTS OF THE READY AND IO QUEUE ARE PRINTED OUT=====================================	
		if (simu_time % 5 == 0)
		{
			content_file << "\tI\O QUEUE" << endl;
			content_file << "\t___________" << endl;

			log_file << "\tI\O QUEUE" << endl;
			log_file << "\t___________" << endl;

			if (IO_Queue.front == NULL)
			{
				content_file << "\tQUEUE IS EMPTY!!!!!!" << endl << endl;
				log_file << "\tQUEUE IS EMPTY!!!!!!" << endl << endl;;
			}

			else if (IO_Queue.front != NULL)
			{
				for (temp = IO_Queue.front; temp != NULL; temp = temp->next)
				{
					content_file << "\tP ID: " << temp->p->p_id << endl;
					content_file << "\tstate: " << temp->p->state << endl << endl;

					log_file << "\tP ID: " << temp->p->p_id << endl;
					log_file << "\tstate: " << temp->p->state << endl << endl;
				}

			}
		}
		log_file << "EVENTS" << endl;
		log_file << "==========" << endl << endl;			
		//==============================================================ARRIVAL============================================
		//=================================CHECKS IF CPU IS IDLE AND CHECKS READY QUEUE=========================================
		//cout << Ready_Queue.front->p->Ready_WAIT << endl;
		if (CPU_idle == true && Ready_Queue.front != NULL)
		{
			if (curr_proc->state != "BLOCK" || Ready_Queue.front->p->state != "BLOCK")
			{

				if (Ready_Queue.front->p->state == "READY" &&  Ready_Queue.front->p->arrival == false)
				{
					CPU_idle = false;
					myCPU.pr = Ready_Queue.front->p;
					curr_proc = Ready_Queue.front->p;
					Ready_Queue.deQueue();
				}
				else if (Ready_Queue.front->p->arrval_time == simu_time && Ready_Queue.front->p->state == "READY" && Ready_Queue.front->p->arrival == true)
				{

					Ready_Queue.front->p->arrival = false;

					CPU_idle = false;
					myCPU.pr = Ready_Queue.front->p;
					curr_proc = Ready_Queue.front->p;
					Ready_Queue.deQueue();
					log_file << "\tP ID: " << curr_proc->p_id << " NEW PROCESS HAS STARTED " << endl << endl;

				}
				else if (Ready_Queue.front->p->arrval_time < simu_time  && Ready_Queue.front->p->arrival == true)
				{
					Ready_Queue.front->p->arrival = false;

					CPU_idle = false;
					myCPU.pr = Ready_Queue.front->p;
					curr_proc = Ready_Queue.front->p;
					Ready_Queue.deQueue();
				}
				else if (Ready_Queue.front->p->state == "EXIT")
				{
					Ready_Queue.deQueue();
				}

				if (curr_proc->cpu_burst[curr_proc->N_temp] < quanTN)
				{
					cpu_countdown = curr_proc->cpu_burst[curr_proc->N_temp];

					if (curr_proc->type == PREEMPTION)
					{
						pre_finish = true;
						curr_proc->type == IO_REQUEST;
					}
				}
				else if (curr_proc->cpu_burst[curr_proc->N_temp] >= quanTN)
				{
					cpu_countdown = quanTN;
					curr_proc->type = PREEMPTION;
					preemption = true;
				}
				spent_in_cpu++;
			}
		}
		//================================================PREEMPTION EVENT=============================================
		if (preemption == true && cpu_countdown < 2)
		{
			curr_proc->state = "BLOCK";
			preemption = false;
			CPU_idle = false;
			curr_proc->cpu_burst[curr_proc->N_temp] -= quanTN;

			Ready_Queue.enQueue(curr_proc);
			log_file << "\tPID: " << curr_proc->p_id << " HAS BEEN PREEMPTED" << endl << endl;
		}


		//========================================WHEN CPU FINISHES AND BECOMES IDLE================================
		if (CPU_idle == false && cpu_countdown < 2)
		{
			CPU_idle = true;
			//myCPU.pr = NULL;
			if (curr_proc->type != PREEMPTION || pre_finish == true)
			{
				if (curr_proc->N - 1 == curr_proc->N_temp)
				{
					curr_proc->type = TERMINATION;
					curr_proc->state = "EXIT";
					curr_proc->COMPLETION_TM = simu_time;
					log_file << "\tPID: " << curr_proc->p_id << " HAS COMPLETED!!" << endl << endl;
				}
				else
				{
					log_file << "\tPID: " << curr_proc->p_id << " HAS FINISHED IN CPU!!" << endl << endl;
					log_file << "\tIO REQUEST INITIATED!! " << endl << endl;
					curr_proc->state = "BLOCK";
					curr_proc->type = IO_REQUEST;
					IO_Queue.IO_enQueue(curr_proc);

					curr_proc->N_temp++; // implement if statement
				}
				pre_finish = false;
			}
		}
		//========================AFTER COMPLETION OF CPU, GOES TO IO QUEUE AFTER NON-PREEMPTED OR PREEMPTED EVENTS===============================

		//----------------------------------------------------------------------------------------------
		//        IO BURST IS INITIATED / IO IS OCCUPIED -NOT IN IDLE
		//---------------------------------------------------------------------------------------------
		if (IO_idle == true && IO_Queue.front != NULL)
		{
			myIO.pr = IO_Queue.front->p;
			IO_idle = false;
			IO_Queue.deQueue();
			io_countdown = myIO.pr->io_burst[myIO.pr->IO_N];
		}
		//----------------------------------------------------------------------------------------------
		//             IO BURST COMPLETED / IO IS IN IDLE
		//----------------------------------------------------------------------------------------------
		if (io_countdown == 0 && IO_idle == false)
		{
			IO_idle = true;
			myIO.pr->IO_N++;
			Ready_Queue.enQueue(myIO.pr);
			//myIO.pr = NULL;
			myIO.pr->state = "READY";
			log_file << "\tP ID: " << myIO.pr->p_id << " I/O BURST " << myIO.pr->IO_N << " HAS COMPLETED!!! " << endl << endl;
			log_file << "\tP ID: " << myIO.pr->p_id << " HAS CHANGED STATES TO " << myIO.pr->state << endl << endl;
			log_file << "\tP_ID: " << myIO.pr->p_id << "WILL BE SENT BACK TO READY QUEUE!" << endl << endl;
		}
//============================DISPLAYS THE CURRENT PROCESS HAPPENING THE CPU AND IO PROCESSORS
		if (CPU_idle == false)
		{
			spent_in_cpu++;
			cpu_countdown--;
			curr_proc->state = "RUNNING";
			log_file << "\tPID: " << myCPU.pr->p_id << " IS RUNNING IN THE CPU!!" << endl << endl;
		}
		if (IO_idle == false)
		{
			io_countdown--;
			myIO.pr->state = "RUNNING";
			log_file << "\tPID: " << myIO.pr->p_id << " IS RUNNING IN THE I/O!! " << endl << endl;
		}

		Ready_Queue.Calc_Wait_Ready_Queue();
		IO_Queue.Calc_Wait_IO_Queue();

		simu_time++;
	}
	CPU_Util = (float)(spent_in_cpu / simu_time) * 100;
//=========================PRINTS THE STATISTICS INCLUDING CPU UTILIZATION, TAT, READY QUEUE WAIT TIME AND I/0 QUEUE WAIT  TIME 
	cout << endl;
	cout << "CPU UTILIZATION : " << CPU_Util << endl;
	for (int i = 0; i < Num_Proc; i++)
	{
		cout << "P ID: " << PCB[i]->p_id << endl;
		cout << "TAT (TURN AROUND TIME): " << PCB[i]->COMPLETION_TM - PCB[i]->arrval_time << endl;
		cout << "READY WAIT TIME: " << PCB[i]->Ready_WAIT << endl;
		cout << "I/O WAIT TIME: " << PCB[i]->IO_WAIT << endl << endl;
	}

	system("PAUSE");

	return 0;
}
