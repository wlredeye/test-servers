#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

static void
child_handler(int sig){
	pid_t pid;
	int status;
	while((pid = waitpid(-1, &status, WNOHANG)) > 0){}
}

int main(int argc, char **argv){
	pid_t cpid = fork();
	pid_t pid = getpid();
	FILE *fp;
	if(cpid){
		fp = fopen("pid_parent", "w");
	} else {
		fp = fopen("pid_child", "w");
	}
	fprintf(fp, "%d", pid);
	fclose(fp);

	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = child_handler;

	sigaction(SIGCHLD, &sa, NULL);
	while(1){};
	return 0;
}
