from flask import Flask
from matplotlib.pyplot import table
from flask import request
from concurrent.futures import ThreadPoolExecutor

from table import Table

app = Flask(__name__)

@app.route("/", methods=['GET'])
def table_controller():
    print(request.method)
    print(request.json)
    fill_table(request.json)
    return "200"

def fill_table_cell(cell_fill_data, table):
    
    print("call")
    x = cell_fill_data[0]
    y = cell_fill_data[1]
    value = cell_fill_data[2]
    table[x][y] = value

    return None

def fill_table(fill_data):

    w = fill_data['width']
    h = fill_data['height']
    default = fill_data['default']

    table = list()
    for i in range(h):
        row = list()
        for j in range(w):
            row.append(default)
        table.append(row)

    with ThreadPoolExecutor(max_workers=3) as executor:
        res = executor.map(lambda d: fill_table_cell(d, table))
        futures = [executor.submit(fill_table_cell, d, table) for d in fill_data['fill_data']]

    #print(futures.result)
    #print(list(map(lambda d: fill_table_cell(d, table), fill_data['fill_data'])))
    print(table)
        
