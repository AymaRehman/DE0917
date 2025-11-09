# DE0917_Lab1

Author: Ayma Rehman  
Student ID: 241ADB165  
GitHub Repository: https://github.com/AymaRehman/DE0917_Lab1   

## Aim of the Assignment

Write a C program that evaluates arithmetic expressions from text files using Python-like operator rules and standard C arithmetic semantics. It should read input files (or directories), parse and evaluate expressions, report results or error locations, and save outputs using a defined naming convention.  

## The parser supports:

- Integers and floating-point numbers (double)  
- Operators: +, -, *, /, and exponentiation **  
- Parentheses for grouping expressions ( ... )  
- Multiple lines in input files (up to 10,000 characters)  
- Error reporting with precise 1-based character positions  
- Batch processing of multiple .txt files in a folder  
- Comment lines starting with # (ignored during parsing)  
- Custom output directory with default fallback  

## Compilation Instructions

Compile using GCC:  
`gcc -v calc.c -o calc.exe`    

Run with:  
`./calc -d e:/c/source_folder -o e:/c/output_folder text-file_to_process.txt`  

Software:  
This code will work only on Windows, neither on Linux nor on MacOS as indicated in code comments.  
`// directory path should be separated by one forward slash / without any slash at the end `  

## Main Arguments  

This program expects at least 2 and at most 5 arguments:  

`-d or --dir and then source directory name`  
These are mandatory.  

` -o or --output-dir and then output directory name`   
These are optional, however, if omitted, default will be created.  

`source file name`  
This is optional, however, if omitted, all .txt files in source directory will be processed.  

## Basic Arithmetic Parsing Details  

- Supports integer and floating-point arithmetic  
- Operator precedence (highest → lowest):  
  - `** (right-associative)`  
  - `*, / (left-associative)`  
  - `+, - (left-associative)`  
- Parentheses handled with correct precedence and associativity  
- Division by zero triggers an error (ERROR:<position>)  
- Invalid expressions report the first error character position  

## References  

### Online Websites On Google:  

header libraries: https://www.geeksforgeeks.org/c/header-files-in-c-cpp-and-its-uses/  
file handling: https://www.geeksforgeeks.org/c/basics-file-handling-c/  
read/write structure from/to a file: https://www.geeksforgeeks.org/c/read-write-structure-from-to-a-file-in-c/  
EOF, getc() and feof(): https://www.geeksforgeeks.org/c/eof-and-feof-in-c/  
error handling: https://www.geeksforgeeks.org/c/error-handling-in-c/  
goto for exception handling: https://www.geeksforgeeks.org/c/using-goto-for-exception-handling-in-c/   
error handling during file operations: https://www.geeksforgeeks.org/c/error-handling-during-file-operations-in-c-c/  
handle 0 division error: https://www.geeksforgeeks.org/c/c-program-to-handle-divide-by-zero-and-multiple-exceptions/  
perror: https://www.geeksforgeeks.org/c/perror-in-c/  
tokenization: https://www.geeksforgeeks.org/cpp/string-tokenization-in-c/  
helper functions: https://www.geeksforgeeks.org/javascript/what-are-the-helper-functions/  
command line arguments in C: https://www.geeksforgeeks.org/cpp/command-line-arguments-in-c-cpp/   
dynamic array: https://www.geeksforgeeks.org/c/dynamic-array-in-c/  
stacks: https://www.geeksforgeeks.org/c/implement-stack-in-c/  
  
### Other Resources:  
 
Presentations for Course, DE0917, "Programming Languages(English)(1),25/26-R" on ORTUS  
Assignments and Notes from the Course "Object Oriented Programming" for previous semester (2) (in C++)   

### Additional Notes:  

Above resources were used solely to understand the implementation of certain logics in C.  
None of the code is copied, rather understood and applied in the assignment after understanding.  
Concepts not part of references were already known to a good extent.  
Only a few minor programming details including syntax were occasionally referred to at a basic level; hence, this is not included in the references.  

## License  

This project is submitted as part of a university assignment. All code is authored by Ayma Rehman.  


