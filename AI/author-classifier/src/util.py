def load_corpus(fnames):
    text_corpus = []

    for fname in fnames:
        with open(fname, 'r') as f:
            text_corpus.append(''.join(f.read().splitlines()))

    return text_corpus