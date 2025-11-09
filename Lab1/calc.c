//====================================================================
//                                calc.c
//                          AUTHOR: AYMA REHMAN
//                         STUDENT ID: 241ADB165
//====================================================================
// Compile with:  gcc -v calc.c -o calc.exe
// Run with: ./calc -d e:/c/source_folder -o e:/c/output_folder text-file_to_process.txt

// --------------------------------------------------------------------
// A R G U M E N T S 
// --------------------------------------------------------------------
// This program is expecting at least 2 and at most 5 arguments
// -d or --dir and then source directory name, these are mandatory
// -o or --output-dir and then output directory name, these are optional if omitted default will be created
// directory path should be separated by one forward slash / without any slash at the end 
// source file name, it's optional if omitted all .txt files in source directory will be processed

// --------------------------------------------------------------------
// F U N C T I O N A L I T Y 
// --------------------------------------------------------------------
// This program reads arithmetic expressions from text files,
// evaluates them line by line in pythonic way, and writes the results 
// to output files.

// --------------------------------------------------------------------
// USED HEADER LIBRARIES 
// --------------------------------------------------------------------
#include <stdio.h>      // Standard Input/Output functions (printf, scanf, fopen, etc.)
#include <stdlib.h>     // Memory allocation, process control, and conversions (malloc, free, exit, etc.)
#include <dirent.h>     // Directory operations (opendir, readdir, closedir) - mainly for UNIX/Linux
#include <string.h>     // String handling functions (strcpy, strcmp, strlen, strcat, etc.)
#include <math.h>       // Mathematical functions (sqrt, pow, sin, cos, etc.)
#include <direct.h>     // Directory creation/manipulation on Windows (_mkdir, _chdir, etc.)
#include <sys/stat.h>   // File/directory information and permissions (stat, mkdir, chmod, etc.)


// --------------------------------------------------------------------
// Main Variables and Constants 
// --------------------------------------------------------------------

// Structure to hold data for each character in the file
// All numbers will be stored as type 'n' in integer format
// Operators as type 'o' and all other characters as errors type 'e'
// spaces will be ignored during tokenization
// line number and column number will be stored for error reporting
// error message will be stored in case of error type

typedef struct {
    char type;  // 'n' = number, 'o' = operator, 'e' = error
    int num;    // value if type = 'n'
    char op;    // operator if type = 'o'
    int lineno; // line number
    int colno;  // column number
    char error_msg[100]; // error message if type = 'e'
} file_data;

// Dynamic array to hold multiple file_data entries
typedef struct {
    file_data *data;
    size_t size;
    size_t capacity;
} file_data_array;

// --------------------------------------------------------------------
// Helper Functions 
// --------------------------------------------------------------------

// Function to initialize the dynamic array file_data_array
void init_file_data(file_data_array *arr, size_t initialCapacity) {
    arr->data = malloc(initialCapacity * sizeof(file_data));
    arr->size = 0;
    arr->capacity = initialCapacity;
}

// Function to add an entry to the dynamic array file_data_array
void add_file_data(file_data_array *arr, char type, int num, char op, int lineno, int colno, const char *error_msg ) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(file_data));
    }
    arr->data[arr->size].type = type;
    arr->data[arr->size].num = num;
    arr->data[arr->size].op = op;
    arr->data[arr->size].lineno = lineno;
    arr->data[arr->size].colno = colno;
    strcpy(arr->data[arr->size].error_msg, error_msg); // Initialize to empty string
    arr->size++;
}

// Function to free the dynamic array file_data_array at the end of its use
void freeArray(file_data_array *arr) {
    free(arr->data);
    arr->data = NULL;
    arr->size = arr->capacity = 0;
}

// This function tokenizes the input file contents into file_data_array
// For simplacity it processes max 10000 characters from the input
// We assume that file may contain data like below and 
// sent as input to this function
    //     3 + 4 * 2 / (1 - 5)
    //     10 + 20 * 3
    //     5 * 7 - (4 / 2)
    //     8 / 0
// Each line is treated as a separate expression to evaluate
// It identifies numbers, operators, parentheses, and errors
// It also tracks line and column numbers for error reporting
// It returns a dynamic array of tokenized data

