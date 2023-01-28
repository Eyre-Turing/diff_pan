#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#define DEBUG   1

#define DEBUG_PRINT(fmt, ...)                                       \
do {                                                                \
if (DEBUG) {                                                        \
    fprintf(stderr, "%s:%d:%s: ", __FILE__, __LINE__, __func__);    \
    fprintf(stderr, fmt, ##__VA_ARGS__);                            \
}                                                                   \
} while(0)

char self_path[260];
char url[256];
char token[16];
char pwd[1024];
int request_fd;

int builtin_seturl(const char *url)
{
    DEBUG_PRINT("url: %s\n", url);
    return 0;
}

int builtin_login(const char *user, const char *password)
{
    DEBUG_PRINT("user: %s, password: %s\n", user, password);
    return 0;
}

int builtin_logout()
{
    DEBUG_PRINT("no param\n");
    return 0;
}

int builtin_dir()
{
    DEBUG_PRINT("no param\n");
    return 0;
}

int builtin_ls(const char *path)
{
    DEBUG_PRINT("path: %s\n", path);
    return 0;
}

int builtin_cd(const char *path)
{
    DEBUG_PRINT("path: %s\n", path);
    return 0;
}

int builtin_pwd()
{
    DEBUG_PRINT("no param\n");
    return 0;
}

int builtin_changes(const char *file)
{
    DEBUG_PRINT("file: %s\n", file);
    return 0;
}

int builtin_diff(const char *commit)
{
    DEBUG_PRINT("commit: %s\n", commit);
    return 0;
}

int builtin_cat(const char *commit)
{
    DEBUG_PRINT("commit: %s\n", commit);
    return 0;
}

int builtin_rm(const char *file)
{
    DEBUG_PRINT("file: %s\n", file);
    return 0;
}

/*
 * builtin_mv("xxx", "/abc/def", 0): xxx -> /abc/def （如果/abc目录不存在将自动创建）
 * builtin_mv("xxx", "/abc/def", 1): xxx -> /abc/def/xxx （如果/abc/def目录不存在将自动创建）
 */
int builtin_mv(const char *file, const char *new_file, int mv_to_dir)
{
    DEBUG_PRINT("file: %s, new: %s, mv_to_dir: %d\n", file, new_file, mv_to_dir);
    return 0;
}

/*
 * tag_type表示判断的类型, 0为文件或文件夹、1为文件、2为文件夹
 * print_type表示是否要打印查询结果
 * 
 * 如果tag存在且tag_type正确, 则返回0, 否则返回1
 */
int builtin_access(const char *tag, int tag_type, int print_type)
{
    DEBUG_PRINT("tag: %s, type: %s\n", tag, 
        (tag_type==0?"any":(tag_type==1?"file":"dir"))
        );
    return 0;
}

int exec_seturl(int argc, char *argv[])
{
    if (argc < 2) return 1;
    return builtin_seturl(argv[1]);
}

int exec_login(int argc, char *argv[])
{
    if (argc < 3) return 1;
    return builtin_login(argv[1], argv[2]);
}

int exec_logout(int argc, char *argv[])
{
    return builtin_logout();
}

int exec_dir(int argc, char *argv[])
{
    return builtin_dir();
}

int exec_ls(int argc, char *argv[])
{
    int i;
    if (argc < 2) return builtin_ls(pwd);
    if (argc == 2) return builtin_ls(argv[1]);
    for (i = 1; i < argc; ++i) {
        fprintf(stdout, "in `%s':\n", argv[i]);
        fprintf(stdout, "exit with code: %d\n", builtin_ls(argv[i]));
    }
    return 0;
}

int exec_cd(int argc, char *argv[])
{
    if (argc < 2) return 1;
    return builtin_cd(argv[1]);
}

int exec_pwd(int argc, char *argv[])
{
    return builtin_pwd();
}

int exec_changes(int argc, char *argv[])
{
    int i;
    if (argc < 2) return 1;
    if (argc == 2) return builtin_changes(argv[1]);
    for (i = 1; i < argc; ++i) {
        fprintf(stdout, "file `%s':\n", argv[i]);
        fprintf(stdout, "exit with code: %d\n", builtin_changes(argv[i]));
    }
    return 0;
}

int exec_diff(int argc, char *argv[])
{
    int i;
    if (argc < 2) return 1;
    if (argc == 2) return builtin_diff(argv[1]);
    for (i = 1; i < argc; ++i) {
        fprintf(stdout, "commit `%s':\n", argv[i]);
        fprintf(stdout, "exit with code: %d\n", builtin_diff(argv[i]));
    }
    return 0;
}

int exec_cat(int argc, char *argv[])
{
    int i;
    if (argc < 2) return 1;
    if (argc == 2) return builtin_cat(argv[1]);
    for (i = 1; i < argc; ++i) {
        fprintf(stdout, "commit `%s':\n", argv[i]);
        fprintf(stdout, "exit with code: %d\n", builtin_cat(argv[i]));
    }
    return 0;
}

int exec_rm(int argc, char *argv[])
{
    int i;
    if (argc < 2) return 1;
    if (argc == 2) return builtin_rm(argv[1]);
    for (i = 1; i < argc; ++i) {
        fprintf(stdout, "file `%s':\n", argv[i]);
        fprintf(stdout, "exit with code: %d\n", builtin_rm(argv[i]));
    }
    return 0;
}

int exec_mv(int argc, char *argv[])
{
    int i;
    if (argc < 3) return 1;
    if (argc == 3) return builtin_mv(argv[1], argv[2], builtin_access(argv[2], 2, 0) == 0);
    for (i = 0; i < argc - 1; ++i) {
        builtin_mv(argv[i], argv[argc - 1], 1);
    }
    return 0;
}

int exec_access(int argc, char *argv[])
{
    int i;
    if (argc < 2) return 1;
    for (i = 1; i < argc; ++i) {
        builtin_access(argv[i], 0, 1);
    }
    return 0;
}

int exec_help(int argc, char *argv[])
{
    fprintf(stdout,
"actions:\n"
"  seturl <url>\n"
"  login <user> <password>\n"
"  logout\n"
"  dir\n"
"  ls [path1] [path2] ...\n"
"  cd <path>\n"
"  pwd\n"
"  changes <file1> [file2] [file3] ...\n"
"  diff <commit1> [commit2] [commit3] ...\n"
"  cat <commit1> [commit2] [commit3] ...\n"
"  rm <file1> [file2] [file3] ...\n"
"  mv <file1> [file2] [file3] ... [filen] <new>\n"
"  access <file1> [file2] [file3] ...\n"
    );
    return 0;
}

/*
 * seturl <url>
 * login <user> <password>
 * logout
 * dir
 * ls [path1] [path2] ...
 * cd <path>
 * pwd
 * changes <file1> [file2] [file3] ...
 * diff <commit1> [commit2] [commit3] ...
 * cat <commit1> [commit2] [commit3] ...
 * rm <file1> [file2] [file3] ...
 * mv <file1> [file2] [file3] ... [filen] <new>
 * access <file1> [file2] [file3] ...
 */
int main(int argc, char *argv[])
{
    int i;
    if (argc < 2) {
        exec_help(0, NULL);
        return 1;
    }
    realpath(argv[0], self_path);
    struct {
        const char *action;
        int (*func)(int, char **);
    } actions[] = {
        {"seturl", exec_seturl},
        {"login", exec_login},
        {"logout", exec_logout},
        {"dir", exec_dir},
        {"ls", exec_ls},
        {"cd", exec_cd},
        {"pwd", exec_pwd},
        {"changes", exec_changes},
        {"diff", exec_diff},
        {"cat", exec_cat},
        {"rm", exec_rm},
        {"mv", exec_mv},
        {"access", exec_access},
        {"help", exec_help},
        {"--help", exec_help},
        {"-h", exec_help},
        {"-?", exec_help},
        {NULL, NULL}
    };
    for (i = 0; actions[i].action; ++i) {
        if (strcmp(argv[1], actions[i].action) == 0) {
            return actions[i].func(argc - 1, argv + 1);
        }
    }
    fprintf(stderr, "no such action called `%s'\n", argv[1]);
    return 1;
}
