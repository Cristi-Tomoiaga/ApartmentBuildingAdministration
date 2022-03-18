// Problema 7. Administrator imobil
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "expense.h"
#include "list.h"
#include "repository.h"
#include "service.h"

/*
* Prints the menu and reads a command from the user
* @returns the command (char)
*/
char getCommand() {
	char cmd;
	printf("Meniu:\na - Adauga o cheltuiala\nm - Modifica o cheltuiala\nd - Sterge o cheltuiala\np - Afiseaza toate cheltuielile\nf - Filtrare dupa o proprietate\ns - Sortare dupa o proprietate\ng - Genereaza date de test\nq - Parasire aplicatie\n");
	printf("> ");
	scanf_s(" %c%*[^\n]", &cmd, 1); 
	return cmd;
}

/*
* Prints the filter menu and reads a command from the user
* @returns the command (char)
*/
char getFilterCommand() {
	char cmd;
	printf("Filtrare:\ns - Dupa suma\nt - Dupa tip\na - Dupa apartament\n");
	printf(": ");
	scanf_s(" %c%*[^\n]", &cmd, 1);
	return cmd;
}

/*
* Prints the sorting menu and reads a command from the user
* @returns the command (char)
*/
char getSortCommand() {
	char cmd;
	printf("Sortare:\ns - Dupa suma crescator\nS - Dupa suma descrescator\nt - Dupa tip crescator\nT - Dupa tip descrescator\n");
	printf("? ");
	scanf_s(" %c%*[^\n]", &cmd, 1);
	return cmd;
}

/*
* Flushes the standard input stream
*/
void flushStdin() {
	int c = getchar();
	while (c != '\n' && c != EOF)
		c = getchar();
}

/*
* Trims the whitespaces from the end of the string
* @param s - the string
*/
void trimWhitespaces(char* s) {
	size_t i = strlen(s) - 1;
	while (i > 0 && isspace(s[i]))
		i--;
	s[i + 1] = '\0'; // move the \0 to the right position
}

/*
* Reads data from the user and adds a new expense using the service
* @param srv - the service
*/
void uiAddExpense(Service* srv) {
	int id, apNum;
	double sum;
	char type[10];
	
	printf("ID: ");
	scanf_s("%d", &id);

	printf("AP: ");
	scanf_s("%d", &apNum);

	printf("SUMA: ");
	scanf_s("%Lf", &sum);

	flushStdin();
	printf("TIP: ");
	fgets(type, 10, stdin);
	trimWhitespaces(type);
	if (strlen(type) == 9)
		flushStdin();

	int res = addExpenseService(srv, id, apNum, sum, type);
	if (res == -1) {
		printf("Datele sunt invalide!\n");
	}
	else if (res == -2) {
		printf("Cheltuiala cu id-ul %d exista deja!\n", id);
	}
	else if(res == 0) {
		printf("Cheltuiala a fost adaugata!\n");
	}
}

/*
* Reads data from the user and modifies an existing expense using the service
* @param srv - the service
*/
void uiModifyExpense(Service* srv) {
	int id;
	double sum;
	char type[10];

	printf("ID: ");
	scanf_s("%d", &id);

	printf("SUMA: ");
	scanf_s("%Lf", &sum);

	flushStdin();
	printf("TIP: ");
	fgets(type, 10, stdin);
	trimWhitespaces(type);
	if (strlen(type) == 9)
		flushStdin();

	int res = modifyExpenseService(srv, id, sum, type);
	if (res == -1) {
		printf("Datele sunt invalide!\n");
	}
	else if (res == -3) {
		printf("Nu exista cheltuieli cu id-ul %d!\n", id);
	}
	else if (res == 0) {
		printf("Cheltuiala a fost modificata!\n");
	}
}

/*
* Reads an id from the user and deletes the corresponding expense using the service
* @param srv - the service
*/
void uiDeleteExpense(Service* srv) {
	int id;

	printf("ID: ");
	scanf_s("%d", &id);

	int res = deleteExpenseService(srv, id);
	if (res == -3) {
		printf("Nu exista cheltuieli cu id-ul %d!\n", id);
	}
	else if (res == 0) {
		printf("Cheltuiala a fost stearsa!\n");
	}
}

