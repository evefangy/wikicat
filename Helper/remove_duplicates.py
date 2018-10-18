
# check against check duplicates again!!!!!!!!!!!!!!!! there're still some error 


if __name__ == "__main__":
	counter = 0 # num of repetitive elements
	num = 0 # total num of elements
	seen = set()

	art_file = open("/Users/eve/Desktop/unique_combined.txt", "w")

	line = ""
	#load already processed article
	with open("/Users/eve/Desktop/art_w_major_editor_cate_0.txt") as f:
	   
		for line in f:
			if len(line.strip()) != 0 :
				num+=1
				line = line.strip('\n')
				if line in seen:
					counter += 1
				else:
					seen.add(line)
					art_file.write(line)
					art_file.write('\n')
	
	print "There are "+str(num)+" articles with "+str(counter)+" repetitions"	

	
	num = 0
	counter = 0
	with open("/Users/eve/Desktop/art_w_major_editor_cate_1.txt") as f:
	   
		for line in f:
			if len(line.strip()) != 0 :
				num+=1
				line = line.strip('\n')
				if line in seen:
					counter += 1
				else:
					seen.add(line)
					# art_file.write(line.replace("&#039;","'"))
					art_file.write(line)
					art_file.write('\n')
	print "There are "+str(num)+" articles with "+str(counter)+" repetitions"	
	# with open("/Users/eve/Desktop/Wiki/Life/"+name+"_2.txt") as f:
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
	# 	            art_file.write(line)
	# 	            art_file.write('\n')

	print counter
	print num