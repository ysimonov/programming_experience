import re
from faker import Faker

def main():
    faker = Faker()

    for i in range(200):
        print(faker.email())

if __name__ == "__main__":
    main()