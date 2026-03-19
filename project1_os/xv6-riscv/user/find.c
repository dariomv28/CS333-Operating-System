#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
 
void find(char* InitDir, char* FileName) {
    int fd_dir; // fd for Directory
    char Buf[512] = { 0, };
    char *p;
    struct stat st;
    struct dirent de;
 
    if ((fd_dir = open(InitDir, 0)) < 0) {
        fprintf(2, "[!] failed to open %s\n", InitDir);
        close(fd_dir);
        exit(1);
    }
 
    if (fstat(fd_dir, &st) < 0) {
        fprintf(2, "[!] fstat() failed\n");
        close(fd_dir);
        exit(1);
    }
 
    if (st.type != T_DIR) {
        fprintf(2, "[!] %s is not a directory\n", InitDir);
        fprintf(2, "Usage: find [target_dir] [file_name]\n");
        close(fd_dir);
        exit(1);
    }
 
    strcpy(Buf, InitDir);
    p = Buf+strlen(Buf);
    *p++ = '/';
 
    while (read(fd_dir, &de, sizeof(de)) == sizeof(de) && de.name[0]) {
        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;
        
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
 
        stat(Buf, &st);
        
        if(!strcmp(de.name, FileName) && st.type == T_FILE){
            printf("%s\n", Buf);
        }
 
        if (st.type == T_DIR) {
            find(Buf, FileName);
        }
    }
 
    close(fd_dir);
}
 
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: find [target_dir] [file_name]\n");
        exit(1);
    }
    
    find(argv[1], argv[2]);
 
    exit(0);
}