import sys
import crypt
import string

salt = str(50)
alphabet = string.ascii_letters

#function to hash all 1 digit alphabetical characters, and compare to the passed in hash
def cracker1(hash):
    for i in range(len(alphabet)):
        password = '%s' % (alphabet[i])
        test_hash = crypt.crypt(password, salt)
        if test_hash in hash:
            print(password)
            sys.exit()

#function to hash all 2 digit alphabetical characters, and compare to the passed in hash
def cracker2(hash):
    for i in range(len(alphabet)):
        for j in range(len(alphabet)):
            password = '%s%s' % (alphabet[i], alphabet[j])
            test_hash = crypt.crypt(password, salt)
            if test_hash in hash:
                print(password)
                sys.exit()

#function to hash all 3 digit alphabetical characters, and compare to the passed in hash
def cracker3(hash):
    for i in range(len(alphabet)):
        for j in range(len(alphabet)):
            for k in range(len(alphabet)):
                password = '%s%s%s' % (alphabet[i], alphabet[j], alphabet[k])
                test_hash = crypt.crypt(password, salt)
                if test_hash in hash:
                    print(password)
                    sys.exit()

#function to hash all 4 digit alphabetical characters, and compare to the passed in hash
def cracker4(hash):
    for i in range(len(alphabet)):
         for j in range(len(alphabet)):
            for k in range(len(alphabet)):
                for l in range(len(alphabet)):
                    password = '%s%s%s%s' % (alphabet[i], alphabet[j], alphabet[k], alphabet[l])
                    test_hash = crypt.crypt(password, salt)
                    if test_hash in hash:
                        print(password)
                        sys.exit()

def main():
    #check theres only 1 command line argument
    if len(sys.argv) != 2:
        sys.exit("Usage: ./crack hash")

    #check the command line argument is 13 characters long
    if len(sys.argv[1]) != 13:
        sys.exit("Usage: ./crack hash")

    hash = sys.argv[1]

    #run the exclusive crackers in turn, exiting when one of them finds the password
    cracker1(hash)
    cracker2(hash)
    cracker3(hash)
    cracker4(hash)

    #if none of the crackers find the password, print result
    sys.exit("Password not found. Was the input a real DES hash?")

if __name__ == "__main__":
    main()