#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int day = tm.tm_mday;
	int month = tm.tm_mon + 1;
	int year = tm.tm_year + 1900;
	char date[11];
	for (int i = 9; i >= 0; i--) {
		if (i == 2 || i == 5) {
			date[i] = '/';
		}
		else if (i > 5) {
			date[i] = year % 10 + '0';
			year /= 10;
		}
		else if (i > 2) {
			date[i] = month % 10 + '0';
			month /= 10;
		}
		else {
			date[i] = day % 10 + '0';
			day /= 10;
		}
	}
	strncpy_s(termData->transactionDate, 11, date, _TRUNCATE);
	return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {

	if (cardData.cardExpirationDate[3] < termData.transactionDate[8]) {
		return EXPIRED_CARD;
		}
	if (cardData.cardExpirationDate[3] > termData.transactionDate[8]) {
		return TERMINAL_OK;
	}
	else {
		if (cardData.cardExpirationDate[4] < termData.transactionDate[9]) {
			return EXPIRED_CARD;
		}
		else if (cardData.cardExpirationDate[4] > termData.transactionDate[9]) {
			return TERMINAL_OK;
		}
	}
	if (cardData.cardExpirationDate[0] < termData.transactionDate[3]) {
		return EXPIRED_CARD;
	}
	if (cardData.cardExpirationDate[0] > termData.transactionDate[3]) {
		return TERMINAL_OK;
	}
	else {
		if (cardData.cardExpirationDate[1] < termData.transactionDate[4]) {
			return EXPIRED_CARD;
		}
		if (cardData.cardExpirationDate[1] > termData.transactionDate[4]) {
			return TERMINAL_OK;
		}
	}
	return EXPIRED_CARD;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	float transactionAmount;
	printf("\nPlease enter the transaction amount:\n");
	scanf_s("%f", &transactionAmount);
	if (transactionAmount <= 0) {
		return INVALID_AMOUNT;
	}
	else {
		termData->transAmount = transactionAmount;
		return isBelowMaxAmount(termData);
	}
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount) {
	if (maxAmount <= 0) {
		return INVALID_MAX_AMOUNT;
	}
	else {
		termData->maxTransAmount = maxAmount;
		return TERMINAL_OK;
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount > termData->maxTransAmount) {
		return EXCEED_MAX_AMOUNT;
	}
	else {
		return TERMINAL_OK;
	}
}