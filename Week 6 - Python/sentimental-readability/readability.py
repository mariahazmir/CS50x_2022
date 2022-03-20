# TODO

# Main function
def main():
    # Get text
    text = input("Text: ")
    # Declare variables
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)
    # Calculate Coleman-Liau index
    L = 100 * (letters / words)
    S = 100 * (sentences / words)
    index = round(0.0588 * L - 0.296 * S - 15.8)
    # Print grade
    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


# Count number of letters in text
def count_letters(text):
    # Set counter
    countlet = 0
    for c in text:
        # If character in text is an alphabet
        if (c.isalpha()) == True:
            # Add 1 to counter
            countlet += 1
    return countlet


# Count number of words in text
def count_words(text):
    # Set counter
    countwor = 1
    for c in text:
        space = 32
        # If character in text is a space
        if c == chr(space):
            # Add 1 to counter
            countwor += 1
    return countwor


# Count number of sentences in text
def count_sentences(text):
    # Set counter
    countsen = 0
    for c in text:
        exclam = 33
        period = 46
        quest = 63
        # If character in text is an exclamation mark, question mark or period
        if c == chr(exclam) or c == chr(period) or c == chr(quest):
            # Add 1 to counter
            countsen += 1
    return countsen


main()