from pathlib import Path
import re 
import io
import numpy as np
import nlu
import string

data = Path('../data')
stojanov_zapiski_split_pattern = re.compile('(Глава [VIX]+\.) | (Том първи) | (Том втори) | (Том трети)')

def read_stopwords(path):
    
    with open(path) as f:
        stopwords = f.read().splitlines()

    return stopwords

def read_word_embeddings(path):

    fin = io.open(path, 'r', encoding='utf-8', newline='\n', errors='ignore')
    n, d = map(int, fin.readline().split())
    data = {}
    for line in fin:
        tokens = line.rstrip().split(' ')
        data[tokens[0]] = list(map(float, tokens[1:]))
        
    return data

def split_text_to_chapters(path, delimiter):
    
    with open(path, encoding='utf-8-sig') as f:
        rich_text = f.read().replace('\t', ' ')

    split_text = re.split(delimiter, rich_text)
    counter = 0
    for chapter in split_text:
        
        if chapter == None: continue
        
        with open(path.parent / str(counter), 'w') as f:
            f.write(chapter)
        counter += 1

def lemmatize_array(corpus):
    lemmatizer = nlu.load('bg.lemma')
    lemmatized_corpus = []
    for text in corpus:
        print('text')
        print(text)
        lemmatized_corpus.append(lemmatize(text, lemmatizer))

    print("lemmatized")
    print(lemmatized_corpus)
    return lemmatized_corpus
    
def lemmatize(text, lemmatizer):
    
    lemmatized = lemmatizer.predict(text)
    return [token for token in lemmatized.lem.tolist() if str(token) != 'nan']

def remove_stopwords(text, stopwords):
    
    #Text is expected to be lemmatized
    print("remove stopwords")
    return [word for word in text if word.lower() not in stopwords]

def remove_punctuation(text):
    return [word for word in text if word not in string.punctuation and word != '—']


def run_analyze_pipeline(corpus):

    lemmatized = lemmatize_array(corpus)
    print("Lemmatization finished.")
    without_stop_words = [remove_stopwords(text, read_stopwords(data / 'stopwords.txt')) for text in lemmatized]
    without_punctuation = [remove_punctuation(text) for text in without_stop_words]
    return without_punctuation


def run_analyze_pipeline_for_text(text):
    
    lemmatizer = nlu.load('bg.lemma')

    lemmatized = lemmatize(text, lemmatizer)
    without_stop_words = remove_stopwords(lemmatized, read_stopwords(data / 'stopwords.txt'))
    without_punctuation = remove_punctuation(without_stop_words)

    return ' '.join(without_punctuation)


def get_words_set(corpus):
    words = set()
    for text in corpus:
        for word in text:
            words.add(word)
    return words
            

def build_count_matrix(corpus, words = None):
    words_set = words if words != None else sorted(list(get_words_set(corpus)))

    words_count_per_text = []

    for text in corpus:
        words_count = {}
        for word in text:
            if word in words_count:
                words_count[word] += 1
            else:
                words_count[word] = 1
        words_count_per_text.append(words_count)
    
    count_matrix = []
    for i in range(len(corpus)):
        matrix_row = []
        for word_idx, word in enumerate(words_set):
            matrix_row.append(
                words_count_per_text[i][word] 
                if word in words_count_per_text[i] 
                else 0)
        count_matrix.append(matrix_row)

    return count_matrix
    

if __name__ == '__main__':
    

    pass
