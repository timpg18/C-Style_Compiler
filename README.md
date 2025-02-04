# C-Style_Compiler

This project is a C-style compiler, and it includes a lexer written using Flex. Below are the steps to install Flex, run the Makefile, execute the lexer, and test the implementation.

## Prerequisites

Before proceeding, ensure you have the following:

1. **Windows 10/11** with WSL (Windows Subsystem for Linux) installed.
2. **Flex** and **GCC** installed on WSL.

### Step 1: Install WSL on Windows (if not already installed)

Open **PowerShell** as Administrator and run:

```bash
wsl --install
```

Restart the system if prompted.

### Step 2: Install Flex and GCC on WSL

Run the following command in WSL:

```bash
sudo apt update && sudo apt install flex gcc -y
```

Verify installation:

```bash
flex --version
gcc --version
```

## Compiling the Lexer using Makefile

The Makefile is configured to generate the lexer executable from the Flex file.

### Running the Makefile:

To compile the lexer, navigate to the `LEX_SOURCE` directory and run:

```bash
make
```

To clean generated files:

```bash
make clean
```

## Running the Lexer

After compiling, you can run the lexer manually on test cases. To run it on a specific test file:

```bash
./lexer < ./testing/test1.c
```

You can replace `test1.c` with any other test case file.

## Running All Test Cases Using `run.sh`

To automate testing, use the `run.sh` script.

### Make the script executable (only needed once):

```bash
chmod +x run.sh
```

### Run all test cases:

```bash
./run.sh
```

This will execute the lexer on all test files in the `testing/` directory.

## Running Individual Files Manually

If you want to test specific files without the script, you can do so with:

```bash
./lexer < ./testing/test2.c
```

or for another file:

```bash
./lexer < ./testing/test3.c
```

This ensures that the lexer correctly processes different test inputs.

---

By following these steps, you can successfully compile and run the lexer, ensuring all test cases are processed correctly.

