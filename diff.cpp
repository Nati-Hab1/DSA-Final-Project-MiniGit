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