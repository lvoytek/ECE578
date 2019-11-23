import matplotlib.pyplot as plt
from math import pow as pw

class ASTopologyNode:
	def __init__(self, as_name):
		self._as_name = as_name
		self._degree = 1
		self._customers = []
		self._ip_prefixes = []

	def add_degree(self):
		self._degree += 1

	def add_customer(self, customer):
		self._customers.append(customer)

	def add_prefix(self, prefix, prefix_length, is_ipv6=False):
		self._ip_prefixes.append((prefix, prefix_length, is_ipv6))

	def get_degree(self):
		return self._degree

	def get_customers(self):
		return self._customers

	def get_number_of_customers(self):
		return len(self._customers)

	def get_ip_prefixes(self):
		return self._ip_prefixes

	def get_number_of_ipv4_addresses(self):
		count = 0

		# For each prefix entry, the number of owned addresses is 2^(32bits - prefix length)
		for prefix in self._ip_prefixes:
			if not prefix[2]:
				count += pw(2, 32-prefix[1])

		return count

	def get_number_of_ipv6_addresses(self):
		count = 0

		# For each prefix entry, the number of owned addresses is 2^(128bits - prefix length)
		for prefix in self._ip_prefixes:
			if prefix[2]:
				count += pw(2, 128 - prefix[1])

		print(count)
		return count


class ASTopology:

	def __init__(self, relationships_filename, prefix2as_ipv4filename, prefix2as_ipv6filename):
		self._relationships_filename = relationships_filename
		self._prefix2as4filename = prefix2as_ipv4filename
		self._prefix2as6filename = prefix2as_ipv6filename
		self._as_data = dict()

	def run(self):
		relationships = open(self._relationships_filename, 'r')

		for line in relationships:
			if '#' not in line:
				line = line.split('|')
				line[0] = int(line[0])
				line[1] = int(line[1])
				line[2] = int(line[2])

				# Add to node degrees of both items
				if line[0] in self._as_data:
					self._as_data[line[0]].add_degree()
				else:
					self._as_data[line[0]] = ASTopologyNode(line[0])

				if line[1] in self._as_data:
					self._as_data[line[1]].add_degree()
				else:
					self._as_data[line[1]] = ASTopologyNode(line[0])

				# Add customer if applicable
				if line[2] == -1:
					self._as_data[line[0]].add_customer(line[1])

		relationships.close()

		prefix2as4 = open(self._prefix2as4filename, 'r')

		for line in prefix2as4:
			line = line.split()
			line[1] = int(line[1])
			line[2] = int(line[2].split('_')[0].split(',')[0])

			if line[2] in self._as_data:
				self._as_data[line[2]].add_prefix(line[0], line[1])
			else:
				self._as_data[line[2]] = ASTopologyNode(line[1])
				self._as_data[line[2]].add_prefix(line[0], line[1])

		prefix2as4.close()

		prefix2as6 = open(self._prefix2as6filename, 'r')

		for line in prefix2as6:
			line = line.split()
			line[1] = int(line[1])
			line[2] = int(line[2].split('_')[0].split(',')[0])

			if line[2] in self._as_data:
				self._as_data[line[2]].add_prefix(line[0], line[1], True)
			else:
				self._as_data[line[2]] = ASTopologyNode(line[1])
				self._as_data[line[2]].add_prefix(line[0], line[1], True)

		prefix2as6.close()

	def show(self):
		self._show_node_degree()
		self._show_ip_space_v4()
		self._show_ip_space_v6()

	def _show_node_degree(self):
		# Show AS node degree distribution
		bins = [0, 0, 0, 0, 0, 0]

		for item in self._as_data.values():
			if item.get_degree() == 1:
				bins[0] += 1
			elif item.get_degree() <= 5:
				bins[1] += 1
			elif item.get_degree() <= 100:
				bins[2] += 1
			elif item.get_degree() <= 200:
				bins[3] += 1
			elif item.get_degree() <= 1000:
				bins[4] += 1
			else:
				bins[5] += 1

		bin_names = ["1", "2-5", "5-100", "100-200", "200-1000", ">1000"]

		fig1, ax1 = plt.subplots()
		ax1.bar([0, 1, 2, 3, 4, 5], bins)
		plt.xticks([0, 1, 2, 3, 4, 5], bin_names)
		plt.title("AS Node Degree Distribution")
		plt.xlabel("# of Distinct Links")
		plt.ylabel("Number of ASes")
		plt.savefig('output/node_degree.png', dpi=300, edgecolor='w', format='png', pad_inches=0.1)
		plt.show()

	def _show_ip_space_v4(self):
		bins = [0, 0, 0, 0, 0, 0]

		for item in self._as_data.values():
			if item.get_number_of_ipv4_addresses() < 1000:
				bins[0] += 1
			elif item.get_number_of_ipv4_addresses() <= 10000:
				bins[1] += 1
			elif item.get_number_of_ipv4_addresses() <= 100000:
				bins[2] += 1
			elif item.get_number_of_ipv4_addresses() <= 1000000:
				bins[3] += 1
			elif item.get_number_of_ipv4_addresses() <= 10000000:
				bins[4] += 1
			else:
				bins[5] += 1

		bin_names = ["<1000", "1000-10K", "10K-100K", "100K-1M", "1M-10M", ">10M"]

		fig1, ax1 = plt.subplots()
		ax1.bar([0, 1, 2, 3, 4, 5], bins)
		plt.xticks([0, 1, 2, 3, 4, 5], bin_names)
		plt.title("AS IP Space Distribution (IPv4)")
		plt.xlabel("# Assigned IPv4 Addresses")
		plt.ylabel("Number of ASes")
		plt.savefig('output/ip_space_ipv4.png', dpi=300, edgecolor='w', format='png', pad_inches=0.1)
		plt.show()

	def _show_ip_space_v6(self):
		bins = [0, 0, 0, 0, 0, 0]

		for item in self._as_data.values():
			if item.get_number_of_ipv6_addresses() == 0:
				bins[0] += 1
			elif item.get_number_of_ipv6_addresses() <= 1.0e+24:
				bins[1] += 1
			elif item.get_number_of_ipv6_addresses() <= 1.0e+26:
				bins[2] += 1
			elif item.get_number_of_ipv6_addresses() <= 1.0e+28:
				bins[3] += 1
			elif item.get_number_of_ipv6_addresses() <= 1.0e+30:
				bins[4] += 1
			else:
				bins[5] += 1

		bin_names = ["0", "0-24", "24-26", "26-28", "28-30", "30+"]

		fig1, ax1 = plt.subplots()
		ax1.bar([0, 1, 2, 3, 4, 5], bins)
		plt.xticks([0, 1, 2, 3, 4, 5], bin_names)
		plt.title("AS IP Space Distribution (IPv6)")
		plt.xlabel("log(# Assigned IPv6 Addresses)")
		plt.ylabel("Number of ASes")
		plt.savefig('output/ip_space_ipv6.png', dpi=300, edgecolor='w', format='png', pad_inches=0.1)
		plt.show()