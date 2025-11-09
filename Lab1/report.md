# Test Report — Pythonic Calculator (Ayma Rehman — 241ADB165)

**Repository:** `DE0917_Lab1`
**File under test:** `calc.c`
**Author:** Ayma Rehman

---

## Summary

* **Total tests observed:** 20
* **Passed:** 10
* **Partial / Warning:** 3
* **Failed:** 7

**Main issues identified:**

* Comments beginning with `#` are not ignored (treated as invalid characters).
* The code is not designed to handle floating-point numbers yet, (decimal points are treated as invalid characters).
* Only single parentheses handling therefore when parentheses are adjacent to operators, it throws error ("double operator" errors, and unmatched/bracket-counting issues).
---

## Detailed test results

### Error test files (expected to throw errors)

| Test     | Status | Details                                                                                                                     |
| -------- | :----: | ------------------------------------------------------------------------------------------------------------------------- |
| Error 1  |    ✅   | Passes as expected.                                                                                                   |
| Error 2  |    ✅   | Passes as expected.                                                                                                   |
| Error 3  |    ❌   | Fails — the program computes an incomplete expression instead of throwing an error.                                          |
| Error 4  |    ✅   | Passes as expected.                                                                                                   |
| Error 5 | ⚠️ | Partial pass — the correct error is displayed, but the column number for the zero division error (where 0 appears at column 5) is not shown. |
| Error 6  |    ✅   | Passes, but with a positional mismatch: my implementation counts characters per line (starting from 1), whereas the expected output counts total characters across all lines. Therefore, my reported error column is 6 instead of 10. |
| Error 7  |    ❌   | Fails — the code produces a result even though a closing bracket is missing.                                            |
| Error 8  |    ❌   | Fails — the program throws an incorrect error due to not handling multiple parentheses correctly. When a multiplication operator appears next to a parenthesis, it triggers a “double operator” error                                  |
| Error 9  |    ❌   | Fails — same issue as Error 8.                                                                                           |
| Error 10 |    ✅   | Passes, but the reported column differs. My program identifies the double operator error at column 4, while the expected output indicates column 6. The difference arises from how operator positioning is counted.|

### Valid test files (should process and return results)

| Test    |  Status  | Details                                                                                                                                                                                                                           |
| ------- | :------: | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Test 1  |     ✅    | Passes as expected.                                                                                                                                                                                                                              |
| Test 2  |     ✅    | Passes as expected.                                                                                                                                                                                                                              |
| Test 3  |     ✅    | Passes as expected.                                                                                                                                                                                                                              |
| Test 4  |     ❌    | Fails — my code does not support floating-point numbers (no decimal point handling), causing an “invalid character” error.                                                                                                                                                             |
| Test 5  |     ✅    | Passes as expected.                                                                                                                                                                                                                              |
| Test 6  |     ✅    | Passes as expected.                                                                                                                                                                                                                              |
| Test 7  |     ❌    | Fails — “double operator” error occurs when parentheses appear adjacent to *. The code does not currently handle cases where parentheses follow or precede a non-numerical value.                                                                                                                                                                  |
| Test 8  |     ❌    | Fails — same issue as Test 7.                                                                                                                                                                                                                 |
| Test 9  | ⚠️ Mixed | Line 1: ❌ Comment line (#) not ignored — treated as invalid character, causing an error.; Line 2: ❌ Parentheses and operator interaction not handled properly.; Line 3: ❌ Same issue as Line 1 (comment not ignored).; Line 4: ✅ Processed correctly.; |                                                                      |
| Test 10 | ⚠️ Mixed | Line 1: ❌ Comment line (#) not ignored — treated as invalid character, causing an error.; Line 2: ✅ Correctly handles the double operator error, even though the expected output computes a result.; Line 3: ❌ Same issue as Line 1 (comment not ignored).; Line 4: ❌ Fails — floating-point values not supported, resulting in “invalid character” error. |
---



*End of report.*
