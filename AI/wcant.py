import math
import pygame
import random
import gi
import numpy
import matplotlib.pyplot as plt

# #creating object for gui
# gi.require_version("Gtk", "3.0")
# from gi.repository import Gtk

# # initialization for pygame animation
# pygame.init()

# BLACK = (  0,   0,   0)
# WHITE = (255, 255, 255)
# BLUE =  (  0,   0, 255)
# GREEN = (  0, 255,   0)
# RED =   (255,   0,   0)

# color = [BLACK, BLUE, GREEN, RED]
# myFont = pygame.font.SysFont("Roboto", 18)

# # Set the height and width of the screen
# size = [800, 500]
# screen = pygame.display.set_mode(size)
# pygame.display.set_caption("Vehicular routing")

# global constants
alpha = 1
beta = 2
# ro = .9

#capacity of each vehicle
capacity = 80


# Manhattan distance
def distance(x1, y1, x2, y2):
    dist = abs(x1 - x2) + abs(y1 - y2)
    return dist

#diferent types of nodes and their locations
# depot = [[100,150]]
# disposal = [[500,100],[400,200]]
# customer = [[350, 100],[400, 100],[700, 150],[350, 200],[500, 200]]
# nodes = [[100,150],[350, 100],[400, 100],[700, 150],[350, 200],[500, 200],[500,100],[400,200]]
# network = [[100,150],[350, 100],[400, 100],[700, 150],[350, 200],[500, 200]]

filename = 'aa'
raw_data = open(filename, 'rt')
data = numpy.loadtxt(raw_data, delimiter=",")

depot = []
disposal = []
customer = []
nodes = []
network = []
time_window = []
visited = []
# iteration = []
# length_of_path = []

for x in xrange(0,data.shape[0]):
    if data[x][4] == 0:
        depot.append([data[x][0],data[x][1]])
        network.append([data[x][0],data[x][1]])
        time_window.append([data[x][2],data[x][3]])
    elif data[x][4] == 1:
        customer.append([data[x][0],data[x][1]])
        network.append([data[x][0],data[x][1]])
        time_window.append([data[x][2],data[x][3]])
    else:
        disposal.append([data[x][0],data[x][1]])

    nodes.append([data[x][0],data[x][1]])

speed = 300

	


# travel time of the vehicle between each and every nodes
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
	garbage.append(random.randint(1,capacity/2))
	print "garbage for {} is {}".format(x,garbage[x])
garbage[0] = 0 # for depot

# Initialization of pheromones on each path
def init():
	pheromones = [[1.0 for i in range(len(nodes))] for i in range(len(nodes))]
	for x in xrange(0,len(nodes)):
		for y in xrange(0,len(nodes)):
			if x == y:
				pheromones[x][y] = -1.0
	capacity = 80

	for x in xrange(0,len(network)):
		visited.append(0)

	return pheromones

#attractive coefficient		
eta = [[0.0 for i in range(len(network))] for i in range(len(network))]


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

# initializing eta values (1/dist) for each pair of nodes in the network
for i in range(0,len(network)):
	for j in range(0,len(network)):
		if distance(network[i][0],network[i][1],network[j][0],network[j][1]):
			eta[i][j] = 10000.0/distance(network[i][0],network[i][1],network[j][0],network[j][1])

# checking if the node is depot or not
def isDepot(node):
	for x in xrange(0,len(depot)):
		if node[0] == depot[x][0] and node[1] == depot[x][1]:
			return 1
	return 0

# checking if the node is disposal or not
def isDisposal(node):
	for x in xrange(0,len(disposal)):
		if node[0] == disposal[x][0] and node[1] == disposal[x][1]:
			return 1
	return 0


# checking if it is perfect neighbour or not
def isPerfectNeighbour(start_time,remaining,i,j):
	if i != j:
		total_time = start_time + travel_time[i][j]
		if total_time >= time_window[j][0] and total_time <= time_window[j][1] and remaining >= garbage[j]:
			return 1
		else:
			return 0
	else:
		return 0

