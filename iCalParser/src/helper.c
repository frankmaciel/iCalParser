#include <stdlib.h>
#include "helper.h"

void simpleParser(char *filename, Calendar **obj)
{
    FILE *fp;
    //char* prevFold;
    //char* nextFold;
    //char* foldName;
    char buffer[100];
    char* tmpLine;
    char *frontTemp = NULL;
    char *backTemp = NULL;
    char *dateTemp = NULL;
    char *timeTemp = NULL;
    Event *eventTemp = NULL;
    Alarm *alarmTemp = NULL;
    Property *propTemp = NULL;
    fp = fopen(filename, "r");
    int semiCount = 0;
    if (fp == NULL)
    { // Edit afterwards to check for readability
        fclose(fp);
        fprintf(stderr, "The file selected is empty or does not exist\n");
    }
    else
    {
        (*obj)->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
        (*obj)->events = initializeList(&printEvent, &deleteEvent, &compareEvents);
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        { //Looping through the .ics file

            if (buffer[0] == ';')
            {
                frontTemp = malloc(1);
                backTemp = malloc(1);
            }
            else if ((checkColon(buffer) > -1) && ((checkColon(buffer) < checkSemi(buffer)) || (checkSemi(buffer) == -1)))
            {
                frontTemp = frontDelim(buffer, ':');
                backTemp = backDelim(buffer, ':');
            }
            else if (((checkColon(buffer) == -1) || (checkColon(buffer) > checkSemi(buffer))) && (checkSemi(buffer) > -1))
            {
                frontTemp = frontDelim(buffer, ';');
                backTemp = backDelim(buffer, ';');
            }
            if (buffer[0] == ';')
            {
                semiCount += 1;
            }
            else if (buffer[0] == ' ')
            {
                tmpLine = memmove(buffer, buffer+1, strlen(buffer+1));
                tmpLine[strlen(buffer+1)-2] = '\0';
            }
            else if (strcmp(backTemp, "VERSION") == 0)
            {
                (*obj)->version = atof(frontTemp);
            }
            else if (strcmp(backTemp, "PRODID") == 0)
            {
                strcpy((*obj)->prodID, frontTemp);
            }
            else if ((strcmp(backTemp, "END") == 0) && strcmp(frontTemp, "VCALENDAR") == 0)
            {
                break;
            }
            else if ((strcmp(backTemp, "BEGIN") == 0) && strcmp(frontTemp, "VCALENDAR") == 0)
            {
            }
            else if ((strcmp(backTemp, "BEGIN") == 0) && (strcmp(frontTemp, "VEVENT") == 0))
            { // Check for begin event
                eventTemp = malloc(sizeof(Event));
                eventTemp->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
                eventTemp->alarms = initializeList(&printAlarm, &deleteAlarm, &compareAlarms);
                free(backTemp);
                free(frontTemp);
                while (fgets(buffer, sizeof(buffer), fp) != NULL)
                { // loop till end of event

                    if (buffer[0] == ';')
                    {
                        frontTemp = malloc(1);
                        backTemp = malloc(1);
                    }
                    else if ((checkColon(buffer) > -1) && ((checkColon(buffer) < checkSemi(buffer)) || (checkSemi(buffer) == -1)))
                    {
                        frontTemp = frontDelim(buffer, ':');
                        backTemp = backDelim(buffer, ':');
                    }
                    else if (((checkColon(buffer) == -1) || (checkColon(buffer) > checkSemi(buffer))) && (checkSemi(buffer) > -1))
                    {
                        frontTemp = frontDelim(buffer, ';');
                        backTemp = backDelim(buffer, ';');
                    }

                    if (buffer[0] == ';')
                    {
                        semiCount += 1;
                    }
                    else if (buffer[0] == ' ')
                    {

                        tmpLine = memmove(buffer, buffer+1, strlen(buffer+1)-2);
                        tmpLine[strlen(buffer+1)-1] = '\0';
                        propTemp = realloc(propTemp, sizeof(propTemp) + sizeof(tmpLine) + 1);
                        strcat(propTemp->propDescr, tmpLine);
                    }
                    else if (strcmp(backTemp, "END") == 0)
                    {
                        insertBack((*obj)->events, eventTemp);
                        break;
                    }
                    else if (strcmp(backTemp, "UID") == 0)
                    {
                        strcpy(eventTemp->UID, frontTemp);
                    }
                    else if (strcmp(backTemp, "DTSTAMP") == 0)
                    {
                        timeTemp = frontDelim(frontTemp, 'T');
                        dateTemp = backDelim(backTemp, 'T');
                        strcpy(eventTemp->creationDateTime.date, dateTemp);
                        strcpy(eventTemp->creationDateTime.time, timeTemp);
                        free(dateTemp);
                        free(timeTemp);
                    }
                    else if (strcmp(backTemp, "DTSTART") == 0)
                    {
                        timeTemp = frontDelim(frontTemp, 'T');
                        dateTemp = backDelim(backTemp, 'T');
                        strcpy(eventTemp->startDateTime.date, dateTemp);
                        strcpy(eventTemp->startDateTime.time, timeTemp);
                        free(dateTemp);
                        free(timeTemp);
                    }
                    else if ((strcmp(backTemp, "BEGIN") == 0) && (strcmp(frontTemp, "VALARM")))
                    {
                        alarmTemp = malloc(sizeof(Alarm));
                        alarmTemp->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
                        while (fgets(buffer, sizeof(buffer), fp) != NULL)
                        {
                            if ((checkColon(buffer) > -1) && ((checkColon(buffer) < checkSemi(buffer)) || (checkSemi(buffer) == -1)))
                            { //Checks for Colons
                                frontTemp = frontDelim(buffer, ':');
                                backTemp = backDelim(buffer, ':');
                            }
                            else if (((checkColon(buffer) == -1) || (checkColon(buffer) > checkSemi(buffer))) && (checkSemi(buffer) > -1))
                            { //Checks for semi-colons
                                frontTemp = frontDelim(buffer, ';');
                                backTemp = backDelim(buffer, ';');
                            }

                            if (buffer[0] == ';') //Checks for comments
                            {
                            }
                            else if (buffer[0] == ' ')
                            {                
                                tmpLine = memmove(buffer, buffer+1, strlen(buffer+1)-2);
                                tmpLine[strlen(buffer+1)-1] = '\0';
                                propTemp = realloc(propTemp, sizeof(propTemp) + sizeof(tmpLine) + 1);
                                strcat(propTemp->propDescr, tmpLine);
                            }
                            else if (buffer[0] == ' ') //Check for linefolding
                            {
                                strcpy(buffer, buffer + 1);
                                strcat(propTemp->propDescr, buffer);
                            }
                            else if ((strcmp(backTemp, "END") == 0))
                            {
                                insertBack(eventTemp->alarms, alarmTemp);
                                break;
                            }
                            else if (strcmp(backTemp, "ACTION") == 0)
                            {
                                strcpy(alarmTemp->action, frontTemp);
                            }
                            else if ((strcmp(backTemp, "TRIGGER") == 0))
                            {
                                alarmTemp->trigger = malloc(strlen(frontTemp) + 1);
                                strcpy(alarmTemp->action, frontTemp);
                            }
                            else
                            {
                                propTemp = initProp(backTemp, frontTemp);
                                insertBack(alarmTemp->properties, propTemp);
                            }
                            free(backTemp);
                            free(frontTemp);
                        }
                        free(backTemp);
                        free(frontTemp);
                    }
                    else
                    {
                        propTemp = initProp(backTemp, frontTemp);
                        insertBack(eventTemp->properties, propTemp);
                    }
                    free(backTemp);
                    free(frontTemp);
                }
            }
            else
            {
                propTemp = initProp(backTemp, frontTemp);
                insertBack((*obj)->properties, propTemp);
            }
            free(frontTemp);
            free(backTemp);
        }
        if (semiCount >= 0)
        {

        }
        else
        {
            free(backTemp);
            free(frontTemp);
        }
        
    }
    fclose(fp);
}

