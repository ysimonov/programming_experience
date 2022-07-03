import re
import regex

def replace_with_upper_case(match):
    return " " + match.group(1).upper()

def replace_with_lower_case(match):
    return match.group(1).lower()

def replace_with_lower_case_with_space(match):
    return " " + match.group(1).lower()


def main():
    """
        Task: Remove all characters, except comma, dot letters and digits
        Sentences can contain unicode characters!
        Semicolon is allowed in the middle of the string and should not be removed, 
        but should be removed if it is located at the end or at the start of the string. 
        If the word following dot is lower case, it should be converted to capital case. 
        If the there is no punctuation before capitalized word, 
        it should be converted to lower case.
        All capitalized words should be split by space.
        Sentences must not start with digits!
    """

    dirty_strings = [
        " $ 2today is aGoodWeather_:",
        "%$who, told$ that @~RegexIsDifficult$.this is Brilliant!",
        "This     String:     Has Too      Many        Spaces!!!!!!!!!!!",
        "I want to SeparateDigits:FromWords90And10Insure1000Words Are Spaced.",
        "@@@@---你??今天过的怎么样",
        "元______   気 -----  で == == = = す????  か"
    ]

    cleaning_regex_start = re.compile(
        pattern=r"^[\W\d]{1,}",
        flags=re.UNICODE|re.IGNORECASE
    )

    cleaning_regex_end = re.compile(
        pattern=r"[\W\d]{1,}$",
        flags=re.UNICODE|re.IGNORECASE
    )

    cleaning_regex_middle = re.compile(
        pattern=r"[^\w ,\.\:]|_",
        flags=re.UNICODE|re.IGNORECASE
    )

    regex_consecutive_spaces = re.compile(
        pattern=r" {2,}"
    )

    # \p{Ll} lower case unicode and \p{Lu} upper case unicode
    regex_unicode_sequence = regex.compile(
        pattern=r"((?<=\p{Ll})\p{Lu})",
        flags=re.UNICODE
    )

    regex_letter_digit = re.compile(
        pattern=r"(\d+|[\D]+)",
        flags=re.UNICODE
    )

    # loop over results
    for dirty_string in dirty_strings:
        # remove noise characters at the start of the string
        dirty_string = cleaning_regex_start.sub('', dirty_string)
        # remove noise characters at the end of the string
        dirty_string = cleaning_regex_end.sub('', dirty_string)
        # remove noise characters in the middle of the string
        dirty_string = cleaning_regex_middle.sub('', dirty_string)
        # insert space between lower and upper cases
        dirty_string = regex_unicode_sequence.sub(replace_with_upper_case, dirty_string)
        # split letters from digits
        print(regex_letter_digit.findall(dirty_string))
        dirty_string = " ".join(regex_letter_digit.findall(dirty_string))
        # convert words without punctuation to lower case
        dirty_string = re.sub(r"((?<!\.) ?\w)", replace_with_lower_case, dirty_string)
        # split at the location of semicolon or comma and convert to lower case
        dirty_string = re.sub(r"((?<=(?:\:|,)) ?\w)", replace_with_lower_case_with_space, dirty_string)
        # split at the location of dot and convert to upper case
        dirty_string = re.sub(r"((?<=\.) ?\w)", replace_with_upper_case, dirty_string)
        # strip spaces at the start and at the end of the string, and remove consecutive spaces
        dirty_string = regex_consecutive_spaces.sub(" ", dirty_string).strip()
        # capitalize first letter
        dirty_string = dirty_string[0].upper() + dirty_string[1:]
        print(dirty_string)

if __name__ == "__main__":
    main()