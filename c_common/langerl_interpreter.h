#ifndef __LANGERL_INTERPRETER_INCLUDE
#define __LANGERL_INTERPRETER_INCLUDE

#include "langerl_logger.h"

#define LOAD_OK 1
#define LOAD_ERROR 2
#define LOAD_ALREADY 3
#define LOAD_MISSING_FILE 4
#define LOAD_EXCEPTION 5
#define EXEC_OK 1
#define EXEC_ERROR 2

int start_interpreter(void **);
int stop_interpreter(void *);
int test_interpreter(int);
int load_file_interpreter(char *);
void * exec_interpreter(char *, int *);
void * call_interpreter(char *, char *, int, void **);

void to_erlang(ei_x_buff *, void *);
void * to_interpreter(ei_x_buff *);
void ** to_interpreter_array(ei_x_buff *);

#endif
