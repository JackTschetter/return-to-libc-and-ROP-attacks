#include <stdarg.h>
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
	    printf("Server exiting on EOF.\n");
	    exit(0);
	} else if (res < 0) {
	    fprintf(stderr, "Server exiting on read error.\n");
	    exit(1);
	}
	*p++ = c;
    } while (c != '\n');
    *p++ = 0; /* add null terminator after newline */
    return line_buf;
}

int overflow(char *p) {
    char buf[24];
    int i;
    int sum = 0;
    for (i = 0; p[i] != '\n'; i++) {
	buf[i] = p[i];
	sum += buf[i] + buf[i/2];
    }
    return sum;
}

void useful_gadget(void) {
    asm volatile ("pop %rdi; ret");
}

typedef int (*print_fn)(const char *, ...);

/* eprintf has the same interface as printf, but prints to stderr
   instead of stdout. */
int eprintf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    return vfprintf(stderr, fmt, ap);
    va_end(ap);
}

void my_memset(void *p, int byte_val, size_t size) {
    memset(p, byte_val, size);
}

int main(int argc, char **argv) {
    char fmt[1024] = "%d\n";
    long n = 0;
    print_fn output_func;
    my_memset(&output_func, 0, sizeof(print_fn));
    if (!output_func)
	output_func = printf;
    setbuf(stdout, 0);
    for (;;) {
	char *line = read_line_unbuffered(0);
	if (line[0] == 'F') {
	    strcpy(fmt, line + 1);
	} else if (line[0] == 'N') {
	    n = strtol(line + 1, 0, 0);
	} else if (line[0] == 'O') {
	    n = overflow(line + 1);
	} else if (line[0] == 'P') {
	    (*output_func)(fmt, n);
	} else if (line[0] == 'S') {
	    output_func = printf;
	} else if (line[0] == 'E') {
	    output_func = eprintf;
	} else if (line[0] == 'Q') {
	    return 0;
	}
    }
}
