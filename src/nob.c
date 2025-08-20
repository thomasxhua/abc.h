#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER "src/"

int main(int argc, char** argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd,
        "gcc",
        "-g", "-O0",
        "-std=c99", "-Wall", "-Wextra", "-pedantic", "-static",
        SRC_FOLDER "test_fibonacci.c",
        SRC_FOLDER "test_pie.c",
        SRC_FOLDER "main.c",
        "-o",
        BUILD_FOLDER "main");
    if (!nob_cmd_run_sync(cmd)) return 1;
    return 0;
}

