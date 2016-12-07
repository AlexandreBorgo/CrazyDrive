#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Car.h"

Cars_list addCar(Cars_list list, Car* element) 
{
	element_list* new_element = malloc(sizeof(element_list));
	new_element->car = element;
	new_element->next = NULL;

	if(list == NULL) 
	{
		return new_element;
	}
	else 
	{
		element_list* tmp = list;

		while(tmp->next != NULL) 
		{
			tmp = tmp->next;
		}
		tmp->next = new_element;
		return list;
	}
}

Cars_list addFirst(Cars_list list, Car* element) 
{
	element_list* new_element = malloc(sizeof(element_list));
	new_element->car = element;
	new_element->next = list;	

	return new_element;
}

Car* createCar(int id, int posx, int posy, float speed, float acc, char state, char color) 
{
	Car* new_car = malloc(sizeof(Car));
	new_car->id = id;
	new_car->posx = posx;
	new_car->posy = posy;
	new_car->speed = speed;
	new_car->acc = acc;
	new_car->state = state;
	new_car->color = color;
	new_car->collision = 0;
	new_car->behind_speed = 0;
	new_car->toposx = posx;
	return new_car;
}


Cars_list delCar(Cars_list list, Car* element) 
{
	if(list == NULL) {
		return NULL;
	} 

	if(list->car == element) {
		Cars_list tmp = list->next;
		free(list);
		return tmp;
	}


	Cars_list tmp1 = list;
	Cars_list tmp2 = list->next;

	while(tmp2 != NULL)
	{
		if(tmp2->car == element) {
			tmp1->next = tmp2->next;
			free(tmp2);
			return list;
		}
		tmp2 = tmp2->next;
		tmp1 = tmp1->next;
	}

	return list;
}

Score_list addScore(Score_list list, int score, char* nom) 
{
	element_score_list* new_element = malloc(sizeof(element_score_list));
	new_element->score = score;
	strcpy(new_element->nom,nom);
	new_element->next = NULL;

	if(list == NULL) 
	{
		return new_element;
	}

	if(score >= list->score) {
		new_element->next = list;
		return new_element;
	}

	element_score_list* tmp = list->next;
	element_score_list* tmp2 = list;

	while(tmp != NULL) 
	{
		if(score >= tmp->score)
			break;
		tmp2 = tmp;
		tmp = tmp->next;
	}
	tmp2->next = new_element;
	new_element->next = tmp;
	return list;
}

Score_list delScore(Score_list list) 
{
	if(list == NULL) {
		return NULL;
	} 

	Score_list tmp1 = list;
	Score_list tmp2 = list;

	while(tmp1 != NULL)
	{
		tmp1 = tmp1->next;
		free(tmp2);		
		tmp2 = tmp1;
	}

	return NULL;
}