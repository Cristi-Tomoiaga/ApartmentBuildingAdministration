#pragma once
#include "expense.h"

/* 
* The type of an element of the list
*/
typedef Expense ElemType;

/*
* Models a generic list of objects
*/
typedef struct {
	ElemType* elems;
	int length;
	int capacity;
} List;

/*
* Creates an empty list
* @returns the new list
*/
List createEmptyList();

/*
* Destroys the list
* @param l - the list
*/
void destroyList(List* l);

/*
* Deep copies the list
* @param l - the list
* @returs the deep copy of l
*/
List copyList(List* l);

/*
* Getter for the length of the list
* @param l - the list
* @returns the length of the list (int)
*/
int lengthList(List* l);

/*
* Gets the element at the given position
* @param l - the list
* @param pos - the position, between 0 and length - 1
* @return the found element
*/
ElemType getElementList(List* l, int pos);

/*
* Modifies the element at the given position
* @param l - the list, between 0 and length - 1
* @param pos - the position
* @param el - the new element
*/
void modifyElementList(List* l, int pos, ElemType* el);

/*
* Deletes the element at the given position
* @param l - the list
* @param pos - the position, between 0 and length - 1
*/
void deleteElementList(List* l, int pos);

/*
* Adds a new element at the end of the list
* @param l - the list
* @param el - the element
*/
void addElementList(List* l, ElemType* el);

/*
* Test for list module
*/
void testCreateDestroyLengthList();
void testGetSetElementCopyList();
void testDeleteElementList();
void testAddElementList();