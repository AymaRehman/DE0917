# ✈️ Python Final Assignment: Flight Schedule Parser and Query Tool

You will have to **show and defend this program** in a laboratory session (similar to the C project).  
**Defense dates:** TBD (most likely late November or early December).

**Submission:**  
- Submit a `.zip` file with all source code (`.py` files) and correct folder structure, **OR/AND**  
- Submit the **URL of your GitHub repository**.

---

## 🧩 Overview

Write a **Python program** that can:

- Parse one or more flight schedule `.csv` files.  
- Validate and separate **valid** and **invalid** records.  
- Export results as:
  - `db.json` → valid flights  
  - `errors.txt` → invalid lines  
- Optionally load an existing JSON database instead of re-parsing.  
- Execute queries from a JSON file and save results.

---

## ⚙️ Command-Line Interface

| Argument | Description |
|-----------|-------------|
| `-i path/to/file.csv` | Parse a single CSV file |
| `-d path/to/folder/` | Parse all `.csv` files in a folder and combine results |
| `-o path/to/output.json` | Optional custom output path for valid flights JSON |
| `-j path/to/db.json` | Load existing JSON database instead of parsing CSVs |
| `-q path/to/query.json` | Execute queries defined in a JSON file on the loaded database |
| `-h` | Show help message |

---

## 🧾 Data Format

### Input CSV
```
flight_id,origin,destination,departure_datetime,arrival_datetime,price
```

### Validation Rules

| Field | Validation Rule |
|--------|----------------|
| `flight_id` | 2–8 alphanumeric characters |
| `origin`, `destination` | 3 uppercase letters |
| `departure_datetime`, `arrival_datetime` | valid `YYYY-MM-DD HH:MM` |
| — | `arrival_datetime` must be after `departure_datetime` |
| `price` | positive float number |

- Invalid rows → written to **errors.txt** with explanation  
- Valid rows → serialized to **db.json** as an array of objects

---

## ✈️ Sample `db.csv`

```csv
flight_id,origin,destination,departure_datetime,arrival_datetime,price
# === Valid flights ===
BA2490,LHR,JFK,2025-11-14 10:30,2025-11-14 13:05,489.99
LH172,FRA,RIX,2025-11-12 07:15,2025-11-12 10:30,159.50
FR1234,RIX,OSL,2025-11-15 08:00,2025-11-15 08:55,99.99
BT102,RIX,HEL,2025-11-14 09:40,2025-11-14 10:25,120.00
AA9999,JFK,LHR,2025-11-15 20:15,2025-11-16 08:10,550.00
DY4501,OSL,ARN,2025-12-01 06:00,2025-12-01 07:10,75.00
AF112,CDG,DXB,2025-11-20 21:10,2025-11-21 05:45,620.00

# === Invalid flights (for testing validation) ===
BADLINE,NO_DATE,NO_TIME
BA_BAD,RIX,LON,2025-11-15 11:00,INVALID_DATE,250.00
SK404,OSL,RIX,2025-11-15 14:00,2025-11-15 12:00,120.00
W61025,XXX,RIX,2025-11-16 11:00,2025-11-16 13:00,80.00
QR1,DOH,SYD,INVALID_DATETIME,2025-11-17 23:30,980.00
KL1999,AMS,,2025-11-14 09:00,2025-11-14 11:15,180.00
AY503,HEL,RIX,2025-11-15 13:20,2025-11-15 14:15,-10.00
LH999999999,FRA,LAX,2025-11-13 09:30,2025-11-13 18:10,700.00
SN2902,BRU,LHR,2025-13-40 10:00,2025-13-40 12:00,99.99
```
## 🧾 Validation Highlights

| Example | Validation Issue |
|----------|------------------|
| `BADLINE,NO_DATE,NO_TIME` | Missing fields |
| `BA_BAD,RIX,LON...` | Invalid destination (not 3 uppercase letters), invalid date |
| `SK404,OSL,RIX...` | Arrival before departure |
| `W61025,XXX,RIX...` | Invalid origin code (not real airport) |
| `QR1,DOH,SYD...` | Invalid datetime |
| `KL1999,AMS,,...` | Missing destination |
| `AY503,HEL,RIX...` | Negative price |
| `LH999999999,FRA,LAX...` | Flight ID too long (more than 8 characters) |
| `SN2902,BRU,LHR,2025-13-40...` | Invalid date format |

---

## 🧾 Sample `errors.txt` After Parsing `db.csv`

This file represents what your **flight parser** should generate after validating `db.csv`.

It includes:

- All invalid rows from the dataset  
- Comment lines like `# === Valid flights ===` and `# === Invalid flights... ===`  
- Human-readable explanations of what went wrong  
- Blank lines are ignored in processing (they don’t appear here)