# checking if it is simple neighbour or not
def isNeighbour(start_time,remaining,i,j):
	if i != j:
		total_time = start_time + travel_time[i][j]
		if total_time <= time_window[j][1] and remaining >= garbage[j]:
			return 1
		else:
			return 0
	else:
		return 0


# checking if all the nodes have been visited or not
def allNotVisited():
	for col in range(0,len(visited)):
		if (not visited[col]):
			return 1
	return 0

# attractive coefficient is following nearest neighbour heuristic
def attract_coeff(x,y):
	return eta[x][y]

# calculating probability for the path (x,neighbour[y]) with respect to the neighbour set
def calculate_prob(pheromones,neighbour_set,x,y):
	numerator = pheromones[x][neighbour_set[y]]**alpha * eta[x][neighbour_set[y]]**beta
	denominator = 0.0
	# print "denominator : "
	for n in range(0,len(neighbour_set)):
		# print pheromones[x][neighbour_set[n]]**alpha * eta[x][neighbour_set[n]]**beta
		if x != neighbour_set[n]:
			denominator = denominator + pheromones[x][neighbour_set[n]]**alpha * eta[x][neighbour_set[n]]**beta
	if denominator != 0.0:
		return numerator / denominator
	else:
		return 0.0
		# print "Exception!! division by zero"

# checking if we have found any neighbour or not
def isNotEmpty(x):
	if(len(x)):
		return 1
	else:
		return 0

# finding the nearest disposal to dump the waste
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


