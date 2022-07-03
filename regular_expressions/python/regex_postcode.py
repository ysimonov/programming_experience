import re

def main():
    addresses = [
        "88234 Brenda Port",
        "Ayalamouth, IA 06203",
        "5458 Juan Creek Suite 495",
        "Lake Michaelchester, NC 22553",
        "6210 Williams Parkways Apt. 824",
        "North Dennis, TX 45145",
        "06110 Miller Groves Suite 612",
        "Billyville, MI 89905",
        "76306 Kevin Lodge Suite 360",
        "Port Lisa, SD 72558",
        "PSC 5119, Box 4487",
        "APO AP 32080",
        "011 Duran Coves Apt. 747",
        "Port James, MO 23796",
        "8159 Weaver Prairie",
        "Garciaport, AL 48283",
        "327 Stephanie Villages",
        "Lake Alexandrafort, AL 23019",
        "42938 Curtis Stream Suite 394",
        "Sarabury, IA 68131",
        "42573 Vicki Courts",
        "Andrewmouth, FL 31248",
        "111 Martinez Tunnel Suite 986",
        "Debraside, WY 40530",
        "3172 Tina Inlet",
        "West Josephburgh, TX 13584",
        "Unit 5126 Box 5891",
        "DPO AE 77792",
        "48643 Carolyn Causeway",
        "Robertsonbury, NJ 38196",
        "Unit 2108 Box 9092",
        "DPO AA 72168",
        "3624 Ryan Neck",
        "North Michaelmouth, MD 08622",
        "PSC 8872, Box 5212",
        "APO AP 15034",
        "USNS Peterson",
        "FPO AE 55871",
        "5203 Wise Mills",
        "Mooreview, VA 49035"
    ]

    regex_address = re.compile(
        pattern=r"^.{7,}(?<!\d)(\d{3,6})",
        flags=re.UNICODE | re.IGNORECASE
    )

    for address in addresses:
        match = regex_address.search(address)
        if match:
            match_groups = match.groups()
            postcode = match_groups[0]
            print(f"Address: {address} | Match: {postcode}")

if __name__ == "__main__":
    main()