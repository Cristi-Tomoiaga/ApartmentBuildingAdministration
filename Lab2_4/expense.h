#pragma once

/*
* Models an expense from the domain of the application
* id - int
* apartmentNumber - int
* sum - double
* type - char array
*/
typedef struct {
	int id;
	int apartmentNumber;
	double sum;
	char* type;
} Expense;

/*
* Creates a new Expense using the provided data
* @param id - int
* @param apartmentNumber - int
* @param sum - double
* @param type - char array
* @returns the created Expense  
*/
Expense createExpense(int id, int apartmentNumber, double sum, char* type);

/*
* Destroys the expense
* @param e - the expense
*/
void destroyExpense(Expense* e);

/*
* Deep copies the expense 
* @param e - the expense
* @returns the deep copy of e
*/
Expense copyExpense(Expense* e);

/*
* Verifies if the two expenses are equal (common id)
* @param first - Expense pointer
* @param second - Expense pointer
* @returns 1 if equal, 0 otherwise
*/
int equalExpenses(Expense* first, Expense* second);

/*
* Validates an expense object
* @param e - Expense pointer
* @returns 1 if valid
*		   0 if the id is negative
*            or sum is negative
*            or type is not "apa", "canal", "incalzire" or "gaz" 
*/
int validateExpense(Expense* e);

/*
* Tests for expense module
*/
void testCreateDestroyCopyEqualExpense();
void testValidateExpense();