#include "server.h"

uint32_t transactions = 1;

ST_accountsDB_t accountsDB[255] = {
	{9000.0, RUNNING, "1234567891234567890"},
	{5000.0, RUNNING, "9876543219876543210"},
	{0.0, RUNNING, "1472583691472583690"},
	{3500.0, BLOCKED, "7894561231234567890"},
	{100000.0, BLOCKED, "7418529637418529630"}
};

ST_transaction_t transactionsDB[255] = { 0 };

int isValidAccount(ST_cardData_t* cardData) {
	char cardPan[20];
	int i = 0;
	strncpy_s(cardPan, 20, cardData->primaryAccountNumber, _TRUNCATE);
	while (i < 255) {
		if (strcmp(&accountsDB[i].primaryAccountNumber, &cardPan) == 0) {
			return i;
		}
		i++;
	}
	return -1;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t termData, int i) {
	if (accountsDB[i].balance >= termData.transAmount) {
		return SERVER_OK;
	}
	else{
		return LOW_BALANCE;
	}
}

EN_serverError_t isBlockedAccount(int i) {
	if (accountsDB[i].state == BLOCKED) {
		return BLOCKED_ACCOUNT;
	}
	else {
		return SERVER_OK;
	}
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t transData) {
	int i = 0;
	while (i < 255) {
		if (transactionsDB[i].transactionSequenceNumber == transactionSequenceNumber) {
			printf("Transaction number %d:\n", transactionSequenceNumber);
			printf("Card Pan: %s\n", transactionsDB[i].cardHolderData->primaryAccountNumber);
			printf("Transaction Amount: %f\n", transactionsDB[i].terminalData->transAmount);
			printf("Transaction Date: %s\n", transactionsDB[i].terminalData->transactionDate);
			if (transData.transState == APPROVED) {
				printf("Transaction State: APPROVED\n");
			}
			else {
				printf("Transaction State: DECLINED\n");
			}
			return SERVER_OK;
		}
		i++;
	}
	return TRANSACTION_NOT_FOUND;
}

EN_serverError_t saveTransaction(ST_transaction_t transData) {
	transData.transactionSequenceNumber = transactions;
	transactions++;
	transData.transState = APPROVED;
	transactionsDB[transactions - 1] = transData;
	if (getTransaction(transData.transactionSequenceNumber, transData) == SERVER_OK) {
		return SERVER_OK;
	}
	else {
		return SAVING_FAILED;
	}
}

EN_transState_t receiveTransactionData(ST_transaction_t transData, ST_cardData_t* cardData, ST_terminalData_t termData) {
	int indexInDB = isValidAccount(cardData);
	if (indexInDB == -1) {
		printf("Transaction Failed. This account doesn't exist.");
		return FRAUD_CARD;
	}
	if (isAmountAvailable(termData, indexInDB) == LOW_BALANCE) {
		printf("Transaction Failed due to Insufficient Funds.");
		return INSUFFICIENT_FUNDS;
	}
	if (isBlockedAccount(indexInDB) == BLOCKED_ACCOUNT) {
		printf("Transaction Failed. This account is blocked.");
		return DECLINED_STOLEN_CARD;
	}
	accountsDB[indexInDB].balance -= termData.transAmount;
	transData.cardHolderData = cardData;
	transData.terminalData = &termData;
	printf("\nTransaction Successful. Your new balance is: %f\n\n", accountsDB[indexInDB].balance);
	saveTransaction(transData);
}
