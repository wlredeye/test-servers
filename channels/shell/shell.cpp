#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include <unistd.h>
#include <memory.h>

std::vector<char *> stringToChar(std::vector<std::string> s){
    std::vector<char *> c;
    
    for(int i = 0; i < s.size(); ++i){
        char *str;
        str = new char[s[i].size() + 1];
        memcpy(str, s[i].c_str(), s[i].size() + 1);
        
        c.push_back(str);
    }
    return c;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        if(!item.empty()){
            elems.push_back(item);
        }
    }
    return elems;
}

int forkProc(int in, int out, std::vector<char*> cmd){
    if(!fork()){
        if (in != STDIN_FILENO){
            dup2(in,STDIN_FILENO);
            close (in);
        }
        
        if (out != 1)
        {
            dup2 (out, 1);
            close (out);
        }
        char **command = &cmd[0];
        return execvp(command[0],command);
    }
    
    return 0;
}

int createPipes(std::vector<std::vector<char*>> &cmds){
    int in = 0;
    int fd[2];
    
    for(int i=0; i < cmds.size()-1; i++){
        pipe(fd);
        
        forkProc(in, fd[1], cmds[i]);
        
        close(fd[1]);
        in = fd[0];
    }
    
    if (in != 0)
        dup2(in, 0);
    FILE *f = fopen("result.out", "w");
    int file_fd = fileno(f);
    if (file_fd != 1){
        dup2(file_fd, 1);
        close(file_fd);
    }
    int lastCmd = cmds.size() - 1;
    char **command = &cmds[lastCmd][0];
    return execvp(command[0], command);
}

int main(int argc, char **argv){
    std::string input;
    getline(std::cin, input);
    std::vector<std::string> commands;
    std::vector<std::vector<char*>> cmds;
    split(input,'|', commands);
    for(auto iter = commands.begin(); iter != commands.end(); iter++){
        std::vector<std::string> cmd;
        split(*iter, ' ', cmd);
        std::cout << cmd.size() << std::endl;

        std::vector<char*> args = stringToChar(cmd);
        args.push_back(NULL);
        cmds.push_back(args);
    }
    
    return createPipes(cmds);
}