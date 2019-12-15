def parse():
    f = open("race_set.txt", "r")
    race_set = set()

    while True:
        line = f.readline()
        if not line: break
        pos = line.find(' ')

        line1 = int(line[:pos])
        line2 = int(line[pos + 1:])

        race_set.add((min(line1, line2), max(line1, line2)))

    return len(race_set)


print(str(parse()))
