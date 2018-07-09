import math
import pygame

pygame.init()

BLACK = (  0,   0,   0)
WHITE = (255, 255, 255)
BLUE =  (  0,   0, 255)
GREEN = (  0, 255,   0)
RED =   (255,   0,   0)

color = [BLACK, BLUE, GREEN, RED]
myFont = pygame.font.SysFont("Roboto", 18)

# Set the height and width of the screen
size = [800, 500]
screen = pygame.display.set_mode(size)
pygame.display.set_caption("Vehicular routing")

# global constants
alpha = 1
beta = 2
ro = .5
eta = [[0.0 for i in range(14)] for i in range(14)]

def distance(x1, y1, x2, y2):
    # Manhattan distance
    dist = abs(x1 - x2) + abs(y1 - y2)
    return dist

nodes = [[350, 100], [700, 100], [200, 200], [400, 400], [650, 300]]

# pheromones = [[-1.0, 20.0, 14.0, 10.0, 2.0, 7.0, 3.0, 20.0, 3.0, 40.0, 1.0, 22.0, 6.0, 20.0],
# [20.0, -1.0, 2.0, 5.0, 4.0, 33.0, 10.0, 30.0, 3.0, 12.0, 42.0,19.0, 8.0, 21.0],
# [14.0, 2.0, -1.0, 10.0, 3.0, 22.0, 10.0, 3.0, 2.0, 33.0, 23.0, 7.0, 27.0, 5.0],
# [10.0, 5.0, 10.0, -1.0, 6.0, 20.0, 20.0, 11.0, 21.0, 21.0,73.0, 6.0, 14.0, 20.0],
# [2.0, 4.0, 3.0, 6.0, -1.0, 1.0, 2.0, 40.0, 12.0, 18.0, 17.0, 25.0, 30.0, 7.0]]
pheromones = [[1.0 for i in range(len(nodes))] for i in range(len(nodes))]
for x in xrange(0,len(nodes)):
	for y in xrange(0,len(nodes)):
		if x == y:
			pheromones[x][y] = -1.0
		

# [7.0, 33.0, 22.0, 20.0, 1.0, -1.0, 40.0, 5.0, 3.0, 2.0,3.0, 11.0, 10.0, 33.0],
# [3.0, 10.0, 10.0, 20.0, 2.0, 40.0, -1.0, 8.0, 4.0, 7.0, 8.0, 24.0, 5.0, 13.0],
# [20.0, 30.0, 3.0, 11.0, 40.0, 5.0, 8.0, -1.0, 9.0, 11.0,4.0, 12.0, 3.0, 19.0],
# [3.0, 3.0, 2.0, 21.0, 12.0, 3.0, 4.0, 9.0, -1.0, 12.0, 42.0, 33.0, 21.0, 18.0],
# [40.0, 12.0, 33.0, 21.0, 18.0, 2.0, 7.0, 11.0, 12.0,-1.0, 6.0, 3.0, 17.0, 4.0],
# [1.0, 42.0, 23.0, 73.0, 17.0, 3.0, 8.0, 4.0, 42.0, 6.0, -1.0, 6.0, 26.0, 8.0],
# [22.0, 19.0, 7.0, 6.0, 25.0, 11.0, 24.0, 12.0, 33.0,1.0, 6.0, -1.0, 20.0, 15.0],
# [6.0, 8.0, 27.0, 14.0, 30.0, 10.0, 5.0, 3.0, 21.0, 17.0, 26.0, 20.0, -1.0, 18.0],
# [20.0, 21.0, 5.0, 20.0, 7.0, 33.0, 13.0, 19.0,18.0, 4.0, 8.0, 15.0, 18.0, -1.0]] 

tour = [[0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0]]
# [0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0],
# [0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0],
# [0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0],
# [0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0],
# [0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0]] 

visited = [0,0,0,0,0]

# , [70, 100], [50, 100],
# [300, 300],[250, 300], [200, 300], [150, 300], [100, 300], [70, 300], [50, 300]]

def create_node():
	for x in xrange(0,len(nodes)):
		pygame.draw.rect(screen, WHITE , [nodes[x][0], nodes[x][1], 20,20])
		pos = myFont.render(str(x), 3, RED)
		screen.blit(pos, (nodes[x][0], nodes[x][1]))
	pygame.display.flip()		

def reset_color():
	for x in xrange(0,len(nodes)):
		for y in xrange(0,len(nodes)):
			pygame.draw.line(screen, BLUE, [nodes[x][0], nodes[x][1]], [nodes[y][0],nodes[y][1]], 1)
	pygame.display.flip()		

for i in range(0,len(nodes)):
	for j in range(0,len(nodes)):
		if distance(nodes[i][0],nodes[i][1],nodes[j][0],nodes[j][1]):
			eta[i][j] = 1.0/distance(nodes[i][0],nodes[i][1],nodes[j][0],nodes[j][1])


