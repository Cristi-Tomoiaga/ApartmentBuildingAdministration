#pragma once
#include "list.h"

/*
* Definition of a memory repository for the expenses in the application
*/
typedef struct {
	List l;
} Repository;

/*
* Creates an empty repository
* @returns the created repository
*/
Repository createRepository();

/*
* Destroys the underlying list in the repository
* @param repo - the repository
*/
void destroyRepository(Repository* repo);

/*
* Gets the current size of the repository
* @param repo - the repository
* @returns the size - int
*/
int sizeRepository(Repository* repo);

/*
* Gets all the expenses from the repository
* @param repo - the repository
* @returns the list of all expenses
*/
List getAllRepository(Repository* repo);

/*
* Finds the expense with the given id
* @param repo - the repository
* @param id - int
* @param e - the returned expense (if the function returned 1)
* @returns 1 if found, 0 otherwise
*/
int findExpenseRepository(Repository* repo, int id, Expense* e);

/*
* Adds a new expense to the repository
* @param repo - the repository
* @param e - the new expense
* @returns 1 if the operation was successful, 0 if the expense with the given id already exists
*/
int addExpenseRepository(Repository* repo, Expense* e);

/*
* Deletes the expense with the given id from the repository
* @param repo - the repository
* @param id - int
* @returns 1 if the operation was successful, 0 if the expense with the given id doesn't exist
*/
int deleteExpenseRepository(Repository* repo, int id);

/*
* Modifies the given the expense with the same id in the repository using the new expense provided
* @param repo - the repository
* @param e - the expense
* @returns 1 if the operation was successful, 0 if the expense with the given id doesn't exist
*/
int modifyExpenseRepository(Repository* repo, Expense* e);

/*
* Tests for repository module
*/
void testCreateDestroySizeGetAllRepo();
void testFindRepo();
void testAddRepo();
void testDeleteRepo();
void testModifyRepo();