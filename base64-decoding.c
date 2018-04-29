//********************************************************************************//
//******** Coded by tic-toc-developer, mail: tic.toc.developer@icloud.com ********//
//********  Could decode the base64 which encoded from ASCII plain text   ********//
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

int reverseBase64Table[256];

unsigned long long InitialString(stringList stringStart);
unsigned long long DecodingString(stringList encodedString, unsigned long long encodedCount, stringList decodedString);
void PrintString(stringList stringStart, unsigned long long count);

int main(void) {
	for(int i = 0; i < 64; i++)
		reverseBase64Table[base64Table[i]] = i;

	unsigned long long encodedCount = 0;
	unsigned long long decodedCount = 0;
	stringList encodedString = (stringList)malloc(sizeof(stringList));
	stringList decodedString = (stringList)malloc(sizeof(stringList));
	
	printf("Input encoded string:\n"); // get encoded string
	encodedCount = InitialString(encodedString);
	
	if(encodedCount) {
		// PrintString(encodedString, encodedCount);
		decodedCount = DecodingString(encodedString, encodedCount, decodedString); // decode string with base64
		printf("Output decoded string:\n");
		PrintString(decodedString, decodedCount); // print decoded string
	} else {
		printf("Input invalid.\n");
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

	if(count % 4)
		return 0;

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

unsigned long long DecodingString(stringList encodedString, unsigned long long encodedCount, stringList decodedString) {
	unsigned long long decodedCount = 0;
	int equalSignCount = 0;
	unsigned long long countTemp = encodedCount;
	stringList encodedStringTemp = encodedString;
	stringList decodedStringTemp = decodedString;

	while(countTemp) {
		
		char transStr[24];
		memset(transStr, 0, sizeof(transStr));
		int index = 0;
		for(int i = 0; i < 4; i++) { // repeat 4 times to get 4 char, each 6 bits (24 bits)
			char charTemp = encodedStringTemp->character;
			encodedStringTemp = encodedStringTemp->next;

			char monoChar[6];
			if (charTemp != '=') {
				int temp = reverseBase64Table[charTemp];
				for(int i = 0; i < 6; i++) {
					monoChar[i] = temp % 2 + '0';
					temp /= 2;
				}
			} else {
				equalSignCount++;
				for(int i = 0; i < 6; i++)
					monoChar[i] = '0';
			}

			for (int i = 5; i >= 0; i--)
				transStr[index++] = monoChar[i];
		}

		index = 0;
		for(int i = 0; i < 3; i++) { // repeat 3 times to put 3 char, each 8 bits (24 bits)
			int temp = 0;
			for(int i = 0; i < 8; i++)
				temp = temp * 2 + (transStr[i + index] - '0');

			index += 8;
			decodedStringTemp->character = temp;
			decodedStringTemp->next = (stringList)malloc(sizeof(stringList));
			decodedStringTemp = decodedStringTemp->next;
		}

		countTemp -= 4;
		decodedCount += 3;
	}

	decodedCount -= equalSignCount;

	return decodedCount;
}




