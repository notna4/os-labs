#include <stdio.h>
#include <sys/stat.h>

int main() {
    char* path = "./myfile.txt";
    struct stat buf;

    if(stat(path, &buf)) {
        printf("error\n");
    }
    else {
        printf("inode: %d\n", (int)buf.st_ino);
        printf("size: %d\n", (int)buf.st_size);
    }

    return 0;
}