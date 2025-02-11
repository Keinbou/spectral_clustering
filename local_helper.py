from sklearn.cluster import DBSCAN, Birch, AffinityPropagation, MeanShift, OPTICS
from sklearn.mixture import GaussianMixture
from numpy import unique
from numpy import where
import matplotlib as mpl
import matplotlib.pyplot as plt

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

def lp_lp_float_c_to_list_2d(lp_p, size):
    list = []
    for i in range(size):
        list.append(lp_p[i][:size])
    return list

def lp_float_c_to_list_2d(lp, size):
    return lp[:size]

#def plot(fig, axis, x, y, title):#, x, y, all_dots, some_dots, w:int):
    #x1 = [dot[0] for dot in some_dots]
    #y1 = [dot[1] for dot in some_dots]
    #axis[0,0].cla()
    #axis[0,0].set_title(title)
    #axis[0,0].set_xlim([-15, 20])
    #axis[0,0].set_ylim([-15, 20])
    #axis[0,0].scatter(x, y, c=l, cmap=jet)
    #axis[0,0].scatter(x1, y1, c='black')
    #axis[0,0].scatter(all_dots[w][0], all_dots[w][1], c='r')

    #display(fig)
    #clear_output(wait=True)
    #plt.pause(1)
    #plt.show()

"""
def playKNN(fig, axis, x, y, all_dots):
    for w in range(read_rows):
        some_dots = []
        for i, dot in enumerate(l[w]):
            if dot==1:
                some_dots.append(all_dots[i])
        plot(fig, axis, x, y, all_dots, some_dots, w)
"""

def k_clusters(l):
    return len(unique(l)) + (-1 if -1 in l else 0)

def dbscan(all_dots, eps, min_samples):
    # initialize the data set we'll work with
    training_data = [[dot[0], dot[1]] for dot in all_dots]

    # define the model
    dbscan_model = DBSCAN(eps=eps, min_samples=min_samples)

    # train the model
    dbscan_model.fit(training_data)
    
    return dbscan_model.labels_

def gaussian(all_dots, k):
    # initialize the data set we'll work with
    training_data = [[dot[0], dot[1]] for dot in all_dots]

    # define the model
    gaussian_model = GaussianMixture(n_components=k)

    # train the model
    gaussian_model.fit(training_data)

    return gaussian_model.predict(training_data)

def birch(all_dots, threshold, n):
    # initialize the data set we'll work with
    training_data = [[dot[0], dot[1]] for dot in all_dots]

    # define the model
    birch_model = Birch(threshold=threshold, n_clusters=n)

    # train the model
    birch_model.fit(training_data)

    return birch_model.predict(training_data)

def affinity(all_dots, damping):
    # initialize the data set we'll work with
    training_data = [[dot[0], dot[1]] for dot in all_dots]

    # define the model
    model = AffinityPropagation(damping=damping)

    # train the model
    model.fit(training_data)

    return model.predict(training_data)

def mean(all_dots):
    # initialize the data set we'll work with
    training_data = [[dot[0], dot[1]] for dot in all_dots]

    # define the model
    mean_model = MeanShift()

    return mean_model.fit_predict(training_data)

def optics(all_dots, eps, min_samples):
    # initialize the data set we'll work with
    training_data = [[dot[0], dot[1]] for dot in all_dots]

    # define the model
    optics_model = OPTICS(eps=eps, min_samples=min_samples)

    return optics_model.fit_predict(training_data)


def setupMPL(filename, cls, cmap):
    font = {'family' : 'sans-serif',
            'weight' : 'bold',
            'size'   : 22}

    mpl.rc('font', **font)

    fig, axis = plt.subplots(4, 2, figsize=(20,40))
    fig.subplots_adjust(top=.94, hspace=.2, wspace=.2) #, bottom=0, left=0, right=1, wspace=.15, hspace=.15)
    fig.suptitle(f"{filename} Clustering", y=.98)

    # classes = cls
    # bounds  = np.arange(classes[0]-.5, classes[-1]+1.5, 1)
    # norm    = mpl.colors.BoundaryNorm(bounds, cmap.N)
    # map     = mpl.cm.ScalarMappable(norm=norm, cmap=cmap)
    # fig.colorbar(map, ax=axis, ticks=classes, orientation='horizontal', pad=0.05)
    return fig, axis


