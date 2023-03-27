#include "su.h"

static void switch_user(struct passwd *user, char **program, char change_enviroment)
{
    if (setgid(user->pw_gid) < 0){
        printf("Could not setgid.\n");
        exit(EXIT_FAILURE);
    }
    if (setuid(user->pw_uid) < 0){
        printf("Could not setuid.\n");
        exit(EXIT_FAILURE);
    }
    char const *term = getenv("TERM");
    if (term) {
        setenv("TERM", term, (int)change_enviroment);
    }
    setenv("HOME", user->pw_dir, (int)change_enviroment);
    setenv("SHELL", user->pw_shell, (int)change_enviroment);
    setenv("USER", user->pw_name, (int)change_enviroment);
    setenv("LOGNAME", user->pw_name, (int)change_enviroment);
    setenv("PATH", (user->pw_uid ? USER_PATH : ROOT_PATH), (int)change_enviroment);

    if (!program) {
        if (!user->pw_shell){
            execl(SHELL, SHELL, (char*)NULL);
            exit(EXIT_SUCCESS);
        }
        execl(user->pw_shell, user->pw_shell, (char*)NULL);
        exit(EXIT_SUCCESS);
    }
    if (execvp(*program, program) == -1) {
        printf("%s: command not found\n", *program);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

static int check_password(struct spwd* shadow)
{
    /*check for empty password*/
    if (!strcmp(shadow->sp_pwdp, "*") || !strcmp(shadow->sp_pwdp, "")) {
#ifdef EMPTY_PASSWORD
        return 0;
#else
        exit(EXIT_FAILURE);
#endif
    }
    char pass[PWD_MAX + 1];
    struct termios term;
    tcgetattr(1, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(1, 0, &term);
    term.c_lflag |= ECHO;
    if (write(1, "Enter the password: ", 20) < 0) {
        tcsetattr(1, 0, &term);
    }
    if (scanf("%200s", pass) != 1){
        printf("Error reading password.\n");
        tcsetattr(1, 0, &term);
        exit(EXIT_FAILURE);
    }
    tcsetattr(1, 0, &term);
    printf("\n");

    char *hashed = NULL;
    hashed = crypt(pass, shadow->sp_pwdp);
    if (!hashed){
        printf("Could not hash password, does your user have a password?.\n");
        exit(EXIT_FAILURE);
    }
    if (strcmp(hashed, shadow->sp_pwdp)){
        printf("Wrong password.\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int main(int argc, char **argv)
{
    uid_t ruid = getuid();
    struct passwd *user = NULL;
    if (argc == 1) {
        user = getpwuid(0);
    }
    else {
#ifdef MINIMAL
        if (argc > 2) {
            printf("USAGE: %s [user]\n", argv[0]);
            return 0;
        }
        user = getpwnam(argv[1]);
#else
        char **ptr = argv;
        while(*(++ptr)) {
            if (!strcmp(*ptr, "-c")) {
                command = ptr + 1;
                break;
            }
            if (!strcmp(*ptr, "-m") || !strcmp(*ptr, "-p")) {
                change_enviroment = 0;
            }
        }
        user = strstr(FLAGS, argv[1]) ? getpwuid(0) : getpwnam(argv[1]);
#endif
    }
    if (!user){
        printf("User does not exist\n");
        return -1;
    }
    if (ruid) {
        if (ruid == user->pw_uid){
            return 0;
        }
        struct spwd* shadow = getspnam(user->pw_name);
        if (!shadow || !shadow->sp_pwdp){
            printf("Could not get shadow entry.\n");
            return 1;
        }
#ifdef REQUIRE_PASSWORD
        if (check_password(shadow)) {
            return 1;
        }
#endif
    }
    switch_user(user, command, change_enviroment);
    return 0;
}
