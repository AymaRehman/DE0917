# Pythonic Arithmetic Parser in C  
  
## Goal
  
Write a C program that evaluates arithmetic expressions from text files using (mostly) Python’s operator rules and standard C (similar to Java) arithmetic semantics. The program must:  
* Read an input file (or a directory of files).  
* Parse and evaluate expressions.  
* Report either the numeric result or a precise error location.  
* Write the result to an output file with a specific naming convention.
  
## CLI & I/O
  
### Invocation   
  
`calc [-d DIR | --dir DIR] [-o OUTDIR | --output-dir OUTDIR] input.txt`  
* If ` -d/--dir ` is provided, process all ` *.txt files ` in DIR (ignore subfolders).  
* If ` -o/--output-dir ` is omitted, create a default output folder:  
` <input_base>_<username>_<studentid> `  
  Example: ` labs_valdis_12345/ `  
* If ` -o/--output-dir ` is provided and does not exist, create it.  
  
### Output file name
  
For each input `task1.txt`, produce:  
`task1_<name>_<lastname>_<studentid>.txt`  
The file contains either the result (single line) or `ERROR:<pos>.`  
> Character position is 1-based, counting every char in the file, including `\n` as one character.  
  
### Input Assumptions (Baseline)
  
* ASCII only.  
* Whitespace may appear anywhere.  
* At minimum (grade 4–6) inputs are single line ≤ 100 chars (later grades extend this).  
* Expressions can be integer and, for higher grades, float (IEEE 754 double).  
  
## Features by Grade (Rubric)  
  
### Grade 4/10 — Minimal Pass  
  
* Single line (≤ 100 chars).  
* Integers only (base-10, optional leading `+`/`-`).  
* Operators: `+` `-` (binary).  
* Output: numeric result (as integer text).  
* On any parse/eval error: `ERROR:<pos>` (1-based char index).  
    
### Grade 5/10 — Positioned Errors  
  
* Same as Grade 4 plus:  
* Accurate error position: if the first error is at character 57, output `ERROR:57`.   
  * Examples of errors: invalid token, unexpected end, two operators in a row, leading operator without operand, etc.  
  
### Grade 6/10 — Multiplication (+ optional floats)  
  
* Add `*` (binary).
* Additional challenge: support floats (64-bit) mixed with ints.
  * If floats are supported, print using `%.15g` (see Output formatting below).
* Still single line ≤ 100 chars.
  
### Grade 7/10 — Division (+ big files challenge)

* Add `/` (true division if floats supported; otherwise integer division).  
* Division by zero ⇒ `ERROR:<pos>` where `<pos>` is the starting char index of the offending divisor token (or the `/` token—either is acceptable if documented).  
* Additional challenge: allow multiple lines and inputs up to 10,000 characters total.  
  
### Grade 8/10 — Parentheses  
  
* Add `(` `)` with correct precedence and associativity.  
* Error reporting must still work (unmatched paren should point to the problematic paren or place where the closing/opening was expected).  
  
### Grade 9/10 — Comments & Batch Mode   
  
* Ignore full lines that start with `#` (Pythonic line comments).  
* Support `-d/--dir` to process all `*.txt` in a folder.  
* Support `-o/--output-dir` to choose/create output folder.  
* Default folder behavior when `-o` not provided as specified above.  
  
### Grade 10/10 — Exponentiation & No Input Size Guarantees  
  
* Add Python power operator: `**`  
  * Right-associative (e.g., `2**3**2 == 2**(3**2) == 512`).  
  * Higher precedence than `*` `/` and `+` `-`.  
  * Precedence is just below unary operators (if you implement unary; see below).  
* No specific length limits beyond “fits in Codespaces VM” (i.e., handle very long files efficiently).  
* Maintain all prior features and error reporting.  
  
----
  
## Language Semantics (Recommended Defaults)  
  
### Tokens  
  
* Integers: `0` or `[1-9][0-9]*` (optionally with leading `+`/`-` if you implement unary).  
* Floats (if implemented): forms accepted by `strtod` are okay (e.g., `12.34`, `.5`, `1e-3`).  
* Operators: `+ - * / **`  
* Parentheses: `(` `)`  
* Whitespace: skip freely.  
* Comments: entire line starting with `#` is ignored (from first non-space `#` to line end).  
  
### Precedence & Associativity  
  
