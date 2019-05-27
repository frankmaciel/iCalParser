#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <string.h>
#include "CalendarParser.h"

void simpleParser(char *filename, Calendar **obj);

char *frontDelim(char *stringLine, char token);

char *backDelim(char *stringLine, char token);

int checkColon(char *toCheck);

int checkSemi(char *toCheck);

Property *initProp(char *propName, char *propDescr);

char *stringCat(char *dest, const char *src);

void lineFold(char **prevLine, char **currentLine);


#endif
