#include <dirent.h>
#include <stdio.h>


void list(const char* dir_path)
{
    DIR* dirp;
    struct dirent* entry;

    if((dirp= opendir(dir_path))==NULL)
    {
        perror("Directory cannot be opened\n");
    }

    while((entry= readdir(dirp)) != NULL){
        if(entry->d_name[0] != '.'){
            printf("%-15s\t",entry->d_name);
        }
        printf("\n");
        closedir(dirp);
    }

}

int main(int argc, char* argv[])
{
    if(argc == 1)
        list("./");

    else if (argc==2){
        list(argv[1]);
    }
    else{
        for (int i = 0; i < argc; ++i) {
            printf("%s:\n",argv[i]);
            list(argv[i]);
            printf("\n");
        }
    }
}
