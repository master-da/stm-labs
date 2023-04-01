#include "StringParser.h"

void modifyString(unsigned char s[]) {
	for (int i=0; s[i] != '\0'; i++) {
		s[i]++;
	}
}
