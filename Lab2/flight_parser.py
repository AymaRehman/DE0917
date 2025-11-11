import csv, os, re, json, argparse
from datetime import datetime

# --- Validation Functions ---
def validate_flight(row):
    flight_id, origin, destination, dep_dt, arr_dt, price = row

    # basic format checks
    if not (2 <= len(flight_id) <= 8 and flight_id.isalnum()):
        return False, "Invalid flight ID"
    # ^ = start, $ = end, [A-Z]{3} = exactly 3 uppercase letters
    if not re.match(r"^[A-Z]{3}$", origin):
        return False, "Invalid origin code"
    if not re.match(r"^[A-Z]{3}$", destination):
        return False, "Invalid destination code"

    # datetime validation
    try:
        dep = datetime.strptime(dep_dt, "%Y-%m-%d %H:%M")
        arr = datetime.strptime(arr_dt, "%Y-%m-%d %H:%M")
        if arr <= dep:
            return False, "Arrival before departure"
    except ValueError:
        return False, "Invalid datetime format"

    # price check
    try:
        if float(price) <= 0:
            return False, "Price must be positive"
    except ValueError:
        return False, "Invalid price"

    return True, None

# --- CSV Parsing ---
def parse_csv(file_path):
    valid, invalid = [], []
    try:
        with open(file_path, newline="") as f:
            reader = csv.reader(f)
            headers = next(reader, None)  # Skip the first header line (if present)

            for i, row in enumerate(reader, start=2):  # start=2 to account for header line
                if not row or row[0].startswith("#"):
                    continue
                if row == headers:
                    continue  # skip repeated header lines
                if len(row) < 6:
                    invalid.append((i, row, "Less number of columns than expected"))
                elif len(row) > 6:
                    invalid.append((i, row, "More number of columns than expected"))
                else:  # when len(row) == 6
                    ok, reason = validate_flight(row)
                    if ok:
                        valid.append({
                            "flight_id": row[0],
                            "origin": row[1],
                            "destination": row[2],
                            "departure_datetime": row[3],
                            "arrival_datetime": row[4],
                            "price": float(row[5])
                        })
                    else:
                        invalid.append((i, row, reason))
    except FileNotFoundError:
        print(f"⚠️ File not found: {file_path}")
    except Exception as e:
        print(f"⚠️ Error reading {file_path}: {e}")
    return valid, invalid

# --- JSON Save / Load ---
def save_json(data, path):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        json.dump(data, f, indent=2)

def save_errors(errors, path):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        for line_no, row, reason in errors:
            f.write(f"Line {line_no}: {','.join(row)} → {reason}\n")

def load_json(path):
    with open(path, "r") as f:
        return json.load(f)

# --- Directory Parsing ---
def process_directory(directory):
    all_valid, all_invalid = [], []
    for filename in os.listdir(directory):
        if filename.endswith(".csv"):
            file_path = os.path.join(directory, filename)
            print(f"Processing: {filename}")
            valid, invalid = parse_csv(file_path)
            all_valid.extend(valid)
            all_invalid.extend([(f"{filename}:{ln}", row, reason) for ln, row, reason in invalid])
    return all_valid, all_invalid

# --- Query Filtering ---
def matches_query(flight, query):
    for field, value in query.items():
        if field in ["flight_id", "origin", "destination"]:
            if flight.get(field) != value:
                return False
        elif field == "departure_datetime":
            if datetime.strptime(flight[field], "%Y-%m-%d %H:%M") < datetime.strptime(value, "%Y-%m-%d %H:%M"):
                return False
        elif field == "arrival_datetime":
            if datetime.strptime(flight[field], "%Y-%m-%d %H:%M") > datetime.strptime(value, "%Y-%m-%d %H:%M"):
                return False
        elif field == "price":
            if flight[field] > value:
                return False
    return True

def run_queries(flights, query_file):
    queries = load_json(query_file)
    if not isinstance(queries, list):
        queries = [queries]
    responses = []
    for q in queries:
        matches = [f for f in flights if matches_query(f, q)]
        responses.append({"query": q, "matches": matches})
    return responses

# --- Paths for Lab2
BASE_DIR = os.path.dirname(os.path.abspath(__file__)) 
OUTPUT_JSON = os.path.join(BASE_DIR, "data/db.json")
ERRORS_TXT = os.path.join(BASE_DIR, "data/errors.txt")
RESPONSE_JSON = os.path.join(BASE_DIR, "data/response.json")

# --- Main ---
def main():
    parser = argparse.ArgumentParser(description="Flight Data Parser and Query Tool")
    parser.add_argument("-i", "--input", metavar = "CSV_FILE", help="Parse a single CSV file. Format: -i path/to/file.csv")
    parser.add_argument("-d", "--directory", metavar = "CSV_FOLDER", help="Parse a folder containing multiple CSV files. Format: -d path/to/folder")
    parser.add_argument("-o", "--output", metavar = "OUTPUT_JSON", default=OUTPUT_JSON, help="Output JSON database path. Default: Lab2/data/db.json; Format: -o path/to/output.json")
    parser.add_argument("-e", "--errors", metavar = "ERROR_LOG", default=ERRORS_TXT, help="Output errors log path. Default: Lab2/data/errors.txt; Format: -e path/to/errors.txt")
    parser.add_argument("-j", "--json", metavar = "JSON_DB", help="Load existing JSON database rather than parsing CSVs. Format: -j path/to/file.json")
    parser.add_argument("-q", "--query", metavar = "QUERY_JSON", help="Respond to queries provided in JSON query file. Format: -q path/to/query.json")
    parser.add_argument("-r", "--response", metavar = "RESPONSE_JSON", default=RESPONSE_JSON, help="Output responses to queries in JSON file. Default: Lab2/data/response.json; Format: -r path/to/response.json")
    args = parser.parse_args()

    # Load or parse flights
    if args.json:
        flights = load_json(args.json)
        invalid = []
    elif args.directory:
        flights, invalid = process_directory(args.directory)
    elif args.input:
        flights, invalid = parse_csv(args.input)
    else:
        print("❌ Please provide either -i <file> or -d <directory>, or -j <json>")
        return

    save_json(flights, args.output)
    save_errors(invalid, args.errors)
    print(f"✅ Saved {len(flights)} valid flights and {len(invalid)} errors.")

    # Run queries if requested
    if args.query:
        responses = run_queries(flights, args.query)
        if args.response:
            save_json(responses, args.response)
        else:
            save_json(responses, "Lab2/data/response.json")
        print(f"✅ Query executed. {len(responses)} responses saved.")

if __name__ == "__main__":
    main()