import string


def main():
    text = input("Text: ")
    words = text.split()  # Use whitespaces to split string into a list of words

    n_words = len(words)
    n_letters = count_letters(words)
    n_sentences = count_sentences(text)
    grade = get_grade(n_words, n_letters, n_sentences)

    if grade > 0 and grade < 16:
        print(f'Grade {grade}')
    elif grade > 16:
        print('Grade 16+')
    else:
        print('Before Grade 1')


def count_letters(text):
    n_words = len(text)
    for i in range(n_words):
        text[i] = text[i].translate(str.maketrans('', '', string.punctuation))  # Strip punctutation from each word

    n_letters = 0
    for i in range(n_words):
        n_letters += len(text[i])  # Count total number of letters in text

    return n_letters


def count_sentences(text):
    sentence_endings = ['.', '!', '?']  # Any occurence of '.', '!', or '?' will denote the end of a sentence
    n_sentences = 0

    for char in text:
        if char in sentence_endings:
            n_sentences += 1

    return n_sentences


def get_grade(n_words, n_letters, n_sentences):
    '''Use the Coleman-Liau formula to calculate the Coleman-Liau index round it to get the reading grade'''
    letters_per_100_words = n_letters/n_words * 100
    sentences_per_100_words = n_sentences/n_words * 100

    index = 0.0588 * letters_per_100_words - 0.296 * sentences_per_100_words - 15.8
    grade = round(index)

    return grade


if __name__ == "__main__":
    main()