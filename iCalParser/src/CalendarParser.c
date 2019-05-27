#include "helper.h"
#include "CalendarParser.h"
#include "LinkedListAPI.h"

ICalErrorCode createCalendar(char *fileName, Calendar **obj)
{
    char buffer[5];
    int count = 4;

    for (int i = 0; i < 4; i++)
    { //Saving the last four characters of filename
        buffer[i] = fileName[strlen(fileName) - count--];
    }
    buffer[4] = '\0';
    if (strcmp(".ics", buffer) != 0)
    { //checks if the last four characters are .ics
        fprintf(stderr, "The file entered does not have a .ics extension!\n");
        exit(1);
    }
    else
    { // if it is a .ics extension we call the parser
        (*obj) = malloc(sizeof(Calendar));
        (*obj)->properties = NULL;
        (*obj)->events = NULL;

        simpleParser(fileName, obj);
        
    }

    if (strcmp((*obj)->prodID, "") == 0)
    { // if the object parses incorrectly
        return INV_FILE;
    }
    else
    { // if the object parsed successfully
        return OK;
    }
}

char *printCalendar(const Calendar *obj)
{ // print function for a calendar
    char *str;
    int lenVersion;
    int lenPRODID;
    lenVersion = (sizeof((*obj).version)) / 4;
    lenPRODID = strlen(obj->prodID);
    str = malloc(lenVersion + lenPRODID + 20);
    sprintf(str, "VERSION:%.1f\nPRODID:%s\n", obj->version, obj->prodID);
    return str;
}

void deleteCalendar(Calendar *obj)
{ // deletes a calendar object
    if (obj == NULL)
    {
        return;
    }
    else
    {
        if (obj->events != NULL)
        {
            freeList(obj->events);
        }
        if (obj->properties != NULL)
        {
            freeList(obj->properties);
        }
    }
}

