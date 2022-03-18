#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 
#include <assert.h>
#include <string.h>
#include <math.h>
#include "service.h"

Service createService(Repository* repo) {
	Service srv;
	srv.repo = *repo;
	return srv;
}

void destroyService(Service* srv) {
	destroyRepository(&srv->repo);
}

List getAllExpensesService(Service* srv) {
	return getAllRepository(&srv->repo);
}

int addExpenseService(Service* srv, int id, int apartmentNumber, double sum, char* type) {
	Expense exp = createExpense(id, apartmentNumber, sum, type);
	if (!validateExpense(&exp)) {
		destroyExpense(&exp);
		return -1;
	}
	if (!addExpenseRepository(&srv->repo, &exp)) {
		destroyExpense(&exp);
		return -2;
	}

	destroyExpense(&exp);

	return 0;
}

int deleteExpenseService(Service* srv, int id) {
	if (!deleteExpenseRepository(&srv->repo, id))
		return -3;

	return 0;
}

int modifyExpenseService(Service* srv, int id, double sum, char* type) {
	Expense exp;
	int res = findExpenseRepository(&srv->repo, id, &exp);
	if (!res)
		return -3;

	Expense newExp = createExpense(id, exp.apartmentNumber, sum, type);
	if (!validateExpense(&newExp)) {
		destroyExpense(&newExp);
		return -1;
	}
	modifyExpenseRepository(&srv->repo, &newExp); // already checked that the expense exists in the repository

	destroyExpense(&newExp);

	return 0;
}

void generateTestExpensesService(Service* srv) {
	// empty the repository first
	List l = getAllExpensesService(srv);
	for (int i = 0; i < lengthList(&l); i++) {
		Expense e = getElementList(&l, i);
		deleteExpenseService(srv, e.id);
	}

	// generate the data
	addExpenseService(srv, 1, 1, 150.5, "apa");
	addExpenseService(srv, 2, 1, 300, "gaz");
	addExpenseService(srv, 3, 2, 70, "apa");
	addExpenseService(srv, 4, 2, 500, "canal");
	addExpenseService(srv, 5, 4, 400, "incalzire");
	addExpenseService(srv, 6, 4, 290, "gaz");
	addExpenseService(srv, 7, 4, 30, "apa");
	addExpenseService(srv, 8, 5, 10, "apa");
	addExpenseService(srv, 9, 5, 900, "gaz");
	addExpenseService(srv, 10, 6, 150, "canal");

	destroyList(&l);
}

List filterExpensesService(Service* srv, Filter filter, void* value){
	List filteredList = createEmptyList();

	List actualList = getAllExpensesService(srv);
	for (int i = 0; i < lengthList(&actualList); i++) {
		Expense e = getElementList(&actualList, i);
		if (filter(&e, value))
			addElementList(&filteredList, &e);
	}

	destroyList(&actualList);

	return filteredList;
}

int filterByApartment(Expense* e, void* apartmentNumber) {
	int actualApartmentNumber = *((int*)apartmentNumber); // convert back to int
	if (e->apartmentNumber == actualApartmentNumber)
		return 1;
	return 0;
}

int filterBySum(Expense* e, void* sum) {
	double actualSum = *((double *)sum);
	if (e->sum <= actualSum)
		return 1;
	return 0;
}

int filterByType(Expense* e, void* type){
	char* actualType = (char*)type;
	if (strcmp(e->type, actualType) == 0)
		return 1;
	return 0;
}

List sortExpensesService(Service* srv, Compare cmp) {
	List sortedList = getAllExpensesService(srv);

	for(int i = 0; i < lengthList(&sortedList) - 1; i++)
		for (int j = i + 1; j < lengthList(&sortedList); j++) {
			Expense e1 = getElementList(&sortedList, i);
			Expense e2 = getElementList(&sortedList, j);
			if (cmp(&e1, &e2) > 0) {
				Expense e1c = copyExpense(&e1);
				Expense e2c = copyExpense(&e2);
				modifyElementList(&sortedList, i, &e2c);
				modifyElementList(&sortedList, j, &e1c);
				destroyExpense(&e1c);
				destroyExpense(&e2c);
			}
		}

	return sortedList;
}

int cmpTypeAsc(Expense* e1, Expense* e2) {
	return strcmp(e1->type, e2->type);
}

int cmpTypeDesc(Expense* e1, Expense* e2) {
	return -strcmp(e1->type, e2->type);
}

int cmpSumAsc(Expense* e1, Expense* e2) {
	if (fabs(e1->sum - e2->sum) < 1e-5)
		return 0;
	else if (e1->sum < e2->sum)
		return -1;
	return 1;
}

int cmpSumDesc(Expense* e1, Expense* e2) {
	if (fabs(e1->sum - e2->sum) < 1e-5)
		return 0;
	else if (e1->sum < e2->sum)
		return 1;
	return -1;
}

