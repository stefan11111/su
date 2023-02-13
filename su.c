#include "su.h"

static int switch_user(struct passwd *user)
{
    if (setgid(user->pw_gid) < 0){
        printf("Could not setgid.\n");
        return -1;
    }
    if (setuid(user->pw_uid) < 0){
        printf("Could not setuid.\n");
        return -1;
    }
    if(!user->pw_uid){
        putenv("HOME=/root");
    }
    else{
        putenv(HOME);
    }
    if(!user->pw_shell){
        return execl(SHELL, SHELL, (char*)NULL) == -1;
    }
    return execl(user->pw_shell, user->pw_shell, (char*)NULL) == -1;
}

int main(int argc, char **argv)
{
    if(argc > 2){
        printf("USAGE: %s [user]\n", argv[0]);
        return 0;
    }
    uid_t ruid = getuid();
    struct passwd *user = NULL;
    if(argc == 1){
        user = getpwuid(0);
    }
    else{
        user = getpwnam(argv[1]);
    }
    if(!user){
        printf("User does not exist\n");
        return -1;
    }
    strcat(HOME, user->pw_name);
    if(!ruid){
        return switch_user(user);
    }
    if(ruid == user->pw_uid){
        return 0;
    }
    struct spwd* shadow = getspnam(user->pw_name);

    if (!shadow || !shadow->sp_pwdp){
        printf("Could not get shadow entry.\n");
        return 1;
    }

    if (!strcmp(shadow->sp_pwdp, "*")) {
        return switch_user(user);
    }
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

    char *hashed = NULL;
    hashed = crypt(pass, shadow->sp_pwdp);
    if(!hashed){
        printf("Could not hash password.\n");
        return 1;
    }

    if(strcmp(hashed, shadow->sp_pwdp)){
        printf("Wrong password.\n");
        return 1;
    }

    return switch_user(user);
}
