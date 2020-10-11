#include <string>
#include <iostream>
#include <experimental/filesystem>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fstream>
#include <pwd.h>
#include <sys/types.h>
#include <iomanip>

#define PROC_DIRECTORY "/proc/"
using namespace std;

void help(){
  cout << "AUTHORS:" << endl << "Goriunova" << endl << "Eremin" << endl << "Dultseva" << endl << endl;
  cout << "AVAILABLE OPTIONS: " << endl;
  cout << "-mf: move a file" << endl;
  cout << "EXAMPLE: ./oslab2 -mf text.txt test/text.txt" << endl;
  cout << "-cf: copy a file" << endl;
  cout << "EXAMPLE: ./oslab2 -cf test/text.txt ../text.txt" << endl;
  cout << "-rmf: remove a file" << endl;
  cout << "EXAMPLE: ./oslab2 -rmf test/text.txt" << endl;
  cout << "-ssf: show size of file" << endl;
  cout << "EXAMPLE: ./oslab2 -ssf test" << endl;
  cout << "-saf: show all files in directory" << endl;
  cout << "EXAMPLE: ./oslab2 -saf test" << endl;
  cout << "-sap: show all processes" << endl;
  cout << "EXAMPLE: ./oslab2 -sap" << endl << endl;
  cout << "PROJECT DESCRIPTION: " << endl << "Working with the file system" << endl << endl;
}

void show_all_files(const char *path){
  DIR *dir = opendir(path);
  dirent *entry;
	  if (dir == NULL){
		return;
	  }

	  while ((entry = readdir(dir)) != NULL){
		cout << entry->d_name << endl;
	  }
	  closedir(dir);
}
unsigned long size (string fileName) {
	 unsigned long contentSize = 0;
	 
    struct stat fileInfo;
    if (stat(fileName.c_str(), &fileInfo) != 0)
    {
        std::string error = "Cannot get info about file \"" + fileName + "\"";
        throw error;
    }
    if (S_ISDIR(fileInfo.st_mode))
    {
        DIR *dir = opendir(fileName.c_str());
        if (dir == nullptr)
        {
            std::string error = "Cannot open dir \"" + fileName + "\"";
            throw error;
        }

        dirent *contentInfo;
        while ((contentInfo = readdir(dir)) != nullptr)
        {
            if ((strcmp(contentInfo->d_name, ".") != 0) && (strcmp(contentInfo->d_name, "..") != 0))
            {
                std::string pathToNestedFile = fileName + "/" + contentInfo->d_name;
                contentSize += size(pathToNestedFile);
            }
        }
        closedir(dir);
        return contentSize;
    }
    else
    {
        return fileInfo.st_size;
    }
}

char *getContent(char *str)
{
    char *buf = new char(64);
    char ch;
    int i = 0;

    do
    {
        ch = str[i];
        buf[i] = ch;
        ++i;
    } while (ch != '\\' && ch != '\0');

    buf[i] = '\0';

    return buf;
}

string getPidInfo(ifstream &file, const int &offset)
{
    string buf;
    for (int i = 0; i < offset; i++)
    {
        buf.clear();
        file >> buf;
    }
    return buf;
}

int showp()
{
    DIR *dir = opendir("/proc/");
    if (dir == nullptr)
    {
        return -1;
    }
    dirent *contentInfo;

    cout << std::setiosflags(ios::left);
    cout << std::setw(20) << "COMM";
    cout << std::setw(6) << "PID";
    cout << std::setw(6) << "STATE";
    cout << std::setw(6) << "PPID";
    cout << std::setw(6) << "NICE" << endl;

    while ((contentInfo = readdir(dir)) != nullptr)
    {
        char *pid = getContent(contentInfo->d_name);
        if (atoi(pid) != 0)
        {
            string pidPath = "/proc/";
            pidPath += pid;

            ifstream pidInfo(pidPath + "/stat", ios_base::in);
            if (!pidInfo.is_open())
            {
                string error = "Cannot open file " + pidPath;
                throw error;
            }

            cout << std::setw(20) << getPidInfo(pidInfo, 2);
            cout << std::setw(6) << pid;
            cout << std::setw(6) << getPidInfo(pidInfo, 1);
            cout << std::setw(6) << getPidInfo(pidInfo, 1);
            cout << std::setw(6) << getPidInfo(pidInfo, 15) << endl;
        }
        delete pid;
    }

    closedir(dir);
    return 0;
}

bool copyFile(const char *copyfromfile, const char *copytofile){
  ifstream cff(copyfromfile);
  ofstream ctf(copytofile);
 
  char sym;
  // цикл посимвольного чтения
  while (cff)
  {
    cff.get(sym);
    if (cff) {
      ctf.put(sym);
    }
  }
  cff.close();
  ctf.close();
  return 0;
}



int main(int argc, char *argv[]){
  if (string(argv[1]) == "-h"){
    help();
  } 
    
  if (string(argv[1]) == "--help"){
    help();
  }

  if (string(argv[1]) == "-mf"){
    rename(argv[2] , argv[3]);
  }

  if (string(argv[1]) == "-rmf"){
    remove(argv[2]);
  }

  if (string(argv[1]) == "-cf"){
    copyFile(argv[2], argv[3]);
  }

  if (string(argv[1]) == "-saf"){
    show_all_files(argv[2]);
  }

  if (string(argv[1]) == "-sap"){
    showp();
  }

  if (string(argv[1]) == "-ssf"){
    cout << "Size of this file is " << size(argv[2]) << " bytes" << endl;
  }

return 0;
}