void testCreateDestroySizeGetAllService() {
	Repository repo = createRepository();
	Expense el = createExpense(1, 2, 100, "gaz");
	Expense el2 = createExpense(2, 2, 50, "apa");
	Expense el3 = createExpense(3, 2, 300, "incalzire");
	addExpenseRepository(&repo, &el);
	addExpenseRepository(&repo, &el2);
	addExpenseRepository(&repo, &el3);
	Service srv = createService(&repo);

	assert(sizeRepository(&repo) == sizeRepository(&srv.repo));
	List l = getAllExpensesService(&srv);
	assert(lengthList(&l) == lengthList(&repo.l));

	destroyService(&srv);
	assert(srv.repo.l.elems == NULL);

	destroyExpense(&el);
	destroyExpense(&el2);
	destroyExpense(&el3);
	destroyList(&l);
}

void testAddService() {
	Repository repo = createRepository();
	Service srv = createService(&repo);

	int rezP = addExpenseService(&srv, 1, 2, 100, "apa");
	assert(rezP == 0);
	assert(sizeRepository(&srv.repo) == 1);

	int rezNValid = addExpenseService(&srv, -2, 2, 100, "ceva");
	assert(rezNValid == -1);
	assert(sizeRepository(&srv.repo) == 1);

	int rezNValid2 = addExpenseService(&srv, 2, 2, -100, "ceva");
	assert(rezNValid2 == -1);
	assert(sizeRepository(&srv.repo) == 1);

	int rezNId = addExpenseService(&srv, 1, 2, 100, "apa");
	assert(rezNId == -2);
	assert(sizeRepository(&srv.repo) == 1);

	addExpenseService(&srv, 2, 3, 2000, "gaz");
	assert(sizeRepository(&srv.repo) == 2);

	destroyService(&srv);
}

void testDeleteService() {
	Repository repo = createRepository();
	Expense el = createExpense(1, 2, 100, "gaz");
	Expense el2 = createExpense(2, 2, 50, "apa");
	Expense el3 = createExpense(3, 2, 300, "incalzire");
	addExpenseRepository(&repo, &el);
	addExpenseRepository(&repo, &el2);
	addExpenseRepository(&repo, &el3);
	Service srv = createService(&repo);

	assert(sizeRepository(&srv.repo) == 3);
	int rezP = deleteExpenseService(&srv, 1);
	assert(rezP == 0);
	assert(sizeRepository(&srv.repo) == 2);

	assert(sizeRepository(&srv.repo) == 2);
	int rezNId = deleteExpenseService(&srv, 4);
	assert(rezNId == -3);
	assert(sizeRepository(&srv.repo) == 2);

	destroyService(&srv);
	destroyExpense(&el);
	destroyExpense(&el2);
	destroyExpense(&el3);
}

void testModifyService() {
	Repository repo = createRepository();
	Expense el = createExpense(1, 2, 100, "gaz");
	Expense el2 = createExpense(2, 2, 50, "apa");
	Expense el3 = createExpense(3, 2, 300, "incalzire");
	addExpenseRepository(&repo, &el);
	addExpenseRepository(&repo, &el2);
	addExpenseRepository(&repo, &el3);
	Service srv = createService(&repo);

	int rezP = modifyExpenseService(&srv, 1, 1000, "gaz");
	assert(rezP == 0);

	int rezNValid = modifyExpenseService(&srv, 1, -100, "ceva");
	assert(rezNValid == -1);

	int rezNId = modifyExpenseService(&srv, 10, 100, "incalzire");
	assert(rezNId == -3);

	destroyService(&srv);
	destroyExpense(&el);
	destroyExpense(&el2);
	destroyExpense(&el3);
}

void testGenerateService() {
	Repository repo = createRepository();
	Service srv = createService(&repo);

	assert(sizeRepository(&srv.repo) == 0);
	generateTestExpensesService(&srv);
	assert(sizeRepository(&srv.repo) == 10);

	generateTestExpensesService(&srv);
	assert(sizeRepository(&srv.repo) == 10);

	destroyService(&srv);
}