### 🧾 Example `errors.txt`
```
Line 2: # === Valid flights === → comment line, ignored for data parsing
Line 10: # === Invalid flights (for testing validation) === → comment line, ignored for data parsing
Line 11: BADLINE,NO_DATE,NO_TIME → missing required fields
Line 12: BA_BAD,RIX,LON,2025-11-15 11:00,INVALID_DATE,250.00 → invalid destination code, invalid arrival datetime
Line 13: SK404,OSL,RIX,2025-11-15 14:00,2025-11-15 12:00,120.00 → arrival before departure
Line 14: W61025,XXX,RIX,2025-11-16 11:00,2025-11-16 13:00,80.00 → invalid origin code
Line 15: QR1,DOH,SYD,INVALID_DATETIME,2025-11-17 23:30,980.00 → invalid departure datetime
Line 16: KL1999,AMS,,2025-11-14 09:00,2025-11-14 11:15,180.00 → missing destination field
Line 17: AY503,HEL,RIX,2025-11-15 13:20,2025-11-15 14:15,-10.00 → negative price value
Line 18: LH999999999,FRA,LAX,2025-11-13 09:30,2025-11-13 18:10,700.00 → flight_id too long (more than 8 characters)
Line 19: SN2902,BRU,LHR,2025-13-40 10:00,2025-13-40 12:00,99.99 → invalid date format
```

---

## 🧩 Parsed Good JSON Database Example (`db.json`)

```json
[
  {
    "flight_id": "BA2490",
    "origin": "LHR",
    "destination": "JFK",
    "departure_datetime": "2025-11-14 10:30",
    "arrival_datetime": "2025-11-14 13:05",
    "price": 489.99
  },
  {
    "flight_id": "LH172",
    "origin": "FRA",
    "destination": "RIX",
    "departure_datetime": "2025-11-12 07:15",
    "arrival_datetime": "2025-11-12 10:30",
    "price": 159.50
  }
]
```
## 🔍 Query File Format (`query.json`) 
A query file can contain a single object or an array of query objects.  
Each query object can include any subset of the flight fields:  
```json
[
  { "origin": "LHR", "destination": "JFK" },
  { "price": 200.00 },
  { "departure_datetime": "2025-11-15 00:00", "arrival_datetime": "2025-11-16 00:00" }
]
```
## Filtering Logic

| **Field**              | **Filter behavior**                                             |
|-------------------------|----------------------------------------------------------------|
| `flight_id`, `origin`, `destination` | Exact match                                               |
| `departure_datetime`    | Include flights with `departure_datetime ≥` given value         |
| `arrival_datetime`      | Include flights with `arrival_datetime ≤` given value           |
| `price`                 | Include flights with `price ≤` given value    

---

## 🧠 Query Result File
The program must produce one **response JSON file** named:  
```
response_<studentid>_<name>_<lastname>_<YYYYMMDD_HHMM>.json
```

## File Structure
Always an array, even for a single query.  
Each element corresponds to one query:  

``` json
[
  {
    "query": { "origin": "LHR", "destination": "JFK" },
    "matches": [
      {
        "flight_id": "BA2490",
        "origin": "LHR",
        "destination": "JFK",
        "departure_datetime": "2025-11-14 10:30",
        "arrival_datetime": "2025-11-14 13:05",
        "price": 489.99
      }
    ]
  },
  {
    "query": { "price": 200.00 },
    "matches": [
      {
        "flight_id": "LH172",
        "origin": "FRA",
        "destination": "RIX",
        "departure_datetime": "2025-11-12 07:15",
        "arrival_datetime": "2025-11-12 10:30",
        "price": 159.50
      },
      {
        "flight_id": "FR1234",
        "origin": "RIX",
        "destination": "OSL",
        "departure_datetime": "2025-11-15 08:00",
        "arrival_datetime": "2025-11-15 08:55",
        "price": 99.99
      }
    ]
  }
]
```
---

## 🎯 Grading Rubric

| **Grade** | **Requirements** | **Core Concepts** |
|------------|------------------|-------------------|
| **4** | Read and print CSV content. | file I/O, loops |
| **5** | Basic field splitting, minimal validation. | strings, branching |
| **6** | Full validation rules, valid vs invalid separation, output to files. | comprehensions, dicts |
| **7** | JSON output for valid flights. | json module |
| **8** | CLI arguments `-i`, `-d`, `-o`. | argparse, functions |
| **9** | Folder parsing, combined output, robust error handling. | modular design |
| **10** | Adds `-j` (load JSON), `-q` (query JSON), datetime filtering, query response serialization. | OOP / functional design, datetime, filtering logic |

🕓 **Late Penalties**  
- 1–7 days late → **−1 Grade**  
- 8–14 days late → **−2 Grades**  

---

# ⚙️ Example Commands

### Parse CSV and export
```bash
python flight_parser.py -i data/db.csv
```
Parse multiple CSVs in folder
```
python flight_parser.py -d data/flights/
```
Use existing JSON database
```
python flight_parser.py -j data/db.json
```
Run queries from file
```
python flight_parser.py -j data/db.json -q data/query.json
 ```
**Notes: If you do not have your own Python in your own computer, you can use Github Codespaces (Python 3.13.9) or any other virtual environment that you can show to me running.**    
**Libraries: If you use extra libraries a requirements.txt file should be present. The teacher WILL NOT BE installing any libraries! :)**    
**Resources: If you are not familiar with JSON and its usage in Python, the following post is a good introduction: Working With JSON Data in Python – Real Python**  
