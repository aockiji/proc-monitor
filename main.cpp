#include <iostream> 
#include <string>
#include <vector>
#include <dirent.h>
#include <algorithm>
#include <cctype>
#include <filesystem>

using namespace std;

vector<int> getProcessIDs(const string& dirName = "/proc"){
    vector<int> PIDs;
    DIR *dir = opendir(dirName.c_str());

    if(!dir){
        cout << "Could not open directory";
        return PIDs;
    }

    struct dirent *entry;

    while((entry = readdir(dir)) != nullptr){
        string strDName = entry -> d_name;
        
        if (all_of(strDName.begin(), strDName.end(), ::isdigit)){
            PIDs.push_back(stoi(strDName));
        }
    }

    closedir(dir);
    return PIDs;
}

void printSymlinkTargets(const string& fdPath){
    DIR *dir = opendir(fdPath.c_str());

    if(!dir){
        cout << "None" << endl;
        return;
    }

    struct dirent *entry;

    try{
        while((entry = readdir(dir)) != nullptr){
            if(entry -> d_name[0] == '.'){
                continue;
            }
            string symlinkPath = string(fdPath) + "/" + string(entry -> d_name);
        
            if(filesystem::is_symlink(symlinkPath)){
                filesystem::path target = filesystem::read_symlink(symlinkPath); 
                cout << symlinkPath << " -> " << target << endl;
            }  
        }
    }
    catch(const exception& e){
        cerr << "Exception: " << e.what() << endl;
    }
}

int main(){
    string dir = "/proc";
    vector<int> PIDs = getProcessIDs(dir);

    for(int PID : PIDs){
        cout << "PROCESS ID: " << PID << endl;
        string fdPath = (dir + "/" + to_string(PID) + "/fd").c_str();
        printSymlinkTargets(fdPath);
        cout << endl;
    }

    return 0;
}