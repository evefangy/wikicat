# The code parse and reformat the revision history to the following form

# article title, major editor 1, major editor 2...

# Note 1. The major author is defined as authors with more than 10% edition history compare to the whole article
#      2. Unregistered editor is not recorded in the output
#      3. Users with a comma in the name is considered and the comma portion of the name is replaced by "_,"
#	   4. Articles with size smaller than 100B doesn't have their major editors recorded

import sys

if __name__ == "__main__":
	if len(sys.argv) < 6:
		print "--------------------------------------------------------------------------------"
		print "Too few arguments. Please provide the information in the following format. "
		print "Parsed_rev_file | Output_address | category_range_and_output_version | starting_line_number(starting from 1) | used_article_addr"
		print "(e.g ./parsed_rev_cate_0_to_100000.txt ./output 0_to_100000_2 1 used_art.txt" 
		print "--------------------------------------------------------------------------------"
		sys.exit()

	output_address = sys.argv[2]+"/art_w_major_editor_cate_"+sys.argv[3]+".txt"
	input_address = sys.argv[1]
	start_line = int(sys.argv[4])
	art_name = sys.argv[5]

	art_file = open(output_address, "w")

	#local variables
	new_art = False
	istitle = 0 #flag indicates the current line is a article title 
	if start_line == 1:
		new_art = True
		istitle = 1
	
	art_title = ""
	targetArt = False

	c = 0 #current line number (starting at 1)


	used_art = set()
	with open(art_name,'r') as a_f:
		for line in a_f:
			line = line.strip()
			if not line:
				print "detect empty line"
			else:
				used_art.add(line)


	print(len(used_art))


	art_size = 0
	page_size = 0
	is_small_article = False

	with open(input_address,'r') as f:
		for line in f:
			c+=1
			if c >= start_line:
				# if c%1000 == 0:
				# 	print c
				line = line.strip()
				# print line
				
				# empty line indicates the end of an article
				if not line:
#                    print "detetct empty line"
					istitle = 1
					new_art = True
	
				else:
					# title or page size
					if new_art == True:
						# if it's a page size
						if istitle == 0:
							istitle = 1
							new_art = False
							page_size = int(line)
							if page_size>=100:
								is_small_article = False


						# if it's an article title
						else:
							istitle = 0
							is_small_article = True
							if line in used_art:
								art_file.write('\n') 
								art_title = line
								art_file.write(art_title)
								targetArt = True
							else:
								targetArt = False
							
							
					
					# rev history
					else:
						# if the article is the target article, get its revision history
						if targetArt == True and is_small_article == False:
							tmp = line.split(", ")

							
							if(len(tmp)>4):
								# if it's a registered user
								if tmp[len(tmp)-3] != "0":
									#editor name
									art_file.write(", ")

									art_file.write(tmp[len(tmp)-3])
									#edit size
									art_file.write(", ")
									
									art_file.write(tmp[len(tmp)-1])
							else:
								if tmp[1] != "0":
									#editor name
									art_file.write(", ")
									art_file.write(tmp[1])
									#edit size
									art_file.write(", ")
									art_file.write(tmp[3])

			
	art_file.close()
	print "Remember to remove the duplicates in the end if multiple versions of outputs are created"






