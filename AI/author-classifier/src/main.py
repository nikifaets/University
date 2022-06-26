from logging.config import stopListening
from pathlib import Path

if __name__ == '__main__':
    data = Path('../data')
    stopwords = data / 'stopwords.txt'

    print(stopwords)