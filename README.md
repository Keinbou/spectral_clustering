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

&emsp;C  
&emsp;&emsp;- **this2this** - converting between datatypes.  
&emsp;&emsp;- **catlog** - shorthand printing functions. Can be perfectly substituted with built-in output functions.  
&emsp;&emsp;- **file** - file reading/observing package.  
&emsp;&emsp;- **matrix** - matrix operations. All functions except matrices as VLAs (variable-length arrays).  
&emsp;&emsp;- **spectral** - entry point for .c code. Reads dataset, performs clustering and returns list with assigned cluster to each dot, preserving order.  

&emsp;*Available clustering algorihtms implementations*: Spectral (doesn't work), K-Means.  

&emsp;Python  
&emsp;&emsp;- **c2py** - bridge between .c and .py code.  
&emsp;&emsp;- **file** - file reading.  
&emsp;&emsp;- **plot** - plotting datapoints with Matplotlib.  
&emsp;&emsp;- **widgets** - Jupyter Notebook widgets initialization.  
&emsp;&emsp;- **clustering_algorithms** - functions to perform various clustering algorithms from Scikit-learn package and to plot 'em.  
&emsp;&emsp;- **exceptions** - locally-defined exceptions.  
    
&emsp;*Available clustering algorihtms*: DBSCAN, Gaussian Mixture Model, BIRCH, Affinity Propagation, Mean-Shift, OPTICS.  