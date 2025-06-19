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