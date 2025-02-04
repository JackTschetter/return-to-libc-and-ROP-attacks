#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

char line_buf[1024];

char *read_line_unbuffered(int fd) {
    char *p = line_buf;
    char c;
    do {
	int res = read(fd, &c, 1);
	if (res == 0) {
	    printf("Attacker exiting on EOF.\n");
	    exit(0);
	} else if (res < 0) {
	    fprintf(stderr, "Attacker exiting on read error.\n");
	    exit(1);
	}
	*p++ = c;
    } while (c != '\n');
    *p++ = 0; /* add null terminator after newline */
    return line_buf;
}

void write_str(int fd, const char *s) {
    int len = strlen(s);
    int res = write(fd, s, len);
    if (res == -1) {
	fprintf(stderr, "Attacker write failed: %s\n", strerror(errno));
	exit(1);
    } else if (res != len) {
	fprintf(stderr, "Attacker unhandled partial write\n");
	exit(1);
    }
}

void write_long(int fd, unsigned long x) {
    int res = write(fd, &x, sizeof(long));
    if (res == -1) {
	fprintf(stderr, "Attacker write failed: %s\n", strerror(errno));
	exit(1);
    } else if (res != sizeof(long)) {
	fprintf(stderr, "Attacker unhandled partial write\n");
	exit(1);
    }
}

void pad(int fd, int n) {
    int i;
    for (i = 0; i < n; i++)
	write_str(fd, "A");
}

void attack(int to_fd, int from_fd) {
    char *line;
    unsigned long printf_addr;
    unsigned long libc_base;

    /* You'll need to fill in the rest of this function to implement
       your attack. You may want to use the functions write_str,
       write_long, pad, and read_line_unbuffered defined above, as
       well as the standard library function strtol to parse a
       number. */
}

/* [0] is the read file descriptor, [1] is the write side */
int pipe_to_subproc[2];
int pipe_from_subproc[2];

int main(int argc, char **argv) {
    int res;
    pid_t child_pid;
    res = pipe(pipe_to_subproc);
    if (res) {
	fprintf(stderr, "Pipe (to) creation failed: %s\n", strerror(errno));
	exit(1);
    }
    res = pipe(pipe_from_subproc);
    if (res) {
	fprintf(stderr, "Pipe (from) creation failed: %s\n", strerror(errno));
	exit(1);
    }
    child_pid = fork();
    if (child_pid == 0) {
	/* child process */
	/* printf("In child process\n"); */
	res = dup2(pipe_to_subproc[0], 0);
	if (res == -1) {
	    fprintf(stderr, "Dup to stdin failed: %s\n", strerror(errno));
	    exit(1);
	}
	res = dup2(pipe_from_subproc[1], 1);
	if (res == -1) {
	    fprintf(stderr, "Dup to stdout failed: %s\n", strerror(errno));
	    exit(1);
	}
	res = execl("./printf-server", "./printf-server", (char *)0);
	/* Exec returns only if it fails */
	assert(res == -1);
	fprintf(stderr, "Exec failed: %s\n", strerror(errno));
	exit(1);
    } else if (child_pid == -1) {
	fprintf(stderr, "Fork failed: %s\n", strerror(errno));
	exit(1);
    }

    /* Here, still in parent process */
    /* printf("In parent/attacker process\n"); */
    attack(pipe_to_subproc[1], pipe_from_subproc[0]);

    return 0;
}