file_data_array tokenize_file_contents(char my_data[10000]){

    file_data_array fda; // variable to hold the tokenized data in dynamic array
    init_file_data(&fda, 10); // initialize with capacity of 10 entries at start
    // variables to manipulate opening and closing brackets
    int openning_bracket_found = 0;
    int openning_bracket_array_position = 0;
    int closing_bracket_found = 0;
    int closing_bracket_array_position = 0;
    // start line and column numbers
    int lineno = 1, colno = 0;

    // loop through each character in the input data
    for (int i = 0; my_data[i] != '\0'; i++) {
        char c = my_data[i];
         
        if (c == '\n') {
            lineno++;
            colno = 0;
            if (openning_bracket_found && !closing_bracket_found){ 
                // there is no closing bracket for this opening bracket
                // so mark it as error
                fda.data[openning_bracket_array_position].type = 'e';
                strcpy(fda.data[openning_bracket_array_position].error_msg , "No matching closing bracket found");
            } else if (!openning_bracket_found && closing_bracket_found){
                // there is no opening bracket for this closing bracket
                // so mark it as error
                fda.data[closing_bracket_array_position].type = 'e';
                strcpy(fda.data[closing_bracket_array_position].error_msg , "No matching opening bracket found");
               
            }

            openning_bracket_found = 0;
            closing_bracket_found = 0;
            openning_bracket_array_position = 0;
            closing_bracket_array_position = 0;
            continue;
        }
        colno++;

        if (c >= '0' && c <= '9') {
            int digit = c - '0';
            // Combine multi-digit numbers
            if (fda.size > 0 && fda.data[fda.size - 1].type == 'n' && fda.data[fda.size - 1].lineno == lineno) {
                fda.data[fda.size - 1].num = fda.data[fda.size - 1].num * 10 + digit;
            } else {
                add_file_data(&fda, 'n', digit, '\0', lineno, colno, "");
            }
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
            if (fda.size > 0 && fda.data[fda.size - 1].type == 'o' &&
                fda.data[fda.size - 1].lineno == lineno &&
                c != '(' && c != ')') {
                add_file_data(&fda, 'e', 0, c, lineno, colno, "Invalid double operator");

                } // invalid double operator
            else {
                add_file_data(&fda, 'o', 0, c, lineno, colno, "");
                if (c == '(') {
                    openning_bracket_found = 1;
                    openning_bracket_array_position = fda.size - 1;
                } else if (c == ')') {
                    closing_bracket_found = 1;
                    closing_bracket_array_position = fda.size - 1;
                    
                }
            }
        } else if (c == ' ' || c == '\t') {
            continue; // skip whitespace
        } else {
            add_file_data(&fda, 'e', 0, c, lineno, colno, "Invalid character");
        }
    }
    
    // For debugging purpose we can display tokens here
    // Just uncomment the below code block and run to see the tokenized output

    // for (size_t i = 0; i < fda.size; i++) {
    //     printf("%c : %d : %c : %d : %d \n",
    //            fda.data[i].type,
    //            fda.data[i].num,
    //            fda.data[i].op,
    //            fda.data[i].lineno,
    //            fda.data[i].colno,
    //            fda.data[i].error_msg);
    // }

    // return the tokenized data array
    return fda;

}

// Function to remove the .txt extension from a filename
// it is used to build output file names based on input file names
// by removing .txt extension and appending _output.txt to it
// to have a good correspondence between input and output file names

void remove_extension(char *filename) {
    // find the last occurrence of '.' in the filename to locate the extension
    char *ext = strrchr(filename, '.'); 
    // check if the dot is found and if the extension is .txt
    if (ext != NULL && strcmp(ext, ".txt") == 0) {
        // remove the extension by replacing the dot with null character
        // it will effectively cut the string at the dot position from filename
        *ext = '\0'; 
    }
    
}


// Helper: check if a directory exists
// it is used to verify if output directory exists or not
static int directory_exists(const char *path) {
    struct _stat info;
    if (_stat(path, &info) != 0)
        return 0; // Doesn't exist
    return (info.st_mode & _S_IFDIR) != 0; // True if directory
}

