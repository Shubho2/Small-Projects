import math
import pygame
import random
import numpy
# pygame.init()

# BLACK = (  0,   0,   0)
# WHITE = (255, 255, 255)
# BLUE =  (  0,   0, 255)
# GREEN = (  0, 255,   0)
# RED =   (255,   0,   0)

# color = [BLACK, BLUE, GREEN, RED]
# myFont = pygame.font.SysFont("Roboto", 18)

# Set the height and width of the screen
# size = [800, 500]
# screen = pygame.display.set_mode(size)
# pygame.display.set_caption("Vehicular routing")

# global constants
alpha = 1
beta = 2
ro = .9

#capacity of each vehicle
capacity = 200


def distance(x1, y1, x2, y2):
    # Manhattan distance
    dist = abs(x1 - x2) + abs(y1 - y2)
    return dist

#diferent types of nodes and their locations
# depot = [[100,150]]
# disposal = [[500,100],[400,200]]
# customer = [[350, 100],[400, 100],[700, 150],[350, 200],[500, 200]]
# nodes = [[100,150],[350, 100],[400, 100],[700, 150],[350, 200],[500, 200],[500,100],[400,200]]
# network = [[100,150],[350, 100],[400, 100],[700, 150],[350, 200],[500, 200]]

# speed = 300


# depot = [[20,40]]
# disposal = [[20,20]]
# customer = [[20,30],[10,20],[20, 10],[30, 20]]
# nodes = [[20,40],[20,30],[10,20],[20, 10],[30, 20],[20,20]]
# network = [[20,40],[20,30],[10,20],[20, 10],[30, 20]]

filename = 'a'
raw_data = open(filename, 'rt')
data = numpy.loadtxt(raw_data, delimiter=",")

depot = []
disposal = []
customer = []
nodes = []
network = []
time_window = []

for x in xrange(0,data.shape[0]):
    if data[x][4] == 0:
        depot.append([data[x][0],data[x][1]])
        network.append([data[x][0],data[x][1]])
    elif data[x][4] == 1:
        customer.append([data[x][0],data[x][1]])
        network.append([data[x][0],data[x][1]])
        time_window.append([data[x][2],data[x][3]])
    else:
        disposal.append([data[x][0],data[x][1]])

    nodes.append([data[x][0],data[x][1]])

speed = 300


#customer time window
time_window = [[0.0,0.0],[2.0,3.0],[6.0,7.0],[10.0,12.0],[14.0,16.0]]

travel_time = [[0.0 for i in range(len(nodes))] for i in range(len(nodes))]

for x in xrange(0,len(nodes)):
	for y in xrange(0,len(nodes)):
		travel_time[x][y] = distance(nodes[x][0],nodes[x][1],nodes[y][0],nodes[y][1])/speed
		print travel_time[x][y],
	print ""
# service time at each node
service_time = 2.0

#garbage amount initialization
garbage = []
for x in xrange(0,len(network)):
	garbage.append(random.randint(1,100))
garbage[0] = 0 # for depot

# pheromone for all 
pheromones = [[1.0 for i in range(len(nodes))] for i in range(len(nodes))]
for x in xrange(0,len(nodes)):
	for y in xrange(0,len(nodes)):
		if x == y:
			pheromones[x][y] = -1.0

#attractive coefficient		
eta = [[0.0 for i in range(len(network))] for i in range(len(network))]
 

visited = [1,0,0,0,0,0] # depot initially visited


# def create_node():
# 	for x in xrange(0,len(nodes)):
# 		pygame.draw.rect(screen, WHITE , [nodes[x][0], nodes[x][1], 20,20])
# 		pos = myFont.render(str(x), 3, RED)
# 		screen.blit(pos, (nodes[x][0], nodes[x][1]))
# 	pygame.display.flip()		

