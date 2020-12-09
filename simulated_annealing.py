import math, random

from LightsOutBoard import LightsOutBoard1000x1000, LightsOutBoard8x8, LightsOutBoard4x3, LightsOutBoard5x5

class SimulatedAnnealing:
	def __init__(self, board: 'LightsOutBoardnxm', initial_temperature: float = 1.0, temperature_multiplier: float = 0.999, current_parameters: 'LightsOutBoardnxm' = None, maximum_mutation_probability: float = 0.001, minimum_mutation_probability: float = 0.000001, maximum_consecutive_increasing_suboptimal: int = 10):
		self._board = board
		self.initial_temperature = self.current_temperature = initial_temperature
		self.temperature_multiplier = temperature_multiplier

		self.current_parameters = current_parameters if current_parameters else self._board.clone()

		self.maximum_mutation_probability = maximum_mutation_probability
		self.minimum_mutation_probability = minimum_mutation_probability
		self.maximum_consecutive_increasing_suboptimal = maximum_consecutive_increasing_suboptimal

		self.best_parameters = self.current_parameters.clone()
		self.best_cost = self.current_cost = self._board.get_cost(self.best_parameters)

		self.consecutive_increasing_suboptimal = 0

	def _get_mutated_parameters(self) -> 'mutated parameters':
		mutated_parameters = self.current_parameters.clone()

		mutation_probability = self.minimum_mutation_probability + (self.maximum_mutation_probability - self.minimum_mutation_probability) * self.current_temperature

		# try to always mutate at least one
		if mutation_probability * self.current_parameters.get_num_parameters() < 1:
			mutation_index = random.randrange(0, self.current_parameters.get_num_parameters())
			mutated_parameters.mutate(mutation_index)
			return mutated_parameters

		mutations = mutated_parameters.clone()
		mutations.set_random(mutation_probability)
		mutations.keep_relevant_mutations(self._board.get_still_on(self.current_parameters))
		mutated_parameters.mutate_board(mutations)

		return mutated_parameters

	def _get_acceptance_probability(self, cost: float) -> float:
		if cost < self.current_cost:
			return 1.0 # definitely accept

		return math.exp((self.current_cost - cost) / self.current_temperature * 100)

	def run_one_cycle(self):
		mutated_parameters = self._get_mutated_parameters()
		cost = self._board.get_cost(mutated_parameters)

		acceptance_probability = self._get_acceptance_probability(cost)
		# print(acceptance_probability, self.current_cost, cost)

		if random.random() < acceptance_probability:
			if cost > self.current_cost and cost > self.best_cost:
				self.consecutive_increasing_suboptimal += 1

				if self.consecutive_increasing_suboptimal > self.maximum_consecutive_increasing_suboptimal:
					mutated_parameters = self.best_parameters
					cost = self.best_cost

			self.current_parameters = mutated_parameters
			self.current_cost = cost

		if cost <= self.best_cost:
			self.best_parameters = mutated_parameters
			self.best_cost = cost
			self.consecutive_increasing_suboptimal = 0

		self.current_temperature *= self.temperature_multiplier


if __name__ == '__main__':
	board = LightsOutBoard1000x1000()
	board.set_random(0.2)

	# board.set_list([0, 1, 0, 0, 1,
	#                 0, 1, 0, 1, 0,
	#                 1, 1, 1, 1, 0,
	#                 1, 0, 0, 1, 0,
	#                 0, 1, 0, 0, 0])
	# board.set_list([1, 0, 0, 1, 0,
	#                 0, 0, 0, 0, 0,
	#                 1, 1, 1, 1, 0,
	#                 0, 1, 0, 0, 1,
	#                 1, 0, 0, 1, 1])

	# print(board.pretty())

	parameters = LightsOutBoard1000x1000()
	# parameters.set_list([0, 1, 0, 0, 1,
	#                      1, 0, 1, 1, 0,
	#                      1, 0, 0, 1, 0,
	#                      1, 0, 1, 1, 1,
	#                      1, 0, 0, 1, 0])

	# print(1 - board.get_cost(parameters))
	# exit(0)


	sa = SimulatedAnnealing(board, 1, 0.99999, current_parameters = parameters)
	for i in range(1000000):
		print(f'{i:5d}: {sa.current_temperature:.6e} {sa.current_cost:.6f} {sa.best_cost:.6f}')
		sa.run_one_cycle()
	# sa.run_one_cycle()
	# print(f'{0:5d}: {sa.current_temperature:.6e} {sa.current_cost:.6f} {sa.best_cost:.6f}')
	print(sa.best_parameters.get_num_on())



# 1000x1000, 0.5 probability results
# result | # iterations | initial temperature | temperature multiplier | additional details
# ------ + ------------ + ------------------- + ---------------------- + ------------------
# 0.3933 |    10,000    |         1           |         0.99999        | first value seems to be optimal value
# 0.3934 |    10,000    |         1           |         0.99999        | that it finds ;(
#
