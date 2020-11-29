/* a.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/termios.h>

int kbhit()
{
    struct timeval timeout;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    return select(1, &readfds, NULL, NULL, &timeout) != 0;
}

int main(int argc, const char* argv[])
{
    struct termios original_tio;
    char c;
    tcgetattr(STDIN_FILENO, &original_tio);
    struct termios new_tio = original_tio;
    new_tio.c_lflag &= ~ICANON & ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    while (1) {
       if (kbhit()) {
          c=getchar();
          putc(c+1,stdout);
          fflush(stdout);  /* necessary to push out character */
       }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio); /* restore cooked mode */
}