From highest to lowest:  
1. `**` (right-associative)  
2. Unary `+` `-` (optional, left-to-right apply to a single primary)  
3. `*` `/` (left-associative)  
4. `+` `-` (left-associative)    
> Note: You can reach 10/10 without unary `+`/`-`. If you do implement unary, document it and ensure it doesn’t break error positions.  
  
### Numeric Types & Output Formatting  
  
* Integers: use 64-bit (`long long`) for safety. Overflow handling is not required but certainly a bonus (document behavior if you detect it).
* Floats: use `double` (`strtod`).
* Result printing:
  * If your evaluator is int-only: print integer like `-42`.
  * If mixed numeric:
    * If the final value is integral (e.g., `3.0`), print without decimal (e.g., `3`).
    * Else print with `%.15g`.
  * Trailing spaces/newlines: one newline at end is fine.  
   
----
        
## Errors & Positions (Required)  
  
On the first encountered error, stop and write:  
`ERROR:<pos>`  
* `<pos>` is 1-based char index in the entire file (count `\n` as 1).  
* Examples:   
  * `12 + * 5` → error at `*` position.  
  * `((2+3)` → error at the place the parser expected `)` (you may choose to report at EOF position).  
  * `10/0` → division by zero; report at the start of `0` (or the `/`)—be consistent and document.  
> Optional enhancement (not required for grade): You may also write `ERROR:<line>:<col>` in addition to the required format on the next line.
   
---- 
  
## Grammar (suggested, EBNF-ish, but you can use something else)  
  
You can implement via recursive descent or shunting-yard.  
If no unary:  
```
expr    := term { ('+' | '-') term }  
term    := power { ('*' | '/') power }    
power   := primary { '**' primary }         // implement as right-associative  
primary := NUMBER | '(' expr ')'    
```
If with unary:  
```
expr    := term { ('+' | '-') term }  
term    := factor { ('*' | '/') factor }  
factor  := power  
power   := unary { '**' unary }             // right-assoc: parse power -> unary ( '**' power )?  
unary   := ('+' | '-') unary | primary  
primary := NUMBER | '(' expr ')'  
```
  
---
  
## Examples  
  
### Simple  
   
Input (`task1.txt`):  
`2 + 2`  
Output file:  
`task1_name_lastname_studentid.txt`  
Contents:  
`4`  
  
### Error with position  
   
`7 + * 3`  
`*` is the 5th character (assuming single spaces). Output:  
`ERROR:5`
  
### Parentheses & precedence  
  
```  
2 + 3*4  
(2 + 3) * 4  
```  
→ `14` and `20`  
  
### Power (Grade 10)  
  
`2**3**2`  
→ `512`  
  
### Comments & multi-file (Grade 9+)  
  
```
# this is ignored  
2 + 3  
```  
→ `5`  

## Implementation Hints (C, GCC on Ubuntu/Codespaces)  

* Tokenizer: produce a stream of tokens (NUMBER, PLUS, MINUS, STAR, SLASH, POW, LPAREN, RPAREN, EOL/EOF) with each token carrying start index (1-based) for error reporting.  
* Numbers: parse with `strtoll` (int) and/or `strtod` (double). Keep original char index for error pointing.  
* Parsing:  
  * Recursive descent is straightforward and keeps precedence clear.  
  * For `**` right-associativity, implement `power := unary ('**' power)?`.  
* Big input (Grade 10): stream the file into memory (e.g., `mmap` or `fread`), but avoid quadratic concatenation. Tokenize once, then parse.  
* Error handling:  
  * Define a single function `fail(pos, message)` that stores the first error position.  
  * Return early to short-circuit evaluation.  
* Division by zero:  
  * For ints: `b == 0` → error at token `b`.  
  * For floats: if divisor numerically equals 0.0, treat as error (do not return `inf`).  
* Output directory:  
  * Use `mkdir` (POSIX) with `0775`. If it exists, proceed.  
  * Build paths with `snprintf` safely.  
* Globbing (`-d`): read directory entries, include files ending with `.txt` (case-sensitive).  
* Comments: skip a line if first non-space char is `#`.  
* Makefile: provide `make`, `make test`, `make run`.
  
---
  
## 📤 Project Submission Instructions  
  
You must submit exactly one file:  
`calc.c`  
  
### File Header (Mandatory & Optional Lines)  
  
