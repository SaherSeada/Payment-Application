#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	char cardHolderName[27];
	printf("Please enter the Card Holder Name: \n");
	fgets(cardHolderName, 27, stdin);
	if (strlen(cardHolderName) - 1 > 24 || strlen(cardHolderName) - 1 < 20) {
		return WRONG_NAME;
	}
	else {
		strncpy_s(cardData->cardHolderName, 25, cardHolderName, _TRUNCATE);
		return CARD_OK;
	}
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	char cardExpiryDate[8];
	printf("\nPlease enter the Card Expiry Date: \n");
	fgets(cardExpiryDate, 8, stdin);
	if (strlen(cardExpiryDate) - 1 != 5) {
		return WRONG_EXP_DATE;
	}
	for (int i = 0; i < 5; i++) {
		if (i == 2) {
			if (cardExpiryDate[i] != '/') {
				return WRONG_EXP_DATE;
			}
		}
		else {
			if (isdigit(cardExpiryDate[i])) {
				continue;
			}
			else {
				return WRONG_EXP_DATE;
			}
		}
	}
	strncpy_s(cardData->cardExpirationDate, 6,  cardExpiryDate, _TRUNCATE);
	return CARD_OK;
}

EN_cardError_t getCardPan(ST_cardData_t* cardData)
{
	char cardPan[22];
	printf("\nPlease enter the Card's Primary Account Number: \n");
	fgets(cardPan, 22, stdin);
	if (strlen(cardPan) - 1 > 19 || strlen(cardPan) - 1 < 16) {
		return WRONG_PAN;
	}
	for (int i = 0; i < strlen(cardPan) - 1; i++) {
		if (isdigit(cardPan[i])) {
			continue;
		}
		else {
			return WRONG_PAN;
		}
	}
	strncpy_s(cardData->primaryAccountNumber, 20, cardPan, _TRUNCATE);
	return CARD_OK;
}
