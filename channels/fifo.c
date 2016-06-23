#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv){
	const char *in = "in.fifo";
	const char *out = "out.fifo";
	mkfifo(in, 0666);
	mkfifo(out, 0666);
	int fd_in = open(in, O_RDWR);
	int fd_out = open(out, O_RDWR);
	while(1){
		static char buf[1024];
		ssize_t nbytes = read(fd_in, buf, sizeof(buf));
		write(fd_out, buf, nbytes);
	}
	return 0;
}
