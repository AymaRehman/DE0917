# DE0917_Lab2

Author: Ayma Rehman  
Student ID: 241ADB165  
GitHub Repository: https://github.com/AymaRehman/DE0917_Lab2

## Aim of the Assignment

Create a Python program that parses flight schedule CSV files, validates and separates valid/invalid flights, exports results to JSON and text files, and supports query execution from JSON.

## Features

- Parse single CSV files or all CSV files in a folder.
- Validate records using:
  - Flight ID: 2–8 alphanumeric characters
  - Origin/Destination: 3 uppercase letters
  - Departure/Arrival datetime: valid `YYYY-MM-DD HH:MM` format
  - Arrival after departure
  - Price: positive float
- Separate valid flights into `db.json` and invalid records into `errors.txt`.
- Optionally load an existing JSON database.
- Execute queries from JSON files and export responses.
- CLI with `argparse` for flexible file/folder parsing, query execution, and custom output paths.


## License
This project is submitted as part of a university assignment. All code is authored by Ayma Rehman.
