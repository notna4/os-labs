#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int checkFileType(char *path) {
    struct stat buf;

    if(lstat(path, &buf) == 0) {
        if(S_ISLNK(buf.st_mode) != 0) {
            return 0;
        }
    }
    if(stat(path, &buf) == 0) {
        if(S_ISREG(buf.st_mode) != 0) {
            return 1;
        }
        else if(S_ISDIR(buf.st_mode) != 0) {
            return 2;
        }
    }

    return -1;
}

int verifyInput(char *params, int type) {
    //printf("p: %s\n", params);
    int not = 0;

    //printf("size: %d\n", strlen(params));

    if(type == 0) {
        for(int i = 1; i < strlen(params)-1; i++) {
            //printf("- %c\n", params[i]);
            if(params[i] == 'n' || params[i] == 'd' || params[i] == 'h' || params[i] == 'm' || params[i] == 'a' || params[i] == 'l') {
                //
            }
            else {
                not = 1;
            }
        }

        if(not) 
            return 0;
        else
            return 1;
    }
    else {
        for(int i = 1; i < strlen(params)-1; i++) {
            //printf("- %c\n", params[i]);
            if(params[i] == 'n' || params[i] == 'l' || params[i] == 'd' || params[i] == 't' || params[i] == 'a') {
                //
            }
            else {
                not = 1;
            }
        }

        if(not) 
            return 0;
        else
            return 1;
    }
}

void handleSym(char *path) {
    char params[10];

    int deleted = 0;

    struct stat buf;
    
    printf("---- MENU ----\n");
    printf("* n: name of symbolic link\n");
    printf("* l: delete symbolic link\n");
    printf("* d: size of symbolic link\n");
    printf("* t: size of target\n");
    printf("* a: access rights\n");
    printf("\n---- EXAMPLE INPUT ----\n");
    printf("-nl\n");
    printf("Used for displaying name of file and deleting the symbolic link.\n");

    printf("\n\nPlease enter your options\n\n");
    printf("STANDARD INPUT: ");
    fgets(params, 10, stdin);

    if(verifyInput(params, 1)) {
        for(int i = 1; i < strlen(params)-1; i++) {
            if (params[i] == 'n') {
                printf("Name: %s\n", path);
                printf("\n");
            }
            if (params[i] == 'd') {
                if(lstat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Size of symbolic link: %d bytes\n", (int)buf.st_size);
                    printf("\n");
                }
            }
            if (params[i] == 'd') {
                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Size of target file: %d bytes\n", (int)buf.st_size);
                    printf("\n");
                }
            }
            if (params[i] == 'l') {
                if(lstat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    if(unlink(path) == 0) {
                        printf("Symlink deleted successfully.\n");
                    }
                    else {
                        printf("Error deleting the symlink.\n");
                    }
                    printf("\n");
                }
            }
            if (params[i] == 'm') {
                if(lstat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Time of last modification: %ld\n", buf.st_mtime);
                    printf("\n");
                }
            }
            if (params[i] == 'a') {
                if(lstat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Access rights: \n");

                    printf("User:\n");
                    buf.st_mode & S_IRUSR ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWUSR ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXUSR ? printf("   Execute - yes\n") : printf("   Execute - no\n");
                        
                    printf("Group:\n");
                    buf.st_mode & S_IRGRP ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWGRP ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXGRP ? printf("   Execute - yes\n") : printf("   Execute - no\n");

                    printf("Other:\n");
                    buf.st_mode & S_IROTH ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWOTH ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXOTH ? printf("   Execute - yes\n") : printf("   Execute - no\n");

                    printf("\n");
                }
            }

            if (params[i] == 'l') {
                char link[15];

                printf("Enter name of symlink: ");
                fgets(link, 15, stdin);

                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    if(symlink(path, link) == 0) {
                        printf("Success!");
                    }
                    else {
                        printf("It didn't work.");
                    }
                    printf("\n");
                }
            }
        }
    }
    else {
        printf("\n\nERROR. Input not right.\n");
    }

    printf("\n\n-----------------------------------------------\n\n");
}

void handleRegular(char *path) {
    char params[10];

    struct stat buf;
    
    printf("---- MENU ----\n");
    printf("* n: name of file\n");
    printf("* d: size of file\n");
    printf("* h: count of hard links\n");
    printf("* m: last time of modifications\n");
    printf("* a: access rights\n");
    printf("* l: create symbolic link *waits for input*\n");
    printf("\n---- EXAMPLE INPUT ----\n");
    printf("-nh\n");
    printf("Used for displaying name of file and the count of hard links\n");

    printf("\n\nPlease enter your options\n\n");
    printf("STANDARD INPUT: ");
    fgets(params, 10, stdin);

    if(verifyInput(params, 0)) {
        for(int i = 1; i < strlen(params)-1; i++) {
            if (params[i] == 'n') {
                printf("Name: %s\n", path);
                printf("\n");
            }
            if (params[i] == 'd') {
                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Size: %d bytes\n", (int)buf.st_size);
                    printf("\n");
                }
            }
            if (params[i] == 'h') {
                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Hard links count: %d\n", (int)buf.st_nlink);
                    printf("\n");
                }
            }
            if (params[i] == 'm') {
                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Time of last modification: %ld\n", buf.st_mtime);
                    printf("\n");
                }
            }
            if (params[i] == 'a') {
                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Access rights: \n");

                    printf("User:\n");
                    buf.st_mode & S_IRUSR ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWUSR ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXUSR ? printf("   Execute - yes\n") : printf("   Execute - no\n");
                        
                    printf("Group:\n");
                    buf.st_mode & S_IRGRP ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWGRP ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXGRP ? printf("   Execute - yes\n") : printf("   Execute - no\n");

                    printf("Other:\n");
                    buf.st_mode & S_IROTH ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWOTH ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXOTH ? printf("   Execute - yes\n") : printf("   Execute - no\n");

                    printf("\n");
                }
            }

            if (params[i] == 'l') {
                char link[15];

                printf("Enter name of symlink: ");
                fgets(link, 15, stdin);

                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    if(symlink(path, link) == 0) {
                        printf("Success!");
                    }
                    else {
                        printf("It didn't work.");
                    }
                    printf("\n");
                }
            }
        }
    }
    else {
        printf("\n\nERROR. Input not right.\n");
    }

    printf("\n\n-----------------------------------------------\n\n");
}

int main(int argc, char **argv) {

    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            int type = checkFileType(argv[i]);

            if(type == 0) {
                printf("%s : SYMBOLIC LINK\n", argv[i]);
                printf("\n\n==========================\n\n");
                handleSym(argv[i]);
            }
            else if(type == 1) {
                printf("%s : REGULAR FILE\n", argv[i]);
                printf("\n\n==========================\n\n");
                handleRegular(argv[i]);
            }
            else if(type == 2) {
                printf("%s : DIRECTORY\n", argv[i]);
                printf("\n\n==========================\n\n");
            }
            else {
                printf("You didn't enter a regular file/directory/symbolic link");
            }
        }
    }
    return 0;
}