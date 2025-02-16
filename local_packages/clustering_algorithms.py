from sklearn.cluster import DBSCAN, Birch, AffinityPropagation, MeanShift, OPTICS
from sklearn.mixture import GaussianMixture
from numpy import unique, where
from matplotlib import pyplot as plt
import matplotlib as mpl

COLS = 4; ROWS = 2


def setupMPL(filename, cols, rows, cls, cmap):
    font = {'family' : 'sans-serif',
            'weight' : 'bold',
            'size'   : 22}
    mpl.rc('font', **font)


    width  = rows*10
    length = cols*10
    fig, axis = plt.subplots(COLS, ROWS, figsize=(width,length))
    fig.subplots_adjust(top=.94, hspace=.2, wspace=.2) #, bottom=0, left=0, right=1, wspace=.15, hspace=.15)
    fig.suptitle(f"{filename}", y=.98)
    if 'moon' in filename:
        fig.text(s=f"looks more like bananas tho.", x=0.5, y=.965, fontsize='x-small', fontweight='light')

    # classes = cls
    # bounds  = np.arange(classes[0]-.5, classes[-1]+1.5, 1)
    # norm    = mpl.colors.BoundaryNorm(bounds, cmap.N)
    # map     = mpl.cm.ScalarMappable(norm=norm, cmap=cmap)
    # fig.colorbar(map, ax=axis, ticks=classes, orientation='horizontal', pad=0.05)
    return fig, axis


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


def gaussian(all_dots, k):
    # initialize the data set we'll work with
    training_data = [[dot[0], dot[1]] for dot in all_dots]

    # define the model
    gaussian_model = GaussianMixture(n_components=k)

    # train the model
    gaussian_model.fit(training_data)

    return gaussian_model.predict(training_data)

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


def birch(all_dots, threshold, n):
    # initialize the data set we'll work with
    training_data = [[dot[0], dot[1]] for dot in all_dots]

    # define the model
    birch_model = Birch(threshold=threshold, n_clusters=n)

    # train the model
    birch_model.fit(training_data)

    return birch_model.predict(training_data)

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


def affinity(all_dots, damping):
    # initialize the data set we'll work with
    training_data = [[dot[0], dot[1]] for dot in all_dots]

    # define the model
    model = AffinityPropagation(damping=damping)

    # train the model
    model.fit(training_data)

    return model.predict(training_data)

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


def mean(all_dots):
    # initialize the data set we'll work with
    training_data = [[dot[0], dot[1]] for dot in all_dots]

    # define the model
    mean_model = MeanShift()

    return mean_model.fit_predict(training_data)

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


def optics(all_dots, eps, min_samples):
    # initialize the data set we'll work with
    training_data = [[dot[0], dot[1]] for dot in all_dots]

    # define the model
    optics_model = OPTICS(eps=eps, min_samples=min_samples)

    return optics_model.fit_predict(training_data)

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