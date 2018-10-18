if __name__ == "__main__":
	counter = 0 # num of repetitive elements
	num = 0 # total num of elements
	seen = set()
	index_to_remove = [] #note that since we use num as a counter, the index starts at 0

	id_file = open("/Users/eve/Desktop/past_ids", "w")
	name_file = open("/Users/eve/Desktop/past_name", "w")

	line = ""

	# load all processed article names
	with open("/Users/eve/Desktop/combined.txt") as f:

	    for line in f:
	    	if len(line.strip()) != 0:
	    		num+=1
		        line = line.strip('\n')
		        if line in seen:
		        	counter += 1
		        else:
					seen.add(line)
					# print line

	print "finish scanning original file"
 	print counter
 	print num

	counter = 0 # num of repetitive elements
	num = 0 # total num of elements

	with open("/Users/eve/Desktop/unique_name.txt") as f:

	    for line in f:
	    	if len(line.strip()) != 0:
	    		num+=1
		        line = line.strip('\n')
		        # print line
		        if line in seen:
		        	name_file.write(line)
		        	name_file.write('\n')
		        else:
					index_to_remove.append(num)
					counter+=1;


	print "file 2"
 	print counter
 	print num

	counter = 0 # num of repetitive elements
	num = 0 # total num of elements

	c = 0
	with open("/Users/eve/Desktop/unique_id.txt") as f:
	    for line in f:
	    	if len(line.strip()) != 0:
	    		num+=1
		        line = line.strip('\n')
		        if c<len(index_to_remove) and num == index_to_remove[c]:
		        	c +=1
		        	counter +=1
		        else:
					id_file.write(line)
					id_file.write('\n')


	print "file 3"
 	print counter
 	print num

