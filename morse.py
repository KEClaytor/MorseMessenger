# Decode morse code characters
# dit is represented with 1
# dah with a 3
# Can the sum uniquely specify the character?

chars = {
        'A': [1, 3, 0, 0, 0, 0],
        'B': [3, 1, 1, 1, 0, 0],
        'C': [3, 1, 3, 1, 0, 0],
        'D': [3, 1, 1, 0, 0, 0],
        'E': [1, 0, 0, 0, 0, 0],
        'F': [1, 1, 3, 1, 0, 0],
        'G': [3, 3, 1, 0, 0, 0],
        'H': [1, 1, 1, 1, 0, 0],
        'I': [1, 1, 0, 0, 0, 0],
        'J': [1, 3, 3, 3, 0, 0],
        'K': [3, 1, 3, 0, 0, 0],
        'L': [1, 3, 1, 1, 0, 0],
        'M': [3, 3, 0, 0, 0, 0],
        'N': [3, 1, 0, 0, 0, 0],
        'O': [3, 3, 3, 0, 0, 0],
        'P': [1, 3, 3, 1, 0, 0],
        'Q': [3, 3, 1, 3, 0, 0],
        'R': [1, 3, 1, 0, 0, 0],
        'S': [1, 1, 1, 0, 0, 0],
        'T': [3, 0, 0, 0, 0, 0],
        'U': [1, 1, 3, 0, 0, 0],
        'V': [1, 1, 1, 3, 0, 0],
        'W': [1, 3, 3, 0, 0, 0],
        'X': [3, 1, 1, 3, 0, 0],
        'Y': [3, 1, 3, 3, 0, 0],
        'Z': [3, 3, 1, 1, 0, 0],
        '1': [1, 3, 3, 3, 3, 0],
        '2': [1, 1, 3, 3, 3, 0],
        '3': [1, 1, 1, 3, 3, 0],
        '4': [1, 1, 1, 1, 3, 0],
        '5': [1, 1, 1, 1, 1, 0],
        '6': [3, 1, 1, 1, 1, 0],
        '7': [3, 3, 1, 1, 1, 0],
        '8': [3, 3, 3, 1, 1, 0],
        '9': [3, 3, 3, 3, 1, 0],
        '0': [3, 3, 3, 3, 3, 0],
        }

lens = []
totals = []
for key in chars:
    total = 0
    clen = 0
    for i in range(6):
        print("I: {0}, E: {1} :: {2}^{3}= {4}".format(
            i, chars[key][i], chars[key][i]+1, i+1, (chars[key][i]+1)**(i+1) ))
        total += (chars[key][i]+1)**(i+1)
        if chars[key][i]:
            clen += 1
    totals.append(total)
    lens.append(clen)
    #print("{0}:{2}-{1}".format(key, total, clen))
    print("case {0}: mc = '{1}'; break;".format(total, key))

print(len(totals))
print(len(set(totals)))
#for i in range(len(totals)):
    #print("{1}-{0}".format(totals[i], lens[i]))
