# The code parse and reformat the revision history to the following form

# article title, major editor 1, major editor 2...

# Note 1. The major author is defined as authors with more than 10% edition history compare to the whole article
#      2. Unregistered editor is not recorded in the output
#      3. Users with a comma in the name is considered and the comma portion of the name is replaced by "_,"
#	   4. Articles with size smaller than 100B doesn't have their major editors recorded

import sys

if __name__ == "__main__":
	

	output_address = "/Users/eve/Desktop/not_exits.txt"
	outfile = open(output_address, "w")

	used_address = "/Users/eve/Dropbox/Wiki/two_trees/used_articles.txt"

	parsed_artname_addr = "/Volumes/easystore/Wiki/COLING/All_Materials_for_Paper/Experiment/2_Parsed_Rev_History/artname_0_104624.txt"


	c = 0


	part1 = set()
	
	name = ""

	num = 0


	with open(parsed_artname_addr,'r') as a_f:		
		for line in a_f:
			line = line.strip()
			if not line:
				print "detect empty line"
			else:
				parsed = line.split(', ')
				part1.add(parsed[0])
				
	a_f.close()
	print "Parsed 1 has .. articles: "
	print(len(part1))

	
	exited_counter = 0
	with open(used_address,'r') as f:
		for line in f:
			
			line = line.strip()
			if len(line)>0:
				c+=1
				if line in part1:
					exited_counter += 1
				else:
					outfile.write(line)
					outfile.write('\n')
	f.close()
			
	
	print(c)
	print(exited_counter) 




