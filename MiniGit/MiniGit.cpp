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

void add(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cout << "File not found: " << filename << endl;
        return;
    }
    stringstream buffer;
    buffer << inFile.rdbuf();
    string content = buffer.str();
    string hash = Hash(content);

    ofstream outFile(OBJECTS_DIR + "/" + hash);
    outFile << content;
    outFile.close();

    ofstream indexOut(INDEX_FILE, ios::app);
    indexOut << filename << ":" << hash << endl;

    cout << "Staged file: " << filename << endl;
}

string getCurrentBranch() {
    ifstream headIn(HEAD_FILE);
    string branch;
    getline(headIn, branch);
    return branch;
}

string getBranchHead(const string& branchName) {
    ifstream branchesIn(BRANCHES_FILE);
    string line;
    while (getline(branchesIn, line)) {
        size_t sep = line.find(":");
        if (line.substr(0, sep) == branchName) {
            return line.substr(sep + 1);
        }
    }
    return "null";
}

void updateBranchHead(const string& branch, const string& newHash) {
    ifstream in(BRANCHES_FILE);
    stringstream updated;
    string line;
    while (getline(in, line)) {
        size_t sep = line.find(":");
        string name = line.substr(0, sep);
        if (name == branch) {
            updated << name << ":" << newHash << "\n";
        } else {
            updated << line << "\n";
        }
    }
    ofstream out(BRANCHES_FILE);
    out << updated.str();
}

void commit(const string& message) {
    ifstream indexIn(INDEX_FILE);
    if (!indexIn) {
        cout << "There is nothing to commit." << endl;
        return;
    }

    stringstream commitData;
    string line;
    while (getline(indexIn, line)) {
        commitData << line << endl;
    }

    string timestamp = getCurrentTime();
    string branch = getCurrentBranch();
    string parent = getBranchHead(branch);

    string metadata = "message: " + message + "\n" +
                      "timestamp: " + timestamp +
                      "parent: " + parent + "\n" +
                      "branch: " + branch + "\n";

    string commitHash = Hash(metadata + commitData.str());
    ofstream commitOut(COMMITS_DIR + "/" + commitHash);
    commitOut << metadata;
    commitOut << commitData.str();

    updateBranchHead(branch, commitHash);

    ofstream(INDEX_FILE); // clear staging area
    cout << "Committed. Hash: " << commitHash << endl;
}

void log() {
    string branch = getCurrentBranch();
    string current = getBranchHead(branch);

    while (current != "null") {
        string path = COMMITS_DIR + "/" + current;
        ifstream in(path);
        string line;
        cout << "Commit: " << current << endl;
        while (getline(in, line)) {
            if (line.rfind("message:", 0) == 0 ||
                line.rfind("timestamp:", 0) == 0 ||
                line.rfind("parent:", 0) == 0) {
                cout << line << endl;
            }
        }
        cout << "###################################" << endl;

        in.clear();
        in.seekg(0, ios::beg);
        while (getline(in, line)) {
            if (line.rfind("parent:", 0) == 0) {
                current = line.substr(8);
                break;
            }
        }
    }
}

void createBranch(const string& branchName) {
    string current = getBranchHead(getCurrentBranch());
    ofstream branchesOut(BRANCHES_FILE, ios::app);
    branchesOut << branchName << ":" << current << "\n";
    cout << "Created branch: " << branchName << endl;
}

void checkout(const string& target) {
    string hash = getBranchHead(target);
    if (hash != "null") {
        ofstream(HEAD_FILE) << target;
        cout << "Switched to branch: " << target << endl;
        return;
    }

    if (fs::exists(COMMITS_DIR + "/" + target)) {
        cout << "Checked out commit: " << target << endl;
    } 
    
    else {
        cout << "Branch or commit not found: " << target << endl;
    }
}

