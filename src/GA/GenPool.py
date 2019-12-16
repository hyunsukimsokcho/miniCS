import random
import numpy as np
from eval import get_score

def intersect_chk(arr, x):
	for y in arr:
		if is_intersect(x, y):
			return True
	return False 

def is_intersect(x, y):
	return not ((x[1] <= y[0]) or (y[1] <= x[0]))

def merge_range(x, y):
	return (min(x[0],y[0]), max(x[1],y[1]))

def random_gen(arr):
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
	def __init__(self, population, iteration, src, arr):
		if population % 2 > 0:
			population = population + 1
		self.population = population
		self.iteration = iteration
		self.src = src
		self.arr = arr
		self.pool = [random_gen(arr) for i in range(self.population)]
		self.best = []

	def get_best(self):
		n = self.iteration // 100
		for i in range(self.iteration):
			self.selection()

			if i%n == n-1:
				print("{}% clear!".format(i/n))
				print(self.best)
				print(get_score(self.src, self.best))

		return self.best, get_score(self.src, self.best)

	def tournament_selection(self):
		K = self.population // 4

		ret = self.pool[0]
		best = (99999999, 99999999)
		for i in range(K):
			candidate = random.choice(self.pool)
			score = get_score(self.src, candidate)
			if best > score:
				best = score
				ret = candidate
		return ret

	def selection(self):
		# TODO

		new_pool = []
		for i in range(self.population // 2):
			p0 = self.tournament_selection()
			p1 = self.tournament_selection()
			
			(c0, c1) = self.crossover(p0, p1)
			new_pool.append(c0)
			new_pool.append(c1)
		self.pool = new_pool

		for i in self.pool:
			if get_score(self.src, self.best) < get_score(self.src, i):
				self.best = i

		for i in range(self.population):
			mut_prob = random.random()
			if mut_prob < 0.1:
				self.add_mutation(i)
			elif mut_prob < 0.2:
				self.remove_mutation(i)


	def add_mutation(self, num):
		gen = self.pool[num]
		random.shuffle(self.arr)

		for i in self.arr:
			if not intersect_chk(gen, i):
				gen.append(i)
				break
		gen.sort()

	def remove_mutation(self, num):
		gen = self.pool[num]
		l = len(gen)
		if l != 0:
			del gen[np.random.randint(l)]

	def crossover(self, gen0, gen1):
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