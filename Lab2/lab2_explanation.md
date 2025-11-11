# DE0917_Lab2

Author: Ayma Rehman  
Student ID: 241ADB165  
GitHub Repository: https://github.com/AymaRehman/DE0917/tree/main/Lab2

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

## Folder Structure
```
Lab2/
├── data/
│   ├── inputs/
│   │   ├── flights_invalid.csv
│   │   ├── flights_valid.csv
│   │   └── query.json
│   ├── db.json
│   ├── errors.txt
│   └── response.json
├── .gitignore
├── flight_parser.py
├── assignment.md
├── lab2_explanation.md
├── report.md
└── requirements.txt
```
## How To Run
  
All commands are executed from the project root (where the `Lab2/` folder is located).
  
## 🆘 View Help / Usage Guide
  
See all available options and their descriptions:  
```
python Lab2/flight_parser.py -h
```

### Parse a Single CSV File
Parse one CSV file and save valid flights + errors:  
```
python Lab2/flight_parser.py -i Lab2/data/inputs/flights.csv
```    
  
✅ Valid flights → `Lab2/data/db.json`  
⚠️ Errors → `Lab2/data/errors.txt`  
  
### Parse All CSV Files in a Folder
Parse multiple `.csv` files inside a directory:  
```
python Lab2/flight_parser.py -d Lab2/data/inputs
```  

### Load Existing Database
Load a previously saved JSON database (no CSV parsing):  
```
python Lab2/flight_parser.py -j Lab2/data/db.json
```  

### Run Queries on the Database
Run queries (from a JSON file) against the database:  
```
python Lab2/flight_parser.py -j Lab2/data/db.json -q Lab2/data/inputs/query.json
```  

### Parse and Query in One Step
Parse all CSVs and immediately run queries:  
```
python Lab2/flight_parser.py -d Lab2/data/inputs -q Lab2/data/inputs/query.json
```  

### Custom Output Paths
It is possible to override default output locations:  
``` 
python Lab2/flight_parser.py -i Lab2/data/inputs/flights.csv \  
  -o Lab2/custom/db.json \  
  -e Lab2/custom/errors.txt \  
  -r Lab2/custom/response.json   
  ```
    
----
  
## License
This project is submitted as part of a university assignment. All code is authored by Ayma Rehman.

