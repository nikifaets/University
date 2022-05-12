import requests
import json
import random

def request_generator(w = 100, h = 100, workers = 5, default = 5):
    
    req = {
        "width" : w,
        "height" : h,
        "workers" : workers,
        "default" : default,
        "fill_data" : []}

    for i in range(w):
        for j in range(h):
            value = random.randint(1, 5000000)
            req["fill_data"].append([i, j, value])

    with open('generated_request.json', 'w') as f:
        json.dump(req, f)
    return req

if __name__ == '__main__':
    
    with open('request.json') as f:
        req = json.load(f)
    print(req)

    r = requests.get('http://localhost:5000/', json=request_generator(w = 1000, h = 1000, workers=5))
    print(r.json)
    print(len(r.content))

