#include "CalendarParser.h"

int main(){

    ICalErrorCode code;
    Calendar* obj = NULL;

    code = createCalendar("mLineProp1.ics", &obj);
    printf("\n%s\n\n", obj->prodID);
    printf("%.2f\n\n", obj->version);
    printf("%s\n\n", obj->properties->tail->data);
    

    deleteCalendar(obj);
    free(obj);

    return 0;
}
