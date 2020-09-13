#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
	char data[] = "foobar\n";
	char rdata[50];
	ssize_t rlen;

	int fd = posix_openpt(O_RDWR);
	char* pname = ptsname(fd);

	printf("fd: %d, name: %s\n", fd, pname);

	sleep(10);

	printf("writing to %s\n", pname);
	write(fd, data, strlen(data));

	printf("reading from %s\n", pname);
	rlen = read(fd, rdata, sizeof(rdata));
	if (rlen > 0) {
		printf("read: %s\n", rdata);
	}

	sleep(10);

	close(fd);
}

