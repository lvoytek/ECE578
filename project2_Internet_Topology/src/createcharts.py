#!venv/bin/python

import ASClassification


def main():
	classification = ASClassification.ASClassification('data/ASClassification.txt')
	classification.run()
	classification.show()


if __name__ == '__main__':
	main()