if __name__ == "__main__":
	
	name = "revision_history"
	seen = set()
	text_file = open("/Users/eve/Desktop/Wiki/Life/"+name+".txt", "a")
	with open("/Users/eve/Desktop/Wiki/Life/"+name+"_2.txt",'r') as f:
	    
	    for line in f:
	    	text_file.write(line)

	text_file.write('\n')
	text_file.close()