/*
* Prints all the expenses from the given list
* @param l - the list
*/
void uiPrintListExpenses(List* l) {
	if (lengthList(l) == 0) {
		printf("Lista de cheltuieli vida!\n");
		return;
	}

	printf("Lista cheltuielilor:\n");
	printf("%3s | %3s | %7s | %9s\n", "ID", "AP", "SUMA", "TIP");
	for (int i = 0; i < lengthList(l); i++) {
		Expense exp = getElementList(l, i);
		printf("%3d | %3d | %7.2Lf | %9s\n", exp.id, exp.apartmentNumber, exp.sum, exp.type);
	}
	printf("\n");
}

/*
* Prints all the expenses added in the app using the service
* @param srv - the service
*/
void uiPrintExpenses(Service* srv) {
	List l = getAllExpensesService(srv);
	uiPrintListExpenses(&l);
	destroyList(&l);
}

/*
* Generates test data for the expenses service
* @param srv - the service
*/
void uiGenerateTestExpenses(Service* srv) {
	generateTestExpensesService(srv);
	printf("Cheltuielile au fost generate cu succes!\n");
}

/*
* Filters the expenses stored in the app using a given property
* @param srv - the service
*/
void uiFilterExpenses(Service* srv) {
	char cmd = getFilterCommand();
	if (cmd == 's') {
		double sum;
		printf("SUMA: ");
		scanf_s("%lf", &sum);

		List l = filterExpensesService(srv, filterBySum, &sum);
		uiPrintListExpenses(&l);
		destroyList(&l);
	}
	else if (cmd == 't') {
		char type[10];
		printf("TIP: ");
		flushStdin();
		fgets(type, 10, stdin);
		trimWhitespaces(type);
		if (strlen(type) == 9)
			flushStdin();

		List l = filterExpensesService(srv, filterByType, type);
		uiPrintListExpenses(&l);
		destroyList(&l);
	}
	else if (cmd == 'a') {
		int apartmentNumber;
		printf("AP: ");
		scanf_s("%d", &apartmentNumber);

		List l = filterExpensesService(srv, filterByApartment, &apartmentNumber);
		uiPrintListExpenses(&l);
		destroyList(&l);
	}
	else {
		printf("Comanda de filtrare invalida!\n");
	}
}

/*
* Sorts the expenses stored in the app by a given property
* @srv - the service
*/
void uiSortExpenses(Service* srv) {
	char cmd = getSortCommand();
	if (cmd == 's') {
		List l = sortExpensesService(srv, cmpSumAsc);
		uiPrintListExpenses(&l);
		destroyList(&l);
	}
	else if (cmd == 'S') {
		List l = sortExpensesService(srv, cmpSumDesc);
		uiPrintListExpenses(&l);
		destroyList(&l);
	}
	else if (cmd == 't') {
		List l = sortExpensesService(srv, cmpTypeAsc);
		uiPrintListExpenses(&l);
		destroyList(&l);
	}
	else if (cmd == 'T') {
		List l = sortExpensesService(srv, cmpTypeDesc);
		uiPrintListExpenses(&l);
		destroyList(&l);
	}
	else {
		printf("Comanda de sortare invalida!\n");
	}
}

/*
* Runs the user interface of the application using the service
* @param srv - the service
*/
void runUI(Service* srv) {
	char cmd;
	while (1) {
		cmd = getCommand();
		if (cmd == 'a')
			uiAddExpense(srv);
		else if (cmd == 'm')
			uiModifyExpense(srv);
		else if (cmd == 'd')
			uiDeleteExpense(srv);
		else if (cmd == 'p')
			uiPrintExpenses(srv);
		else if (cmd == 'f')
			uiFilterExpenses(srv);
		else if (cmd == 's')
			uiSortExpenses(srv);
		else if (cmd == 'g')
			uiGenerateTestExpenses(srv);
		else if (cmd == 'q')
			break;
		else
			printf("Comanda invalida!\n");
	}
}

/*
* Runs all the tests in the application
*/
void runTests() {
	testCreateDestroyCopyEqualExpense();
	testValidateExpense();

	testCreateDestroyLengthList();
	testGetSetElementCopyList();
	testDeleteElementList();
	testAddElementList();

	testCreateDestroySizeGetAllRepo();
	testFindRepo();
	testAddRepo();
	testDeleteRepo();
	testModifyRepo();

	testCreateDestroySizeGetAllService();
	testAddService();
	testDeleteService();
	testModifyService();
	testGenerateService();
	testFilterService();
	testSortService();
}

int main() {
	runTests();

	Repository repo = createRepository();
	Service srv = createService(&repo);
	runUI(&srv);
	destroyService(&srv);

	_CrtDumpMemoryLeaks();
	return 0;
}