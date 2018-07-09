import numpy
import matplotlib.pyplot as plt

filename = 'guru4.txt'
raw_data = open(filename, 'rt')
data1 = numpy.loadtxt(raw_data, delimiter=" ")


filename = 'guru6.txt'
raw_data = open(filename, 'rt')
data2 = numpy.loadtxt(raw_data, delimiter=" ")

filename = 'guru9.txt'
raw_data = open(filename, 'rt')
data3 = numpy.loadtxt(raw_data, delimiter=" ")

    
plt.plot(data1[:,[0]], data1[:,[1]], label='$\epsilon$ = 0.4')
plt.plot(data1[:,[0]], data2[:,[1]], label='$\epsilon$ = 0.6')
plt.plot(data1[:,[0]], data3[:,[1]], label='$\epsilon$ = 0.9')

plt.xlabel('epoch')
plt.ylabel('best_path_distance')
plt.ylim(5000,7000)

plt.title("minimized_distance vs epoch for different evaporation rate")

plt.legend()

plt.savefig('best_distance_2000_rho.png')

plt.show()

