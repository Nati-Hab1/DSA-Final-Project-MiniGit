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