# tour construction using ant colony optimization
def tour_const(x):
	pheromones = init()
	ro = x
	f = open("uru.3","a+")
	epoch = 2000
	temp_epoch = epoch
	min_dist = 10000000
	# keeps best tour calculated after each epoch
	length_of_path = []
	best_tour = []
	while epoch:
		start_time = 0.0 # initializing start time as 0
		curr = 0 # initially depot
		visited[curr] = 1 
		next_node = 0
		remaining = capacity
		tour = []
		tour.append(depot[0])
		while allNotVisited():
			perfect_neighbour_set = []
			neighbour_set = []
			# print "start_time: {}".format(start_time)
			# print "remaining : {}".format(remaining)
			for x in xrange(0,len(network)):
				# find all neighbour of x
				if isPerfectNeighbour(start_time,remaining,curr,x) and visited[x] == 0:
					perfect_neighbour_set.append(x)
			# if neighbour set not empty select next node
			if isNotEmpty(perfect_neighbour_set):
				# print "perfect found"
				# print "Perfect neighbours: "
				# for x in xrange(0,len(perfect_neighbour_set)):
				# 	print "{} --> {}".format(curr,perfect_neighbour_set[x])
				q = random.uniform(0,1)
				maxi = 0.0
				if q > 0.3:
					for j in range(0,len(perfect_neighbour_set)):
						prob = calculate_prob(pheromones,perfect_neighbour_set,curr,j)
						# print "perfect node: {} prob : {}".format(perfect_neighbour_set[j],prob)
						if maxi < prob:
							maxi = prob
							next_node = perfect_neighbour_set[j]

				# randomly choosing for exploration
				else:
					for j in range(0,len(perfect_neighbour_set)):
						prob = attract_coeff(curr,perfect_neighbour_set[j])
						# print "perfect node: {} prob : {}".format(perfect_neighbour_set[j],prob)
						if maxi < prob:
							maxi = prob
							next_node = perfect_neighbour_set[j]

				# print "next node perfect nei: {}".format(next_node)
				# update next neighbour
				start_time = start_time + travel_time[curr][next_node] + service_time
				remaining = remaining - garbage[next_node]
				curr = next_node
				visited[curr] = 1
				# keep path
				tour.append(network[curr])

			else:
				for x in xrange(0,len(network)):
					if isNeighbour(start_time,remaining,curr,x) and visited[x] == 0:
						neighbour_set.append(x)

				if isNotEmpty(neighbour_set):
					# print "Simple neighbours: "
					# for x in xrange(0,len(neighbour_set)):
					# 	print "{} --> {}".format(curr,neighbour_set[x])
					q = random.uniform(0,1)
					maxi = 0.0
					if q > 0.3:
						for j in range(0,len(neighbour_set)):
							prob = calculate_prob(pheromones, neighbour_set,curr,j)
							# print "simple node: {} prob : {}".format(neighbour_set[j],prob)
							if maxi < prob:
								maxi = prob
								next_node = neighbour_set[j]
					
					# randomly choosing for exploration
					else:
						for j in range(0,len(neighbour_set)):
							prob = attract_coeff(curr,neighbour_set[j])
							# print "simple node: {} prob : {}".format(neighbour_set[j],prob)
							if maxi < prob:
								maxi = prob
								next_node = neighbour_set[j]

					# update next neighbour
					# print "next node simple nei: {}".format(next_node)
					start_time = time_window[next_node][0] + service_time
					remaining = remaining - garbage[next_node]
					curr = next_node
					visited[curr] = 1
					# keep path
					tour.append(network[curr])
				else:
					for x in xrange(1,len(network)):
						if remaining >= garbage[x]:
							break
					if x == len(customer):
						# goto disposal
						remaining = capacity
						# find nearest disposal
						disp = nearest_disposal(curr)
						tour.append(disposal[disp])
						pos = getpos(disposal[disp])
						start_time = start_time + travel_time[curr][pos]*2 + service_time
					else:
						# print "[curr] : before comimg to depot : {}".format(curr)
						# Use another vehicle to satisfy other customers
						start_time = 0.0
						curr = 0
						remaining = capacity
						tour.append(depot[0])
						
			# for x in xrange(0,len(tour)):
			# 	if isDepot(tour[x]) and not(x == 0):
			# 		print "Vehicle change --> ",
			# 	if isDisposal(tour[x]):
			# 		print "Going to disposal --> ",
			# 	print "{} --> ".format(tour[x]),
			# print ""

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
			if not isDepot(tour[y]):	
				pheromones[prev_pos][pos] = pheromones[prev_pos][pos] + 1.0/dist
			prev_pos = pos 

		# compute best tour
		new_dist = 0
		prev_node = nodes[0]
		for x in range(0,len(tour)):
			if not isDepot(tour[x]):
				new_dist = new_dist + distance(prev_node[0],prev_node[1],tour[x][0],tour[x][1])
			prev_node = tour[x]
		if min_dist > new_dist:
			min_dist = new_dist
			best_tour = tour

		print"-------------------------------------------------------"
		# iteration.append(temp_epoch-epoch)
		f.write("%d "% (temp_epoch-epoch))
		f.write("%d\n"% min_dist)
		length_of_path.append(min_dist)
		epoch = epoch - 1			
	# print best tour 
	# print iteration
	print length_of_path


	for x in xrange(0,len(best_tour)):
		if isDepot(best_tour[x]) and not(x == 0):
			print "Vehicle change --> ",
		if isDisposal(best_tour[x]):
			print "Going to disposal --> ",
		print "{} --> ".format(best_tour[x]),
	print ""
	f.close()


tour_const(0.9)

# ind = .1
# for x in xrange(0,len(paths)):
# 	plt.plot([1,1000],paths[x])
# 	plt.title('epoch_1000 vs best_path_distance_' + str(.6+ind))
# 	plt.ylim(5000,7000)
# 	plt.savefig('best_distance_1000_' + str(.6+ind)+'.png')
# 	ind = ind + .1

# plt.xlabel('epoch')
# plt.ylabel('best_path_distance')
# plt.legend()
# plt.show()

# pygame.quit()
