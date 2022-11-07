#include "app.h"

void appStart(void) {
	ST_cardData_t card;
	ST_terminalData_t terminal;
	ST_cardData_t* card_ptr = &card;
	ST_terminalData_t* terminal_ptr = &terminal;
	setMaxAmount(terminal_ptr, 8000);
	EN_cardError_t cardError = getCardHolderName(card_ptr);
	while (cardError != CARD_OK) {
		printf("Invalid Name. Please try again. \n\n");
		cardError = getCardHolderName(card_ptr);
	}
	cardError = getCardExpiryDate(card_ptr);
	while (cardError != CARD_OK) {
		printf("Invalid Expiration Date. Please try again. \n\n");
		cardError = getCardExpiryDate(card_ptr);
	}
	cardError = getCardPan(card_ptr);
	while (cardError != CARD_OK) {
		printf("Invalid PAN. Please try again. \n\n");
		cardError = getCardPan(card_ptr);
	}
	getTransactionDate(terminal_ptr);
	EN_terminalError_t terminalError = isCardExpired(card, terminal);
	if (terminalError != TERMINAL_OK) {
		printf("Transaction Declined. Your card has expired");
		exit(1);
	}
	terminalError = getTransactionAmount(terminal_ptr);
	while (terminalError != TERMINAL_OK) {
		if (terminalError == EXCEED_MAX_AMOUNT) {
			printf("Transaction exceeds maximum amount. Maximum amount is %f. Please try again", terminal_ptr->maxTransAmount);
		}
		else {
			printf("Invalid Transaction Amount. Please enter a positive number.");
		}
		terminalError = getTransactionAmount(terminal_ptr);
	}
	ST_transaction_t transaction;
	ST_transaction_t* transaction_ptr = &transaction;
	receiveTransactionData(transaction, card_ptr, terminal);
}

int main() {	
	appStart();
}
