import csv
from random import shuffle, uniform
from statistics import mean

class Predictor:

    def __init__(self):
        self.age_factor = uniform(0, 10)
        self.weight_factor = uniform(0, 10)
        self.bias = uniform(0,10)

    def predict(self, age, weight):
        return self.age_factor * age + self.weight_factor * weight + self.bias

    def update(self, y, gt, lr):

        factor_derivative = lambda factor, y, gt: 2 * (y - gt) * factor
        bias_derivative = lambda y, gt: 2 * (y - gt)

        self.age_factor -= factor_derivative(self.age_factor, y, gt) * lr
        self.weight_factor -= factor_derivative(self.weight_factor, y, gt) * lr
        self.bias -= bias_derivative(y, gt) * lr


def train_predictor(data, iterations = 100, lr = .0006, logging = False):
    
    mse = lambda y, gt: (y - gt) ** 2

    predictor = Predictor()

    data_len = len(data)

    for it in range(1, iterations + 1):

        loss = 0

        for sample in data:
            
            y = predictor.predict(*sample[:-1])
            loss += mse(y, sample[-1])

            predictor.update(y, sample[-1], lr)

        loss /= data_len

        if it % 5 == 0 and logging == True:
            pass
            print("Iteration: ", it)
            print("Loss: ", loss)

        shuffle(data)

    return predictor


def test_predictor(data, predictor):

    return [predictor.predict(*d[:-1]) - d[-1] for d in data]


def read_data(fname = 'age_weight.csv'):

    with open(fname) as file:
        data_csv = csv.reader(file, delimiter=',')
        print(data_csv.__next__())
        data = [list(map(lambda r: float(r), d)) for d in data_csv]
    return data
        
        
def split_data(data, split = .8):
    data_len = len(data)
    split_point = int(data_len * split)
    return (data[:split_point], data[split_point:])
    
def hyperparameter_search(train, test):

    
    learning_rates = [.00002, .00005, .00007, .0001, .0004, .0007, .001, .003, .005, .01]
    best_result = {'mean_error' : 10000, 'lr' : 0}
    predictors = []

    for lr in learning_rates:
        tain, val = split_data(train, .8)
        predictor = train_predictor(train)
        result = abs(mean(test_predictor(val, predictor)))
        print('lr: ', lr, ' Acc: ', result)
        if result < best_result['mean_error']:
            best_result = {'mean_error' : result, 'lr': lr}

        predictors.append({'predictor': predictor, 'lr': lr})
        
        shuffle(train)

    print("Best learning rate according to hyperparameter search: ", best_result['lr'])

    learning_rates_sorted = sorted(predictors, key = lambda pr: test_predictor(test, pr['predictor']))
    print("Actual best learning rate: ", learning_rates_sorted[0]['lr'])


    
    

if __name__ == '__main__':
    
    data = read_data()
    shuffle(data)
    train, test = split_data(data)
    print(len(train), len(test))

    predictor = train_predictor(train, logging = True)
    result = test_predictor(test, predictor)

    print(mean(result))

    hyperparameter_search(train, test)