// Create the main directory and a default subdirectory if not present
// it will retun boolean 1 , 2 or 3 for success and 0 for failure
// 1 means provided output directory exists or created successfully
// 2 means default output directory created successfully
// 3 that the provided output directory is not valid so default created successfully

int create_directory(const char *dir, const char *default_dir) {

    int status = 0;
    // Check if default_dir is provided as it is mandatory
    if (!default_dir) {
        printf("Default directory is required.\n");
        return 0;
    }

    // Check if the main directory exists
    if (dir && directory_exists(dir)) {
        {printf("Directory already exists: %s\n", dir);
        return 1;
        }   
    } else {
        if (dir)
            {
            printf("Directory not found: %s\n", dir);
            status = 3;
        }
        else
            {
            printf("No output directory provided.\n");
            status = 2;
        }
        
        printf("Going to create default directory: %s\n", default_dir);

        if (!directory_exists(default_dir)) {
            if (_mkdir(default_dir) == 0) {
                printf("Created default directory: %s\n", default_dir);
            } else if (errno != EEXIST) {
                perror("Failed to create default directory");
                return 0;
            }
        } else {
            printf("Default directory already exists: %s\n", default_dir);
        }
    }
    // everything was fine so return success
    return status;
}

// write contents to a file inside a given directory
int write_to_file(const char *directory, const char *filename, const char contents[10000]) {
    char filepath[1024];

    // Build full path (e.g., "dir/file.txt")
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);
    printf("Writing to file: %s\n", filepath);
    // Open file for writing
    FILE *fp = fopen(filepath, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return 1; // nonzero = error
    }

    // Write the contents
    if (fputs(contents, fp) == EOF) {
        perror("Error writing to file");
        fclose(fp);
        return 2; // error code for write failure
    }

    fclose(fp);
    // printf("File successfully written: %s\n", filepath);
    return 0; // success
}

// Now we shall start the evaluation of tokenized data
// below are helper functions for evaluation
// below function applies the operator on two operands and returns the result
double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b == 0) ? INFINITY : a / b;
        default: return 0;
    }
}

// function to return precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// below is the core function that evaluates the tokenized data line by line,
// calculates the result or identifies errors,
// prints the results to console, 
// prepares output contents to be written to output file and directory received as arguments