def isNeighbour(i,j):
	if pheromones[i][j] < 0:
		return 0
	else:
		return 1

def allNotVisited():
	for col in range(0,len(visited)):
		if not visited[col]:
			return 1
	return 0

def calculate_prob(x,y):
	numerator = pheromones[x][y]**alpha * eta[x][y]**beta
	denominator = 0.0
	for n in range(0,len(nodes)):
		if x != n:
			denominator = denominator + pheromones[x][n]**alpha * eta[x][n]**beta
	if denominator != 0.0:
		return numerator / denominator
	else:
		print "Exception!! division by zero"

def tour_const():
	epoch = 0
	epoch_no = 1000
	index = 0
	done = False
	clock = pygame.time.Clock()
	screen.fill(BLACK)
	create_node()
	reset_color()
	while not done:
		while epoch < epoch_no:
			# for each ant do following
			eno = myFont.render("Epoch no: ",1, GREEN)
			epos = myFont.render(str(epoch), 1, GREEN)
			screen.blit(eno,(680,10))
			screen.blit(epos, (770, 10))
			pygame.display.flip()
			for k in range(0,len(nodes)):
				ant = myFont.render("Ant no: ",1, GREEN)
				antpos = myFont.render(str(k), 1, GREEN)
				screen.blit(ant,(680,30))
				screen.blit(antpos, (770, 30))
				pygame.display.flip()

				curr = k
				visited[curr] = 1  
				iteration = 0
				while allNotVisited():
					maxi = 0.0
					for j in range(0,len(nodes)):
						if isNeighbour(curr,j) and visited[j] == 0:
							prob = calculate_prob(curr,j)
							if maxi < prob:
								maxi = prob
								next_node = j
					# update next neighbour
					curr = next_node
					visited[curr] = 1
					# keep path
					tour[k][iteration] = curr
					iteration = iteration + 1
				tour[k][len(nodes)-1] = k  # source added at last pos
				# print tour[k]
				for x in xrange(0,len(nodes)):
					print tour[k][x],
				pygame.time.wait(5000) # Wait one second between frames.
				nn = k
				for x in xrange(0,len(nodes)):
					print nn,x
					pygame.draw.line(screen, GREEN, [nodes[nn][0], nodes[nn][1]], [nodes[tour[k][x]][0],nodes[tour[k][x]][1]], 1)
					nn = tour[k][x]
					pygame.display.flip()
					pygame.time.wait(1000) # Wait one second between frames.
				reset_color()
				# reset visited for next ant
				for x in range(0,len(visited)):
				 	visited[x] = 0
				ant = myFont.render("Ant no: ",1, BLACK)
				prev = myFont.render(str(k), 1, BLACK)
				screen.blit(ant,(680,30))
				screen.blit(prev, (770, 30))
				pygame.display.flip()
			# Update pheromone (decay) to all path
			for x in range(0,len(nodes)):
				for y in range(0,len(nodes)):
					pheromones[x][y] = (1 - ro)*pheromones[x][y]

			# Add pheromone to the tour
			for x in range(0,len(nodes)):
				curr = x
				dist = 0
				for y in xrange(0,len(nodes)):
					dist = dist + distance(nodes[curr][0],nodes[curr][1],nodes[tour[x][y]][0],nodes[tour[x][y]][1])
					curr = tour[x][y]
					# print 1.0/dist
				for y in range(0,len(nodes)):
					pheromones[curr][tour[x][y]] = pheromones[curr][tour[x][y]] + 1.0/dist

			eno = myFont.render("Epoch no: ",1, BLACK)
			epos = myFont.render(str(epoch), 1, BLACK)
			screen.blit(eno,(680,10))
			screen.blit(epos, (770, 10))
			pygame.display.flip()

			# compute best path from tour matrix
			best = float("inf")
			# index = 0
			for x in xrange(0,len(nodes)):
				curr = x
				cost = 0
				for y in xrange(0,len(nodes)):
					cost = cost + distance(nodes[curr][0],nodes[curr][1],nodes[tour[x][y]][0],nodes[tour[x][y]][1])
					curr = tour[x][y]
				if cost < best :
					best = cost
					index = x

			cur = index
			for x in xrange(0,len(nodes)):
				# print tour[index][x],
				pygame.draw.line(screen, RED, [nodes[cur][0], nodes[cur][1]], [nodes[tour[index][x]][0],nodes[tour[index][x]][1]], 1)
				cur = tour[index][x]
				pygame.display.flip()
				pygame.time.wait(3000) # Wait one second between frames.
			epoch = epoch + 1
			if epoch == epoch_no:
				reset_color()
			

		clock.tick(10)
		for event in pygame.event.get(): # User did something
		    if event.type == pygame.QUIT: # If user clicked close
		        done=True # Flag that we are done so we exit this loop


tour_const()
pygame.quit()
