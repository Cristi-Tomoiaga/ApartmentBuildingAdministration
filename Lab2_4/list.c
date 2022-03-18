#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include <string.h>
#include <assert.h>
#include <math.h>
#include "list.h"

List createEmptyList() {
	List l;
	l.length = 0;
	l.capacity = 2;
	l.elems = malloc(l.capacity * sizeof(ElemType));
	return l;
}

void destroyList(List* l) {
	for (int i = 0; i < l->length; i++) {
		destroyExpense(&l->elems[i]);
	}
	free(l->elems);
	l->elems = NULL;
}

List copyList(List* l) {
	List copy;
	copy.length = l->length;
	copy.capacity = l->capacity;
	copy.elems = malloc(copy.capacity * sizeof(ElemType));
	for (int i = 0; i < copy.length; i++) {
		copy.elems[i] = copyExpense(&l->elems[i]);
	}

	return copy;
}

int lengthList(List* l) {
	return l->length;
}

ElemType getElementList(List* l, int pos) {
	return l->elems[pos];
}

void modifyElementList(List* l, int pos, ElemType* el) {
	destroyExpense(&l->elems[pos]);
	l->elems[pos] = copyExpense(el);
}

void deleteElementList(List* l, int pos) {
	destroyExpense(&l->elems[pos]);
	for (int i = pos; i < l->length - 1; i++)
		l->elems[i] = l->elems[i + 1];
	l->length--;
}

void addElementList(List* l, ElemType* el) {
	if (l->length == l->capacity) { // resize the list
		int newCapacity = l->capacity * 2;

		ElemType* newElems = malloc(newCapacity * sizeof(ElemType));
		for (int i = 0; i < l->length; i++) {
			newElems[i] = l->elems[i];
		}

		free(l->elems);

		l->elems = newElems;
		l->capacity = newCapacity;
	}
	l->elems[l->length] = copyExpense(el);
	l->length++;
}

void testCreateDestroyLengthList() {
	List l = createEmptyList();
	assert(l.length == 0);
	assert(lengthList(&l) == 0);

	l.length = 2; // simulates two 'add operations'
	assert(lengthList(&l) == 2);

	l.length = 0;

	destroyList(&l);
	assert(l.elems == NULL);
}

void testGetSetElementCopyList() {
	List l = createEmptyList();
	ElemType el = createExpense(1, 2, 100, "gaz");
	ElemType el2 = createExpense(2, 2, 50, "apa");
	addElementList(&l, &el);
	addElementList(&l, &el2);

	List lc = copyList(&l);
	assert(lc.length == l.length);
	assert(lc.capacity == l.capacity);
	assert(equalExpenses(&lc.elems[0], &l.elems[0]));
	assert(equalExpenses(&lc.elems[1], &l.elems[1]));

	ElemType el2c = createExpense(3, 2, 50, "apa");
	assert(l.capacity == 2);
	addElementList(&l, &el2c);
	assert(l.capacity == 4);
	assert(equalExpenses(&l.elems[0], &el));
	assert(equalExpenses(&l.elems[1], &el2));
	assert(equalExpenses(&l.elems[2], &el2c));

	ElemType el3 = getElementList(&l, 0);
	assert(el3.id == 1);
	assert(el3.apartmentNumber == 2);
	assert(fabs(el3.sum - 100) < 1e-5);
	assert(strcmp(el3.type, "gaz") == 0);

	modifyElementList(&l, 1, &el);
	ElemType el4 = getElementList(&l, 1);
	assert(el4.id == 1);
	assert(el4.apartmentNumber == 2);
	assert(fabs(el4.sum - 100) < 1e-5);
	assert(strcmp(el4.type, "gaz") == 0);

	destroyList(&l);
	destroyList(&lc);
	destroyExpense(&el);
	destroyExpense(&el2);
	destroyExpense(&el2c);
}

void testDeleteElementList() {
	List l = createEmptyList();
	ElemType el = createExpense(1, 2, 100, "gaz");
	ElemType el2 = createExpense(2, 2, 50, "apa");
	ElemType el4 = createExpense(3, 2, 300, "incalzire");
	addElementList(&l, &el);
	addElementList(&l, &el2);
	addElementList(&l, &el4);
	
	assert(lengthList(&l) == 3);
	deleteElementList(&l, 0);
	assert(lengthList(&l) == 2);
	ElemType el3 = getElementList(&l, 0);
	assert(el3.id == 2);
	assert(el3.apartmentNumber == 2);
	assert(fabs(el3.sum - 50) < 1e-5);
	assert(strcmp(el3.type, "apa") == 0);

	deleteElementList(&l, 1);
	assert(lengthList(&l) == 1);

	deleteElementList(&l, 0);
	assert(lengthList(&l) == 0);

	destroyList(&l);
	destroyExpense(&el);
	destroyExpense(&el2);
	destroyExpense(&el4);
}

void testAddElementList() {
	List l = createEmptyList();
	ElemType el = createExpense(1, 2, 100, "gaz");
	ElemType el2 = createExpense(2, 2, 50, "apa");
	assert(lengthList(&l) == 0);

	addElementList(&l, &el);
	ElemType el3 = getElementList(&l, 0);
	assert(el3.id == 1);
	assert(el3.apartmentNumber == 2);
	assert(fabs(el3.sum - 100) < 1e-5);
	assert(strcmp(el3.type, "gaz") == 0);
	assert(lengthList(&l) == 1);

	addElementList(&l, &el2);
	ElemType el4 = getElementList(&l, 1);
	assert(el4.id == 2);
	assert(el4.apartmentNumber == 2);
	assert(fabs(el4.sum - 50) < 1e-5);
	assert(strcmp(el4.type, "apa") == 0);
	assert(lengthList(&l) == 2);

	destroyList(&l);
	destroyExpense(&el);
	destroyExpense(&el2);
}