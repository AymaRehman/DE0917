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