# PLOTTING FUNCTIONS FOR EACH CLUSTERING ALGORITHM

def plotSPECTRAL(fig, axis, i,j, x,y, l, cmap, title):
    axis[i,j].cla()
    axis[i,j].set_title(title)
    axis[i,j].scatter(x, y, c=l, cmap=cmap)


def plotDBSCAN(fig, axis, i,j, x,y, l, training_data, title):
    axis[i,j].cla()
    axis[i,j].set_title(title)

    dbscan_clusters = unique(l)
    # plot the DBSCAN clusters
    for dbscan_cluster in dbscan_clusters:
        # get data points that fall in this cluster
        index = where( l==dbscan_cluster )
        c = "black" if dbscan_cluster==-1 else None
        # make the plot
        x = []
        y = []
        for g in index[0]:
            x.append(training_data[g][0])
            y.append(training_data[g][1])
        axis[i,j].scatter(x, y, c=c, cmap=None)


def plotGaussian(fig, axis, i,j, x,y, l, training_data, title):
    axis[i,j].cla()
    axis[i,j].set_title(title)

    gaussian_clusters = unique(l)
    for gaussian_cluster in gaussian_clusters:
        index = where( l==gaussian_cluster )
        # make the plot
        x = []
        y = []
        for g in index[0]:
            x.append(training_data[g][0])
            y.append(training_data[g][1])
        axis[i,j].scatter(x, y, cmap=None)


def plotKMEANS(fig, axis, i,j, x,y, l, cmap, title):
    axis[i,j].cla()
    axis[i,j].set_title(title)
    axis[i,j].scatter(x, y, c=l, cmap=cmap)


def plotBIRCH(fig, axis, i,j, x,y, l, training_data, title):
    axis[i,j].cla()
    axis[i,j].set_title(title)
    
    birch_clusters = unique(l)
    for birch_cluster in birch_clusters:
        index = where( l==birch_cluster )
        # make the plot
        x = []
        y = []
        for g in index[0]:
            x.append(training_data[g][0])
            y.append(training_data[g][1])
        axis[i,j].scatter(x, y, cmap=None)


def plotAffinity(fig, axis, i,j, x,y, l, training_data, title):
    axis[i,j].cla()
    axis[i,j].set_title(title)
    
    affinity_clusters = unique(l)
    for affinity_cluster in affinity_clusters:
        index = where( l==affinity_cluster )
        c = "black" if affinity_cluster==-1 else None
        # make the plot
        x = []
        y = []
        for g in index[0]:
            x.append(training_data[g][0])
            y.append(training_data[g][1])
        axis[i,j].scatter(x, y, c=c, cmap=None)


def plotMEAN(fig, axis, i,j, x,y, l, training_data, title):
    axis[i,j].cla()
    axis[i,j].set_title(title)
    
    mean_clusters = unique(l)
    for mean_cluster in mean_clusters:
        index = where( l==mean_cluster )
        c = "black" if mean_cluster==-1 else None
        # make the plot
        x = []
        y = []
        for g in index[0]:
            x.append(training_data[g][0])
            y.append(training_data[g][1])
        axis[i,j].scatter(x, y, c=c, cmap=None)


def plotOPTICS(fig, axis, i,j, x,y, l, training_data, title):
    axis[i,j].cla()
    axis[i,j].set_title(title)

    optics_clusters = unique(l)
    # plot the DBSCAN clusters
    for optics_cluster in optics_clusters:
        # get data points that fall in this cluster
        index = where( l==optics_cluster )
        c = "black" if optics_cluster==-1 else None
        # make the plot
        x = []
        y = []
        for g in index[0]:
            x.append(training_data[g][0])
            y.append(training_data[g][1])
        axis[i,j].scatter(x, y,c=c, cmap=None)