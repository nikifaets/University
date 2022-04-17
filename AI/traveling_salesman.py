import random
from copy import copy
import itertools
import math

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

            child_agent_solution = starting_node + child_agent_solution + starting_node
                    
            return Agent(child_agent_solution)
    
    class FilterFunction:

        @staticmethod
        def top_and_random(population, fitness_func, top_slice = .4, other_slice = .1):
            assert top_slice + other_slice == .5

            sorted_population = sorted(population, key = lambda ag: fitness_func(ag))
            #print([i.solution for i in sorted_population])
            top = sorted_population[math.ceil(top_slice * len(population)):]
            bottom = random.sample(sorted_population[:math.ceil(top_slice * len(population))], math.ceil(other_slice * len(population)))

            return bottom + top


class Agent:

    def __init__(self, solution):
        self.solution = solution

    def get_fitness(self, fitness_func):
        return fitness_func(self.solution)

class Population(list):

    def get_split(self):
        pass


class Experiment:
    
    def __init__(self, max_iterations, max_agents, crossover_func, mutation_func, fitness_func, filter_func, adj_table, starting_node):
        self.crossover_func = crossover_func
        self.mutation_func = mutation_func
        self.adj_table = adj_table
        self.max_iterations = max_iterations
        self.fitness_func = fitness_func
        self.starting_node = starting_node
        self.max_agents = max_agents
        self.filter_func = filter_func

    def generate_population(self):

        nodes = set(self.adj_table.keys())
        nodes.remove(self.starting_node)
        perm_it = itertools.permutations(nodes, len(nodes))

        return Population(map(
            lambda f: Agent(self.starting_node + ''.join(f) + self.starting_node),
            [perm_it.__next__() for i in range(0, min(math.factorial(len(nodes)), self.max_agents))]))

    def run(self):

        self.population = self.generate_population()
        
        for epoch in range(0, self.max_iterations):
            print("epoch", [i.solution for i in self.population])
            self.population = self.filter_func(self.population, self.fitness_func)
            # crossover
            # mutate
            
            # collect best solution
            if epoch > 2:
                return

if __name__ == '__main__':
    adj_table = {
        'A': {'B': 5, 'C': 10, 'D': 3},
        'B': {'A': 5, 'C': 12, 'D': 8},
        'C': {'A': 10, 'B': 12, 'D': 5},
        'D': {'A': 3, 'B': 8, 'C': 5}}

    ag = Agent('ADBCA')
    #print(UtilFunction.FitnessFunction.path_sum(ag, adj_table))

    agent1 = Agent('ABCDHJGSA')
    agent2 = Agent('AJGABCDHA')
    #print(UtilFunction.CrossoverFunction.one_point_crossover(agent1, agent2).solution)

    experiment = Experiment(
        1000,
        30,
        UtilFunction.CrossoverFunction.one_point_crossover,
        lambda a: a,
        lambda ag: UtilFunction.FitnessFunction.path_sum(ag, adj_table),
        lambda pop, fitness_func: UtilFunction.FilterFunction.top_and_random(pop, fitness_func),
        adj_table,
        'A'
    )


    #population = experiment.generate_population()
    #print([i.solution for i in population])
    #print([i.solution for i in UtilFunction.FilterFunction.top_and_random(population, lambda f: UtilFunction.FitnessFunction.path_sum(f, adj_table))])
    experiment.run()
    