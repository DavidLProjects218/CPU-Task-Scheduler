#include "Queue.h"


Queue::Queue()
{
	front = NULL;
	rear = NULL;
	next = NULL;
}

 
void Queue :: enQueue(Process *p_in)
{
	Queue *temp;

	temp = new Queue;;
	
	p_in->inside = true;
	temp->p = p_in;
	p_in->state = "READY";
	
	if (front == NULL)
	{
		front = rear = temp;
		rear->next = NULL;
	}
	else
	{
		temp->next = NULL;
		rear->next = temp;
		rear = temp;
	}
}

void Queue::IO_enQueue(Process *p_in)
{
	Queue *temp;

	temp = new Queue;

	temp->p = p_in;
	
	if (front == NULL)
	{
		front = rear = temp;
		rear->next = NULL;
	}
	else
	{
		temp->next = NULL;
		rear->next = temp;
		rear = temp;
	}
}
void Queue :: deQueue()
{
	Queue *temp;
	//p_in->inside = false;

	if (front == NULL)
	{
		cout << "\nQUEUE IS EMPTY!!!!" << endl << endl;
	}
	else
	{
		front->p->inside = false;
		temp = front;
		front = front->next;
		delete(temp);
	}
}

void Queue :: Calc_Wait_Ready_Queue()
{
	Queue *temp2 ;

	if( front == NULL)
	{
	}
	else
	{
		for (temp2 = front; temp2 != NULL; temp2 = temp2->next)
		{
			
			if ( temp2->p->state == "READY")
			{
				temp2->p->Ready_WAIT++;
			}
			
		}
		
	}
}

void Queue:: Calc_Wait_IO_Queue()
{
	Queue *temp2;

	if (front == NULL)
	{
		
	}
	else
	{
		for (temp2 = front; temp2 != NULL; temp2 = temp2->next)
		{
			
			if (temp2->p->state == "BLOCK")
			{
				temp2->p->IO_WAIT++;
			}
		}
		
	}
}
void Queue:: PRINT_Queue()
{
	Queue *temp;
	
	cout << endl;

	cout << "=================" << endl <<endl;
	if (front == NULL)
	{
		cout << "QUEUE IS EMPTY!!!!!!" << endl << endl;;
		cout << "================" << endl;
	}

	else if (front != NULL)
	{
		for (temp = front; temp != NULL; temp = temp->next)
		{
			cout << " P ID: " << temp->p->p_id << endl;
			cout << " state: " << temp->p->state << endl << endl;
		}
		cout << "==================" << endl;
	}

}