void merge(const string& targetBranch) {
    string currentBranch = getCurrentBranch();
    string currentCommit = getBranchHead(currentBranch);
    string targetCommit = getBranchHead(targetBranch);

    if (targetCommit == "null") {
        cout << "Branch not found or no commits in target branch." << endl;
        return;
    }

    ifstream targetIn(COMMITS_DIR + "/" + targetCommit);
    string line, mergedData;
    unordered_map<string, string> mergedFiles;

    while (getline(targetIn, line)) {
        if (line.find(":") != string::npos && line.find("message:") != 0 &&
            line.find("timestamp:") != 0 && line.find("parent:") != 0 &&
            line.find("branch:") != 0) {
            size_t sep = line.find(":");
            string file = line.substr(0, sep);
            string hash = line.substr(sep + 1);
            mergedFiles[file] = hash;
        }
    }

    ifstream currentIn(COMMITS_DIR + "/" + currentCommit);
    while (getline(currentIn, line)) {
        if (line.find(":") != string::npos && line.find("message:") != 0 &&
            line.find("timestamp:") != 0 && line.find("parent:") != 0 &&
            line.find("branch:") != 0) {
            size_t sep = line.find(":");
            string file = line.substr(0, sep);
            string hash = line.substr(sep + 1);
            if (mergedFiles.find(file) != mergedFiles.end() && mergedFiles[file] != hash) {
                cout << "CONFLICT: both modified " << file << endl;
            }
            mergedFiles[file] = hash; // prefer current
        }
    }

    ofstream indexOut(INDEX_FILE);
    for (const auto& [file, hash] : mergedFiles) {
        indexOut << file << ":" << hash << endl;
    }

    stringstream msg;
    msg << "Merged branch " << targetBranch;
    commit(msg.str());
}

void diff(const string& commit1, const string& commit2) {
    ifstream in1(COMMITS_DIR + "/" + commit1);
    ifstream in2(COMMITS_DIR + "/" + commit2);
    if (!in1 || !in2) {
        cout << "One of the commits or both are not found." << endl;
        return;
    }

    unordered_map<string, string> map1, map2;
    string line;
    while (getline(in1, line)) {
        if (line.find(":") != string::npos && line.find("message:") != 0 && line.find("timestamp:") != 0 && line.find("parent:") != 0 && line.find("branch:") != 0) {
            size_t sep = line.find(":");
            map1[line.substr(0, sep)] = line.substr(sep + 1);
        }
    }
    while (getline(in2, line)) {
        if (line.find(":") != string::npos && line.find("message:") != 0 && line.find("timestamp:") != 0 && line.find("parent:") != 0 && line.find("branch:") != 0) {
            size_t sep = line.find(":");
            map2[line.substr(0, sep)] = line.substr(sep + 1);
        }
    }

    cout << "### Diff between " << commit1 << " and " << commit2 << " ###" << endl;
    for (const auto& [file, hash1] : map1) {
        if (map2.find(file) == map2.end()) {
            cout << file << " was removed in " << commit2 << endl;
        } else if (map2[file] != hash1) {
            cout << file << " was modified." << endl;
        }
    }
    for (const auto& [file, hash2] : map2) {
        if (map1.find(file) == map1.end()) {
            cout << file << " was added in " << commit2 << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout<<"If minigit is not added to system path you can use the commands listed below as they are."<<endl;
        cout<<"But if minigit is added to system path you can remove the ./ before minigit to use it."<<endl;
        cout<<endl;
        cout << "1. ./minigit init" << endl;
        cout << "2. ./minigit add <file>" << endl;
        cout << "3. ./minigit commit -m 'commit message goes here'" << endl;
        cout << "4. ./minigit log" << endl;
        cout << "5. ./minigit branch branch_name" << endl;
        cout << "6. ./minigit checkout branch_name" << endl;
        cout << "7. ./minigit merge branch_name  (But don't forget to checkout to the specific branch first.)" << endl;
        cout << "8. ./MiniGit diff <commit1> <commit2>  (You can get the commit hash of two files using log.)" << endl;
        cout<<endl;
        return 1;
    }

    string command = argv[1];
    if (command == "init") {
        init();
    }

    else if (command == "add" && argc >= 3) {
        add(argv[2]);
    }

    else if (command == "commit" && argc >= 4 && string(argv[2]) == "-m") {
        commit(argv[3]);
    }

    else if (command == "log") {
        log();
    }

    else if (command == "branch" && argc >= 3) {
        createBranch(argv[2]);
    }

    else if (command == "checkout" && argc >= 3) {
        checkout(argv[2]);
    }

    else if (command == "merge" && argc >= 3) {
        merge(argv[2]);
    }

    else if (command == "diff" && argc >= 4) {
        diff(argv[2], argv[3]);
    }

    else {
        cout << "Unknown or incomplete command." << endl;
    }

    return 0;
}