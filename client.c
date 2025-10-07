//Implement a mini key-value store server using Unix domain sockets and a client that can set/get values.
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/keyvaluestore.sock"
#define BUF_SIZE 1024

int main(void) 
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) 
    { 
        perror("socket"); exit(1); 
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path)-1);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) 
    {
        perror("connect"); exit(1);
    }

    char line[BUF_SIZE];
    char buf[BUF_SIZE];

    while (1) 
    {
        printf("kv> ");
        if (!fgets(line, sizeof(line), stdin)) break;

        write(fd, line, strlen(line));

        ssize_t n = read(fd, buf, sizeof(buf)-1);
        if (n <= 0) break;
        buf[n] = '\0';
        printf("%s", buf);
    }

    close(fd);
    return 0;
}
