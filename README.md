# C-Style_Compiler

This project is a C-style compiler, and it includes a lexer written using Flex. Below are the steps to set up and use the lexer, especially if you're on Windows and need to use WSL (Windows Subsystem for Linux) to run it.

## Prerequisites

Before proceeding, ensure you have the following:

1. **Windows 10/11** with WSL (Windows Subsystem for Linux) installed.
2. **Flex** and **GCC** (or **G++**) installed on WSL.

### Step 1: Install WSL on Windows (if not already installed)

Follow these steps to install WSL (Windows Subsystem for Linux) on Windows:

1. Open **PowerShell** as Administrator and run the following command to enable WSL:
   ```bash
   wsl --install