void evaluate_tokens(file_data_array *tokens, const char *outdir, const char *output_filename) {
    if (tokens->size == 0) return;

    double values[100];
    char ops[100];
    int vtop = -1, otop = -1;
    int error_found = 0;
    int current_line = tokens->data[0].lineno;
    char output_contents[10000] = "";
    char buffer[400] = "";

    printf("\nEvaluation:\n");
    printf("------------------\n");

    for (size_t i = 0; i < tokens->size; i++) {
        file_data tok = tokens->data[i];
        
        // When line changes, evaluate the previous line
        if (tok.lineno != current_line) {
            // Process any remaining operators
            while (otop >= 0 && vtop >= 1) {
                double b = values[vtop--];
                double a = values[vtop--];
                char op = ops[otop--];
                
                if (op == '/' && b == 0) {
                    printf("Line %d : <<Error>> : Division by zero\n", current_line);
                    snprintf(buffer, sizeof(buffer), "Line %d : <<Error>> : Division by zero\n", current_line);
                    strncat(output_contents, buffer, sizeof(output_contents) - strlen(output_contents) - 1);
                    error_found = 1;
                    break;
                }

                values[++vtop] = applyOp(a, b, op);
            }

            if (!error_found && vtop == 0) {
                printf("Line %d : <<OK>> : Result = %.2f\n", current_line, values[vtop]);
                snprintf(buffer, sizeof(buffer), "Line %d : <<OK>> : Result = %.2f\n", current_line, values[vtop]);
                strncat(output_contents, buffer, sizeof(output_contents) - strlen(output_contents) - 1);
            } else if (!error_found) {
                printf("Line %d : <<Error>> : Invalid expression\n", current_line);
                snprintf(buffer, sizeof(buffer), "Line %d : <<Error>> : Invalid expression\n", current_line);
                strncat(output_contents, buffer, sizeof(output_contents) - strlen(output_contents) - 1);
            }

            // Reset stacks for next line
            vtop = otop = -1;
            error_found = 0;
            current_line = tok.lineno;
        }

        if (error_found) continue;

        // Handle different token types
        if (tok.type == 'e') {
            printf("Line %d : <<Error>> : at column %d : %s\n", tok.lineno, tok.colno, tok.error_msg);
            snprintf(buffer, sizeof(buffer), "Line %d : <<Error>> : at column %d : %s\n",
                     tok.lineno, tok.colno, tok.error_msg);
            strncat(output_contents, buffer, sizeof(output_contents) - strlen(output_contents) - 1);
            error_found = 1;
            continue;
        }

        if (tok.type == 'n') {
            // Push number to the values stack
            values[++vtop] = tok.num;
        } else if (tok.op == '(') {
            // Push opening parenthesis to the operators stack
            ops[++otop] = '(';
        } else if (tok.op == ')') {
            // Handle closing parenthesis
            int foundOpening = 0;
            while (otop >= 0) {
                if (ops[otop] == '(') {
                    foundOpening = 1;
                    otop--; // pop '('
                    break;
                }
                if (vtop < 1) break;
                double b = values[vtop--];
                double a = values[vtop--];
                char op = ops[otop--];
                // handle division by zero error
                if (op == '/' && b == 0) {
                    printf("Line %d : <<Error>> : Division by zero\n", tok.lineno);
                    snprintf(buffer, sizeof(buffer), "Line %d : <<Error>> : Division by zero\n", tok.lineno);
                    strncat(output_contents, buffer, sizeof(output_contents) - strlen(output_contents) - 1);
                    error_found = 1;
                    break;
                }

                values[++vtop] = applyOp(a, b, op);
            }
            // handle unmatched closing parenthesis error
            if (!foundOpening && !error_found) {
                printf("Error: Unmatched ')' at line %d, column %d\n", tok.lineno, tok.colno);
                snprintf(buffer, sizeof(buffer),
                         "Error: Unmatched ')' at line %d, column %d\n", tok.lineno, tok.colno);
                strncat(output_contents, buffer, sizeof(output_contents) - strlen(output_contents) - 1);
                error_found = 1;
            }
        // handle operators +, -, *, /
        } else if (tok.op == '+' || tok.op == '-' || tok.op == '*' || tok.op == '/') {
            // Apply higher or equal-precedence operators
            while (otop >= 0 && precedence(ops[otop]) >= precedence(tok.op)) {
                if (vtop < 1) {
                    printf("Error: Insufficient values for operation at line %d\n", tok.lineno);
                    snprintf(buffer, sizeof(buffer),
                             "Error: Insufficient values for operation at line %d\n", tok.lineno);
                    strncat(output_contents, buffer, sizeof(output_contents) - strlen(output_contents) - 1);
                    error_found = 1;
                    break;
                }
                // calculate intermediate result
                double b = values[vtop--];
                double a = values[vtop--];
                char op = ops[otop--];
                if (op == '/' && b == 0) {
                    printf("Line %d : <<Error>> : Division by zero\n", tok.lineno);
                    snprintf(buffer, sizeof(buffer), "Line %d : <<Error>> : Division by zero\n", tok.lineno);
                    strncat(output_contents, buffer, sizeof(output_contents) - strlen(output_contents) - 1);
                    error_found = 1;
                    break;
                }
                values[++vtop] = applyOp(a, b, op);
            }
            if (!error_found) ops[++otop] = tok.op;
        }

        // below code block is for debugging purpose
        // to see the status of stacks after each token processed
        // just uncomment it and run the program when debugging is needed

        // printf(" vtop is %d , ", vtop);
        // printf(" otop is %d \n, ", otop);
        // for (int l = 0; l <= vtop; l++) {
        //         printf("%.2f , ", values[l]);
        //     } 
        // printf("\n");       
        // for (int k = 0; k <= otop; k++) {
        //     printf("%c, ", ops[k]);
        // } 
        // printf("\n");  
    }

    
    // Process the last line after the loop
    if (!error_found) {
        while (otop >= 0 && vtop >= 1) {
            double b = values[vtop--];
            double a = values[vtop--];
            char op = ops[otop--];

            if (op == '/' && b == 0) {
                    printf("Line %d : <<Error>> : Division by zero\n", current_line);
                    snprintf(buffer, sizeof(buffer), "Line %d : <<Error>> : Division by zero\n", current_line);
                    strncat(output_contents, buffer, sizeof(output_contents) - strlen(output_contents) - 1);
                    error_found = 1;
                    break;
                }

            values[++vtop] = applyOp(a, b, op);
        }

        if (vtop == 0) {
            printf("Line %d : <<OK>> : Result = %.2f\n", current_line, values[vtop]);
            snprintf(buffer, sizeof(buffer), "Line %d : <<OK>> : Result = %.2f\n", current_line, values[vtop]);
            strncat(output_contents, buffer, sizeof(output_contents) - strlen(output_contents) - 1);
        } else {
            printf("Line %d : <<Error>> : Invalid expression\n", current_line);
            snprintf(buffer, sizeof(buffer), "Line %d : <<Error>> : Invalid expression\n", current_line);
            strncat(output_contents, buffer, sizeof(output_contents) - strlen(output_contents) - 1);
        }
    }

    if (output_filename != "")
    {
    // After processing all lines, write the output contents to the specified file
    
    int result = write_to_file(outdir, output_filename, output_contents);
    
    if (result != 0)
        printf("Error writing file (code %d)\n", result);
    
    printf("Output written to file: %s/%s\n", outdir, output_filename);

    }
    
}


