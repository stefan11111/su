#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include <shadow.h>
#include <termios.h>
#include <unistd.h>

#define PWD_MAX 200
#define USER_PATH "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/bin:/usr/lib/llvm/16/bin:/usr/lib/llvm/15/bin"
#define ROOT_PATH "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/bin:/usr/lib/llvm/16/bin:/usr/lib/llvm/15/bin"
#define FLAGS "-c-m-p"

#define SHELL "/bin/bash"	/* chose what shell to spawn if user han no shell */
/*#define MINIMAL*/			/* uncomment to build without support for flags*/
#define REQUIRE_PASSWORD	/* comment to not require a password to become root */
/*#define EMPTY_PASSWORD*/		/* allow empty passwords */

static char change_enviroment = 1;
static char **command = NULL;
