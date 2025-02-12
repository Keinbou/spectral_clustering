# spectral_clustering
An implementation and comparison of Spectral Clustering with other algorithms. (WIP)

The code is intended to be experienced through **main.ipynb** - a Jupyter Notebook file.
All other **Python code** is stored in `./local_packages` folder.
All **.c code** in stored in `./c` folder.

The main file main.ipynb uses .so file to run .c code.
To build the **./c/c_code.so** file, run:

>gcc -c spectral.c
>gcc *.o -dynamiclib -o c_code.so

while being in the ./c/ folder.

***

## Packages description:

    C
        - **this2this** - converting between datatypes.
        - **catlog** - shorthand printing functions. Can be perfectly substituted with built-in output functions.
        - **file** - file reading/observing package.
        - **matrix** - matrix operations. All functions except matrices as VLAs (variable-length arrays).
        - **spectral** - entry point for .c code. Reads dataset, performs clustering and returns list with assigned cluster to each dot, preserving order.

    *Available clustering algorihtms implementations*: Spectral (doesn't work), K-Means.

    Python
        - **c2py** - bridge between .c and .py code.
        - **file** - file reading.
        - **plot** - plotting datapoints with Matplotlib.
        - **widgets** - Jupyter Notebook widgets initialization.
        - **clustering_algorithms** - functions to perform various clustering algorithms from Scikit-learn package and to plot 'em.
        - **exceptions** - locally-defined exceptions.
    
    *Available clustering algorihtms*: DBSCAN, Gaussian Mixture Model, BIRCH, Affinity Propagation, Mean-Shift, OPTICS.