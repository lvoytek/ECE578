import matplotlib.pyplot as plt


class ASClassification:
	def __init__(self, classificationfilename):
		self._classificationFilename = classificationfilename
		self._transits = []
		self._contents = []
		self._enterprises = []

	def run(self):
		cfile = open(self._classificationFilename, 'r')

		for line in cfile:
			if not '#' in line:
				line = line.split('|')

				if 'Content' == line[2].strip():
					self._contents.append((line[0], line[1]))
				elif 'Transit/Access' == line[2].strip():
					self._transits.append((line[0], line[1]))
				elif 'Enterprise' == line[2].strip():
					self._enterprises.append((line[0], line[1]))

		cfile.close()

	def show(self):
		labels = 'Transit ASes', 'Content ASes', 'Enterprise ASes'
		sizes = [len(self._transits), len(self._contents), len(self._enterprises)]
		explode = (0, 0, 0)

		fig1, ax1 = plt.subplots()
		ax1.pie(sizes, explode=explode, labels=labels, autopct='%1.1f%%', shadow=False, startangle=90)
		ax1.axis('equal')
		plt.savefig('output/asclassification.png', dpi=300, edgecolor='w', format='png', pad_inches=0.1)
		plt.show()
