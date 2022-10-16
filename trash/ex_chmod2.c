#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
void print_usage(char* prog_name) {
    printf("Default usage: %s -f <file name> -m <mode>\n",prog_name);
    printf("-- Change the permission of the <file name> to <mode>\n");
    printf("-- additional options:\n");
    printf(" -v: add <mode> to original permission\n");
    printf(" -h: remove <mode> from original permission\n");
}
int main(int argc, char* argv[]) {

    int opt;
    int flag_v = 0, flag_h = 0;
    while ((opt = getopt(argc, argv, "vh")) != -1) {
        switch(opt) {
            case 'v':
                flag_v = 1; break;
            case 'h':
                flag_h = 1; break;
            default:
                print_usage(argv[0]);
                return -1;
        }
    }


    printf("%d\n",optind);

    if(flag_h == 1){
        print_usage(argv[0]);
        return -1;
    }

    if(argc - optind != 2)
    {
        printf("Missing operand\n");
        printf("Try ‘ex_chmod -h’ for more information\n");
        return -1;
    }

    mode_t newmode;

    if (access(argv[optind + 1],F_OK) != 0) {
        perror("file is not existed\n");
        return -1;
    }
    sscanf(argv[optind], "%o", &newmode);
    if (chmod(argv[optind + 1], newmode) == -1) {
        perror("mode change failed\n");
        return -3;
    }
    else{
        if(flag_v == 1){
            printf("mode of %s is changed to %s\n" ,argv[optind + 1],argv[optind]);
        }
        else{
            printf("mode changed\n");
        }
    }

}
