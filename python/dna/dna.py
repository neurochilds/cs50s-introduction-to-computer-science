import csv
import sys

'''
This program identifies to whom a sequence of DNA belongs.
'''


def main():
    if len(sys.argv) != 3:  # Ensure correct commandline arguments are provided
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Read csv file into a list. Each element in the list is a dictionary representing a person.
    # The keys represent the words on the first line of the file, i.e., the headings.
    # The values for each dictionary/person represent the data in each row of the csv file.
    # The 'name' key is set to the persons name, while all remaining keys are DNA repeat sequences.
    # The values associated with the DNA repeat sequences denote the maximum consecutive repeats of that sequence in the persons dna.
    people = []
    with open(sys.argv[1]) as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            people.append(row)

    with open(sys.argv[2]) as txtfile:
        mystery_dna = txtfile.read()

    list_of_dna_repeats = list(people[0].keys())[1:]  # Add all dictionary keys to a list, excluding the 'name' key

    longest_repeat = find_longest_repeat(mystery_dna, list_of_dna_repeats)

    mystery_person = 'No match'
    for person in people:
        match = True
        for repeat in list_of_dna_repeats:
            if int(person[repeat]) != longest_repeat[repeat]:
                match = False
                break
        if match:
            mystery_person = person['name']

    print(mystery_person)


def find_longest_repeat(sequence, list_of_subsequences):
    """Returns dicionary of subsequence keys and the longest run of repeats of that subsequence as values."""
    repeat_counter = {}
    count = 0
    len_sequence = len(sequence)

    for subsequence in list_of_subsequences:
        if subsequence in sequence:  # If subsequence in sequence, count how many consecutive repeats there
            len_subsequence = len(subsequence)
            max_count = 0
            index = -1

            for i in range(len_sequence - len_subsequence):
                index += 1

                if index > (len_sequence - len_subsequence - 1):  # This prevents us indexing beyond the length of the sequence
                    repeat_counter[subsequence] = max_count
                    break

                if sequence[index: (index + len_subsequence)] == subsequence:
                    count += 1
                    index += len_subsequence - 1  # Move the index along to the end of the current subsequence

                else:  # If a repeat is not found at this index, update max_count if necessary and then reset count to 0
                    if count > max_count:
                        max_count = count
                    count = 0
        else:
            repeat_counter[subsequence] = 0

    return repeat_counter


main()