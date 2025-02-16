from ctypes import *
import matplotlib as mpl
import matplotlib.pyplot as plt


SO_FILE_PATH = "./c/c_code.so"


def lp_lp_float_c_to_list_2d(lp_p, n_rows, n_cols):
    list_2d = []
    for i in range(n_rows):
        list_2d.append(lp_p[i][:n_cols])
    return list_2d


def list_2d_to_lp_lp_float(list_2d, n_rows, n_cols):
    floatPointerArray = (POINTER(c_float)*n_rows)
    floatArray        = (c_float*n_cols)
    float_p_arr = floatPointerArray()
    for i in range(n_rows):
        float_arr = floatArray()
        for j in range(n_cols):
            float_arr[j] = list_2d[i][j]
        float_p = cast(float_arr, POINTER(c_float))
        float_p_arr[i] = float_p
    float_pp = cast(float_p_arr, POINTER(POINTER(c_float)))
    return float_pp


def lp_float_c_to_list_1d(lp, len):
    return lp[:len]


class C():
    '''
    Function list:
                    spectral_embedding(int, int, char*, int, int);
                    assign_clusters(int, int, float**, int);
                    kmeans(int, int, char*, int, int);
                    free_floatP(float*);
                    free_floatPP(float**);
    '''
    def __init__(self, so_file):
        self.so_file = so_file
        self.c_code  = CDLL(so_file)

        self.c_code.spectral_embedding.argstype = [c_int, c_int, POINTER(c_char), c_int, c_int]
        self.c_code.spectral_embedding.restype  = POINTER(POINTER(c_float))

        self.c_code.assign_clusters.argtypes = [c_int, c_int, POINTER(POINTER(c_float)), c_int]
        self.c_code.assign_clusters.restype  = POINTER(c_float)

        self.c_code.kmeans.argstype = [c_int, c_int, POINTER(c_char), c_size_t]
        self.c_code.kmeans.restype  = POINTER(c_float)

        self.c_code.free_floatP.argtypes = [POINTER(c_float)]
        self.c_code.free_floatP.restype  = None

        self.c_code.free_floatPP.argtypes = [POINTER(POINTER(c_float))]
        self.c_code.free_floatPP.restype  = None

    def spectral_embedding(self, data_len:int, read_rows:int, path:str, knn:int, k_clusters:int) -> list[list[float]]:
        path = path.encode('utf-8')
        out = self.c_code.spectral_embedding(data_len, read_rows, path, knn, k_clusters)
        spectral_embedding = lp_lp_float_c_to_list_2d(out, n_rows=read_rows, n_cols=read_rows)
        self.c_code.free_floatPP(out)
        return spectral_embedding
    
    def assign_clusters(self, n_rows:int, n_cols:int, spectral_embedding, k_clusters:int) -> list[list[float]]:
        out = self.c_code.assign_clusters(n_rows, n_cols, spectral_embedding, k_clusters)
        clustered_list = lp_float_c_to_list_1d(out, len=n_rows)
        self.c_code.free_floatP(out)
        return clustered_list
    
    def kmeans(self, data_len:int, read_rows:int, path:str, k_clusters:int) -> list[float]:
        path = path.encode('utf-8')
        out = self.c_code.kmeans(data_len, read_rows, path, k_clusters)
        m = lp_float_c_to_list_1d(out, len=read_rows)
        self.c_code.free_floatP(out)
        return m


def plot_embedding(spectral_embedding, projection='3d'):
    font = {'family' : 'sans-serif',
            'weight' : 'light',
            'size'   : 10}
    mpl.rc('font', **font)


    fig = plt.figure(figsize=(8,7))
    labelpad = 15
    size     = 15
    x = spectral_embedding[0]
    y = spectral_embedding[1]

    if projection=='3d':
        z = spectral_embedding[2]
        dim = '3D'

        ax = fig.add_subplot(projection='3d')
        ax.set_box_aspect(None, zoom=0.85)
        ax.scatter(x,y,z, marker='^', s=70)
        ax.set_xlabel("X", fontdict={"weight":"bold", 'size':size}, labelpad=labelpad)
        ax.set_ylabel("Y", fontdict={"weight":"bold", 'size':size}, labelpad=labelpad)
        ax.set_zlabel("Z", fontdict={"weight":"bold", 'size':size}, labelpad=labelpad/2)
    else:
        dim = '2D'

        ax = fig.add_subplot()
        fig.subplots_adjust(left=0.1, top=0.75, right=0.75, bottom=0.1)
        ax.scatter(x,y, marker='^', s=70)
        ax.set_xlabel("X", fontdict={"weight":"bold", 'size':size}, labelpad=labelpad)
        ax.set_ylabel("Y", fontdict={"weight":"bold", 'size':size}, labelpad=labelpad)
        
    fig.suptitle(f"Spectral Embedding in {dim}", fontsize=30, y=0.85, x=0.45)
    plt.show()


def plotSPECTRAL(fig, axis, i,j, x,y, l, cmap, title):
    axis[i,j].cla()
    axis[i,j].set_title(title)
    axis[i,j].scatter(x, y, c=l, cmap=cmap)


def plotKMEANS(fig, axis, i,j, x,y, l, cmap, title):
    axis[i,j].cla()
    axis[i,j].set_title(title)
    axis[i,j].scatter(x, y, c=l, cmap=cmap)