#ifndef STRING_FUNCS_HEADER_H
#define STRING_FUNCS_HEADER_H

char* trim(char* entry);
char* slice(char* string, int start, int end);
int isNum(char entry);
int isLetter(char entry);
int charIn(char entry, char* comparator);

#endif 