The first lines of your `calc.c` must follow this format:  
```  
// Name Lastname StudentID  
// (optional) GitHub repository link for this assignment  
// Compilation instructions (e.g., gcc -O2 -Wall -Wextra -std=c17 -o calc calc.c)  
```  
  
* Line 1: your full Name, Lastname, StudentID  
* Line 2 (optional): link to your GitHub repo for this project.  
  * If provided and the repo is well-structured, it may improve grading.  
* Next lines: any special compilation or run instructions if your program needs them (beyond standard `gcc`).  
  
---  
  
## Code Commenting Requirements  
  
* Your `calc.c` must contain plentiful, clear, and meaningful comments.  
* Focus on explaining WHY you are doing something, not just what the code does.  
  * ✅ Good example:  
    `// Using recursive descent parsing here because it naturally reflects operator precedence`  
  *❌ Bad example:  
    `// Add x and y`  
* If you use external resources (books, StackOverflow answers, documentation, AI prompts, etc.),  
  * cite them in comments at the relevant place in the code.  
  * Example:  
    ```
    // Tokenizer logic inspired by K&R Chapter 5 example  
    // Prompt used with ChatGPT: "recursive descent parser in C for arithmetic"  
    ```
   
---    
  
## Grading Notes  
  
* Unclear, missing, or low-quality comments will lower your grade.  
* Bad or misleading comments are worse than no comments.  
* Explicitly citing resources you used is required. Failure to do so will negatively impact the grade.  
-- 
👉 Only `calc.c` will be accepted. Do not upload Makefiles, executables, or multiple `.c/.h` files.  
--
  
## 📑 Sample `calc.c` File Header  
```  
// Alice Johnson 221234  
// https://github.com/alicejohnson/c-final-project   <-- optional  
// Compile with: gcc -O2 -Wall -Wextra -std=c17 -o calc calc.c  
```
  
---    
  
## Grading Checklist

| Item | Points | Notes |
|:------|:-------:|:------|
| Grade 4 baseline (int `+ -`, single line ≤100) | 4 | result or `ERROR:<pos>` |
| Correct `ERROR:<pos>` location | +1 | First error only |
| Add `*` (int) | +1 | Optional float support (challenge) |
| Add `/` (int or float) + div-by-zero detection | +1 | Report at sensible position |
| Handle multi-line & up to 10,000 chars | +1 | Additional challenge |
| Parentheses with proper precedence | +1 | Keep error reporting |
| `#` comment lines; `-d/--dir` batch; `-o/--output-dir` | +1 | Create folder if missing; default folder naming |
| `**` (right-assoc) + robust long-file handling | +2 | Full score 10 |

**Code quality modifiers (up to ±1):**
- +0.5: clear structure, no UB, checks for I/O errors, robust CLI.  
- +0.5: concise tests and documentation.  
- −1.0: memory leaks/UB/crashes on normal inputs.  
- ±2.0: instructor discretion for code clarity and comments.

---  
  
## Anti-cheating & Libraries  
  
* Do not shell out to Python/BC/expr/etc.  
* Only use standard C library (stdio/stdlib/string/ctype/math/errno/dirent/sys/stat).  
* No parser generators for the core (flex/bison) — write your own tokenizer & parser.  
* If you use code assistance such as LLMs (all prompts are required in comments as well as specific tool!)  
* Relying fully on LLM or others is against spirit of this assignment and will hurt you  
  
---  
  
## Starter Test Ideas  
  
### Valid    
* `42`  
* `10 + 20 - 5`  
* `2 * 3 + 4`  
* `2 + 3 * 4`  
* `(2 + 3) * 4`  
* `2**3**2`  
* With floats (if supported): `3/2`, `1.5*4`, `.5 + .25`, `1e3 + 2`  
### Invalid  
* `+` (no operand)  
* `2 **` (trailing op)  
* `2 /* 3`  
* `((1+2)`  
* `10/0`  
* `2 ** ** 3`  
  
---   
  
## Timeline & Deliverables  
  
* Submit on Time  
* -1 grade for being up to 1 week late  
* -2 grade for being up to 2 weeks late  
  
---  
  
## Tips  
  
* Keep a global char index during scanning to make `ERROR:<pos>` trivial.
* Store each token’s start index; on errors report the most relevant token’s start.
* To print integers from double results, check `fabs(val - llround(val)) < 1e-12`.
* For `**`, beware left-to-right bugs; test `2**3**2` rigorously.  
  
   
