if __name__ == "__main__":
	counter = 0 # num of repetitive elements
	num = 0 # total num of elements
	# line_num = 0;
	seen = set()
	with open("/Users/eve/Desktop/Wiki/Life/article.txt",'r') as f:
	    
	    for line in f:
	    	# line_num+=1
	    	if len(line.strip()) != 0 :
	    		num+=1
		        line = line.lower()
		        line = line.strip('\n')
		        if line == "tibetan_meditation":
		        	print line
		        	print num
		        if line in seen:
		            print(line)
		            counter += 1
		        else:
		            seen.add(line)

	# with open("/Users/eve/Desktop/Wiki/Life/category_2.txt") as f:
	#     for line in f:
	#     	if len(line.strip()) != 0 :
	#     		num+=1
	# 	        line = line.lower()
	# 	        line = line.strip('\n')
	# 	        if line in seen:
	# 	            print(line)
	# 	            counter += 1
	# 	        else:
	# 	            seen.add(line)
 	print counter
 	print num
 	# print line_num