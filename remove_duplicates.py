if __name__ == "__main__":
	counter = 0 # num of repetitive elements
	num = 0 # total num of elements
	seen = set()
	name = "category"
	art_file = open("/Users/eve/Desktop/Wiki/Life/"+name+"_.txt", "w")
	with open("/Users/eve/Desktop/Wiki/Life/"+name+".txt") as f:
	   
	    for line in f:
	    	if len(line.strip()) != 0 :
	    		num+=1
	    		# print(line)
		        line = line.lower()
		        line = line.strip('\n')
		        if line in seen:
		            counter += 1
		        else:
		            seen.add(line)
		            art_file.write(line)
		            art_file.write('\n')

	with open("/Users/eve/Desktop/Wiki/Life/"+name+"_2.txt") as f:
	    for line in f:
	    	if len(line.strip()) != 0 :
	    		num+=1
		        line = line.lower()
		        line = line.strip('\n')
		        if line in seen:
		            print(line)
		            counter += 1
		        else:
		            seen.add(line)
		            art_file.write(line)
		            art_file.write('\n')

 	print counter
 	print num