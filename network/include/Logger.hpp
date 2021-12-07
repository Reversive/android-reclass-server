#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef enum {DEBUG=0, INFO, ERROR, FATAL} LOG_LEVEL;
extern LOG_LEVEL current_level;
#define log(level, fmt, ...)   {if(level >= current_level) {\
	FILE * toPrint = ( level >= ERROR ) ? stderr : stdout; \
	\
	if(level == DEBUG) \
		fprintf(toPrint, "%s[ %s ]\033[0m ", "\033[0;32m", "DEBUG"); \
	else if(level == INFO) \
		fprintf(toPrint, "%s[ %s ]\033[0m ", "\033[0;36m", "INFO"); \
	else \
		fprintf(toPrint, "%s[ %s ]\033[0m ", "\033[0;31m", "FATAL"); \
	fprintf(toPrint, fmt, ##__VA_ARGS__); \
	fprintf(toPrint,"\n"); }\
	if ( level==FATAL) exit(1);}

#endif