// main function where execution starts

int main(int argc, char **argv) {
    
    // Command line arguments will be stored here in below variables
    const char *mydir = NULL;
    const char *outdir = NULL;
    const char *input = NULL;
    const char *default_out_dir_suffix = "/default-output-folder";
    char default_outdir[PATH_MAX];  // default output directory path buffer to hold upto system max path length
    int output_directory_setup_status = 0;

    // below variables will hold the detail of the student (me) submitting the assignment
    // these will be used in output file names
    // these will not be editable in the code programmatically
    // so defining name in upper case, constant, to just distinguish to avoid accidental changes
    const char *STUDENT_NAME = "ayma";
    const char *STUDENT_LASTNAME = "rehman";
    const char *STUDENT_ID = "241ADB165";

    printf("\n\n\n");
    printf("<<<<< ************************************************   >>>>>\n");
    printf("<<<<<            WELCOME TO PYTHONIC CALCULATOR          >>>>>\n");
    printf("<<<<<                         BY                         >>>>>\n");
    printf("<<<<<               A Y M A      R E H M A N             >>>>>\n");
    printf("<<<<< *************************************************  >>>>>\n\n");

    // ------------- HANDELLING THE COMMAND LINE ARGUMENTS  ----------------
    printf("=================== COMMAND LINE ARGUMENTS ===================\n");
  
    // parameter argc contains the count of command line arguments
    // we are expecting at least 2 and at most 5 at most (-d dir -o outdir input.txt)
    // the first argument argv[0] is the program name so we shall check 
    // argc for 3 and 6 arguments to validate
    
    // printf("Welcome to program: %s\n", argv[0]);
    printf("Number of provided arguments: %d \n" , argc-1);

    if (argc > 6) {
        printf("Too many arguments, Maximum of 5 are allowed.\n");
        printf("Help:\n"
                "-d or --dir and then source directory name, these are mandatory \n"
                "-o or --output-dir and then output directory name, these are optional \n"
                "directory path should be separated by one forward slash / without any slash at the end \n"
                "source .txt file name, it's optional \n"
            );
        return 1;
    }
    if (argc < 3) {
        printf("Insufficient arguments, at least two are needed.\n");

         printf("Help:\n"
                "-d or --dir and then source directory name, these are mandatory \n"
                "-o or --output-dir and then output directory name, these are optional \n"
                "directory path should be separated by one forward slash / without any slash at the end \n"
                "source .txt file name, it's optional \n"
            );

        return 1;
    }

    // argv is an array of strings (char pointers) containing the values of
    // command line arguments
    // argv[0] contains the name of the program itself so we start from 1
    // We will loop through all arguments and check for -d or --dir , -o or --output-dir
    // and input file

   
    for (int counter = 1; counter < argc; ++counter) {
        // check for source directory argument, it is received with -d or --dir
        if (strcmp(argv[counter], "-d") == 0 || strcmp(argv[counter], "--dir") == 0) {
            mydir = argv[++counter];
            printf("Source Directory Received in Argument(%d): %s\n", counter, mydir);
        
        // check for output directory argument, it is received with -o or --output-dir
        } else if (strcmp(argv[counter], "-o") == 0 || strcmp(argv[counter], "--output-dir") == 0) {
            outdir = argv[++counter];
            printf("Output Directory Received in Argument(%d): %s\n", counter, outdir);

        // check for input file argument, it is received as .txt file 
        // and not starting with -
        } else if (argv[counter][0] != '-' && strstr(argv[counter], ".txt")) {
            input = argv[counter];
            printf("Source File Received as Argument(%d): %s \n", counter, argv[counter]);
        
        // if it is none of the above then it's an invalid argument
        } else {
            printf("Invalid Argument: %s\n", argv[counter]);
             printf("Help:\n"
                "-d or --dir and then source directory name, these are mandatory \n"
                "-o or --output-dir and then output directory name, these are optional \n"
                "directory path should be separated by one forward slash / without any slash at the end \n"
                "source .txt file name, it's optional \n"
            );
            return 1;
        }
    }
    // check if source directory is provided since it is mandatory
    // otherwise exit with error
    if (!mydir) {
        printf("No input directory provided.\n");
        return 1;
    }

    
    // check if input file is provided just for information
    // although its optional but if not provided all .txt files in source directory 
    // will be processed so inform user about it
    if (!input) {
        printf("As no specific file provided so all .txt files from source directory will be processed.\n");
        
    }
    
    // build the default output directory path inside source directory
    // using the suffix defined earlier
    snprintf(default_outdir, sizeof(default_outdir), "%s%s", mydir, default_out_dir_suffix);


    // check if provided output directory is valid then it's ok
    // otherwise create default output direcory at the path built above 

    output_directory_setup_status = create_directory(outdir, default_outdir);
    
    if (output_directory_setup_status != 0){
        printf("Output directory setup completed.\n");
    }
    // in case both are provided and default output directory creation failed
    // then inform user that no output will be saved
    else {
        printf("Output directory setup failed with code %d .\n", output_directory_setup_status);
        printf("So no output will be saved insted it will only be shown on screen \n");
    }

    printf("===============================================================\n");
    printf("                    Processing Files\n");
    // process the files in the source directory
    const char *folder = mydir; // change to your folder path
    DIR *dir = opendir(folder);
    if (dir == NULL) {
        perror("Open Directory");
        return 1;
    }

    struct dirent *entry;
    char filepath[1024];

    while ((entry = readdir(dir)) != NULL) {

        // Skip "." and ".." system's directory entries
        // also skip files that are not .txt files
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 
            || !strstr(entry->d_name, ".txt"))
            continue;
        
        // Construct full path
        snprintf(filepath, sizeof(filepath), "%s/%s", folder, entry->d_name);
        
        // If input file is specified, skip other files
        if (input && strcmp(entry->d_name, input) != 0) 
            continue; // skip if it is not the specified input file
    
        // printf("Contents of %s:\n", filepath);
        printf("\n---------------------------------------------------------------\n");
        printf("Source file name:  %s:", entry->d_name);
        

        // Open the file for reading
        FILE *file = fopen(filepath, "r");
        if (file == NULL) {
            // if there is an error in opening the file, 
            // skip it and continue to next file
            // following code block is for debugging purpose
            // printf("File %s:\n", filepath);
            // perror("fopen ");
            // and continue to next file
            continue;
        }
        
        // build output file name using student details and input file name
        char output_filename[256] = "";
        char base[256];
        if (output_directory_setup_status){
           
            strcpy(base, entry->d_name); // copy file name to base to manipulate
            // printf("Base name is: %s\n", base);
            // remove the .txt extension from base name
            remove_extension(base);
            // printf("Base name after removing extension is: %s\n", base);
            snprintf(output_filename, sizeof(output_filename), "%s_%s_%s_%s_output.txt", base, STUDENT_NAME, STUDENT_LASTNAME, STUDENT_ID);
            printf("\nOutput file name:  %s\n", output_filename);
            printf("---------------------------------------------------------------\n");
           
            printf("\nContents of source file:\n");
            printf("----------------------------\n");
        }

        // Read and print file contents
        char buffer[1024];
        char file_contents[10000] = ""; // to hold entire file contents
        while (fgets(buffer, sizeof(buffer), file)) {
            // printf("%s", buffer);
            // printf("\n********************");
            strcat(file_contents, buffer);
        }
        printf("%s", file_contents);
        printf("\n");
        char contents[10000];

        // Tokenize the file contents
        file_data_array tokens = tokenize_file_contents(file_contents);
        
        // Evaluate the tokens by line
        evaluate_tokens(&tokens , (output_directory_setup_status==1) ? outdir:default_outdir , output_filename);
        
        // Free the tokenized data array after use
        freeArray(&tokens);
        // print a new line for better readability between files
        printf("\n");

        // close the opened file
        fclose(file);
    }

    // close the opened directory
    closedir(dir);

    // print goodbye message
    printf("\n\n\n");
    printf("<<<<< ************************************************   >>>>>\n");
    printf("<<<<<                  G O O D    B Y E                  >>>>>\n");
    printf("<<<<< *************************************************  >>>>>\n\n");

    return 0;
}

