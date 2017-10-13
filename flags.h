#ifndef FLAGS_H
#define FLAGS_H


/* Flag used to synchronize the execution of tasks.
 * Without this one of the flags listed below might be raised between a task
 * and another and break their priorities. */
extern bit Ready;
/* Flags indicating whether a task is pending
 * No multiple pending tasks should be allowed */ 
extern bit FlagAcc;
extern bit FlagTer;
extern bit FlagLcd;


#endif
