def process_dots(l:list[str]) -> list[tuple[float]]:
    n = []
    for row in l:
        for i, el in enumerate(row):
            row[i] = float(el)
        n.append(tuple(row))
    return n


def readCsv(filepath:str, n:int) -> list[tuple[float]]:
    dots = []
    with open(filepath, "r") as file:
        for i, row in enumerate(file.readlines()):
            if i == 0:
                continue
            if i>n:
                break
            dots.append(row.split(","))
    return process_dots(dots)


def fileDepth(filepath:str|None = None) -> int:
    with open(filepath, "r") as file:
        all_rows = file.readlines()
    return len(all_rows)-1 # skip the first row


def fileWidth(filepath:str|None = None) -> int:
    with open(filepath, "r") as file:
        row = file.readline()
    return len(row.split(","))