if __name__ == "__main__":
	counter = 0 # num of repetitive elements
	num = 0 # total num of elements
	seen = set()
	index_to_remove = [] #note that since we use num as a counter, the index starts at 0

	id_file = open("/Users/eve/Desktop/unique_art_id.txt", "w")
	name_file = open("/Users/eve/Desktop/unique_art_name.txt", "w")

	line = ""

	# with open("/Users/eve/Desktop/combined_artname.txt") as f:

	#     for line in f:
	#     	if len(line.strip()) != 0:
	#     		num+=1
	# 	        line = line.strip('\n')
	# 	        if line in seen:
	# 	        	counter += 1
	# 	        else:
	# 				seen.add(line)
	# 				# print line
	# f.close()
	# print "finish scanning original file"
 # 	print counter
 # 	print num

 	# locate and remove duplicates (store the removed line number to use for removing the artname)
	counter = 0 # num of repetitive elements
	num = 0 # total num of elements

	with open("/Users/eve/Desktop/combined_id.txt") as f:

	    for line in f:
	    	if len(line.strip()) != 0:
	    		num+=1
		        line = line.strip('\n')
		        # print line
		        if line in seen:
		        	index_to_remove.append(num)
		        	counter += 1
		        else:
					seen.add(line)
					id_file.write(line)
					id_file.write('\n')

	print "id file processed"
	f.close()
 	print counter
 	print num

 	# process article name file
	counter = 0 # num of repetitive elements
	num = 0 # total num of elements

	c = 0
	with open("/Users/eve/Desktop/combined_name.txt") as f:
	    for line in f:
	    	if len(line.strip()) != 0:
	    		num+=1
		        line = line.strip('\n')
		        if c<len(index_to_remove) and num == index_to_remove[c]:
		        	c +=1
		        	counter +=1
		        else:
					name_file.write(line)
					name_file.write('\n')


	print "name file processed"
	f.close()
 	print counter
 	print num

