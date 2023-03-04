#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include <shadow.h>
#include <termios.h>
#include <unistd.h>

#define PWD_MAX 200
#define MAX_PATH 200
static char home[MAX_PATH] = "HOME=/home/";

#define SHELL "/bin/bash"	/* chose what shell to spawn if user han no shell */
#define COMMAND			/* comment to build without support for -c option*/
#define REQUIRE_PASSWORD	/* comment to not require a password to become root */
