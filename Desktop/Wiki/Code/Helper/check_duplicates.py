if __name__ == "__main__":
	counter = 0 # num of repetitive elements
	num = 0 # total num of elements
	line_num = 0 #inclusing empty lines
	seen = set()

	line = ""
	# with open("/Users/eve/Desktop/Wiki/Life/Opticode_tester/COLING/tree_enc/group_ite_old.txt",'r') as file:
	# 	for line in file:
	# 		line_num+=1

	# 		if len(line.strip())!=0: 
	# 			num += 1
	# 			line = line.strip()
	# 			#print 
	# 			# if num == 291450:
	# 			# 	print line
	# 			# 	print num
	# 			# 	print line_num
	# file.close()

	# print counter
	# print num
	# print line_num
	# print "finish one file"






	with open("/Volumes/easystore/Wiki/All_Materials_for_Paper/Experiment/3_Art_w_Major_Editors/unique_combined.txt",'r') as f3:
	# with open("/Users/eve/Desktop/Wiki/Life/WikiTree/cate_name.txt",'r') as f3:
		    for line in f3:
		    	if len(line.strip()) != 0 :
		    		num+=1
			        line = line.strip('\n')
			        
			        if line in seen:
			        	# print line
			        	counter += 1
			        else:
			           
			            seen.add(line)
	f3.close()

	print "Number of repetitions: "
 	print counter
 	print "Number of elements: "
 	print num
 	print "finish three file"

	# counter = 0
	# num = 0
	# # seen = set()
 # 	# num = 0
 # 	# with open("/Users/eve/Desktop/Wiki/Life/WikiTree/cate_name.txt",'r') as f2:
	# with open("/Users/eve/Desktop/Result_for_new_create_tree_code/30_pairs/thresh/tree_enco.txt",'r') as f2:
	#     for line in f2:
	#     	if len(line.strip()) != 0 :
	#     		num+=1
	#     		# if num == 1000:
	#     		# 	break

	# 	        line = line.strip('\n')
	# 	        # if line == "1926287":
	# 	        # 	print num

	# 	        if line in seen:
	# 	        	# print(line)
	# 	        	# print num
	# 	        	counter += 1
	# 	        	# break
	# 	        # else:
		           
	# 	            # seen.add(line)

	# f2.close()

	# print "Number of repetitions: "
 # 	print counter
 # 	print "Number of elements: "
 # 	print num
 # 	print "finish two file"
