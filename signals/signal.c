#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv){
	pid_t pid = getpid();
	FILE *fp = fopen("pid", "w");
	fprintf(fp, "%d", pid);
	fclose(fp);
	sigset_t mask;
	sigaddset(&mask,SIGTERM);
	sigaddset(&mask,SIGINT);
	sigprocmask(SIG_SETMASK, &mask, 0);
	while(1){};
	return 0;
}
