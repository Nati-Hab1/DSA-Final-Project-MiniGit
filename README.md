# DSA-Final-Project-MiniGit

A simplified Git-like version control system built in C++.  
MiniGit allows users to track versions of files, make commits, manage branches, perform merges, and compare commits — all via command-line interaction and local file system operations.

---

## 🚀 Features

✅ `init` – Initialize a MiniGit repository  
✅ `add <filename>` – Stage a file for commit  
✅ `commit -m "<message>"` – Save staged changes with a message  
✅ `log` – View commit history  
✅ `branch <branchname>` – Create a new branch  
✅ `checkout <branch/commit>` – Switch to another branch or commit  
✅ `merge <branch>` – Merge a branch into the current branch  
✅ `diff <commit1> <commit2>` – Show differences between two commits

---

## 📁 Project Structure

```
MiniGit/
├── MiniGit.cpp             # Full unified implementation
├── test.txt, hello.txt     # Sample test files
├── .minigit/               # MiniGit internal data
│   ├── objects/            # Stored file snapshots (by hash)
│   ├── commits/            # Commit metadata
│   ├── HEAD.txt            # Current branch pointer
│   ├── branches.txt        # Branch-to-commit map
│   └── index.txt           # Staging area
```

---

## 🛠️ How to Compile

### On Linux/macOS:
```bash
g++ -std=c++17 MiniGit.cpp -o MiniGit
```

### On Windows (PowerShell or CMD):
```powershell
g++ -std=c++17 MiniGit.cpp -o MiniGit.exe
```

---

## 💻 How to Use

### ➕ Initialize Repository
```bash
./MiniGit init
```

### 📄 Create and Add a File
```bash
echo Hello > hello.txt
./MiniGit add hello.txt
```

### 📝 Commit the File
```bash
./MiniGit commit -m "Initial commit"
```

### 📜 View Commit Log
```bash
./MiniGit log
```

### 🌱 Create and Switch Branches
```bash
./MiniGit branch dev
./MiniGit checkout dev
```

### 🔀 Merge a Branch
```bash
./MiniGit checkout main
./MiniGit merge dev
```

### 🧮 Compare Commits
```bash
./MiniGit log  # get commit hashes
./MiniGit diff <commit1> <commit2>
```

---

## 🧪 Sample Files

The repo includes several trial files such as:
- `test.txt`, `hello.txt`, `file1.txt`, etc.
- These are used to demonstrate the add, commit, and diff functionality.

---

## 📦 Final Version

- ✅ Full functionality in a single `MiniGit.cpp` file  
- ✅ All group contributions merged into `main` branch  
- ✅ Sample test files and internal state included  
- ✅ Clean commit history for grading and review

---

## 👥 Contributors

This project was developed by 2nd-year Software Engineering students at Addis Ababa University:

- **Natnael Habteselassie**  
- **Wengelle Yohannes**  
- **Ezana Mulatu**  
- **Samuel Abraham**  
- **Omer Abubeker**

---

## 🏁 Project Status

✅ Fully Completed  
🧪 Tested with multiple sample runs  
📁 Ready for final submission  
