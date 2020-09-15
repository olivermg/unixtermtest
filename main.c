#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void child() {
	char foo[] = "hello from slave\n";
	char buf[50];
	int rlen;

	printf("child: reading from stdinx\n");
	rlen = read(0, buf, sizeof(buf));
	if (rlen > 0) {
		printf("child: read %s\n", buf);
	}

	sleep(2);

	printf("child: writing to stdout\n");
	write(1, foo, strlen(foo));

	sleep(3);

	exit(0);
}

void spawn(char* spts) {
	pid_t cpid;
	int sin;
	int sout;

	cpid = fork();
	if (0 == cpid) {
		close(1);
		close(0);
		sin = open(spts, O_RDONLY);
		sout = open(spts, O_WRONLY);
		dup2(sin, 0);
		dup2(sout, 1);

		child();
	}
}

int main(int argc, char* argv[]) {
	char data[] = "foobar\n";
	char rdata[50];
	ssize_t rlen;

	int mfd = posix_openpt(O_RDWR);
	char* sname = ptsname(mfd);

	printf("parent: master fd: %d, slave device: %s\n", mfd, sname);

	spawn(sname);

	sleep(2);

	printf("parent: writing to master\n");
	write(mfd, data, strlen(data));

	printf("parent: reading from master...\n");
	rlen = read(mfd, rdata, sizeof(rdata));
	while (rlen > 0) {
		printf("%s", rdata);
		rlen = read(mfd, rdata, sizeof(rdata));
	}

	sleep(10);

	close(mfd);

	wait(NULL);
}
