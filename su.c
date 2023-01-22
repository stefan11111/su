#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#include <shadow.h>
#include <termios.h>
#include <unistd.h>

#define PWD_MAX 200

int main()
{
    if(!getuid()){
        putenv("HOME=/root");
        return system("/bin/bash") != -1;
    }
    struct passwd *user = getpwuid(0);
    char pass[PWD_MAX + 1];
    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(1, 0, &term);
    term.c_lflag |= ECHO;
    if (write(1, "Enter the password: ", 20) < 0)
        tcsetattr(1, 0, &term);
    if(scanf("%200s", pass) != 1){
        printf("Error reading password.\n");
        tcsetattr(1, 0, &term);
        return 0;
    }
    tcsetattr(1, 0, &term);
    printf("\n");

    struct spwd* shadow = getspnam(user->pw_name);

    if (!shadow || !shadow->sp_pwdp){
        printf("Could not get shadow entry.\n");
        return 1;
    }

    char *hashed = NULL;
    hashed = crypt(pass, shadow->sp_pwdp);
    if(!hashed){
        printf("Could not hash password, does root have a password?");
        return 1;
    }

    if(strcmp(hashed, shadow->sp_pwdp)){
        printf("Wrong password.\n");
        return 1;
    }

    if (setuid(0) < 0){
        printf("Could not setuid.\n");
        return -1;
    }

    if (setgid(0) < 0){
        printf("Could not setgid.\n");
        return -1;
    }

    putenv("HOME=/root");
    return system("/bin/bash") != -1;
}
