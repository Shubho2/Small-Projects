import numpy as np
import matplotlib.pyplot as plt

maze = np.zeros((4,16))
maze.fill(1)

#create the maze    
maze[0][4]=0;maze[0][10]=0;maze[0][14]=0;maze[1][0]=0
maze[1][1]=0;maze[1][4]=0;maze[1][6]=0;maze[1][7]=0
maze[1][9]=0;maze[1][11]=0;maze[1][13]=0;maze[1][14]=0
maze[1][15]=0;maze[2][0]=0;maze[2][4]=0;maze[2][6]=0
maze[2][13]=0;maze[2][14]=0;maze[3][2]=0
maze[3][6]=0;maze[3][11]=0

state_dict = {}
# numbering empty cell
cell_no = 1
for i in range(0,4):
	for j in range(0,16):
		if maze[i][j] == 1:
			state_dict[i,j] = cell_no
			cell_no += 1
# coordinate contains position , cell_no pair
coordinate = {}
for i in range(1,44):
	for key,val in state_dict.items():
		coordinate[val] = key

# generate neighbour list
list_of_neighbours = []
# sensor_output has value of sensors at every stage 
sensor_output = []

list_of_neighbours.append(-1)
sensor_output.append(-1)
for _i in range(1,44):
	cell_no_list = []
	sensor_bits = []
	x,y = coordinate[_i]

	if( state_dict.has_key((x+1,y)) == True ):
		cell_no_list.append( state_dict[x+1,y] )
		sensor_bits.append(1)
	else:
		sensor_bits.append(0)
	if( state_dict.has_key((x-1,y)) == True ):
		cell_no_list.append( state_dict[x-1,y] )
		sensor_bits.append(1)
	else:
		sensor_bits.append(0)

	if( state_dict.has_key((x,y-1)) == True ):
		cell_no_list.append( state_dict[x,y-1] )
		sensor_bits.append(1)
	else:
		sensor_bits.append(0)
	if( state_dict.has_key((x,y+1)) == True ):
		cell_no_list.append( state_dict[x,y+1] )
		sensor_bits.append(1)
	else:
		sensor_bits.append(0)


	sensor_output.append(sensor_bits)
	list_of_neighbours.append( cell_no_list )

# generate a random walk of max_time steps
def random_walk(max_time):
	c_no = np.random.randint(1,44)
	path = []
	i = 1
	while(i <= max_time):
		path.append(c_no)
		if(len(list_of_neighbours[c_no]) == 0):
			continue
		c_no = list_of_neighbours[ c_no ][ np.random.randint(0,len(list_of_neighbours[c_no]) ) ]
		i += 1
	return path


def get_sensor_data(path, epsilon):
	ret = []
	for i in range(0, len(path)):
		censor_bits = []
		for j in range(0,4):
			rand_sample = np.random.random_sample(None)
			if( rand_sample <= epsilon ):
				censor_bits.append(int(1- sensor_output[path[i]][j] ))
			else:
				censor_bits.append(int( sensor_output[path[i]][j] ))
		ret.append(censor_bits)
	return ret


T_table = np.zeros((43,43))
def transition_table():
	for i in range(1,44):
		list1 = list_of_neighbours[i]
		if(len(list1) == 0):
			T_table[i-1][i-1] = 1
			continue
		for j in list1:
			T_table[i-1][j-1] = 1.0/float(len(list1))


# def print_Table():
# 	for i in range(0,43):
# 		for j in range(0,43):
# 			print (T_table[i][j]),
# 		print "\n"



# walk contains the actual robot walk
# print "Enter no of steps: (time)"
max_time = 90
path = random_walk(max_time)
print "Enter epsilon value (sensor error) : "
epsilon = float(input())
sensor_reading = get_sensor_data(path, epsilon)
transition_table()
# print_Table()

# Observation table creation
Observation = np.zeros((max_time,43,43))
for t in range(0,max_time):
	obs = sensor_reading[t]
	for i in range(0,43):
		dit = 0
		for j in range(0,4):
			if(obs[j] != sensor_output[i+1][j]):
				dit += 1
		Observation[t][i][i] = ((1.0 - epsilon)**(4.0 - dit))*((epsilon)**(dit))

# Filtering inference algorithm 
f = np.zeros(43)
f.fill(1.0/43.0)

forward_arr = []
forward_arr.append(f)
for t in range(0,max_time-1):
	f_ = np.matmul( Observation[t+1] , np.matmul(T_table.T , forward_arr[t] ) )
	sum_m = 0.0
	for j in range(0,43):
		sum_m += f_[j]
	for j in range(0,43):
		f_[j] = f_[j] / sum_m
	forward_arr.append(f_)

state_predicted = []
for i in range(0,len(forward_arr)):
	if(i == 0):
		state_predicted.append( np.random.randint(1,44) )
	else:
		state_predicted.append( 1 + np.argmax(forward_arr[i]) )



# viterbi algorithm

VT1 = np.zeros((43,max_time))
VT2 = np.zeros((43,max_time))
for i in range(0,43):
	VT1[i,0] = (1.0/43.0)*Observation[0][i][i]

Z = np.zeros(max_time)
X = np.zeros(max_time)
for t in range(1,max_time):
	for j in range(0,43):
		max_i = 0
		max_k = 0
		for _k in range(0,43):
			if( VT1[_k,t-1]*T_table[_k][j]*Observation[t][j][j] > max_i ):
				max_i = VT1[_k,t-1]*T_table[_k][j]*Observation[t][j][j]
				max_k = _k
		VT1[j,t] = max_i
		VT2[j,t] = max_k

max_i = 0
max_k = 0
for k in range(0,43):
	if( VT1[k][max_time-1] >= max_i ):
		max_i = VT1[k][max_time-1]
		max_k = k

Z[max_time-1] = max_k
X[max_time-1] = max_k

for i in list(reversed(range(1, max_time))):
	Z[i-1] = VT2[ int(Z[i]) ][i]
	X[i-1] = Z[i-1]

viterbi_path = []
for i in range(0,len(X)):
	viterbi_path.append( int(X[i]+1) )

# comparision of prediction at every step vs actual location
print "Random generated path: "
print path
print "Estimated robot location at particular times: "
print state_predicted
print "Most likely path (viterbi): "
print viterbi_path

def manhattan_distance(s1,s2):
	x1,y1 = coordinate[s1]
	x2,y2 = coordinate[s2]
	return abs(x2-x1) + abs(y2-y1)
j = 1
xval = []
yval = []
man_error = 0.0
for i in range(0,len(path)):
	man_error += manhattan_distance(path[i],state_predicted[i])
	if(i == 5*j):
		j = j+1
		yval.append(man_error/i)
		xval.append(i)

plt.plot(xval,yval)
plt.xlabel('Localization error')
plt.title('Localization error for epsilon ' + str(epsilon))
plt.ylabel('No of observations')
plt.savefig('localization_' + str(epsilon)+'.png')
plt.show()
plt.close()

j = 1
yval = []
xval = []
accuracy = 0.0
for i in range(0,len(path)):
	if(state_predicted[i] == viterbi_path[i]):
		accuracy += 1.0
	if(i == 5*j):
		j = j + 1
		yval.append(accuracy/i)
		xval.append(i)
accuracy = accuracy / float(len(path))
print "Accuracy got is: "+str(accuracy*100)+"%"
plt.plot(xval,yval)
plt.title('Path accuracy for epsilon ' + str(epsilon))
plt.xlabel('No of observations')
plt.ylabel('Path accuracy')
plt.savefig('pathaccuracy_' + str(epsilon)+'.png')
plt.show()
plt.close()