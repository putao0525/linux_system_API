#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
    struct stat buf;
    if (stat("cmake-build-debug", &buf) == 0) {
        printf("%d %d %ld \n", buf.st_uid, buf.st_gid, buf.st_size);
    }
    return 0;
}
