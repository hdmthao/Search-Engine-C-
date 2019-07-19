import os
import sys
import re
import random
import base64
import hashlib

def RemoveMark(s):
    d = ",.!?&-:;@..."
    return ' '.join(w for w in re.split("["+"\\".join(d)+"]", s) if w)


def GetTitle(file_to_read, file_to_write):
    fi = open(file_to_read, "r")
    fo = open(file_to_write, "a+")
    if fi.mode == "r":
        try:
            title = fi.readline()
            title = RemoveMark(title)
            title = title.strip().lower()
            if len(title) > 100 or len(title) == 0:
                fi.close()
                fo.close()
                return
            rank = random.randint(1, 10000)
            time = random.randint(615950394, 616871000)
            print(file_to_read + ":" + title)
            fo.write(title + "," + str(rank) + "," + str(time) +  "," + "\n")
        except UnicodeDecodeError:
            # print(file_to_read)
            fi.close()
            fo.close()
            return
    fi.close()
    fo.close()


def DetectUnicode(file_to_read, file_to_write):
    fi = open(file_to_read, "r")
    if fi.mode == 'r':
        fo = open(file_to_write, "a+")
        try:
            contents = fi.read()
            for w in contents:
                if ord(w) > 255:
                    fo.writelines(w + file_to_read[5:-4] + "\n")
                    break
        except UnicodeDecodeError:
            fo.writelines(file_to_read[5:] + "\n")
    fi.close()
    fo.close()


def GetHash(file_to_read, file_to_write):
    fo = open(file_to_write, "a+")
    hash_object = hashlib.md5(file_to_read[5:-4].encode())
    # print(hash_object.hexdigest())
    fo.writelines(hash_object.hexdigest() + ".txt\n")
    path = "clean/" + hash_object.hexdigest() + ".txt" 
    os.rename(file_to_read, path)
    fo.close()


if __name__ == "__main__":
    option = -1
    if str(sys.argv[1]) == "--detect-unicode":
        option = 1
    elif str(sys.argv[1]) == "--fake-history":
        option = 2
    elif str(sys.argv[1]) == "--hash-file-name":
        option = 3
    else:
        option = 0
        print("Pls give the option")
    if option == 0:
        print("Nothing to do")
    else:
        file_to_write = str(sys.argv[2])
        if os.path.isfile(file_to_write):
            os.remove(str(sys.argv[2]))
        print("Write to " + file_to_write)
        count = 0
        count_wrong_format = 0

        for file in os.listdir("data"):
            if file.lower().startswith("group") == False:
                count_wrong_format += 1
            if file.endswith(".txt"):
                count += 1
                file_to_read = "data/" + file
                if option == 1:
                    DetectUnicode(file_to_read, file_to_write)
                elif option == 2:
                    GetTitle(file_to_read, file_to_write)
                elif option == 3:
                    GetHash(file_to_read, file_to_write)
        print("Wrong name format (Group_XXX): " + str(count_wrong_format))
