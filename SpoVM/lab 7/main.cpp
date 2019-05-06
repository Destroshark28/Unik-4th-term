#include "filesystem/disk.h"
#include "filesystem/fs.h"

#include <sstream>
#include <string>
#include <stdexcept>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define equals(a, b) (strcmp((a), (b)) == 0)

bool commandOpen(Disk &disk, char *arg1);
bool commandCreate(Disk &disk, char *arg1, char *arg2);
void commandDebug(Disk &disk, int args);
void commandFormat(Disk &disk, int args);
void commandMount(Disk &disk, FileSystem &fs, int args);
void commandCopyOut(Disk &disk, FileSystem &fs, int args, char *arg1, char *arg2);
void commandCopyIn(Disk &disk, FileSystem &fs, int args, char *arg1);
void commandCreate(Disk &disk, FileSystem &fs, int args);
void commandRemove(Disk &disk, FileSystem &fs, int args, char *arg1);
void commandStat(Disk &disk, FileSystem &fs, int args, char *arg1);
void commandHelp();

bool copyOut(FileSystem &fs, size_t iNodeNumber, const char *path);
bool copyIn(FileSystem &fs, const char *path, size_t iNodeNumber);

int main(int argc, char *argv[]) {
    char line[BUFSIZ], cmd[BUFSIZ], arg1[BUFSIZ], arg2[BUFSIZ];

    Disk disk;
    FileSystem fs;

    while (true) {
        printf("Usage: \n");
        printf("\topen diskFile\n");
        printf("\tcreate diskFile size\n");
        printf("> ");

        if (fgets(line, BUFSIZ, stdin) == nullptr) {
            break;
        }
        int args = sscanf(line, "%s %s %s", cmd, arg1, arg2);
        if (args == 0) continue;

        if (equals(cmd, "open")) {
            if (!commandOpen(disk, arg1)) {
                printf("Unable to open disk %s: %s\n", argv[1], strerror(errno));
                return EXIT_FAILURE;
            }
            break;
        } else if (equals(cmd, "create")) {
            if (!commandCreate(disk, arg1, arg2)) {
                printf("Unable to create disk %s: %s\n", argv[1], strerror(errno));
                return EXIT_FAILURE;
            }
            break;
        }
    }

    while (true) {
        printf("filesystem> ");

        if (fgets(line, BUFSIZ, stdin) == nullptr) {
            break;
        }
        int args = sscanf(line, "%s %s %s", cmd, arg1, arg2);
        if (args == 0) continue;

        if (equals(cmd, "debug")) {
            commandDebug(disk, args);
        } else if (equals(cmd, "format")) {
            commandFormat(disk, args);
        } else if (equals(cmd, "mount")) {
            commandMount(disk, fs, args);
        } else if (equals(cmd, "copyIn")) {
            commandCopyIn(disk, fs, args, arg1);
        } else if (equals(cmd, "copyOut")) {
            commandCopyOut(disk, fs, args, arg1, arg2);
        } else if (equals(cmd, "create")) {
            commandCreate(disk, fs, args);
        } else if (equals(cmd, "remove")) {
            commandRemove(disk, fs, args, arg1);
        } else if (equals(cmd, "stat")) {
            commandStat(disk, fs, args, arg1);
        } else if (equals(cmd, "help")) {
            commandHelp();
        } else if (equals(cmd, "exit")) {
            break;
        } else {
            printf("Unknown command: %s", line);
            printf("Type 'help' for a list of commands.\n");
        }
    }

    return EXIT_SUCCESS;
}


bool commandOpen(Disk &disk, char *arg1) {
    try {
        disk.open(arg1);
    } catch (std::runtime_error &e) {
        return false;
    }
    return true;
}

bool commandCreate(Disk &disk, char *arg1, char *arg2) {
    try {
        disk.create(arg1, strtol(arg2, nullptr, 0));
    } catch (std::runtime_error &e) {
        return false;
    }
    return true;
}

