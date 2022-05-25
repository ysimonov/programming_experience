from levenshtein import similarity_ratio

def test():
    while 1:
        str1 = input("Enter first string: ")
        str2 = input("Enter second string: ")
        ratio = similarity_ratio(str1, str2)
        print(f"Similarity ratio of\n\t{str1}\nand\n\t{str2}\nis {ratio}")
        response = input("Would you like to continue? Enter 'q' to exit or any button to continue: ")
        if response.lower() == "q":
            break

if __name__ == "__main__":
    test()