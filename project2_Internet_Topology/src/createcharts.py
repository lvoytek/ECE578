#!venv/bin/python

import ASClassification
import ASTopology

def main():
	classification = ASClassification.ASClassification('data/ASClassification.txt')
	classification.run()
	classification.show()

	topology = ASTopology.ASTopology('data/ASClassification.txt', 'data/ASRelationships.txt', 'data/prefix2as.txt', 'data/prefix2as_6.txt', 'data/ASOrganizations.txt', 'data/as2orgid.txt')
	topology.run()
	topology.show()


if __name__ == '__main__':
	main()