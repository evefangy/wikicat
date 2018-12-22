if __name__ == "__main__":
	
	print ("Make sure duplicates in the end of the file is removed (overlaps)")
	print("Adjust the spaces between the transition of files")
	print("")
	print ("Start...")
	#output file
	text_file = open("/Users/eve/Desktop/Edit_Edit_Size_Tree/1_combined.txt",'w')
	
	#open file 1
	# c = 1
	# p = False
	with open("/Users/eve/Desktop/Edit_Edit_Size_Tree/1_article_editor_edit_size_1.txt",'r') as f:
		for line in f:
			text_file.write(line)	
	f.close()

	print("Finished one file")


	text_file.write("\n")

	# #open file 2
	with open("/Users/eve/Desktop/Edit_Edit_Size_Tree/1_article_editor_edit_size_2.txt",'r') as f:
		
		for line in f:
			text_file.write(line)

	f.close()
	print("Finished two files")

	text_file.write("\n")

	# # # #open file 3
	# with open("/Volumes/Eve/Wiki/All_Materials_for_Paper/Experiment/Art_w_Major_Editors/art_w_major_editor_cate_104624_1.4M.txt",'r') as f:
		
	# 	for line in f:
	# 		text_file.write(line)
	# f.close()
	
	# # text_file.write("\n")

	# print("Finished three files")

	# # # 	# #open file 3
	# with open("/Volumes/Eve/Wiki/Life/Rev/revision_history_cate_31790_to_104624_2.txt",'r') as f:
		
	# 	for line in f:
	# 		text_file.write(line)

	# f.close()

	# print("Finished four files")
	# text_file.write("\n")

	# # # 	# #open file 3
	# with open("/Volumes/Eve/Wiki/Life/Rev/revision_history_cate_31790_to_104624_2.txt",'r') as f:
		
	# 	for line in f:
	# 		text_file.write(line)

	# f.close()
	
	# print("Finished five files")

	text_file.close()