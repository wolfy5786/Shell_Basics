#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

void cd(char *path);
void pwd();
void removedir(char *directory);
void help();
void make(char *directory);
void ls(char *directory);
void mv(char *source, char *destination);
void cp(char *source, char *destination);
void rm(char *file);

int main(int argc, char** argv)
{
    char input[1024];
    char cwd[1024];
    while(1){
        pwd();
        fgets(input, 1024, stdin);
        input[strcspn(input, "\n")] = 0;
        char *command = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, " ");
        if(strcmp(command,"cd") == 0){
            cd(arg1);
        } else if(strcmp(command, "pwd") == 0){
            pwd_();
        } else if(strcmp(command, "exit") == 0){
            return EXIT_SUCCESS;
        } else if(strcmp(command, "help") == 0){
            help();
        } else if(strcmp(command, "mkdir") == 0){
            make(arg1);
        } else if(strcmp(command, "rmdir") == 0){
            removedir(arg1);
        }else if(strcmp(command, "ls") == 0){
            ls(getcwd(cwd, sizeof(cwd)));
        }else if(strcmp(command, "cp") == 0){
            cp(arg1,arg2);
        }else if(strcmp(command, "mv") == 0){
            mv(arg1,arg2);
        }else{
            printf("Unknown command: %s\n", command);
        }
        pwd();
    }
    return EXIT_SUCCESS;
}

void help(){
    printf("cd - change directory\n");
    printf("pwd - print working directory\n");
    printf("exit - exit\n");
    printf("help - help\n");
    printf("mkdir <directory> - create a directory\n");
    printf("rmdir <directory> - remove a directory\n");
    printf("ls <directory> - list files in directory\n");
    printf("cp <source> <destination> - copy a file\n");
    printf("mv <source> <destination> - move a file\n");
    printf("rm <file> - remove a file\n");
}


void cd(char *path)
{
    if(path == NULL){
    char c[1024];
        if(getcwd(c, sizeof(c))!= NULL){
            printf("%s\n", c);
        }
        return;
    }
    if(chdir(path) == 0){
        printf("Directory changed to %s\n", path);
    }else{
        perror("Error changing directory");
    }
}
void pwd_()
{
    pwd();
    printf("/n");
}

void pwd()
{
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s> ", cwd);
    }else{
        perror("Error getting current directory");
    }
}

void removedir(char *directory)
{
    if(directory == NULL){
        printf("Usage: rmdir <directory>\n");
        return;
    }

    if(rmdir(directory) == 0){
        printf("Directory removed: %s\n", directory);
    } else{
        perror("Error in removing directory");
    }
}

void make(char *directory)
{
    if(directory == NULL){
        printf("Usage: mkdir <directory>\n");
        return;
    }

    if(mkdir(directory) == 0){
        printf("Directory created: %s\n", directory);
    } else{
        perror("Error in creating directory");
    }
}

void ls(char *directory)
{
    DIR *dir;
    struct dirent *entry;

    dir =opendir(directory);
    if(dir == NULL) {
        perror("Error opening directory");
        return;
    }

    while((entry= readdir(dir))!= NULL){
        if(strcmp(entry->d_name, ".") !=0 && strcmp(entry->d_name, "..")!= 0) {
            printf("%s\t",entry->d_name);
        }
    }
    printf("\n");
    closedir(dir);
}

void rm(char *file)
{
    if(unlink(file) == 0){
        printf("File removed");
    }
    else{
        printf("Error in removing file");
    }
}

void cp(char *source, char *destination)
{
    int src, dest;
    char buffer[1024];
    int content;

    src= open(source, O_RDONLY);
    if(src == -1){
        perror("Error opening source file");
        return;
    }

    dest = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(dest== -1){
        perror("Error opening destination file");
        close(src);
        return;
    }

    while((content = read(src, buffer, sizeof(buffer))) > 0){
        write(dest, buffer,content);
    }

    close(src);
    close(dest);
}

void mv(char *source, char *destination)
{
    cp(source, destination);
    unlink(source);
}