# def reset_color():
# 	for x in xrange(0,len(nodes)):
# 		for y in xrange(0,len(nodes)):
# 			pygame.draw.line(screen, BLUE, [nodes[x][0], nodes[x][1]], [nodes[y][0],nodes[y][1]], 1)
# 	pygame.display.flip()		

# initializing eta values (1/dist) for each pair of nodes in network
for i in range(0,len(network)):
	for j in range(0,len(network)):
		if distance(network[i][0],network[i][1],network[j][0],network[j][1]):
			eta[i][j] = 1.0/distance(network[i][0],network[i][1],network[j][0],network[j][1])

# def isDisposal(node):
# 	for x in xrange(0,len(disposal)):
# 		if node[0] == disposal[x][0] and node[1] == disposal[x][1]:
# 			return 1
# 	return 0

def isPerfectNeighbour(start_time,remaining,i,j):
	if i != j:
		total_time = start_time + travel_time[i][j]
		if total_time >= time_window[j][0] and total_time <= time_window[j][1] and remaining >= garbage[j]:
			return 1
		else:
			return 0
	else:
		return 0

def isNeighbour(start_time,remaining,i,j):
	if i != j:
		total_time = start_time + travel_time[i][j]
		if total_time <= time_window[j][1] and remaining >= garbage[j]:
			return 1
		else:
			return 0
	else:
		return 0

def allNotVisited():
	for col in range(0,len(visited)):
		# print "status: {}".format(visited[col])
		if (not visited[col]):
			return 1
	return 0

def attract_coeff(x,y):
	return eta[x][y]

def calculate_prob(neighbour_set,x,y):
	numerator = pheromones[x][neighbour_set[y]]**alpha * eta[x][y]**beta
	denominator = 0.0
	for n in range(0,len(neighbour_set)):
		if x != n:
			denominator = denominator + pheromones[x][neighbour_set[n]]**alpha * eta[x][neighbour_set[n]]**beta
	if denominator != 0.0:
		return numerator / denominator
	else:
		print "Exception!! division by zero"

def isNotEmpty(x):
	if(len(x)):
		return 1
	else:
		return 0

def  nearest_disposal(curr):
	mini = distance(network[curr][0], network[curr][1] , disposal[0][0], disposal[0][1])
	index = 0
	for x in xrange(1,len(disposal)):
		temp = distance(network[curr][0], network[curr][1] , disposal[x][0], disposal[x][1])
		if mini > temp:
			mini = temp
			index = x
	return index

# get positon of tour node
def getpos(t):
	for x in xrange(0,len(nodes)):
		if nodes[x][0] == t[0] and nodes[x][1] == t[1]:
			return x

