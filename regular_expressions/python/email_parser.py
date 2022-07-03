import re

def main():
    addresses = """ <title> This is a random title for random email lisa94@example.com - Email</title>
            <body> This is a random body for another random email luis67@example.net </body>",
            <body> To: mcfarlanddavid@example.org </body>
            <body> From: allenlaura@example.org </body>
            <body> CC: sarahpotter@example.net, jennifer39@example.com, carolyn61@example.org </body>"""
    
    
    regex_emails_simple = re.compile(
        pattern=r"[\._a-zA-Z0-9]{1,}\@[\._a-zA-Z0-9]{1,}\.(?:org|net|com)",
        flags=re.UNICODE
    )

    matches = regex_emails_simple.findall(addresses)
    for match in matches:
        print(match)

if __name__ == "__main__":
    main()