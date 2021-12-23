#include <assert.h>
#include <dirent.h>
#include <ftw.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
using namespace std;

string input;
vector<string> command; // command and parameter
vector<string> history;

int cd();
void process();

int main()
{
    command.clear();
    while (1) {
        process();
        if (strcmp(command[0].c_str(), "cd") == 0) {
            cd();
            if (history.size() > 10) {
                history.erase(history.begin());
            }
            history.push_back(input);
            command.clear();
        } else if (strcmp(command[0].c_str(), "exit") == 0) {
            if (history.size() > 10) {
                history.erase(history.begin());
            }
            history.push_back(input);
            command.clear();
            exit(0);
        } else if (strcmp(command[0].c_str(), "record") == 0) {
            if (history.size() > 10) {
                history.erase(history.begin());
            }
            for (auto it = history.begin(); it != history.end(); it++) {
                cout << *it << endl;
            }
            history.push_back(input);
            command.clear();
        } else {
            char path[256] = { 0 };
            strcpy(path, "/bin/");
            strcat(path, command[0].c_str());
            char* argv[100]={0};
            for (size_t i = 0; i < command.size(); i++) {
                char temp[20]={0};
                command[i].copy(temp, sizeof(command[i]));
                argv[i]=temp;
            }
            pid_t pid = fork();
            assert(pid != -1);
            if (pid == 0) {
                execv(path, argv);
                perror("\033[;31mexecv error\033[0m");
            } else {
                wait(NULL);
            }
            if (history.size() > 10) {
                history.erase(history.begin());
            }
            history.push_back(input);
            command.clear();
        }
    }
}

void process()
{
    char dir[100];
    getcwd(dir, sizeof(dir));
    strcat(dir, "$");
    cout << dir;
    getline(cin, input);
    istringstream s(input);
    string para;
    while (s >> para) {
        command.push_back(para);
    }
}

int cd()
{
    if (command.size() > 2) {
        cout << "too many parameters" << endl;
        return 0;
    } else if (command.size() == 1) {
        // return previous directory
        char dir[200];
        getcwd(dir, sizeof(dir));
        char* index = strrchr(dir, '/');

        string temp = "";
        for (size_t i = 0; i < index - dir; i++) {
            temp += dir[i];
        }
        if (!(chdir(temp.c_str()) + 1)) {
            cout << "change failed." << endl;
            return 0;
        } else {
            return 1;
        }

    } else if (command.size() == 2) {
        char dir[100];
        getcwd(dir, sizeof(dir));
        DIR* Dir = NULL;
        strcat(dir, "/");
        strcat(dir, command[1].c_str());
        Dir = opendir(dir);
        if (!dir) {
            fprintf(stderr, "open directory error\n");
            return 0;
        } else {
            chdir(dir);
        }
        closedir(Dir);
        return 1;
    }
    return 0;
}