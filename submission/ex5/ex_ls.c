#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ftw.h>
#include <sys/sysmacros.h>

int flag_a, flag_l, flag_r = 0;

unsigned int major(dev_t dev);
unsigned int minor(dev_t dev);

int print_mode(mode_t st_mode){
    short perms_number[9] = {0400,0200,0100,0040,0020,0010,0004,0002,0001};
    mode_t types_number[6] = {0120000,0060000,0010000,0020000,0040000,0100000};
    char types[6] = {'l', 'b','p','c','d','-'};
    char perms[10] = "rwxrwxrwx";
    char mode[11];
    for(int i=0;i<6;i++){
        if((st_mode & 0170000) == types_number[i]){
            mode[0] = types[i];
            break ;
        }
    }
        for(int i=0;i<9;i++){	
		if(st_mode & perms_number[i]) mode[i+1] = perms[i];
            else 
		mode[i+1] = '-';
	    
        }

    
    
    printf("%10s\t",mode);
    if (mode[0]=='c' || mode[0]=='b')
	    return 0; // dev
    else
	    return 1; // not dev
}

void list(const char* dir_path)
{
    DIR* dirp;
    struct dirent* entry;

    if((dirp=opendir(dir_path))==NULL){
        perror("Directory cannot be opened\n");
    }

    while((entry=readdir(dirp)) != NULL){
        if(flag_a | entry->d_name[0] !='.')
            printf("%-15s\t",entry->d_name);
    }
    printf("\n");
    closedir(dirp);
}


void list_long(const char* dir_path)
{
    DIR* dirp;
    struct dirent* entry;
    struct stat buf;
    char prev_dir[512];
    if(getcwd(prev_dir,512)==NULL) perror("getcwd error");
    if((dirp=opendir(dir_path))==NULL) perror("Directory open error");
    if(chdir(dir_path) != 0) perror("chdir error");

    while((entry=readdir(dirp)) != NULL){
        if(flag_a | entry->d_name[0] != '.'){
            if(lstat(entry->d_name, &buf)== -1) continue;
            int tf = print_mode(buf.st_mode);
            printf("%3ld\t", buf.st_nlink);
            printf("%6d\t", buf.st_uid);
            printf("%6d\t", buf.st_gid);
	    if (tf == 0){
		dev_t d = buf.st_rdev;
		printf("%5d, %5d\t",major(d),minor(d));
	    }
	    else
            	printf("%12ld\t", buf.st_size);
            printf("%-15s\t\n", entry->d_name);
        }
    }
    closedir(dirp);
    chdir(prev_dir);
}

int list_recursive(const char *path, const struct stat *buf, int type)
{
    if(type == FTW_D){
        printf("%s:\n",path);
        if(flag_l) list_long(path);
        else list(path);
        printf("\n");
    }

    return 0;
}


int main(int argc, char* argv[])
{
    int opt;
    while((opt = getopt(argc,argv,"alR")) != -1){
        switch(opt){
            case 'a':
                flag_a =1;
                break;
            case 'l':
                flag_l =1;
                break;
            case 'R':
                flag_r =1;
                break;
            default:
                printf("Wrong usage\n");
                return -1;
        }
    }

    if (argc - optind == 0) {
        if(flag_r) ftw("./", list_recursive,10);
        else if(flag_l) list_long("./");
        else list("./");
    }
    else if (argc - optind == 1) {
        if(flag_r) ftw(argv[optind],list_recursive,10);
        else if(flag_l) list_long(argv[optind]);
        else list(argv[optind]);
    }
    else {
        for (int i = optind; i < argc; i++) {
            printf("%s:\n", argv[i]);
            if(flag_r) ftw(argv[i],list_recursive,10);
            else if(flag_l) list_long(argv[i]);
            else list(argv[i]);
            printf("\n");
        }
    }
    return 0;

}
   