void commandDebug(Disk &disk, int args) {
    if (args != 1) {
        printf("Usage: debug\n");
        return;
    }

    FileSystem::debug(&disk);
}

void commandFormat(Disk &disk, int args) {
    if (args != 1) {
        printf("Usage: format\n");
        return;
    }

    if (FileSystem::format(&disk)) {
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
        printf("Usage: copyOut iNode file\n");
        return;
    }
    if (!copyOut(fs, strtol(arg1, nullptr, 0), arg2)) {
        printf("copyOut failed!\n");
    }
}

void commandCopyIn(Disk &disk, FileSystem &fs, int args, char *arg1) {
    if (args != 3) {
        printf("Usage: copyIn iNode file\n");
        return;
    }

    if (!copyIn(fs, arg1, strtol(arg1, nullptr, 0))) {
        printf("copyIn failed!\n");
    }
}

void commandCreate(Disk &disk, FileSystem &fs, int args) {
    if (args != 1) {
        printf("Usage: create\n");
        return;
    }

    ssize_t iNodeNumber = fs.create();
    if (iNodeNumber >= 0) {
        printf("created iNode %ld.\n", iNodeNumber);
    } else {
        printf("create failed!\n");
    }
}

void commandRemove(Disk &disk, FileSystem &fs, int args, char *arg1) {
    if (args != 2) {
        printf("Usage: remove iNode\n");
        return;
    }

    ssize_t iNodeNumber = strtol(arg1, nullptr, 0);
    if (fs.remove(iNodeNumber)) {
        printf("removed iNode %ld.\n", iNodeNumber);
    } else {
        printf("remove failed!\n");
    }
}

void commandStat(Disk &disk, FileSystem &fs, int args, char *arg1) {
    if (args != 2) {
        printf("Usage: stat iNode\n");
        return;
    }

    size_t iNodeNumber = strtol(arg1, nullptr, 0);
    ssize_t bytes = fs.stat(iNodeNumber);
    if (bytes >= 0) {
        printf("iNode %ld has size %ld bytes.\n", iNodeNumber, bytes);
    } else {
        printf("stat failed!\n");
    }
}

void commandHelp() {
    printf("Commands are:\n");
    printf("\tformat\n");
    printf("\tmount\n");
    printf("\tdebug\n");
    printf("\tcreate\n");
    printf("\tremove  iNode\n");
    printf("\tstat    iNode\n");
    printf("\tcopyIn  file iNode\n");
    printf("\tcopyOut iNode file\n");
    printf("\thelp\n");
    printf("\texit\n");
}

bool copyOut(FileSystem &fs, size_t iNodeNumber, const char *path) {
    FILE *stream = fopen(path, "w");
    if (stream == nullptr) {
        printf("Unable to open %s: %s\n", path, strerror(errno));
        return false;
    }

    char buffer[4 * BUFSIZ] = {0};
    size_t offset = 0;
    while (true) {
        ssize_t result = fs.read(iNodeNumber, buffer, sizeof(buffer), offset);
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

bool copyIn(FileSystem &fs, const char *path, size_t iNodeNumber) {
    FILE *stream = fopen(path, "r");
    if (stream == nullptr) {
        printf("Unable to open %s: %s\n", path, strerror(errno));
        return false;
    }

    char buffer[4 * BUFSIZ] = {0};
    size_t offset = 0;
    while (true) {
        ssize_t result = fread(buffer, 1, sizeof(buffer), stream);
        if (result <= 0) {
            break;
        }

        ssize_t actual = fs.write(iNodeNumber, buffer, result, offset);
        if (actual < 0) {
            printf("fs.write returned invalid result %ld\n", actual);
            break;
        }
        offset += actual;
        if (actual != result) {
            printf("fs.write only wrote %ld bytes, not %ld bytes\n", actual, result);
            break;
        }
    }

    printf("%lu bytes copied\n", offset);
    fclose(stream);
    return true;
}
