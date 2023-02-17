import csv
import sys
from cs50 import SQL

''''
This program reads a csv file and uses the data to populate an SQL database of Hogwarts students and their corresponding houses.
'''

def main():
    # Open database
    db = SQL("sqlite:///roster.db")

    if len(sys.argv) != 2:  # Ensure correct commandline arguments are provided.
        sys.exit("Usage: python prophecy.py data.csv")

    #  Read csv file and create a list of dictionaries, where each dictionary contains data on a single person.
    #  Each person has an id, student_name (e.g. Draco Lucius Malfoy), house (e.g. Slytherin), and head (e.g. Severus Snape, the head of Slytherin).
    people = []
    with open(sys.argv[1]) as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            people.append(row)

    #  Populate the students table.
    for person in people:
        db.execute('INSERT INTO students (student_name) VALUES (?)', person['student_name'])

    # Create a dictionary of four key-value pairs, where the key is the Hogwarts house and the value is the head of the house.
    houses = {}
    counter = 0
    while len(houses) < 4:
        houses[people[counter]['house']] = people[counter]['head']
        counter += 1

    #  Populate the houses table.
    for key, value in houses.items():
        db.execute('INSERT INTO houses (house, head) VALUES (?, ?)', key, value)

    #  Populate the assignments table.
    for person in people:
        #  The db.execute command below returns a list of dictionaries, where each dictionary contains the id of a student_name that matches the query
        student_name = person['student_name']
        student_id = db.execute('SELECT id FROM students WHERE student_name = ?', student_name)[0]['id']

        house = person['house']
        house_id = db.execute('SELECT id FROM houses WHERE house = ?', house)[0]['id']

        db.execute('INSERT INTO assignments (student_id, house_id) VALUES (?, ?)', student_id, house_id)


main()