char *frontDelim(char *stringLine, char token)
{
    char *tempString = NULL;
    int k = 0;

    for (int i = 0; i < strlen(stringLine); i++)
    { //loop through entire line
        if (stringLine[i] == token)
        { //check for the delimiter
            tempString = malloc((strlen(stringLine) - i + 2) * sizeof(char));
            for (int j = i + 1; j < (strlen(stringLine) - 1); j++)
            { //Copy everything in front of delimiter
                tempString[k] = stringLine[j];
                k++;
            }
        }
    }
    if (tempString == NULL)
    {
        return tempString;
    }
    else
    {
        tempString[k] = '\0';
        return tempString;
    }
}

char *backDelim(char *stringLine, char token)
{
    char *tempString = NULL;
    for (int i = 0; i < strlen(stringLine) - 1; i++)
    { // loop through line
        if (stringLine[i] == token)
        { // check for delimiter
            tempString = malloc(i + 2);
            for (int j = 0; j < i + 1; j++)
            { //loops up to the delimiter
                if (stringLine[j] == token)
                { //checks for the last index and sets it to null terminator
                    tempString[j] = '\0';
                    return tempString;
                }
                else
                {
                    tempString[j] = stringLine[j];
                }
            }
        }
    }
    return tempString;
}

int checkSemi(char *toCheck)
{ // checks for the first semi-colon and returns its index
    for (int i = 0; i < strlen(toCheck); i++)
    {
        if (toCheck[i] == ';')
        {
            return i;
        }
    }
    return -1;
}

int checkColon(char *toCheck)
{ // checks for the first colon and returns its index
    for (int i = 0; i < strlen(toCheck); i++)
    {
        if (toCheck[i] == ':')
        {
            return i;
        }
    }
    return -1;
}

void lineFold(char **prevLine, char **currentLine)
{ // Function for help with linefolding
    (*prevLine) = realloc((*prevLine), strlen(*prevLine) + strlen(*currentLine) + 1);
    strcat(*prevLine, *currentLine);
}

Property *initProp(char *propName, char *propDescr)
{
    int frontLen = strlen(propDescr);
    Property *propTemp;
    propTemp = malloc(sizeof(Property) + (sizeof(char) * (frontLen + 1)));
    strcpy(propTemp->propName, propName);
    strcpy(propTemp->propDescr, propDescr);
    return propTemp;
}
char *stringCat(char *dest, const char *src)
{
    if (dest != NULL)
    {
        dest = malloc((strlen(dest) * sizeof(char)) + (strlen(src) * sizeof(char)) + 1);
    }
    else
    {
        dest = malloc(strlen(src) + 1);
    }
    strcat(dest, src);
    return dest;
}