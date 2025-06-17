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