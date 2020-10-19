#include <string.h>
#include <ctype.h>

/** 
 * Returns a pointer to a substring of the original 
 * string trimming leading and trailing whitespaces.
 */
char *trim(char *str)
{
    char *end;
    /** 
     * It's needed to cast the argument for isspace to unsigned 
     * char, otherwise it may invoke undefined behavior.
     */
    while (isspace((unsigned char)*str)) {
        str++;
    }

    if (*str == '\0') {
        return str;
    }

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    end[1] = '\0';
    return str;
}

/** 
 * Split str and put each substring into cmd.argv deleting 
 * all whitespaces between each substring. It put the 
 * number of substrings got from str into cmd.argc.
 */
struct cmd parse_command(char *str)
{
    struct cmd cmd;
    char *arg;
    int n = 0;

    n = 0;
    while ((arg = strsep(&str, " "))) {
        if (*arg != '\0') {
            cmd.argv[n++] = trim(arg);
        }
    }
    cmd.argv[n] = NULL;
    cmd.argc = n;
    return cmd;
}

