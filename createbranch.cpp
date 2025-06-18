void createBranch(const string& branchName) {
    string current = getBranchHead(getCurrentBranch());
    ofstream branchesOut(BRANCHES_FILE, ios::app);
    branchesOut << branchName << ":" << current << "\n";
    cout << "Created branch: " << branchName << endl;
}