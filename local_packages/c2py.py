from ctypes import *


SO_FILE_PATH = "./c/c_code.so"


def lp_lp_float_c_to_list_2d(lp_p, size):
    list = []
    for i in range(size):
        list.append(lp_p[i][:size])
    return list


def lp_float_c_to_list_2d(lp, size):
    return lp[:size]


class C():
    def __init__(self, so_file):
        self.so_file = so_file
        self.c_code  = CDLL(so_file)

        # spectral() function
        self.c_code.spectral.argstype = [c_int, c_int, POINTER(c_char), c_int, c_int]
        self.c_code.spectral.restype  = POINTER(c_float)

        # kmeans() function
        self.c_code.kmeans.argstype = [c_int, c_int, POINTER(c_char), c_size_t]
        self.c_code.kmeans.restype  = POINTER(c_float)

        # free_floatPP() function
        self.c_code.free_floatP.argtypes = [POINTER(c_float)]
        self.c_code.free_floatP.restype  = None

    def spectral(self, data_len:int, read_rows:int, path:str, knn:int, k_clusters:int) -> list[float]:
        path = path.encode('utf-8')
        out = self.c_code.spectral(data_len, read_rows, path, knn, k_clusters)
        l = lp_float_c_to_list_2d(out, read_rows)
        self.c_code.free_floatP(out)
        return l
    
    def kmeans(self, data_len:int, read_rows:int, path:str, k_clusters:int) -> list[float]:
        path = path.encode('utf-8')
        out = self.c_code.kmeans(data_len, read_rows, path, k_clusters)
        m = lp_float_c_to_list_2d(out, read_rows)
        self.c_code.free_floatP(out)
        return m


def plotSPECTRAL(fig, axis, i,j, x,y, l, cmap, title):
    axis[i,j].cla()
    axis[i,j].set_title(title)
    axis[i,j].scatter(x, y, c=l, cmap=cmap)


def plotKMEANS(fig, axis, i,j, x,y, l, cmap, title):
    axis[i,j].cla()
    axis[i,j].set_title(title)
    axis[i,j].scatter(x, y, c=l, cmap=cmap)