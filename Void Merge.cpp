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
