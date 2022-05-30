from audioop import cross
import random
from copy import copy
import itertools
import math
import statistics

class UtilFunction:

    class FitnessFunction:

        @staticmethod
        def path_sum(agent, adj_table):
            sum = 0
            for i, gene in enumerate(agent.solution):
                if i >= len(agent.solution) - 1:
                    return -sum

                sum += adj_table[agent.solution[i]][agent.solution[i+1]]

    class CrossoverFunction:
        
        @staticmethod
        def one_point_crossover(agent1, agent2):
            assert len(agent1.solution) == len(agent2.solution)

            starting_node = agent1.solution[0]
            sol_len = len(agent1.solution)
            crossover_point = random.randint(1, sol_len - 2)

            genes_not_used = copy(agent1.solution[crossover_point:-1])
            genes_used = copy(agent1.solution[1:crossover_point])

            child_agent_solution = agent1.solution[1:crossover_point]

            for i, gene in enumerate(agent2.solution[crossover_point:-1]):

                if gene in genes_used:
                    replace_gene = random.choice(genes_not_used)
                    child_agent_solution += replace_gene
                    genes_used += replace_gene
                    genes_not_used = genes_not_used.replace(replace_gene, '')

                else:
                    child_agent_solution += agent2.solution[crossover_point + i]
                    genes_used += agent2.solution[crossover_point + i]
                    genes_not_used = genes_not_used.replace(agent2.solution[crossover_point + i], '')

            child_agent_solution = starting_node + child_agent_solution + starting_node
                    
            return Agent(child_agent_solution)
    
    class FilterFunction:

        @staticmethod
        def top_and_random(population, fitness_func, top_slice = .4, other_slice = .1):
            assert top_slice + other_slice == .5

            sorted_population = sorted(population, key = lambda ag: fitness_func(ag))
            #print([i.solution for i in sorted_population])
            top = sorted_population[math.ceil((1 - top_slice) * len(population)):]
            bottom = random.sample(sorted_population[:math.ceil(top_slice * len(population))], math.ceil(other_slice * len(population)))

            return bottom + top

    class PairingFunction:

        @staticmethod
        def random_pairs(population):
            
            pairs = []
            random.shuffle(population)

            if len(population) % 2 == 1:
                del population[-1]

            for i in range(0, len(population) // 2):
                pairs.append((population[i], population[-i - 1]))
                pairs.append((population[-i - 1], population[i]))
            
            return pairs

    class MutationFunction:

        @staticmethod
        def random_switch(agent, prob = .05):

            if random.random() > prob: return agent

            first,sec = random.sample(agent.solution[1:-1], 2)
            agent.solution = agent.solution \
            .replace(first,'*')  \
            .replace(sec, first) \
            .replace('*', sec)   \

            return agent

class Agent:

    def __init__(self, solution):
        self.solution = solution

    def get_fitness(self, fitness_func):
        return fitness_func(self.solution)

class Population(list):

    pass


class Experiment:
    
    def __init__(self, max_iterations, max_agents, crossover_func, mutation_func, fitness_func, filter_func, pairing_func, adj_table, starting_node):
        self.crossover_func = crossover_func
        self.mutation_func = mutation_func
        self.adj_table = adj_table
        self.max_iterations = max_iterations
        self.fitness_func = fitness_func
        self.starting_node = starting_node
        self.max_agents = max_agents
        self.filter_func = filter_func
        self.pairing_func = pairing_func
        
    def generate_population(self):

        nodes = set(self.adj_table.keys())
        nodes.remove(self.starting_node)
        perm_it = itertools.permutations(nodes, len(nodes))

        population = Population(map(
            lambda f: Agent(self.starting_node + ''.join(f) + self.starting_node),
            [perm_it.__next__() for i in range(0, min(math.factorial(len(nodes)), self.max_agents))]))

        if len(population) < self.max_agents:
            for i in range(0, self.max_agents // len(population)):
                random.shuffle(population)
                for i in range(0, len(population)):
                    if len(population) >= self.max_agents:
                        break
                    population.append(copy(population[i]))
                    
                    
        return population

    def run(self):

        # initialization
        self.population = self.generate_population()

        for epoch in range(0, self.max_iterations):
            print("epoch " + str(epoch))

            # selection
            self.population = self.filter_func(self.population, self.fitness_func)

            # crossover
            crossover_couples = []
            while len(crossover_couples) < self.max_agents:
                crossover_couples += self.pairing_func(self.population)
            
            self.population = Population(
                map(lambda couple: self.crossover_func(couple[0], couple[1]), crossover_couples)
            )

            # mutation
            self.population = Population(
                map(self.mutation_func, self.population)
            )

            # output
            print("max fitness function: ", max(list(map(self.fitness_func, self.population))))
            print("min fitness function: ", min(list(map(self.fitness_func, self.population))))
            print("average fitness: ", statistics.mean(list(map(self.fitness_func, self.population))))

        best_solutions = sorted(list(set(self.population)), key = self.fitness_func)[-10:]

        print("TOP 10 SOLUTIONS AND THEIR PATH LENGTH")
        print([(i.solution, -self.fitness_func(i)) for i in best_solutions])


if __name__ == '__main__':

    adj_table = {
        'A': {'B': 5, 'C': 10, 'D': 3},
        'B': {'A': 5, 'C': 12, 'D': 8},
        'C': {'A': 10, 'B': 12, 'D': 5},
        'D': {'A': 3, 'B': 8, 'C': 5}}

    adj_table1 = {
    'A': {'B': 5, 'C': 10, 'D': 3, 'E': 2},
    'B': {'A': 5, 'C': 12, 'D': 8, 'E': 5},
    'C': {'A': 10, 'B': 12, 'D': 5, 'E': 10},
    'D': {'A': 3, 'B': 8, 'C': 5, 'E': 4},
    'E': {'A': 2, 'B': 5, 'C': 10, 'D': 4}}

    adj_table2 = {
    'A': {'B': 5, 'C': 10, 'D': 3, 'E': 2, 'F': 10},
    'B': {'A': 5, 'C': 12, 'D': 8, 'E': 5, 'F': 12},
    'C': {'A': 10, 'B': 12, 'D': 5, 'E': 10, 'F': 3},
    'D': {'A': 3, 'B': 8, 'C': 5, 'E': 4, 'F': 5},
    'E': {'A': 2, 'B': 5, 'C': 10, 'D': 4, 'F': 8},
    'F': {'A': 10, 'B': 12, 'C': 3, 'D': 5, 'E': 8}}


    experiment = Experiment(
        50,
        300,
        UtilFunction.CrossoverFunction.one_point_crossover,
        UtilFunction.MutationFunction.random_switch,
        lambda ag: UtilFunction.FitnessFunction.path_sum(ag, adj_table2),
        lambda pop, fitness_func: UtilFunction.FilterFunction.top_and_random(pop, fitness_func),
        UtilFunction.PairingFunction.random_pairs,
        adj_table2,
        'A'
    )

    experiment.run()
    