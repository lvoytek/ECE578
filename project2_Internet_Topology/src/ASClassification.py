import matplotlib.pyplot as plt


class ASClassification:
	def __init__(self, classificationfilename):
		self._classificationFilename = classificationfilename
		self._transits = []
		self._contents = []
		self._enterprises = []

	def run(self):
		cfile = open(self._classificationFilename, 'r')

	def show(self):
		labels = 'Transit ASes', 'Content ASes', 'Enterprise ASes'
		sizes = [len(self._transits), len(self._contents), len(self._enterprises)]
		explode = (0, 0, 0)

		fig1, ax1 = plt.subplots()
		ax1.pie(sizes, explode=explode, labels=labels, autopct='%1.1f%%', shadow=True, startangle=90)
		ax1.axis('equal')
		plt.show()