def tour_const():
	epoch = 20
	min_dist = 10000
	# keeps best tour calculated after each epoch
	best_tour = []
	while epoch:
		flag = 0
		start_time = 0.0
		curr = 0 # initially depot
		visited[curr] = 1
		next_node = 0
		remaining = capacity
		tour = []
		while allNotVisited():
			perfect_neighbour_set = []
			neighbour_set = []
			print start_time
			for x in xrange(0,len(network)):
				# find all neighbour of x
				# print curr,visited[x]
				if isPerfectNeighbour(start_time,remaining,curr,x) and visited[x] == 0:
					perfect_neighbour_set.append(x)
			# print len(perfect_neighbour_set)
			# if neighbour set not empty select next node
			if isNotEmpty(perfect_neighbour_set):
				# print "perfect found"
				q = random.uniform(0,1)
				maxi = 0.0
				if q > 0.5:
					for j in range(0,len(perfect_neighbour_set)):
						prob = calculate_prob(perfect_neighbour_set,curr,j)
						print "node: {} prob : {}".format(perfect_neighbour_set[j],prob)
						if maxi < prob:
							maxi = prob
							next_node = perfect_neighbour_set[j]

				# randomly choosing for exploration
				else:
					for j in range(0,len(perfect_neighbour_set)):
						prob = attract_coeff(curr,perfect_neighbour_set[j])
						print "node: {} prob : {}".format(perfect_neighbour_set[j],prob)
						if maxi < prob:
							maxi = prob
							next_node = perfect_neighbour_set[j]

				print "next node perfect nei: {}".format(next_node)
				# update next neighbour
				start_time = start_time + travel_time[curr][next_node] + service_time
				remaining = remaining - garbage[next_node]
				curr = next_node
				visited[curr] = 1
				# keep path
				tour.append([network[curr][0],network[curr][1]])

			else:
				for x in xrange(0,len(network)):
					# print curr,x
					if isNeighbour(start_time,remaining,curr,x) and visited[x] == 0:
						neighbour_set.append(x)

				# print "len of nei set: {}".format(len(neighbour_set))
				# for x in xrange(0,len(neighbour_set)):
				# 	print "nei of {} is : {}".format(curr,neighbour_set[x])
				if isNotEmpty(neighbour_set):
					q = random.uniform(0,1)
					maxi = 0.0
					if q > 0.5:
						for j in range(0,len(neighbour_set)):
							prob = calculate_prob(neighbour_set,curr,j)
							print "node: {} prob : {}".format(neighbour_set[j],prob)
							if maxi < prob:
								maxi = prob
								next_node = neighbour_set[j]
					
					# randomly choosing for exploration
					else:
						for j in range(0,len(neighbour_set)):
							prob = attract_coeff(curr,neighbour_set[j])
							print "node: {} prob : {}".format(neighbour_set[j],prob)
							if maxi < prob:
								maxi = prob
								next_node = neighbour_set[j]

					# update next neighbour
					print "next node simple nei: {}".format(next_node)
					start_time = time_window[next_node][0] + service_time
					remaining = remaining - garbage[next_node]
					curr = next_node
					visited[curr] = 1
					# keep path
					tour.append([network[curr][0],network[curr][1]])
				else:
					for x in xrange(0,len(customer)):
						if remaining >= garbage[x]:
							break
					if x == len(customer):
						# goto disposal
						remaining = capacity
						# find nearest disposal
						disp = nearest_disposal(curr)
						tour.append(disposal[disp][0],disposal[disp][1])
						pos = getpos(disposal[disp])
						start_time = start_time + travel_time[curr][pos]*2 + service_time
					else:
						# goto depot
						for x in xrange(0,len(visited)):
							visited[x] = 1
						print "No path found"
						# flag = 1
			# print "curr : {}".format(curr)
			# print "visit : {}".format(visited[curr])	
			for x in xrange(0,len(tour)):
				print "{} --> ".format(tour[x]),
			print ""

		# if flag == 0:
		# reset visited for next epoch
		for x in xrange(0,len(visited)):
			visited[x] = 0
		# Update pheromone (decay) to all path
		for x in range(0,len(nodes)):
			for y in range(0,len(nodes)):
				pheromones[x][y] = (1 - ro)*pheromones[x][y]


		# Add pheromone to the tour
		dist = 0
		prev_node = nodes[0]
		for x in range(0,len(tour)):
			dist = dist + distance(prev_node[0],prev_node[1],tour[x][0],tour[x][1])
			prev_node = tour[x]

		prev_pos = 0
		for y in range(0,len(tour)):
			pos = getpos(tour[y])
			pheromones[prev_pos][pos] = pheromones[prev_pos][pos] + 1.0/dist
			prev_pos = pos 

		# compute best tour
		new_dist = 0
		prev_node = nodes[0]
		for x in range(0,len(tour)):
			new_dist = new_dist + distance(prev_node[0],prev_node[1],tour[x][0],tour[x][1])
			prev_node = tour[x]
		if min_dist > new_dist:
			min_dist = new_dist
			best_tour = tour

		print"-------------------------------------------------------"
		epoch = epoch -1					
	# print best tour 
	print "{} -->".format(depot[0]),
	for x in xrange(0,len(best_tour)):
		print "{} --> ".format(best_tour[x]),

tour_const()
# pygame.quit()
