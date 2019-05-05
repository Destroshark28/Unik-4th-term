#include "filesystem/disk.h"
#include "filesystem/fs.h"

#include <sstream>
#include <string>
#include <stdexcept>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define equals(a, b) (strcmp((a), (b)) == 0)

void commandDebug(Disk &disk, FileSystem &fs, int args);
void commandFormat(Disk &disk, FileSystem &fs, int args);
void commandMount(Disk &disk, FileSystem &fs, int args);
void commandCopyOut(Disk &disk, FileSystem &fs, int args, char *arg1, char *arg2);
void commandCopyIn(Disk &disk, FileSystem &fs, int args, char *arg1, char *arg2);
void commandCreate(Disk &disk, FileSystem &fs, int args);
void commandRemove(Disk &disk, FileSystem &fs, int args, char *arg1);
void commandStat(Disk &disk, FileSystem &fs, int args, char *arg1);
void commandHelp(Disk &disk, FileSystem &fs);

bool copyOut(FileSystem &fs, size_t inumber, const char *path);
bool copyIn(FileSystem &fs, const char *path, size_t inumber);

int main(int argc, char *argv[]) {
    Disk disk;
    FileSystem fs;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s diskFile nBlocks\n", argv[0]);
        return EXIT_FAILURE;
    }

    try {
        disk.open(argv[1], atoi(argv[2]));
    } catch (std::runtime_error &e) {
        fprintf(stderr, "Unable to open disk %s: %s\n", argv[1], e.what());
        return EXIT_FAILURE;
    }

    while (true) {
        char line[BUFSIZ], cmd[BUFSIZ], arg1[BUFSIZ], arg2[BUFSIZ];

        fprintf(stderr, "filesystem> ");
        fflush(stderr);

        if (fgets(line, BUFSIZ, stdin) == nullptr) {
            break;
        }

        int args = sscanf(line, "%s %s %s", cmd, arg1, arg2);
        if (args == 0) continue;


        if (equals(cmd, "debug")) {
            commandDebug(disk, fs, args);
        } else if (equals(cmd, "format")) {
            commandFormat(disk, fs, args);
        } else if (equals(cmd, "mount")) {
            commandMount(disk, fs, args);
        } else if (equals(cmd, "copyIn")) {
            commandCopyIn(disk, fs, args, arg1, arg2);
        } else if (equals(cmd, "copyOut")) {
            commandCopyOut(disk, fs, args, arg1, arg2);
        } else if (equals(cmd, "create")) {
            commandCreate(disk, fs, args);
        } else if (equals(cmd, "remove")) {
            commandRemove(disk, fs, args, arg1);
        } else if (equals(cmd, "stat")) {
            commandStat(disk, fs, args, arg1);
        } else if (equals(cmd, "help")) {
            commandHelp(disk, fs);
        } else if (equals(cmd, "exit")) {
            break;
        } else {
            printf("Unknown command: %s", line);
            printf("Type 'help' for a list of commands.\n");
        }
    }

    return EXIT_SUCCESS;
}

void commandDebug(Disk &disk, FileSystem &fs, int args) {
    if (args != 1) {
        printf("Usage: debug\n");
        return;
    }

    fs.debug(&disk);
}

void commandFormat(Disk &disk, FileSystem &fs, int args) {
    if (args != 1) {
        printf("Usage: format\n");
        return;
    }

    if (fs.format(&disk)) {
        printf("disk formatted.\n");
    } else {
        printf("format failed!\n");
    }
}

void commandMount(Disk &disk, FileSystem &fs, int args) {
    if (args != 1) {
        printf("Usage: mount\n");
        return;
    }

    if (fs.mount(&disk)) {
        printf("disk mounted.\n");
    } else {
        printf("mount failed!\n");
    }
}

void commandCopyOut(Disk &disk, FileSystem &fs, int args, char *arg1, char *arg2) {
    if (args != 3) {
        printf("Usage: copyOut inode file\n");
        return;
    }

    if (!copyOut(fs, atoi(arg1), arg2)) {
        printf("copyOut failed!\n");
    }
}

void commandCopyIn(Disk &disk, FileSystem &fs, int args, char *arg1, char *arg2) {
    if (args != 3) {
        printf("Usage: copyIn inode file\n");
        return;
    }

    if (!copyIn(fs, arg1, atoi(arg2))) {
        printf("copyIn failed!\n");
    }
}

void commandCreate(Disk &disk, FileSystem &fs, int args) {
    if (args != 1) {
        printf("Usage: create\n");
        return;
    }

    ssize_t inumber = fs.create();
    if (inumber >= 0) {
        printf("created inode %ld.\n", inumber);
    } else {
        printf("create failed!\n");
    }
}

void commandRemove(Disk &disk, FileSystem &fs, int args, char *arg1) {
    if (args != 2) {
        printf("Usage: remove inode\n");
        return;
    }

    ssize_t inumber = atoi(arg1);
    if (fs.remove(inumber)) {
        printf("removed inode %ld.\n", inumber);
    } else {
        printf("remove failed!\n");
    }
}

void commandStat(Disk &disk, FileSystem &fs, int args, char *arg1) {
    if (args != 2) {
        printf("Usage: stat inode\n");
        return;
    }

    ssize_t inumber = atoi(arg1);
    ssize_t bytes = fs.stat(inumber);
    if (bytes >= 0) {
        printf("inode %ld has size %ld bytes.\n", inumber, bytes);
    } else {
        printf("stat failed!\n");
    }
}

void commandHelp(Disk &disk, FileSystem &fs) {
    printf("Commands are:\n");
    printf("    format\n");
    printf("    mount\n");
    printf("    debug\n");
    printf("    create\n");
    printf("    remove  inode\n");
    printf("    stat    inode\n");
    printf("    copyIn  file inode\n");
    printf("    copyOut inode file\n");
    printf("    help\n");
    printf("    exit\n");
}

bool copyOut(FileSystem &fs, size_t inumber, const char *path) {
    FILE *stream = fopen(path, "w");
    if (stream == nullptr) {
        fprintf(stderr, "Unable to open %s: %s\n", path, strerror(errno));
        return false;
    }

    char buffer[4 * BUFSIZ] = {0};
    size_t offset = 0;
    while (true) {
        ssize_t result = fs.read(inumber, buffer, sizeof(buffer), offset);
        if (result <= 0) {
            break;
        }
        fwrite(buffer, 1, result, stream);
        offset += result;
    }

    printf("%lu bytes copied\n", offset);
    fclose(stream);
    return true;
}

bool copyIn(FileSystem &fs, const char *path, size_t inumber) {
    FILE *stream = fopen(path, "r");
    if (stream == nullptr) {
        fprintf(stderr, "Unable to open %s: %s\n", path, strerror(errno));
        return false;
    }

    char buffer[4 * BUFSIZ] = {0};
    size_t offset = 0;
    while (true) {
        ssize_t result = fread(buffer, 1, sizeof(buffer), stream);
        if (result <= 0) {
            break;
        }

        ssize_t actual = fs.write(inumber, buffer, result, offset);
        if (actual < 0) {
            fprintf(stderr, "fs.write returned invalid result %ld\n", actual);
            break;
        }
        offset += actual;
        if (actual != result) {
            fprintf(stderr, "fs.write only wrote %ld bytes, not %ld bytes\n", actual, result);
            break;
        }
    }

    printf("%lu bytes copied\n", offset);
    fclose(stream);
    return true;
}
