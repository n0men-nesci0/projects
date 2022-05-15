import sys
f = open("input.txt")
sys.stdout = open("output.txt", 'w')
for line in f:
    lst = [word[:-1] for word in line.split()]
    for word in lst:
        print("case " + word + ":")
        print("\tstr_lex = " + '"' + word[4:] + '"' + ";")
        print("\tbreak;")
    