#pragma once
#include <string>
#include <iostream>
#include "Process.h"
#include <fstream>
#include <sstream>
using namespace std;

class Queue
{
public:
	
	Queue *front;
	Queue *rear; 
	Queue *next;
	
	Process *p;
	
	Queue();

	void enQueue(Process *p_in);
	void deQueue();
	void PRINT_Queue();
	void Calc_Wait_Ready_Queue();
	void Calc_Wait_IO_Queue();
	void IO_enQueue(Process *p_in);
	
};

