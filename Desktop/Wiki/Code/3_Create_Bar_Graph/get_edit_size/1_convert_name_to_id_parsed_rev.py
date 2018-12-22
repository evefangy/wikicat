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
		print "(e.g ./parsed_rev_cate_0_to_100000.txt ./output 0_to_100000_2 1 artname.txt)" 
		print "--------------------------------------------------------------------------------"
		sys.exit()

	output_address = sys.argv[2]+"/parsed_"+sys.argv[3]+".txt"
	input_address = sys.argv[1]
	start_line = int(sys.argv[4])
	art_name = sys.argv[5]

	art_file = open(output_address, "w")

	#local variables
	new_art = False
	istitle = 0 #flag indicates the current line is a article title 
	if start_line <= 1:
		new_art = True
		istitle = 1
	
	art_title = ""


	c = 0 #current line number (starting at 1)


	id_name = {}
	parsed = []
	name = ""

	num = 0

	with open(art_name,'r') as a_f:		
		for line in a_f:
			line = line.strip()
			if not line:
				print "detect empty line"
			else:
				parsed = line.split(', ')
				for i in range(len(parsed)):
					if i==0:
						continue
					else:
						name += parsed[i]

				# name = name.lower()
				id_name[name] = parsed[0]
				
				name = name.lower()
				if name not in id_name:
					id_name[name] = parsed[0]
				name = ""
				
	a_f.close()
	print(len(id_name))
	print("\n")

	


	article_id = ""
	not_save = False
	no_id = 0
	num_art_w_id = 0
	num_art = 0
	with open(input_address,'r') as f:
		for line in f:
			c+=1
			if c >= start_line:
				line = line.strip()
				
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
							if not_save == False:
								art_file.write(line)
								art_file.write('\n')


						# if it's an article title
						else:
							istitle = 0
							
							art_title = line

							num_art += 1
							# art_title = art_title.lower()

							if art_title not in id_name:
								not_save = True
							else:
								not_save = False

							if not_save == False: 
								article_id = id_name[art_title]
								art_file.write('\n') 
								art_file.write(article_id)	
								art_file.write('\n') 
								# not_save = False
								num_art_w_id += 1
					
					# rev history
					else:
						if not_save == False:
							art_file.write(line)
							art_file.write('\n') 
						

			
	art_file.close()
	print(num_art)
	print(num_art_w_id)





