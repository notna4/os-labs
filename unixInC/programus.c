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

    pid_t pid, w;
    pid = fork();
    int wstatus;

    if(pid < 0) {
        printf("error forking for dir");
        exit(1);
    }

    if(pid == 0) {
        printf("\nBecause this is a directory,\na .txt file will be created with the following name: %s_file.txt\n", path);
        
        char *file_name = strcpy(file_name, path);
        strcat(file_name, "_file.txt");
        FILE *file_ptr = fopen(file_name, "w");
        if (file_ptr == NULL) {
            printf("Error: Unable to create file.\n");
            exit(1);
        }
        printf("%s_file.txt was created successfully.\n", path);
        fclose(file_ptr);

        exit(10);
    }
    else {
        if(pid > 0) {
            // printf("parent of .c part\n");
            w = wait(&wstatus);
            if(WIFEXITED(wstatus)) {
                printf("process with pid %d, exited, status = %d\n\n", w, WEXITSTATUS(wstatus));
            }
        }
    }

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
                DIR *dir;
                struct dirent *ent;
                int count = 0;
                char *extension = ".c";
                char *dir_name;
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
        printf("%s : SYMBOLIC LINK\n", argv);
        printf("\n\n==========================\n\n");
        handleSym(argv);
    }
    else if(type == 1) {
        printf("%s : REGULAR FILE\n", argv);
        printf("\n\n==========================\n\n");
        handleRegular(argv);
    }
    else if(type == 2) {
        printf("%s : DIRECTORY\n", argv);
        printf("\n\n==========================\n\n");
        handleDirectory(argv);
    }
    else {
        printf("You didn't enter a regular file/directory/symbolic link");
    }
}

int handleRegularAutomatic(char *argv) {

    //check if file has .c extension
    //if it has the second child will execute a script
    //and the output data will be sent to the parent
    //which will compute a score based on the no. of errors and warnings

    char *extension = ".c";
    char *file_ext = strrchr(argv, '.');
    if (file_ext != NULL && strcmp(file_ext, extension) == 0) {
        //file has .c extension


        


        // printf("\nBecause this is a regular file with .c extension:\n");
        // printf("Errors \t Warnings\n");
        // execl("/bin/bash", "bash", "bash_proc.bash", path, NULL);
    }
    else if (file_ext != NULL && strcmp(file_ext, extension) != 0) {
        // if the regular file extension is not .c
        // i should print the number of lines
        

        printf("\nBecause this is a regular file that does not have a .c extension,\nwe print the number of lines\n");
        execl("/usr/bin/wc", "wc", "-l", argv, NULL);

    }

    return -1;

}

int handleAutomaticMode(char *argv) {
    int type = checkFileType(argv);

     if(type == 0) {
        printf("%s : SYMBOLIC LINK\n", argv);
        printf("\n\n==========================\n\n");
        handleSym(argv);
    }
    else if(type == 1) {
        printf("%s : REGULAR FILE AUTOMATIC\n", argv);
        printf("\n\n==========================\n\n");
        return handleRegularAutomatic(argv);
    }
    else if(type == 2) {
        printf("%s : DIRECTORY\n", argv);
        printf("\n\n==========================\n\n");
        handleDirectory(argv);
    }
    else {
        printf("You didn't enter a regular file/directory/symbolic link");
    }
}

int main(int argc, char **argv) {

    pid_t pid1, pid2, w;
    int wstatus1, wstatus2;
    int fd[2];
    char buff[1024];
    

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

                int type = checkFileType(argv[i]);

                if(type == 0) {
                    // sym
                }
                else if(type == 1) {
                    // regular file

                    char *extension = ".c";
                    char *file_ext = strrchr(argv[i], '.');
                    if (file_ext != NULL && strcmp(file_ext, extension) == 0) {
                        //file has .c extension

                        close(fd[0]);
                        dup2(fd[1], 1);
                    
                    //   close(fd[1]);
                        execlp("bash", "bash", "bash_proc.bash", argv[i], NULL);
                        // error handling here TODO
                    }
                    else if (file_ext != NULL && strcmp(file_ext, extension) != 0) {    
                        // if the regular file extension is not .c
                        // i should print the number of lines
                        

                        printf("\nBecause this is a regular file that does not have a .c extension,\nwe print the number of lines\n");
                        execl("/usr/bin/wc", "wc", "-l", argv[i], NULL);
                        // error handling here TODO

                    }


                }
                else {
                    // directory
                }

                

                exit(0);
            }

            waitpid(pid1, &wstatus1, 0);
            if(WIFEXITED(wstatus1)) {
                printf("Child process exited with status %d\n", WEXITSTATUS(wstatus1));
            }


            waitpid(pid2, &wstatus2, 0);
            if (WIFEXITED(wstatus2)) {

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

                // while(read(fd[0], buffer, sizeof(buffer)) > 0) {
                //     strcpy(output, buffer);
                // }

                printf("da: %d\n", atoi(&output[0]));

                // for(int i = 0; i < strlen(output); i++) {
                //     printf("%s\n", output[i]);
                // }

                printf("Child process exited with status %d\n", WEXITSTATUS(wstatus2));

            }
        
        }
    }


    return 0;
}










// if(pid < 0) {
//     printf("\nerror at forking\n");
//     exit(1);
// }
// if(pid == 0) {
//     printf("\nwe are in a child process\n");

    

//     exit(5);
// }
// else {
//     if(pid > 0) {
//         w = wait(&wstatus);
//         if(WIFEXITED(wstatus)) {
//             printf("process with pid %d, exited, status = %d\n", w, WEXITSTATUS(wstatus));
//         }
//     }
// }