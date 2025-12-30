import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Wrong number of args: .csv .txt")
        sys.exit(1)
    try:
        database: str = sys.argv[1]
        sequence: str = sys.argv[2]
    except ValueError:
        print("Must enter strings")
        sys.exit(2)

    # TODO: Read database file into a variable
    rows = []
    with open(database) as db:
        database_reader = csv.DictReader(db)
        for row in database_reader:
            rows.append(row)
    # TODO: Read DNA sequence file into a variable
    dna = ""
    with open(sequence) as sq:
        dna = sq.read().strip()

    # TODO: Find longest match of each STR in DNA sequence
    longest = {}
    name_column = list(rows[0].keys())[0]
    str_substrings = list(rows[0].keys())[1:]#get all the substrings/column names except name
    for str in str_substrings:
        longest[str] = longest_match(dna, str)

    # TODO: Check database for matching profiles
    for row in rows:
        is_valid = True
        for i, (key, value) in enumerate(row.items()):
            if i == 0:#skip name
                continue
            if longest[key] != int(row[key]):#doesn't match
                is_valid = False
                break
        if is_valid:#all values match
            print(row[name_column])
            sys.exit(0)
    print("No match")
    sys.exit(0)

def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
