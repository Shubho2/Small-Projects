import numpy as np
import math
from numpy import linalg as la
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import time


# standard deviation and No of Samples
sigma,N = 50,1500

#Array of samples
pts = []


# Target distribution
def q(x):
	return (np.e**(-(x**4))*(2+np.sin(5*x)+np.sin(-2*(x**2))))


def metropolis(N):
	r = -1         # Initialization
	p = q(r)	   
	pts = []

	for i in range(N):
		rn = np.random.normal(r,sigma)  # Proposal Distribution : Gaussian Distribution
		pn = q(rn)                    # Candidate's Probability 
		if pn >= p:
		    p = pn
		    r = rn
		else:
		    u = np.random.rand()
		    if u < pn/p:
				p = pn
				r = rn
		pts.append(r)

	pts = np.array(pts)
	return pts
    
def hist_plot(array):

	# plot the sample values
	plt.plot(array,'-')
	# Create a 5% (0.05) and 10% (0.1) padding in the 
	# x and y directions respectively.
	plt.title('Generated Sample vs Iteration for sigma '+ str(sigma))
	plt.xlabel('Iteration')
	plt.ylabel('Sample Value')
	# plt.margins(0.05, 0.1)
	plt.savefig('samples_'+str(sigma)+'.png')
	# plt.show()
	plt.close()

	t = np.arange(-3, 3, 0.1)
	fig = plt.figure()
	ax = fig.add_subplot(1,1,1,)
	color = 'tab:blue'
	ax.set_xlabel('x')
	ax.set_ylabel('Sample frequency', color=color)
	ax.hist(array, bins=1000, color = color) 

	ax2 = ax.twinx() 
	color = 'tab:green'
	ax2.set_ylabel('Probability density', color=color)
	ax2.plot(t,q(t),color = color)   

	# plt.margins(0.05, 0.1)
	plt.title('Histogram of Samples for sigma ' + str(sigma))
	plt.savefig('histogram_'+str(sigma)+'.png')
	plt.show()
	plt.close()
	
hist_plot(metropolis(N))

