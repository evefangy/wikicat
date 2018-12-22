if __name__ == "__main__":
	counter = 0 # num of repetitive elements
	num = 0 # total num of elements
	line_num = 0 #inclusing empty lines
	seen = set()

	line = ""

	missing_art = "/Users/eve/Desktop/missing_art_index.txt"
	missing_art_file = open(missing_art, 'w')

	with open("/Users/eve/Desktop/Generated_Tree/q_0.5/ctr.txt",'r') as f3:
	    for line in f3:
	    	if len(line.strip()) != 0 :
	    		num+=1
		        line = line.strip('\n')
		        
		        if line in seen:
		        	counter += 1
		        else:
		           
		            seen.add(line)
	f3.close()

	print "Number of repetitions: "
 	print counter
 	print "Number of elements: "
 	print num
 	print "finish three file"


	num = 0
	counter = 0

	with open("/Users/eve/Desktop/Generated_Tree/used_article_index.txt",'r') as f2:
	    for line in f2:
	    	if len(line.strip()) != 0 :
	    		num+=1

		        line = line.strip('\n')

		        if line not in seen:
		        	missing_art_file.write(line)
		        	missing_art_file.write('\n')
		        	counter += 1

	f2.close()

	missing_art_file.close()

	print "Number of missing: "
 	print counter
 	print "Number of elements: "
 	print num
 	