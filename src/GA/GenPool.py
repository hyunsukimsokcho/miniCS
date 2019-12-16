import random
import numpy as np
from eval import get_score
from pprint import pprint

def intersect_chk(arr, x):
	"""
		Return whether elements of arr and x intersect.
		True: not intersect / Flase: otherwise.

		Keyword Arguments:
		arr -- list / list of locks
		x -- tuple with size 2 (lock)
	"""
	for y in arr:
		if is_intersect(x, y):
			return True
	return False 

def is_intersect(x, y):
	"""
		Return whether x and y intersect.
		True: intersects / Flase: otherwise.

		Keyword Arguments:
		x, y -- tuple with size 2 (lock)
	"""
	return not ((x[1] <= y[0]) or (y[1] <= x[0]))

def merge_range(x, y):
	"""
		Return merged intervals from given two intervals

		Keyword Arguments:
		x, y -- tuple with size 2 (lock)
	"""
	return (min(x[0],y[0]), max(x[1],y[1]))

def random_gen(arr):
	"""
		Return random gene.

		Keyword Arguments:
		arr -- list / list of lock candidates.
	"""
	prob = 0.35

	arr_len = len(arr)
	random.shuffle(arr)

	ret = []

	for i in arr:
		if random.random() < prob:
			if not intersect_chk(ret, i):
				ret.append(i)

	ret.sort()
	return ret

class GenPool:
	""" 
		Class for GA/ 
	
		Fields:
		population -- population size of GA
		iteration -- iteration size of GA
		src -- name of test code
		arr -- list of lock candidates
		pool -- list of genes
		best -- best gene
		best_score -- score of best gene
		cache -- scores for genes in pool
	"""

	def __init__(self, population, iteration, src, arr):
		"""
			Create GenPool with given input.

			Keyword argument: 
			population	-- number / population size of GA
			iteration	-- number / iteration size of GA
			src			-- string / name of test code
			arr 		-- list	/ list of lock candidates
		"""
		if population % 2 > 0:
			population = population + 1
		self.population = population
		self.iteration = iteration
		self.src = src
		self.arr = arr
		self.pool = [random_gen(arr) for i in range(self.population)]
		self.best = []
		self.best_score = (99999999,99999999)
		self.cache = [(0,0)] * population

	def get_best(self):
		"""
			Do selection, crossover and mutation for iteration size.
			Also, prints progress of GA.
		"""
		n = self.iteration / 100.0
		cnt = 0
		for i in range(self.iteration):
			self.selection()

			if int(i/n) > cnt:
				cnt += 1
				print("{}% clear!".format(i/n))
				print(self.best)
				print(get_score(self.src, self.best))

				print("pool : ")
				pprint(self.pool)

		return self.best, self.best_score

	def tournament_selection(self):
		"""
			Do tournament selection.
			For K random genes, pick best gene.
		"""
		K = self.population // 4

		ret = self.pool[0]
		best = (99999999,99999999)
		for i in range(K):
			idx = np.random.randint(self.population)
			if best > self.cache[idx]:
				best = self.cache[idx]
				ret = self.pool[idx]
		return ret

	def selection(self):
		"""
			Find best gene for this generation.
			After that, do selection, crossover and mutation.
		"""
		for i in range(self.population):
			self.cache[i] = get_score(self.src, self.pool[i])
			if self.best_score > self.cache[i]:
				self.best = self.pool[i]
				self.best_score = self.cache[i]

		new_pool = []
		for i in range(self.population // 2):
			p0 = self.tournament_selection()
			p1 = self.tournament_selection()
			
			(c0, c1) = self.crossover(p0, p1)
			new_pool.append(c0)
			new_pool.append(c1)
		self.pool = new_pool


		for i in range(self.population):
			mut_prob = random.random()
			if mut_prob < 0.1:
				self.add_mutation(i)
			elif mut_prob < 0.2:
				self.remove_mutation(i)


	def add_mutation(self, num):
		"""
			Do add mutation. Add a new lock in the gene.

			Keyword Arguments:
			num -- number / index of gene in self.pool
		"""
		gen = self.pool[num]
		random.shuffle(self.arr)

		for i in self.arr:
			if not intersect_chk(gen, i):
				gen.append(i)
				break
		gen.sort()

	def remove_mutation(self, num):
		"""
			Do delete mutation. Remove a lock in the gene.

			Keyword Arguments:
			num -- number / index of gene in self.pool
		"""
		gen = self.pool[num]
		l = len(gen)
		if l != 0:
			del gen[np.random.randint(l)]

	def crossover(self, gen0, gen1):
		"""
			Do crossover. Return two childs from given parents.

			Keyword Arguments:
			gen0, gen1 -- list / gene from self.pool
		"""
		len0 = len(gen0)
		len1 = len(gen1)

		if len0 == 0 or len1 == 0:
			return (gen0, gen1)

		if len0 <= 1 and len1 <= 1:
			return (gen0, gen1)

		if len0 <= 1:
			return self.crossover(gen1, gen0)

		i0 = np.random.randint(1,len0)
		if (gen0[i0-1][1] > gen0[i0][0]):
			print(gen0)
			print(gen1)
			assert(is_intersect(gen0[i0-1], gen0[i0]))
		x = np.random.randint(gen0[i0-1][1], gen0[i0][0]+1)

		i1 = len1
		for j in range(len1):
			if gen1[j][0]>=x:
				i1 = j
				break
		
		ret0 = gen0[:i0]
		ret1 = gen1[:i1]

		if i1 < len1 and is_intersect(ret0[i0-1], gen1[i1]):
			ret0[i0-1] = merge_range(ret0[i0-1], gen1[i1])
			del gen1[i1]
		ret0.extend(gen1[i1:])

		if i1 > 0 and is_intersect(ret1[i1-1], gen0[i0]):
			ret1[i1-1] = merge_range(ret1[i1-1], gen0[i0])
			del gen0[i0]
		ret1.extend(gen0[i0:])

		return (ret0, ret1)