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