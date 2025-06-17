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
