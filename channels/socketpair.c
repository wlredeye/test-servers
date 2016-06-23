#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv){
	int fd[2];
	socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
	if(!fork()){
		close(STDOUT_FILENO);
		dup2(fd[1],STDOUT_FILENO);
		close(fd[1]);
		getchar();
	} else {
		close(STDIN_FILENO);
		dup2(fd[0],STDIN_FILENO);
		close(fd[0]);
		getchar();
	}
	return 0;
}
