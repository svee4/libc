#include "string.h"
#include "lib.h"
#include "mem.h"

static bool bitset(char val, int bit) {
	return (val >> bit) & 1;
}

int str_len(char* str) {
	int len = 0;
	for (char* p = str; *p != NULL; p++) {
		if (!bitset(*p, 7)) {
			len++;
		} else if (!bitset(*p, 5)) {
			len++;
			p += 1;
		} else if (!bitset(*p, 4)) {
			len++;
			p += 2;
		} else {
			len++;
			p += 3;
		}
	}
	return len;
}

int str_size(char* str) {
	char* p = str;
	for (; *p != NULL; p++) {}
	return p - str;
}

char* long_to_str(long from) {
	if (from == 0) {
		char* zero = (char*)malloc(sizeof(char) * 2);
		zero[0] = '0';
		return zero;
	}

	int len = 0;
	long n = from;
	while (n != 0) {
		n /= 10;
		++len;
	}

	char ex = 0;
	if (from < 0) {
		ex = 1;
		from = -from;
	}

	char* target = (char*)malloc(sizeof(char) * (len + 1 + ex));
	if (ex) {
		target[0] = '-';
	}

	for (int i = 0; i < len; i++) {
		long num = from;
		for (int j = 1; j < len - i; j++) {
			num /= 10;
		}
		target[i + ex] = (num % 10) + 48;
	}
	return target;
}

char* str_concat(char* str1, char* str2) {
	int str1s = str_size(str1);
	int str2s = str_size(str2);
	char* new = (char*)malloc(sizeof(char) * (str1s + str2s));
	mem_copy(new, str1, str1s);
	mem_copy(new + str1s, str2, str2s);
	return new;
}

/*
 * compares strings until NULL character. Returns false if any byte is not equal
 */
bool str_equals(char* str1, char* str2) {
	while (*str1 != NULL) {
		if (*str1 != *str2) return false;
		str1++;
		str2++;
	}
	if (*str2 != NULL) return false;
	return true;
}
