import collections, re

output_addr = "/Users/eve/Desktop/bow.txt"
outfile = open(output_addr, 'w')

line = ""
c = 0
bagsofwords = []
sumbags = []
text = []
with open("/Users/eve/Desktop/w_o_stop_words.txt",'r') as f:
	for line in f:
		line = line.strip()
		if(len(line)!=0):
			if line == "************************** New Article *************************":
				c+=1
				print(c)
				bagsofwords = [ collections.Counter(re.findall(r'\w+', txt)) for txt in text]

				sumbags = sum(bagsofwords, collections.Counter())
				for i in sumbags:
					outfile.write(i)
					outfile.write(": ")
					outfile.write(str(sumbags[i]))
					outfile.write("\n")
				outfile.write("\n")

				outfile.write("\n\n")
				outfile.write(line)
				outfile.write("\n\n")

				bagsofwords = []
				text = []

			else:
				text.append(line)
			

f.close()
outfile.close()





# sumbags = sum(bagsofwords, collections.Counter())
