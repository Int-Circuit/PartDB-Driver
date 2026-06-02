#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <errno.h>
#include <termios.h>
#include <unistd.h> 
int ConsumeSerial();
int configSerial(char* file, int bauds);
