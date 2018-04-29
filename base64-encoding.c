//********************************************************************************//
//******** Coded by tic-toc-developer, mail: tic.toc.developer@icloud.com ********//
//**************    Could encode ASCII plain text to base64 text    **************//
//********************************************************************************//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stringNode {
    char character;
    struct stringNode *next;
}stringNode, *stringList;

char base64Table[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', \
						'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', \
						'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', \
						'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', \
						'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', \
						'o', 'p', 'q', 'r', 's', 't', 'u', 'v', \
						'w', 'x', 'y', 'z', '0', '1', '2', '3', \
						'4', '5', '6', '7', '8', '9', '+', '/' };

unsigned long long InitialString(stringList stringStart);
unsigned long long EncodingString(stringList originalString, unsigned long long originalCount, stringList encodedString);
void PrintString(stringList stringStart, unsigned long long count);

int main(void) {
	unsigned long long originalCount = 0;
	unsigned long long encodedCount = 0;
	stringList originalString = (stringList)malloc(sizeof(stringList));
	stringList encodedString = (stringList)malloc(sizeof(stringList));

	printf("Input original string:\n"); // get original string
	originalCount = InitialString(originalString);

	if(originalCount) {
		encodedCount = EncodingString(originalString, originalCount, encodedString); // encode string with base64
		printf("Output encoded string:\n");
		PrintString(encodedString, encodedCount); // print encoded string
	} else {
		printf("String length is 0.\n");
	}

	return 0;
}

unsigned long long InitialString(stringList stringStart) {
	unsigned long long count = 0;
	char charTemp = 0;
	stringList stringTemp = stringStart;
	while((charTemp = fgetc(stdin)) != '\n') {
		stringTemp->character = charTemp;
		stringTemp->next = (stringList)malloc(sizeof(stringList));
		stringTemp = stringTemp->next;
		count++;
	}

	while(count % 3) {
		stringTemp->character = '=';
		stringTemp->next = (stringList)malloc(sizeof(stringList));
		stringTemp = stringTemp->next;
		count++;
	}

	return count;
}

void PrintString(stringList stringStart, unsigned long long count) {
	char charTemp = 0;
	stringList stringTemp = stringStart;
	
	for(unsigned long long i = 0; i < count; i++) {
		printf("%c", stringTemp->character);
		stringTemp = stringTemp->next;
	}
	printf("\n");
}

unsigned long long EncodingString(stringList originalString, unsigned long long originalCount, stringList encodedString) {
	unsigned long long encodedCount = 0;
	int equalSignCount = 0;
	unsigned long long countTemp = originalCount;
	stringList originalStringTemp = originalString;
	stringList encodedStringTemp = encodedString;

	while(countTemp) {
		
		char transStr[24];
		memset(transStr, 0, sizeof(transStr));
		int index = 0;
		for(int i = 0; i < 3; i++) { // repeat 3 times to get 3 char, each 8 bits (24 bits)
			char charTemp = originalStringTemp->character;
			originalStringTemp = originalStringTemp->next;

			char monoChar[8];
			if (charTemp != '=') {
				for(int i = 0; i < 8; i++) {
					monoChar[i] = charTemp % 2 + '0';
					charTemp /= 2;
				}
			} else {
				equalSignCount++;
				for(int i = 0; i < 8; i++)
					monoChar[i] = '0';
			}

			for (int i = 7; i >= 0; i--)
				transStr[index++] = monoChar[i];
		}

		index = 0;
		for(int i = 0; i < 4; i++) { // repeat 4 times to put 4 char, each 6 bits (24 bits)
			int temp = 0;
			for(int i = 0; i < 6; i++)
				temp = temp * 2 + (transStr[i + index] - '0');

			index += 6;
			encodedStringTemp->character = base64Table[temp];
			encodedStringTemp->next = (stringList)malloc(sizeof(stringList));
			encodedStringTemp = encodedStringTemp->next;
		}

		countTemp -= 3;
		encodedCount += 4;
	}

	encodedStringTemp = encodedString;
	for(unsigned long long i = 0; i < encodedCount; i++) {
		if(i + equalSignCount >= encodedCount)
			encodedStringTemp->character = '=';

		encodedStringTemp = encodedStringTemp->next;
	}

	return encodedCount;
}




