from pickle import UNICODE
import re


def date_parser(text: str) -> str:
    # matches '17/02/2009' and '17/2/2009' and '17/ 2/2009' and ' 2/02/2009'
    match = re.search(
        pattern=r"(?<!\d)(\d{2}| ?\d{1})\/(\d{2}| ?\d{1})\/(\d{4})",
        string=text,
        flags=re.UNICODE|re.IGNORECASE
    )
    if match:
        groups = match.groups()
        day = groups[0].strip()
        month = groups[1].strip()
        year = groups[2]
        if len(month) == 1:
            month = "0" + month
        if len(day) == 1:
            day = "0" + day
        return "-".join([day, month, year])
    # matches '2009/02/17' and '2009/ 2/17'
    match = re.search(
        pattern=r"(\d{4})\/(\d{2}| ?\d{1})\/(\d{2}| ?\d{1})",
        string=text,
        flags=re.UNICODE|re.IGNORECASE
    )
    if match:
        groups = match.groups()
        year = groups[0]
        month = groups[1].strip()
        day = groups[2].strip()
        if len(month) == 1:
            month = "0" + month
        if len(day) == 1:
            day = "0" + day
        return "-".join([day, month, year])
    # second regex matches strings 
    month_dict = {
        "Jan": "01",
        "Feb": "02",
        "Mar": "03",
        "Apr": "04",
        "May": "05",
        "Jun": "06",
        "Jul": "07",
        "Aug": "08",
        "Sep": "09",
        "Oct": "10",
        "Nov": "11",
        "Dec": "12",
        "January": "01",
        "February": "02",
        "March": "03",
        "April": "04",
        "June": "06",
        "July": "07",
        "August": "08",
        "September": "09",
        "October": "10",
        "November": "11",
        "December": "12"
    }
    # matches 17Feb2009 and 17 Feb, 2009 and 17 February, 2009
    match = re.search(
        pattern=r"(?<!\d)(\d{2}| ?\d{1})\D{0,2}"
                r"(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec|"
                r"January|February|March|April|June|July|August|September|October|November|December)\D{0,2}"
                r"(\d{4})",
        string=text,
        flags=re.UNICODE|re.IGNORECASE
    )
    if match:
        groups = match.groups()
        day = groups[0].strip()
        month_sting = groups[1].capitalize()
        month = month_dict[month_sting]
        year = groups[2]
        if len(day) == 1:
            day = "0" + day
        return "-".join([day, month, year])
    # matches February 17, 2009 and Feb172009 and Feb 17, 2009
    match = re.search(
        pattern=r"(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec|"
                r"January|February|March|April|June|July|August|September|October|November|December)\D{0,2}"
                r"(\d{2}| ?\d{1})\D{0,2}"
                r"(\d{4})",
        string=text,
        flags=re.UNICODE|re.IGNORECASE
    )
    if match:
        groups = match.groups()
        month_sting = groups[0].capitalize()
        month = month_dict[month_sting]
        day = groups[1].strip()
        year = groups[2]
        if len(day) == 1:
            day = "0" + day
        return "-".join([day, month, year])
    # matches 2009, February 17 and 2009, Feb 17
    match = re.search(
        pattern=r"(\d{4})\D{0,2}"
                r"(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec|"
                r"January|February|March|April|June|July|August|September|October|November|December)\D{0,2}"
                r"(\d{2}| ?\d{1})",
        string=text,
        flags=re.UNICODE|re.IGNORECASE
    )
    if match:
        groups = match.groups()
        year = groups[0]
        month_sting = groups[1].capitalize()
        month = month_dict[month_sting]
        day = groups[2].strip()
        if len(day) == 1:
            day = "0" + day
        return "-".join([day, month, year])
    return None


def main():
    """
        Find date pattern and parse dates in unified format dd-mm-yyyy
    """

    # Ref: https://docs.oracle.com/cd/E41183_01/DR/Date_Format_Types.html

    dates = [
        "17/02/2009",            # DD/MM/YYYY        
        "2009/02/17",            # YYYY/MM/DD
        "February 17, 2009",     # Month D, YYYY
        "17/2/2009",             # D/M/YYYY
        "17/ 2/2009",            # bD/bM/YYYY
        "2009/ 2/17",            # YYYY/bM/bD
        "Feb172009",             # MonDDYYYY
        "17Feb2009",             # DDMonYYYY
        "17 February, 2009",     # D Month, YYYY
        "2009, February 17",     # YYYY, Month D
        "Feb 17, 2009",          # Mon DD, YYYY
        "17 Feb, 2009",          # DD Mon, YYYY
        "2009, Feb 17",          # YYYY, Mon DD
    ]

    for date in dates:
        date_parsed = date_parser(date)
        print(f"Date: {date} | Parsed Date: {date_parsed}")

if __name__ == "__main__":
    main()