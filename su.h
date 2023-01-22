#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#include <shadow.h>
#include <termios.h>
#include <unistd.h>

#define PWD_MAX 200

#define SHELL "/bin/bash"	/* chose what shell to spawn */