// --------------------------------------------------------------------
//                          A P P E N D I X
// --------------------------------------------------------------------
//
// REFERENCES:
//
// ONLINE WEBSITES ON GOOGLE:
//
// header libraries: https://www.geeksforgeeks.org/c/header-files-in-c-cpp-and-its-uses/
// file handling: https://www.geeksforgeeks.org/c/basics-file-handling-c/
// read/write structure from/to a file: https://www.geeksforgeeks.org/c/read-write-structure-from-to-a-file-in-c/
// EOF, getc() and feof(): https://www.geeksforgeeks.org/c/eof-and-feof-in-c/
// error handling: https://www.geeksforgeeks.org/c/error-handling-in-c/
// goto for exception handling: https://www.geeksforgeeks.org/c/using-goto-for-exception-handling-in-c/ 
// error handling during file operations: https://www.geeksforgeeks.org/c/error-handling-during-file-operations-in-c-c/
// handle 0 division error: https://www.geeksforgeeks.org/c/c-program-to-handle-divide-by-zero-and-multiple-exceptions/
// perror: https://www.geeksforgeeks.org/c/perror-in-c/
// tokenization: https://www.geeksforgeeks.org/cpp/string-tokenization-in-c/
// helper functions: https://www.geeksforgeeks.org/javascript/what-are-the-helper-functions/
// command line arguments in C: https://www.geeksforgeeks.org/cpp/command-line-arguments-in-c-cpp/ 
// dynamic array: https://www.geeksforgeeks.org/c/dynamic-array-in-c/
// stacks: https://www.geeksforgeeks.org/c/implement-stack-in-c/
//
// OTHER RESOURCES:
// 
// Presentations for Course, DE0917, "Programming Languages(English)(1),25/26-R" on ORTUS
// Assignments and Notes from the Course "Object Oriented Programming" for previous semester (2) (in C++) 
//
// ADDITIONAL NOTES:
//
// Above resources were used solely to understand the implementation of certain logics in C.
// None of the code is copied, rather understood and applied in the assignment after understanding.
// Concepts not part of references were already known to a good extent.
// Only a few minor programming details including syntax were occasionally referred to at a basic level; 
// hence, this is not included in the references.
//
// GITHUB:
// 
// Link to my repository on Github: https://github.com/AymaRehman/DE0917_Lab1 
