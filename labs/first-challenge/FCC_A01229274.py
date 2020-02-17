import sys

def max_substring(word):
    temp = ""
    max = [0, ""]
    for i in range(0, len(word)):
        if word[i] not in temp:
            temp += word[i]
        else:
            if len(temp) > max[0]:
                max[0] = len(temp)
                max[1] = temp
                temp = word[i]
    if len(temp) > max[0]:
        max[0] = len(temp)
        max[1] = temp
    return max[0]


result = max_substring(sys.argv[1])
print(f"Max substring length: {result}")


