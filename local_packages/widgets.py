import ipywidgets as widgets
from glob import glob

FOLDER_PATH = "datasets/"
files = glob(f'{FOLDER_PATH}*')
names = [file.split('/')[-1] for file in files]


datasets_dropdown = widgets.Dropdown (
                    options = names,
                    value = 'three_clouds.csv', # Default value
                    description = 'Dataset:',
                    disabled = False,
                    layout = {'width': 'fit-content'},
                )

knn_slider = widgets.IntSlider(
                value=10,
                min=0,
                max=30,
                step=1,
                description='kNN:',
                disabled=False,
                continuous_update=False,
                orientation='horizontal',
                readout=True,
                readout_format='d'
            )

clusters_slider = widgets.IntSlider(
                    value=3,
                    min=0,
                    max=10,
                    step=1,
                    description='Clusters:',
                    disabled=False,
                    continuous_update=False,
                    orientation='horizontal',
                    readout=True,
                    readout_format='d'
                )


read_rows_button = widgets.ToggleButton(
                        value=True,
                        description='All',
                        disabled=False,
                        button_style='',
                        tooltip='Read all data rows.',
                        icon='check',
                    )

read_rows_text = widgets.IntText(
                    value=-1,
                    description='Rows to read:',
                    disabled=False,
                    style = {'description_width':'initial'},
                )


data_len_button = widgets.ToggleButton(
                    value=True,
                    description='All',
                    disabled=False,
                    button_style='',
                    tooltip='Reads all data columns.',
                    icon='check',
                )

data_len_text = widgets.IntText(
                    value=-1,
                    description='Data Length:',
                    disabled=False,
                    style = {'description_width':'initial'},
                )


box_layout = widgets.Layout(display='flex',
                            justify_content='space-around',
                            margin='10px')
row1 = widgets.Box([datasets_dropdown, knn_slider, clusters_slider],
                   layout = box_layout)
v1 = widgets.Box([read_rows_button, read_rows_text],
                 layout = widgets.Layout(margin='10px'))
v2 = widgets.Box([data_len_button, data_len_text],
                 layout = widgets.Layout(margin='10px'))
row2 = widgets.Box([v1,v2],
                   layout = box_layout)
GUI = widgets.VBox([row1, row2], layout=widgets.Layout(margin='20px'))

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'