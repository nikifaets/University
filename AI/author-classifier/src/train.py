from pathlib import Path
import preprocess
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.naive_bayes import MultinomialNB
import pickle

class Model:
    
    def __init__(self, model, words):
        self.model = model
        self.words = words

data = Path('../data')

# Zahari Stojanov
train_s_files = [
    Path(data / 'stojanov/hristo_botiov/4'),
    Path(data / 'stojanov/prevratat/3'),
    Path(data / 'stojanov/zapiski/1' ),
    Path(data / 'stojanov/zapiski/10'),
    Path(data / 'stojanov/zapiski/15'),
    Path(data / 'stojanov/zapiski/16'),
    Path(data / 'stojanov/zapiski/17'),
    Path(data / 'stojanov/prevratat/4')
]
test_s_files = [
    Path(data / 'stojanov/hristo_botiov/2'),
    Path(data / 'stojanov/prevratat/2'),
    Path(data / 'stojanov/zapiski/2' ),
    Path(data / 'stojanov/zapiski/11')
]

#Aleko Konstantinov
train_a_files = [
    Path(data / 'konstantinov/bai_ganjo/1'),
    Path(data / 'konstantinov/bai_ganjo/3'),
    Path(data / 'konstantinov/drebni_raboti/1'),
    Path(data / 'konstantinov/bai_ganjo/6'),
    Path(data / 'konstantinov/bai_ganjo/9'),
    Path(data / 'konstantinov/bai_ganjo/9'),
    Path(data / 'konstantinov/chestita_godina/1'),
    Path(data / 'konstantinov/razni_hora/1')


]

test_a_files = [
    Path(data / 'konstantinov/bai_ganjo/2'),
    Path(data / 'konstantinov/bai_ganjo/4'),
    Path(data / 'konstantinov/ot_mnogo_um/1'),
    Path(data / 'konstantinov/bai_ganjo/7')
]

encoding_to_name = {0: 'Stojanov', 1: 'Konstantinov'}
name_to_encoding = {'Stojanov': 0, 'Konstantinov': 1}

def load_corpus(fnames):
    text_corpus = []

    for fname in fnames:
        with open(fname, 'r') as f:
            text_corpus.append(''.join(f.read().splitlines()))

    return text_corpus
        
        
if __name__ == '__main__':
    

    train_a = load_corpus(train_a_files)
    train_s = load_corpus(train_s_files)
    train_x = train_a + train_s
    train_y = [1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0]

    print("train a")
    print((train_a))
    processed_x = preprocess.run_analyze_pipeline(train_x)
    print(processed_x)
    words = sorted(list(preprocess.get_words_set(processed_x)))
    matrix = preprocess.build_count_matrix(processed_x, words)

    model = MultinomialNB().fit(matrix, train_y)

    print(model.score(matrix, train_y))
    with open('model', 'wb') as f:
        pickle.dump(Model(model, words), f)

    with open('model', 'rb') as f:
        model_loaded = pickle.load(f)

    test_a = load_corpus(test_a_files)
    test_s = load_corpus(test_s_files)
    test_x = test_s + test_a
    test_processed = preprocess.build_count_matrix(preprocess.run_analyze_pipeline(test_x), model_loaded.words)
    print(model_loaded.model.predict(test_processed))


    #count_vectorizer = CountVectorizer().fit(train_a)
    #print(count_vectorizer.get_feature_names_out())

