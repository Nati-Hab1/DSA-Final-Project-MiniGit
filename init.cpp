#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <ctime>
#include <sstream>

using namespace std; 
namespace fs = filesystem;

//Since we were allowed to use SHA-1 or a custom hash we used our own simpler custom hash.
string Hash(const string& content) {
    hash<string> hasher;
    return to_string(hasher(content));
}

//Here we declared some global paths to avoid code duplication.
const string MINI_GIT_DIR = ".minigit";
const string OBJECTS_DIR = MINI_GIT_DIR + "/objects";
const string COMMITS_DIR = MINI_GIT_DIR + "/commits";
const string HEAD_FILE = MINI_GIT_DIR + "/HEAD.txt";
const string INDEX_FILE = MINI_GIT_DIR + "/index.txt";
const string BRANCHES_FILE = MINI_GIT_DIR + "/branches.txt";

/*This function is written to get the current timestamp, and it's usefull because the
current timestamp is used when a new file is commited and displayed on the log.*/
string getCurrentTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return string(dt);
}

void init() {
    if (fs::exists(MINI_GIT_DIR)) {
        cout << "MiniGit is already initialized." << endl;
        return;
    }
    fs::create_directory(MINI_GIT_DIR);
    fs::create_directory(OBJECTS_DIR);
    fs::create_directory(COMMITS_DIR);
    ofstream(HEAD_FILE) << "main";  // default branch
    ofstream(BRANCHES_FILE) << "main:null\n"; // branch -> commit
    ofstream(INDEX_FILE); // empty staging area
    cout << "Initialized empty MiniGit repository." << endl;
}