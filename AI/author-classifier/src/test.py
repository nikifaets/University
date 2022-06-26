import pickle 
from pathlib import Path
import preprocess
from train import Model
from util import load_corpus
from sklearn.metrics import f1_score
data = Path('../data')

test_a_files = [
    Path(data / 'konstantinov/bai_ganjo/2'),
    Path(data / 'konstantinov/bai_ganjo/4'),
    Path(data / 'konstantinov/ot_mnogo_um/1'),
    Path(data / 'konstantinov/bai_ganjo/7')
]

test_s_files = [
    Path(data / 'stojanov/hristo_botiov/2'),
    Path(data / 'stojanov/prevratat/2'),
    Path(data / 'stojanov/zapiski/2' ),
    Path(data / 'stojanov/zapiski/11'),
]

encoding_to_name = {0: 'Stojanov', 1: 'Konstantinov'}
name_to_encoding = {'Stojanov': 0, 'Konstantinov': 1}


if __name__ == '__main__':

    with open('model', 'rb') as f:
        model = pickle.load(f)

    words = model.words

    test_s = load_corpus(test_s_files)
    test_a = load_corpus(test_a_files)
    test_x = test_s + test_a
    test_y = [0, 0, 0, 0, 1, 1, 1, 1]
    processed_test = preprocess.build_count_matrix(preprocess.run_analyze_pipeline(test_x), words)

    #for idx, row in enumerate(processed_test):
    print(model.model.predict(processed_test), ', expected: ')
    print(model.model.score(processed_test, test_y))
    


    