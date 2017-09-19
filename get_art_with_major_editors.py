# The code parse and reformat the revision history to the following form

# article title, major author 1, major author 2...

# Note that the major author is defined as authors with more than 10% edition history compare to the whole article

if __name__ == "__main__":
	art_file = open("/Users/eve/Desktop/Wiki/art_with_major_editors.txt", "w")
	new_art = True
	art_title = ""
	counter = 0 #indicate if the line is the article title or article size
	with open("/Users/eve/Desktop/Wiki/parsed_rev_history_test.txt",'r') as f:
		for line in f:
			line = line.strip()
			print line
			# print counter
			if not line:
				# print "detetct empty line"
				counter = 0
				if new_art == False:
					new_art = True
			else:
				if new_art == True:
					if counter == 1:
						counter = 0
						new_art = False
					else:
						art_file.write('\n')
						art_title = line
						art_file.write(art_title)
						counter = 1
				else:
					tmp = line.split(", ")
					#filter out minor edits
					if float(tmp[3]) >= 0.1:
						art_file.write(", ")
						art_file.write(tmp[0])
	art_file.close()






