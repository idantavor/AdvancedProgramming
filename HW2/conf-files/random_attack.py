import random

with open('random.attack','w') as f:
	for i in range(100):
		f.write("{}, {}\n".format(str(random.randint(1,10)),str(random.randint(1,10))))