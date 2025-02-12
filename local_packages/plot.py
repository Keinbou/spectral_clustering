from matplotlib import pyplot as plt
from pandas import DataFrame
from local_packages.exceptions import *

def inspect_dataset(df:DataFrame) -> None:
    mandatory_columns = ['x','y','target']
    if any(mandatory_column not in df.columns for mandatory_column in mandatory_columns):
        raise AbsentColumnException("The input DataFrame must have 'x', 'y' and 'target' columns.")
    x = df["x"]
    y = df["y"]
    c = df["target"]
    plt.scatter(x,y, c=c)
    plt.show()


"""
def playKNN(fig, axis, x, y, all_dots):
    for w in range(read_rows):
        some_dots = []
        for i, dot in enumerate(l[w]):
            if dot==1:
                some_dots.append(all_dots[i])
        plot(fig, axis, x, y, all_dots, some_dots, w)
"""