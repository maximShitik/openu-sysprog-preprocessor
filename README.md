# Final Project – Course 20465 (2024B)

**Author**: Maxim Shitik

This project is the final assignment for course **20465 (2024B)**, implementing a complete two-pass assembler for a custom assembly language. It includes a preprocessor, lexer, parser, symbol table construction, and code generation stages.

---

## 📁 Project Structure

```
.
├── pre_prosses/          # Preprocessor module
├── lexer/                # Lexical analysis and AST generation
├── first_pass/           # Symbol table construction and semantic checks
├── second_pass/          # Machine code generation and linking
└── backend/              # Output files generation
```

---

## 🧩 Module Descriptions

### 🔹 Preprocessor (`pre_prosses`)

The **Preprocessor** is the first step in the pipeline. It handles **macro definitions and expansions**, producing a clean `.am` file.

#### Responsibilities:
- **Macro Definition**: Detects and stores macro content using a hash table.
- **Macro Expansion**: Replaces macro calls with their stored body.
- **File Management**: Creates a temporary `.as` file and outputs the processed `.am` file.
- **Error Handling**: Validates macro names and reports syntax errors with line numbers.
- **Cleanup**: Deletes the `.as` copy after processing.

---

### 🔹 Lexer & Parser (`lexer`)

The **Lexer** performs lexical and syntactic analysis of each line from the `.am` file.

#### Key Functions:
- `parse_line()` – Tokenizes and builds an Abstract Syntax Tree (AST).
- `line_type()` – Determines if the line is a command, instruction, or label.
- `operand_group()` – Classifies command types based on operand count.
- `process_command_operands()` – Validates operand types and order.

#### Features:
- Detects syntax errors (e.g., illegal labels, missing operands).
- Ensures macros aren’t reused as labels.
- Constructs a consistent AST for further stages.

---

### 🔹 First Pass (`first_pass`)

The **First Pass** scans the `.am` file to build symbol tables and track instruction/data memory.

#### Workflow:
1. **Initialization**: Sets IC=100 and DC=0.
2. **Line-by-Line Parsing**: Parses into AST and validates each line.
3. **Label Handling**: Adds labels to the symbol table with type and address.
4. **Instruction/Command Processing**: Updates IC and stores data in memory buffers.
5. **Extern/Entry Management**: Adds symbols and flags conflicts.
6. **Address Adjustment**: Increments data symbol addresses by final IC value.
7. **Error Handling**: Logs all errors without halting prematurely.

#### Output:
- A fully populated symbol table
- Data ready for address resolution in the second pass

---

### 🔹 Second Pass (`second_pass`)

The **Second Pass** resolves symbols and generates the final machine code.

#### Responsibilities:
- **Symbol Resolution**: Assigns addresses to labels and handles externals.
- **Instruction Encoding**: Converts AST instructions into binary format.
- **Error Handling**: Reports undefined symbols or encoding issues with line numbers.
- **External Symbol Tracking**: Updates the external symbol list.

#### Key Functions:
- `second_pass()` – Main logic for processing and encoding.
- `set_first_word()` – Encodes instruction words based on operand types.

---

### 🔹 Backend (`backend`)

Responsible for writing output files after both passes are complete.

#### Output Files:
- `.ob` – Object file with encoded instructions and data
- `.ext` – List of external symbols used
- `.ent` – List of entry symbols declared

#### Responsibilities:
- Formats binary data into textual output
- Writes label and memory info to each file
- Ensures consistency between symbol table and encoded data

---

## ✅ Summary

This assembler includes:
- A full macro preprocessor
- A lexer/parser that constructs ASTs
- A two-pass compilation process
- Full symbol resolution with entries and externals
- Output generation in standard assembly formats

The modular architecture ensures clear separation between preprocessing, parsing, symbol handling, encoding, and file generation.