void deleteEvent(void *toBeDeleted)
{
    if ((struct evt *)toBeDeleted != NULL)
    {
        if (((struct evt *)toBeDeleted)->properties != NULL)
        {
            freeList(((struct evt *)toBeDeleted)->properties);
        }
        if (((struct evt *)toBeDeleted)->alarms != NULL)
        {
            freeList(((struct evt *)toBeDeleted)->alarms);
        }
        free((struct evt *)toBeDeleted);
    }
}
int compareEvents(const void *first, const void *second)
{
    if (strcmp((((struct evt *)first)->UID), ((struct evt *)second)->UID) == 0)
    {
        return 0;
    }
    else if (strcmp((((struct evt *)first)->UID), ((struct evt *)second)->UID) < 0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}
char *printEvent(void *toBePrinted)
{
    char *temp = NULL;
    if (toBePrinted == NULL)
    {
        return NULL;
    }
    else
    {
        temp = stringCat(temp, "UID:");
        temp = stringCat(temp, (((Event *)toBePrinted)->UID));
        temp = stringCat(temp, "\nDTSTAMP:");
        temp = stringCat(temp, (((Event *)toBePrinted)->creationDateTime.date));
        temp = stringCat(temp, (((Event *)toBePrinted)->creationDateTime.time));
        temp = stringCat(temp, "\nDTSTART:");
        temp = stringCat(temp, (((Event *)toBePrinted)->creationDateTime.date));
        temp = stringCat(temp, (((Event *)toBePrinted)->creationDateTime.time));
        temp = stringCat(temp, "\n");
        return temp;
    }
}

void deleteAlarm(void *toBeDeleted)
{
    if ((Alarm *)toBeDeleted != NULL)
    {
        if (((Alarm *)toBeDeleted)->properties != NULL)
        {
            freeList(((struct evt *)toBeDeleted)->properties);
        }
        if (((Alarm *)toBeDeleted)->trigger != NULL)
        {
            free(((Alarm *)toBeDeleted)->trigger);
        }
        free((Alarm *)toBeDeleted);
    }
}
int compareAlarms(const void *first, const void *second)
{
    if (strcmp((((Alarm *)first)->action), ((Alarm *)second)->action) == 0)
    {
        return 0;
    }
    else if (strcmp((((Alarm *)first)->action), ((Alarm *)second)->action) < 0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}
char *printAlarm(void *toBePrinted)
{
    char *temp = NULL;
    if (toBePrinted == NULL)
    {
        return NULL;
    }
    else
    {
        temp = stringCat(temp, "ACTION:");
        temp = stringCat(temp, (((Alarm *)toBePrinted)->action));
        temp = stringCat(temp, "\nTRIGGER:");
        if ((((Alarm *)toBePrinted)->trigger) == NULL)
        {
            temp = stringCat(temp, "[NULL]");
        }
        else
        {
            temp = stringCat(temp, (((Alarm *)toBePrinted)->trigger));
        }
        temp = stringCat(temp, "\n");
        return temp;
    }
}
void deleteProperty(void *toBeDeleted)
{
    free((struct prop *) toBeDeleted);
}
int compareProperties(const void *first, const void *second)
{
    if (strcmp(((Property *)first)->propName, ((Property *)second)->propName) == 0)
    {
        return 0;
    }
    else if (strcmp((((Property *)first)->propName), ((Property *)second)->propName) < 0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}
char *printProperty(void *toBePrinted)
{
    char *temp = NULL;
    if (toBePrinted == NULL)
    {
        return NULL;
    }
    else
    {
        temp = stringCat(temp, "propName:");
        temp = stringCat(temp, (((Property *)toBePrinted)->propName));
        temp = stringCat(temp, "\npropDescr:");
        temp = stringCat(temp, (((Property *)toBePrinted)->propDescr));
        temp = stringCat(temp, "\n");
        return temp;
    }
}

void deleteDate(void *toBeDeleted)
{
 //Cannot be used ?
}

int compareDates(const void *first, const void *second)
{
    // Also no used?
    return 0;
}
char *printDate(void *toBePrinted)
{
    // Also not used?
    return NULL;
}

ICalErrorCode writeCalendar(char* fileName, const Calendar* obj){
    
    return OK;
}

ICalErrorCode validateCalendar(const Calendar* obj){

    return OK;
}

char *printError(ICalErrorCode error)
{
    char *stringPrint = NULL;
    if(error == INV_CAL)
    {
        stringPrint = malloc(sizeof("error! {INV_CAL} : invalid iCal Calendar\n"));
        strcpy(stringPrint, "error! {INV_CAL} : invalid iCal Calendar\n");
    }
    else if(error == DUP_VER)
    {
        stringPrint = malloc(sizeof("error! {DUP_VER} : There were more than one VERSION properties present\n"));
        strcpy(stringPrint, "error! {DUP_VER} : There were more than one VERSION properteis present\n");    
    }
    else if(error == DUP_PRODID)
    {
        stringPrint = malloc(sizeof("error! {DUP_PRODID} : There were more than one PRODID properties present\n"));
        strcpy(stringPrint, "error! {DUP_PRODID} : There were more than one PRODID properties present\n");
    }
    else if(error == OTHER_ERROR)
    {
        stringPrint = malloc(sizeof("error! {OTHER_ERROR} : There was non-calendar error which has occured\n"));
        strcpy(stringPrint, "error! {OTHER_ERROR} : There was non-calendar error which has occured\n");
    }
    else if(error == INV_FILE)
    {
        stringPrint = malloc(sizeof("error! {error} : An error has occurred when attempting to read/find the file\n"));
        strcpy(stringPrint, "error! {INV_FILE} : An error has occurred when attempting to read/find the file\n");
    }
    else if(error == INV_VER)
    {
        stringPrint = malloc(sizeof("error! {INV_VER} : The VERSION property is incorrect\n"));
        strcpy(stringPrint, "error! {INV_VER} : VERSION is invalid\n");
    }
    else if(error == INV_PRODID)
    {
        stringPrint = malloc(sizeof("error! {INV_PRODID} : The PRODID property is incorrect\n"));
        strcpy(stringPrint, "error {INV_PRODID} : PRODID is invalid\n");
    }
    else if(error == INV_EVENT)
    {
        stringPrint = malloc(sizeof("error! {INV_EVENT} : There was an invalid Event"));
        strcpy(stringPrint, "error! {INV_EVENT} : There was an invalid Event");
    }
    else if(error == INV_DT)
    {
        stringPrint = malloc(sizeof("error! {INV_DT} : There was an invalid DateTime\n"));
        strcpy(stringPrint, "error! {INV_DT} : There was an invalid DateTime\n");
    }
    else if(error == INV_ALARM)
    {
        stringPrint = malloc(sizeof("error! {INV_ALARM} : There was an invalid alarm "));
        strcpy(stringPrint, "error! {INV_ALARM} : There was an invalid alarm");
    } 
    else
    {
        stringPrint = malloc(sizeof("OK\n"));
        strcpy(stringPrint, "OK\n");
    }
    return stringPrint;

}