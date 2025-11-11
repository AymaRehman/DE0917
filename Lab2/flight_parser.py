import csv, os, re, json, argparse
from datetime import datetime

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

def parse_csv(file_path):
    valid, invalid = [], []
    with open(file_path, newline="") as f:
        reader = csv.reader(f)
        for i, row in enumerate(reader, start=1):
            if not row or row[0].startswith("#"):
                continue
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
        return valid, invalid

def save_json(data, path):
    with open(path, "w") as f:
        json.dump(data, f, indent=2)

def save_errors(errors, path):
    with open(path, "w") as f:
        for line_no, row, reason in errors:
            f.write(f"Line {line_no}: {','.join(row)} → {reason}\n")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", help="CSV input file")
    parser.add_argument("-o", "--output", default="data/db.json")
    parser.add_argument("-e", "--errors", default="data/errors.txt")
    args = parser.parse_args()

    valid, invalid = parse_csv(args.input)
    save_json(valid, args.output)
    save_errors(invalid, args.errors)

if __name__ == "__main__":
    main()