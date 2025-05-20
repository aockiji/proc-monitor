#include <iostream> 
#include <string>
#include <vector>
#include <dirent.h>
#include <algorithm>
#include <cctype>

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

int main(){
    vector<int> PIDs = getProcessIDs();

    for(int PID : PIDs){
        cout << PID << endl;
    }

    return 0;
}