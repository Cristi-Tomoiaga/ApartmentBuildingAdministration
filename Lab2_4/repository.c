#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 
#include <assert.h>
#include <math.h>
#include "repository.h"

Repository createRepository() {
	Repository repo;
	repo.l = createEmptyList();
	return repo;
}

void destroyRepository(Repository* repo) {
	destroyList(&repo->l);
}

int sizeRepository(Repository* repo) {
	return lengthList(&repo->l);
}

List getAllRepository(Repository* repo) {
	return copyList(&repo->l);
}

int findExpenseRepository(Repository* repo, int id, Expense* e) {
	for (int i = 0; i < lengthList(&repo->l); i++) {
		Expense exp = getElementList(&repo->l, i);
		if (exp.id == id) {
			*e = exp;
			return 1; // found the expense
		}
	}
	return 0;
}

int addExpenseRepository(Repository* repo, Expense* e) {
	Expense tmp;
	if (findExpenseRepository(repo, e->id, &tmp))
		return 0; // the expense already exists

	addElementList(&repo->l, e);
	return 1;
}

int deleteExpenseRepository(Repository* repo, int id) {
	Expense exp;
	if (!findExpenseRepository(repo, id, &exp))
		return 0; // the expense does not exist

	for (int i = 0; i < lengthList(&repo->l); i++) {
		Expense expCurr = getElementList(&repo->l, i);
		if (expCurr.id == exp.id) {
			deleteElementList(&repo->l, i);
		}
	}

	return 1;
}

int modifyExpenseRepository(Repository* repo, Expense* e) {
	Expense exp;
	if (!findExpenseRepository(repo, e->id, &exp)) {
		return 0; // the expense does not exist
	}

	for (int i = 0; i < lengthList(&repo->l); i++) {
		Expense expCurr = getElementList(&repo->l, i);
		if (expCurr.id == exp.id) {
			modifyElementList(&repo->l, i, e);
		}
	}

	return 1;
}

void testCreateDestroySizeGetAllRepo() {
	Repository repo = createRepository();
	List l = getAllRepository(&repo);
	assert(lengthList(&l) == 0);
	assert(sizeRepository(&repo) == 0);

	destroyRepository(&repo);
	assert(repo.l.elems == NULL);

	destroyList(&l);
}

void testFindRepo() {
	Repository repo = createRepository();
	Expense el = createExpense(1, 2, 100, "gaz");
	Expense el2 = createExpense(2, 2, 50, "apa");
	Expense el3 = createExpense(3, 2, 300, "incalzire");
	addExpenseRepository(&repo, &el);
	addExpenseRepository(&repo, &el2);
	addExpenseRepository(&repo, &el3);

	Expense elNew;
	int res = findExpenseRepository(&repo, 2, &elNew);
	assert(res == 1);
	assert(equalExpenses(&el2, &elNew));

	int res2 = findExpenseRepository(&repo, 4, &elNew);
	assert(res2 == 0);

	destroyRepository(&repo);
	destroyExpense(&el);
	destroyExpense(&el2);
	destroyExpense(&el3);
}

void testAddRepo() {
	Repository repo = createRepository();
	Expense el = createExpense(1, 2, 100, "gaz");
	Expense el2 = createExpense(2, 2, 50, "apa");
	Expense el3 = createExpense(3, 2, 300, "incalzire");

	Expense elNew;
	assert(sizeRepository(&repo) == 0);
	int resP = addExpenseRepository(&repo, &el);
	assert(resP == 1);
	findExpenseRepository(&repo, 1, &elNew);
	assert(equalExpenses(&elNew, &el));
	assert(sizeRepository(&repo) == 1);

	addExpenseRepository(&repo, &el2);
	assert(sizeRepository(&repo) == 2);
	findExpenseRepository(&repo, 2, &elNew);
	assert(equalExpenses(&elNew, &el2));

	addExpenseRepository(&repo, &el3);
	assert(sizeRepository(&repo) == 3);
	findExpenseRepository(&repo, 3, &elNew);
	assert(equalExpenses(&elNew, &el3));

	int resN = addExpenseRepository(&repo, &el3);
	assert(resN == 0);

	destroyRepository(&repo);
	destroyExpense(&el);
	destroyExpense(&el2);
	destroyExpense(&el3);
}

void testDeleteRepo() {
	Repository repo = createRepository();
	Expense el = createExpense(1, 2, 100, "gaz");
	Expense el2 = createExpense(2, 2, 50, "apa");
	Expense el3 = createExpense(3, 2, 300, "incalzire");
	addExpenseRepository(&repo, &el);
	addExpenseRepository(&repo, &el2);
	addExpenseRepository(&repo, &el3);

	assert(sizeRepository(&repo) == 3);
	int resP = deleteExpenseRepository(&repo, 1);
	assert(resP == 1);
	assert(sizeRepository(&repo) == 2);

	deleteExpenseRepository(&repo, 3);
	assert(sizeRepository(&repo) == 1);

	deleteExpenseRepository(&repo, 2);
	assert(sizeRepository(&repo) == 0);

	int resN = deleteExpenseRepository(&repo, 2);
	assert(resN == 0);

	destroyRepository(&repo);
	destroyExpense(&el);
	destroyExpense(&el2);
	destroyExpense(&el3);
}

void testModifyRepo() {
	Repository repo = createRepository();
	Expense el = createExpense(1, 2, 100, "gaz");
	Expense el2 = createExpense(2, 2, 50, "apa");
	Expense el3 = createExpense(3, 2, 300, "incalzire");
	addExpenseRepository(&repo, &el);
	addExpenseRepository(&repo, &el2);
	addExpenseRepository(&repo, &el3);

	Expense elNew = createExpense(3, 2, 1000, "incalzire");
	int resP = modifyExpenseRepository(&repo, &elNew);
	assert(resP == 1);
	Expense elMod;
	findExpenseRepository(&repo, 3, &elMod);
	assert(equalExpenses(&elMod, &elNew));
	assert(fabs(elNew.sum - elMod.sum) < 1e-5);

	Expense elNew2 = createExpense(4, 2, 1000, "incalzire");
	int resN = modifyExpenseRepository(&repo, &elNew2);
	assert(resN == 0);

	destroyRepository(&repo);
	destroyExpense(&el);
	destroyExpense(&el2);
	destroyExpense(&el3);
	destroyExpense(&elNew);
	destroyExpense(&elNew2);
}