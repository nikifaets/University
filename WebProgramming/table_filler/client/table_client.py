import requests
import json
if __name__ == '__main__':
    
    with open('request.json') as f:
        req = json.load(f)
    print(req)

    r = requests.get('http://localhost:5000/', json=req)