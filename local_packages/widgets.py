import ipywidgets as widgets
from glob import glob

FOLDER_PATH = "datasets/"
files = glob(f'{FOLDER_PATH}*')
names = [file.split('/')[-1] for file in files]

datasets_dropdown = widgets.Dropdown (
                options = names,
                value = names[0],
                description = 'Dataset:',
                disabled = False,
                layout = {'width': 'fit-content'},
            )