void testFilterService() {
	Expense el = createExpense(1, 2, 100, "gaz");

	int ap1 = 2;
	assert(filterByApartment(&el, &ap1) == 1);
	int ap2 = 1;
	assert(filterByApartment(&el, &ap2) == 0);

	double sum1 = 200;
	assert(filterBySum(&el, &sum1) == 1);
	double sum2 = 100;
	assert(filterBySum(&el, &sum2) == 1);
	double sum3 = 50;
	assert(filterBySum(&el, &sum3) == 0);

	char type1[10] = "apa";
	assert(filterByType(&el, type1) == 0);
	char type2[10] = "gaz";
	assert(filterByType(&el, type2) == 1);

	Repository repo = createRepository();
	Service srv = createService(&repo);
	generateTestExpensesService(&srv);

	int apF1 = 2;
	List filteredList1 = filterExpensesService(&srv, filterByApartment, &apF1);
	assert(lengthList(&filteredList1) == 2);
	int apF2 = 10;
	List filteredList2 = filterExpensesService(&srv, filterByApartment, &apF2);
	assert(lengthList(&filteredList2) == 0);

	double sumF1 = 1000;
	List filteredList3 = filterExpensesService(&srv, filterBySum, &sumF1);
	assert(lengthList(&filteredList3) == 10);
	double sumF2 = 40;
	List filteredList4 = filterExpensesService(&srv, filterBySum, &sumF2);
	assert(lengthList(&filteredList4) == 2);
	double sumF3 = 5;
	List filteredList5 = filterExpensesService(&srv, filterBySum, &sumF3);
	assert(lengthList(&filteredList5) == 0);

	char typeF1[10] = "incalzire";
	List filteredList6 = filterExpensesService(&srv, filterByType, typeF1);
	assert(lengthList(&filteredList6) == 1);
	char typeF2[10] = "apa";
	List filteredList7 = filterExpensesService(&srv, filterByType, typeF2);
	assert(lengthList(&filteredList7) == 4);

	destroyService(&srv);
	destroyExpense(&el);
	destroyList(&filteredList1);
	destroyList(&filteredList2);
	destroyList(&filteredList3);
	destroyList(&filteredList4);
	destroyList(&filteredList5);
	destroyList(&filteredList6);
	destroyList(&filteredList7);
}

void testSortService() {
	Expense el1 = createExpense(1, 2, 100, "gaz");
	Expense el2 = createExpense(2, 3, 50, "apa");
	Expense el3 = createExpense(3, 2, 100, "apa");

	assert(cmpSumAsc(&el1, &el2) == 1);
	assert(cmpSumAsc(&el2, &el1) == -1);
	assert(cmpSumAsc(&el1, &el3) == 0);

	assert(cmpSumDesc(&el1, &el2) == -1);
	assert(cmpSumDesc(&el2, &el1) == 1);
	assert(cmpSumDesc(&el1, &el3) == 0);

	assert(cmpTypeAsc(&el1, &el2) == 1);
	assert(cmpTypeAsc(&el2, &el1) == -1);
	assert(cmpTypeAsc(&el2, &el3) == 0);

	assert(cmpTypeDesc(&el1, &el2) == -1);
	assert(cmpTypeDesc(&el2, &el1) == 1);
	assert(cmpTypeDesc(&el2, &el3) == 0);

	Repository repo = createRepository();
	Service srv = createService(&repo);
	addExpenseService(&srv, 1, 1, 150.5, "apa");
	addExpenseService(&srv, 2, 1, 300, "gaz");
	addExpenseService(&srv, 3, 2, 500, "canal");
	addExpenseService(&srv, 4, 4, 400, "incalzire");

	List sortedList1 = sortExpensesService(&srv, cmpSumAsc);
	assert(lengthList(&sortedList1) == 4);
	assert(getElementList(&sortedList1, 0).id == 1);
	assert(getElementList(&sortedList1, 1).id == 2);
	assert(getElementList(&sortedList1, 2).id == 4);
	assert(getElementList(&sortedList1, 3).id == 3);

	List sortedList2 = sortExpensesService(&srv, cmpSumDesc);
	assert(lengthList(&sortedList2) == 4);
	assert(getElementList(&sortedList2, 0).id == 3);
	assert(getElementList(&sortedList2, 1).id == 4);
	assert(getElementList(&sortedList2, 2).id == 2);
	assert(getElementList(&sortedList2, 3).id == 1);

	List sortedList3 = sortExpensesService(&srv, cmpTypeAsc);
	assert(lengthList(&sortedList3) == 4);
	assert(getElementList(&sortedList3, 0).id == 1);
	assert(getElementList(&sortedList3, 1).id == 3);
	assert(getElementList(&sortedList3, 2).id == 2);
	assert(getElementList(&sortedList3, 3).id == 4);

	List sortedList4 = sortExpensesService(&srv, cmpTypeDesc);
	assert(lengthList(&sortedList4) == 4);
	assert(getElementList(&sortedList4, 0).id == 4);
	assert(getElementList(&sortedList4, 1).id == 2);
	assert(getElementList(&sortedList4, 2).id == 3);
	assert(getElementList(&sortedList4, 3).id == 1);

	destroyService(&srv);
	destroyExpense(&el1);
	destroyExpense(&el2);
	destroyExpense(&el3);
	destroyList(&sortedList1);
	destroyList(&sortedList2);
	destroyList(&sortedList3);
	destroyList(&sortedList4);
}