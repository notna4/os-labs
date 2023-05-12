#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

#include<sys/stat.h>
#include<sys/wait.h>
#include<unistd.h>

pid_t pid1, pid2, w;
int wstatus1, wstatus2;
int fd[2];
char buff[1024];

int cond = 0;

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
    if(params[0] != '-') {
        printf("\nThe command must start with an '-'\n");
        return -1;
    }

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
    else if(type == 2) {
        for(int i = 1; i < strlen(params)-1; i++) {
            //printf("- %c\n", params[i]);
            if(params[i] == 'n' || params[i] == 'd' || params[i] == 'a' || params[i] == 'c') {
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

    if(verifyInput(params, 1) != -1) {
        for(int i = 1; i < strlen(params)-1 && deleted == 0; i++) {
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
            if (params[i] == 't') {
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
                        deleted = 1;
                    }
                    else {
                        printf("Error deleting the symlink.\n");
                    }
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

    if(verifyInput(params, 0) != -1) {
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

                //remove the null character at the end if present
                char *newline = strchr(link, '\n');
                if (newline != NULL) {
                    *newline = '\0';
                }

                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    if(symlink(path, link) == 0) {
                        printf("Success! You created a symbolic link: %s\n", link);
                        printf("Here's how it looks:\n");
                        char command[256];
                        sprintf(command, "ls -l %s", link);
                        system(command);
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

void handleDirectory(char *path) {
    char params[10];

    struct stat buf;
    
    printf("---- MENU ----\n");
    printf("* n: name of directory\n");
    printf("* d: size of directory\n");
    printf("* a: access rights\n");
    printf("* c: number of files with the .c extension\n");
    printf("\n---- EXAMPLE INPUT ----\n");
    printf("-nd\n");
    printf("Used for displaying the name of directory and the size of the directory\n");

    printf("\n\nPlease enter your options\n\n");
    printf("STANDARD INPUT: ");
    fgets(params, 10, stdin);

    if(verifyInput(params, 2) != -1) {
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

            if (params[i] == 'c') {
                printf("fisiere c in dir\n");
                DIR *dir;
                struct dirent *ent;
                int count = 0;
                char *extension = ".c";
                char dir_name[1024];
                strcpy(dir_name, path);
                struct stat buf;
                char absolute_path[1024];

                //get absolute path
                if (stat(path, &buf) == 0) {
                    if (S_ISDIR(buf.st_mode)) {
                        printf("%s is a directory\n", path);
                    } else {
                        printf("%s is a file\n", path);
                    }
                    if (realpath(path, absolute_path) == NULL) {
                        perror("realpath");
                    }
                    printf("Absolute path: %s\n", absolute_path);
                } else {
                    perror("stat");
                }

                if ((dir = opendir(dir_name)) != NULL) {
                    while ((ent = readdir(dir)) != NULL) {
                        if (ent->d_type == DT_REG) {
                            char *file_ext = strrchr(ent->d_name, '.');
                            if (file_ext != NULL && strcmp(file_ext, extension) == 0) {
                                //file has .c extension
                                count++;
                            }
                        }
                    }
                    closedir(dir);
                    printf("Number of .c files in %s: %d\n", dir_name, count);
                } else {
                    perror("Could not open directory");
                }

            }
        }
    }
    else {
        printf("\n\nERROR. Input not right.\n");
    }

    printf("\n\n-----------------------------------------------\n\n");
}

void handleInputMode(char *argv) {
    int type = checkFileType(argv);

    if(type == 0) {
        printf("\n\n==========================\n\n");
        printf("%s : SYMBOLIC LINK\n", argv);
        printf("\n\n==========================\n\n");
        handleSym(argv);
    }
    else if(type == 1) {
        printf("\n\n==========================\n\n");
        printf("%s : REGULAR FILE\n", argv);
        printf("\n\n==========================\n\n");
        handleRegular(argv);
    }
    else if(type == 2) {
        printf("\n\n==========================\n\n");
        printf("%s : DIRECTORY\n", argv);
        printf("\n\n==========================\n\n");
        handleDirectory(argv);
    }
    else {
        printf("\n\n?????????????????????\n\n");
        printf("\nYou didn't enter a regular file/directory/symbolic link.\n");
        printf("\n\n?????????????????????\n\n");
    }
}

void handleAutoSym(char *argv) {
    printf("Because this is a symlink, the target file permission will be changed.\n");
    execl("/bin/chmod", "chmod", "u=rwx,g=rw,o=", argv, NULL);
    perror("Failed to execute command");
    exit(EXIT_FAILURE);
}

void handleAutoRegularFile(char *argv) {
    char *extension = ".c";
    char *file_ext = strrchr(argv, '.');
    if (file_ext != NULL && strcmp(file_ext, extension) == 0) {
        //file has .c extension
        cond = 1;
        // printf("c: %d\n", *cond);
        printf("\n**************************\n");
        printf("\n~~ file has .c extension ~~\n");
        printf("\n**************************\n");
        

        close(fd[0]);
        dup2(fd[1], 1);
    
    //   close(fd[1]);
        execlp("bash", "bash", "bash_proc.bash", argv, NULL);
        // error handling here TODO
        perror("Failed to execute command");
        exit(EXIT_FAILURE);
        
    }
    else if (file_ext != NULL && strcmp(file_ext, extension) != 0) {    
        // if the regular file extension is not .c
        // i should print the number of lines
        printf("\n**************************\n");
        printf("\n~~ file does not have .c extension ~~\n");
        printf("\n**************************\n");
        

        printf("\n~~ Because this is a regular file that does not have a .c extension,\n~~ we print the number of lines\n");
        execl("/usr/bin/wc", "wc", "-l", argv, NULL);
        // error handling here TODO
        perror("Failed to execute command");
        exit(EXIT_FAILURE);
    }
}

void handleAutoDirectory(char *argv) {
    char path[256];
    strcpy(path, argv);

    printf("%s\n", path);
    
    printf("\n~~ Because this is a directory,\n~~ a .txt file will be created with the following name: %s_file.txt\n", path);
        
    strcat(path, "_file.txt");
    FILE *file_ptr = fopen(path, "w");
    if (file_ptr == NULL) {
        printf("Error: Unable to create file.\n");
        exit(1);
    }
    printf("\n~~ The %s was created successfully.\n", path);
    fclose(file_ptr);
}

void handleAutomaticMode(char *argv) {
    int type = checkFileType(argv);

    if(type == 0) {
        printf("\n\n**************************\n\n");
        printf("%s : AUTO SYMBOLIC LINK\n", argv);
        printf("\n\n**************************\n\n");

        handleAutoSym(argv);

    }
    else if(type == 1) {
        printf("\n\n**************************\n\n");
        printf("%s : AUTO REGULAR FILE\n", argv);
        printf("\n\n**************************\n\n");

        handleAutoRegularFile(argv);

    }
    else if(type == 2){
        printf("\n\n**************************\n\n");
        printf("%s : AUTO DIRECTORY\n", argv);
        printf("\n\n**************************\n\n");

        handleAutoDirectory(argv);
        
    }
    else {
        printf("\n\n?????????????????????\n\n");
        printf("\nYou didn't enter a regular file/directory/symbolic link.\n");
        printf("\n\n?????????????????????\n\n");
    }
}

int main(int argc, char **argv) {

    if(pipe(fd) == -1) {
        perror("Pipe creation failed.\n");
        exit(1);
    }

    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            // for each argument we create 2 processes

            pid1 = fork();
            if(pid1 == 0) {

                handleInputMode(argv[i]);

                exit(0);
            }

            pid2 = fork();
            if(pid2 == 0) {
                
                handleAutomaticMode(argv[i]);
                
                exit(0);
            }

            waitpid(pid1, &wstatus1, 0);
            if(WIFEXITED(wstatus1)) {
                printf("The process with PID %d has ended with the exit code %d.\n", pid1, WEXITSTATUS(wstatus1));
            }


            waitpid(pid2, &wstatus2, 0);
            if (WIFEXITED(wstatus2)) {
                // printf("c: %d\n", cond);
                if(cond) {
                    int bytes_read;

                    // Read from the pipe
                    bytes_read = read(fd[0], buff, sizeof(buff));

                    // Check for errors
                    if (bytes_read < 0) {
                        perror("Error reading from pipe");
                        exit(EXIT_FAILURE);
                    }

                    char output[1024];
                    // Print the output
                    printf("Output from child process:\n");
                    // printf("%.*s", bytes_read, buff);
                    strcpy(output, buff);

                    char *token;
                    // Extract the first number before the comma for warnings
                    token = strtok(output, ",");
                    int n_warnings = atoi(token);
                    // Extract the second number after the comma for errors
                    token = strtok(NULL, ",");
                    int n_errors = atoi(token);

                    //get the file name
                    token = strtok(NULL, ",");
                    char n_file[256];
                    strcpy(n_file, token);
                    strcat(n_file, "\0");

                    printf("W: %d\t E: %d\t F: %s\n", n_warnings, n_errors, n_file);

                    int score = 0;

                    if(n_errors == 0 && n_warnings == 0) {
                        score = 10;
                    }
                    else if(n_errors > 0) {
                        score = 1;
                    }
                    else if(n_errors == 0 && n_warnings > 10) {
                        score = 2;
                    }
                    else if(n_errors == 0 && n_warnings <= 10) {
                        score = 2 + 8 * (10 - n_warnings)/10;
                    }


                    int file_descriptor = open("grades.txt", O_WRONLY | O_CREAT, 0644);
                    if (file_descriptor == -1) {
                        printf("Failed to open grades.txt for writing.\n");
                        exit(1);
                    }
                    dprintf(file_descriptor, "%s: %d", n_file, score);
                    printf("\n~~ Score written inside grades.txt\n");

                    close(file_descriptor);

                }

                printf("The process with PID %d has ended with the exit code %d.\n", pid2, WEXITSTATUS(wstatus2));

            }
        
        }
    }


    return 0;
}