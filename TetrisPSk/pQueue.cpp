#include "pQueue.h"

#include "Queue.h"

#include <iostream>

pQueue::pQueue()
{
struct Queue* head = NULL;
struct Queue* tail = NULL;
};

void pQueue::enqueue(int d)	// przykladowe wywolanie funkcji: enqueue(5);
{
	Queue* newNode = (struct Queue*)malloc(sizeof(struct Queue));

	if (newNode)
	{
		newNode->color = d;
		newNode->next = NULL;

		if (head == NULL)
		{
			head = newNode;
			tail = newNode;
			newNode->id = 1;
		}
		else
		{
			newNode->id = tail->id + 1;
			tail->next = newNode;
			tail = newNode;
		}
	}
	else
	{
		std::cout << "Nowy element nie zostal utworzony!\n";
	}
}

void pQueue::dequeue()
{
	if (head)
	{
		Queue* tmp = head->next;
		free(head);
		head = tmp;

		if (tmp == NULL)
		{
			tail = NULL;
		}
	}
}

void pQueue::addBag()
{
	/*
	kolory i kszta³ty Mino:
		0 - Z - czerwony
		1 - L - pomarañczowy
		2 - O - ¿ó³ty
		3 - S - zielony
		4 - I - b³êkitny
		5 - J - niebieski
		6 - T - fioletowy
		7 - œmieciomino (obra¿enia) - szary
		8 - œmieciomino (limit czasu) - czarny
	*/

	int bag[7] = { 0, 1, 2, 3, 4, 5, 6 };

	for (int i = 0; i < 7; i++)
	{
		int rndPc = rand() % 7;
		int swap = bag[rndPc];
		bag[rndPc] = bag[i];
		bag[i] = swap;
	}

	for (int i = 0; i < 7; i++)
		enqueue(bag[i]);
}