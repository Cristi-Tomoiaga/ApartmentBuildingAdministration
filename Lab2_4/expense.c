#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include <assert.h>
#include <string.h>
#include <math.h>
#include "expense.h"

Expense createExpense(int id, int apartmentNumber, double sum, char* type) {
	Expense e;
	size_t len;
	e.id = id;
	e.apartmentNumber = apartmentNumber;
	e.sum = sum;
	len = strlen(type) + 1;
	e.type = malloc(len * sizeof(char));
	strcpy_s(e.type, len, type);
	return e;
}

void destroyExpense(Expense* e) {
	free(e->type);
	e->type = NULL;
}

Expense copyExpense(Expense* e) {
	size_t len;
	Expense copy;
	copy.id = e->id;
	copy.apartmentNumber = e->apartmentNumber;
	copy.sum = e->sum;
	len = strlen(e->type) + 1;
	copy.type = malloc(len * sizeof(char));
	strcpy_s(copy.type, len, e->type);

	return copy;
}

int equalExpenses(Expense* first, Expense* second) {
	if (first->id == second->id)
		return 1;
	return 0;
}

int validateExpense(Expense* e) {
	if (e->id < 0)
		return 0;
	if (e->sum < 0)
		return 0;
	if (strcmp(e->type, "apa") != 0 && strcmp(e->type, "canal") != 0 && strcmp(e->type, "incalzire") != 0 && strcmp(e->type, "gaz") != 0)
		return 0;
	return 1;
}

void testCreateDestroyCopyEqualExpense() {
	Expense e1 = createExpense(1, 2, 12.4, "apa");
	assert(e1.id == 1);
	assert(e1.apartmentNumber == 2);
	assert(fabs(e1.sum - 12.4) < 1e-5);
	assert(strcmp(e1.type, "apa") == 0);

	Expense e1c = copyExpense(&e1);
	assert(e1c.id == 1);
	assert(e1c.apartmentNumber == 2);
	assert(fabs(e1c.sum - 12.4) < 1e-5);
	assert(strcmp(e1c.type, "apa") == 0);

	destroyExpense(&e1c);
	assert(e1c.type == NULL);

	Expense e2 = createExpense(2, 3, 100, "gaz");
	Expense e3 = createExpense(1, 2, 12.5, "apa");
	assert(equalExpenses(&e1, &e1));
	assert(equalExpenses(&e1, &e3));
	assert(!equalExpenses(&e1, &e2));

	destroyExpense(&e1);
	destroyExpense(&e2);
	destroyExpense(&e3);
}

void testValidateExpense() {
	Expense eValid = createExpense(1, 2, 100, "apa");
	Expense eIdInvalid = createExpense(-1, 2, 100, "apa");
	Expense eSumInvalid = createExpense(1, 2, -100, "apa");
	Expense eTypeInvalid = createExpense(1, 2, 100, "ceva");
	Expense eAllInvalid = createExpense(-1, 2, -100, "ceva");

	assert(validateExpense(&eValid));
	assert(!validateExpense(&eIdInvalid));
	assert(!validateExpense(&eSumInvalid));
	assert(!validateExpense(&eTypeInvalid));
	assert(!validateExpense(&eAllInvalid));

	destroyExpense(&eValid);
	destroyExpense(&eIdInvalid);
	destroyExpense(&eSumInvalid);
	destroyExpense(&eTypeInvalid);
	destroyExpense(&eAllInvalid);
}