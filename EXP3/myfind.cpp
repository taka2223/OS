#include <dirent.h>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <regex>
#include<time.h>
#include<string>
using namespace std;
time_t time1;
string preProcess(char* file){
    const char* wild1 = "*";
    const char* wild2 = "?";
    string re = "[[:print:]]";

    string tmp = file;
    size_t pos=0;
    string sub1="";
    string sub2;

    while(pos = tmp.find(wild1,pos)+1)
    {
        pos--;
        if (pos != 0)
        {
            sub1 = tmp.substr(0, pos);
            sub2 = tmp.substr(pos, tmp.length());
            tmp = sub1 + re + sub2;
        }else
        {
            tmp= re+tmp;
        }
        pos += re.length()+1;
    }
    pos = 0;
        while(pos = tmp.find(wild2,pos)+1)
    {
        pos--;
        sub1 = tmp.substr(0,pos);
        sub2 = tmp.substr(pos,tmp.length());
        tmp = sub1+re+sub2;
        pos += re.length()+1;
    }
    return tmp;
}
bool isDir(const char* path)
{
    struct stat st;
    lstat(path, &st);
    return 0 != S_ISDIR(st.st_mode);
}
void myfind(char* path,regex pattern,int time);
void myfind(char* path, regex pattern);
void myfind(char* path,int time);

int main(int argc, char* argv[])
{
    time(&time1);
    if (argc < 2) {
        std::cout << "No parameter." << std::endl;
        exit(1);
    }
    if (argv[2] == NULL) {
        DIR* dp;
        struct dirent* filename;
        dp = opendir(argv[1]);
        if (!dp) {
            fprintf(stderr, "open directory error\n");
            return 0;
        }
        while (filename = readdir(dp)) {
            printf("filename:%-10s\td_info:%ld\t d_reclen:%us\n",
                filename->d_name, filename->d_ino, filename->d_reclen);
        }
        closedir(dp);
    }else if (argv[3]==NULL)
    {   
        regex pattern(preProcess(argv[2]));
        myfind(argv[1],pattern);

    }else{
        regex pattern(preProcess(argv[2]));
        string temp = argv[3];
        int time = stoi(temp);
        cout<<time<<endl;
        myfind(argv[1],pattern,time);
    }
    
    return 0;
}

void myfind(char* path, regex pattern)
{
    DIR* pdir;
    struct dirent* pdirent;
    pdir = opendir(path);
    if (!pdir) {
        fprintf(stderr, "open directory error\n");
        return ;
    }
    while (pdirent = readdir(pdir)) {
        if (strcmp(pdirent->d_name, ".") == 0
            || strcmp(pdirent->d_name, "..") == 0)
            continue;
        char temp[256];
        strcpy(temp,path);
        strcat(temp,"/");
        strcat(temp, pdirent->d_name);
        if (isDir(temp)) {
            myfind(temp, pattern);
        } else {
            if (regex_match(pdirent->d_name,pattern) || regex_match(temp,pattern)) {
                std::cout << "Found: " << temp<< std::endl;
            }
        }
    }
    closedir(pdir);
    return;
}

void myfind(char* path,int time){
    DIR* pdir;
    struct dirent* pdirent;
    pdir = opendir(path);
    if (!pdir) {
        fprintf(stderr, "open directory error\n");
        return;
    }
        while (pdirent = readdir(pdir)) {
        if (strcmp(pdirent->d_name, ".") == 0
            || strcmp(pdirent->d_name, "..") == 0)
            continue;
        char temp[256];
        strcpy(temp,path);
        strcat(temp,"/");
        strcat(temp, pdirent->d_name);
        if (isDir(temp)) {
            myfind(temp, time);
        } else {
            struct stat st;
            lstat(path, &st);
            if (difftime(time1,st.st_mtim.tv_sec)>86400*time)
            {
                cout<<"Found file that was modified "<<time<<" day before: "<<temp<<endl;
            }
        }
    }
    closedir(pdir);
    return;
}

void myfind(char* path,regex pattern,int time){
    DIR* pdir;
    struct dirent* pdirent;
    pdir = opendir(path);
    if (!pdir) {
        fprintf(stderr, "open directory error\n");
        return ;
    }
    while (pdirent = readdir(pdir)) {
        if (strcmp(pdirent->d_name, ".") == 0
            || strcmp(pdirent->d_name, "..") == 0)
            continue;
        char temp[256];
        strcpy(temp,path);
        strcat(temp,"/");
        strcat(temp, pdirent->d_name);
        if (isDir(temp)) {
            myfind(temp, pattern);
        } else {
            struct stat st;
            lstat(path, &st);
            if ((regex_match(pdirent->d_name,pattern) || regex_match(temp,pattern))&&(difftime(time1,st.st_mtim.tv_sec)>86400*time)) {
                std::cout << "Found: " << temp<< std::endl;
            }
        }
    }
    closedir(pdir);
    return;
}
