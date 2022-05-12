from textwrap import fill
from flask import Flask
from matplotlib.pyplot import table
from flask import request
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor
import time
import numpy as np

from table import Table

app = Flask(__name__)

@app.route("/", methods=['GET'])
def table_controller():
  
    print("Request received.")
    return str(handle_fill_table_request(request.json))

def fill_table_cell(cell_fill_data, table):
    
    x = cell_fill_data[0]
    y = cell_fill_data[1]
    value = cell_fill_data[2]
    table[x][y] = value


def handle_fill_table_request(fill_data):

    w = fill_data['width']
    h = fill_data['height']
    default = fill_data['default']
    workers = fill_data['workers']

    table = list()
    for i in range(w):
        row = list()
        for j in range(h):
            row.append(default)
        table.append(row)


    print("empty table created ")

    start_concurrent = time.time()
    table_concurrent = fill_table_concurrently(fill_data['fill_data'], table.copy(), workers)
    end_concurrent = time.time()
    time_concurrent = end_concurrent - start_concurrent

    start_sync = time.time()
    table_sync = fill_table_single_thread(fill_data['fill_data'], table)
    end_sync = time.time()
    time_sync = end_sync - start_sync

    print("Time with concurrency: ", time_concurrent)
    print("Time with single thread: ", time_sync)

    return table

    
        
def fill_table_concurrently(fill_data, table, workers):

    split_data = np.array_split(fill_data, workers)
    print("start submit")
    with ThreadPoolExecutor(max_workers=workers) as executor:
        #futures = [executor.submit(fill_table_cell, d, table) for d in fill_data]
        for i, data in enumerate(split_data):
            executor.submit(fill_table_single_thread, data, table, i)

    print("end submit")
    return table

def fill_table_multiprocess(fill_data, table, workers):


    split_data = np.array_split(fill_data, workers)
    print("start submit")
    with ProcessPoolExecutor(max_workers=workers) as executor:
        #futures = [executor.submit(fill_table_cell, d, table) for d in fill_data]
        for i, data in enumerate(split_data):
            executor.submit(fill_table_single_thread, data, table, i)

    print("end submit")
    return table
        
    
    return table

def fill_table_single_thread(fill_data, table, id=1):
    
    print("fill data len", len(fill_data))
    for fd in fill_data: fill_table_cell(fd, table); #print("pooo", id)
    return table
