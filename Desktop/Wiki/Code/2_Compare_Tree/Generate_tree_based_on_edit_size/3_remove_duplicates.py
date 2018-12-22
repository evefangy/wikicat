
# check against check duplicates again!!!!!!!!!!!!!!!! there're still some error 


if __name__ == "__main__":
	counter = 0 # num of repetitive elements
	num = 0 # total num of elements
	seen = set()

	art_file = open("/Users/eve/Desktop/Edit_Edit_Size_Tree/1_unique_combined.txt", "w")

	line = ""
	tmp = []

	used_art = set()
	with open("/Users/eve/Desktop/Edit_Edit_Size_Tree/BoW_used_articles.txt") as f:
	   
		for line in f:
			if len(line.strip()) != 0 :
				line = line.strip()
				used_art.add(line)
				num+=1
	
	print "There are "+str(num)+" used articles"	
	f.close()

	num=0
	num_recorded = 0

	#load already processed article
	with open("/Users/eve/Desktop/Edit_Edit_Size_Tree/1_combined.txt") as f:
	   
		for line in f:
			if len(line.strip()) != 0 :
				num+=1
				line = line.strip('\n')
				tmp = line.split(", ")
				if tmp[0] in seen:
					counter += 1
				else:
					seen.add(tmp[0])
					# if tmp[0] in used_art:
					art_file.write(line)
					art_file.write('\n')
					num_recorded+=1
					# else:
					# 	print(line)

	
	print "There are "+str(num)+" articles with "+str(counter)+" repetitions"	

	print(str(num-counter))
	print(str(num_recorded))