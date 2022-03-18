#pragma once
#include "repository.h"

/*
* Definition of a service(grasp controller) for the actions on the expenses in the application
*/
typedef struct {
	Repository repo;
} Service;

/*
* Function pointer custom type for a filter
* The function accepts an expense and a value and returns 1 if the expense passes the filter and 0 otherwise
*/
typedef int (*Filter)(Expense* e, void* value);

/*
* Function pointer custom type for comparison
* The function accepts two expenses e1, e2 and returns -1 if e1 < e2, 0 if e1 == e2 or +1 if e1 > e2
*/
typedef int (*Compare)(Expense* e1, Expense* e2);

/*
* Creates a new service and links it to the given repository
* @param repo - expense repository
* @returns the new service
*/
Service createService(Repository* repo);

/*
* Destroys the underying repository in the service
* @param srv - the service
*/
void destroyService(Service* srv);

/*
* Gets all the expenses stored in the underlying repository
* @param srv - the service
* @returns the list of the expenses
*/
List getAllExpensesService(Service* srv);

/*
* Adds a new expense to the repository using the data provided
* @param srv - the service
* @param id - int
* @param apartmentNumber - int
* @param sum - double
* @param type - char array of maximum 10 characters
* @returns 0 if the operation was successful
*		  -1 if there was a validation error
*         -2 if an expense with the given id already exists
*/
int addExpenseService(Service* srv, int id, int apartmentNumber, double sum, char* type);

/*
* Deletes the expense with the given id from the repository
* @param srv - the service
* @param id - int
* @returns 0 if the operation was successful
*         -3 if the expense with the given id does not exist
*/
int deleteExpenseService(Service* srv, int id);

/*
* Modifies the expense with the given id (only the sum and type fields can be modified)
* @param srv - the service
* @param id - int
* @param sum - double
* @param type - char array of maximum 10 characters
* @returns 0 if the operation was successful
*         -1 if there was a validation error
*         -3 if the expense with the given id does not exist
*/
int modifyExpenseService(Service* srv, int id, double sum, char* type);

/*
* Generates test expenses and populates the service with them
* @param srv - the service
*/
void generateTestExpensesService(Service* srv);

/*
* Filters the expenses with the given filter and value
* @param srv - the service
* @param filter - a filter (function pointer data type)
* @param value - a value for the filter (void*)
* @returns the filtered list of expenses
*/
List filterExpensesService(Service* srv, Filter filter, void* value);

/*
* Implements a filter for the apartmentNumber
* @param e - an expense
* @param apartmentNumber - the apartment number given as a generic pointer (void*)
* @returns 1 if the expense has the given apartment number or 0 otherwise
*/
int filterByApartment(Expense* e, void* apartmentNumber);

/*
* Implements a filter for the sum
* @param e - an expense
* @param sum - the sum given as a generic pointer (void*)
* @returns 1 if the expense has a sum less than or equal to the given sum or 0 otherwise
*/
int filterBySum(Expense* e, void* sum);

/*
* Implements a filter for the type
* @param e - an expense
* @param tyoe - the type given as a generic pointer (void*)
* @returns 1 if the expense has the given type or 0 otherwise
*/
int filterByType(Expense* e, void* type);

/*
* Sorts the expenses by the given comparison function
* @param srv - the service
* @param cmp - a comparison function pointer (function pointer data type)
* @returns the sorted list of expenses
*/
List sortExpensesService(Service* srv, Compare cmp);

/*
* Compares two expenses by type in ascending order
* @param e1, e2 - the expenses
* @returns -1 if e1 < e2, 0 if e1 == e2 or +1 if e1 > e2
*/
int cmpTypeAsc(Expense* e1, Expense* e2);

/*
* Compares two expenses by type in descending order
* @param e1, e2 - the expenses
* @returns -1 if e1 < e2, 0 if e1 == e2 or +1 if e1 > e2
*/
int cmpTypeDesc(Expense* e1, Expense* e2);

/*
* Compares two expenses by sum in ascending order
* @param e1, e2 - the expenses
* @returns -1 if e1 < e2, 0 if e1 == e2 or +1 if e1 > e2
*/
int cmpSumAsc(Expense* e1, Expense* e2);

/*
* Compares two expenses by sum in descending order
* @param e1, e2 - the expenses
* @returns -1 if e1 < e2, 0 if e1 == e2 or +1 if e1 > e2
*/
int cmpSumDesc(Expense* e1, Expense* e2);

/*
* Tests for service module
*/
void testCreateDestroySizeGetAllService();
void testAddService();
void testDeleteService();
void testModifyService();
void testGenerateService();
void testFilterService();
void testSortService();