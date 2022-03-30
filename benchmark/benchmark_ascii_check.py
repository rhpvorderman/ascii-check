import sys 

from ascii import ascii_check 


def main():
    with open(sys.argv[1], "rb") as file:
        while True:
            block = file.read(128 * 1024)
            if block == b"":
                return 
            if not ascii_check(block):
                raise ValueError("File is not ASCII")
    

if __name__ == "__main__":
    main()
