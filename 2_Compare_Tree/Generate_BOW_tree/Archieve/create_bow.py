import collections, re
from collections import Counter

output_addr = "/Users/eve/Desktop"
input_addr = "/Users/eve/Desktop/test.txt"

bow_mat_addr = output_addr+"/bow_matrix.txt"
bow_art_addr = output_addr+"/bow_words.txt"
bow_whole_addr = output_addr+"/bow_entire_freq.txt"

bow_mat = open(bow_mat_addr, 'w')
bow_words = open(bow_art_addr, 'w')
bow_whole = open(bow_whole_addr, 'w')


line = ""
c = 0
bagsofwords = Counter()
sumbags = Counter()
# sumbags_tmp = Counter()
text = []

bow = Counter()

# get all words in the bow vector
with open(input_addr,'r') as f:
	for line in f:
		line = line.strip()
		if(len(line)!=0):
			if line == "************************** New Article *************************":
				c+=1
				print(c)
				bagsofwords = [ collections.Counter(re.findall(r'\w+', txt)) for txt in text]

				sumbags = sum(bagsofwords, collections.Counter())
				
				bow = sumbags+bow

				bagsofwords = []
				text = []

			else:
				text.append(line)
			

f.close()

# reset all counter in bow to "0" : used in adding the set to produce the bag of words on all articles
for i in bow:
	bow_whole.write(i)
	bow_whole.write(": ")
	bow_whole.write(str(bow[i]))
	bow_whole.write("\n")
	bow[i] = 0
	bow_words.write(i)
	bow_words.write("\n")

bow_whole.close()
bow_words.close()

c = 0
with open(input_addr,'r') as f:
	for line in f:
		line = line.strip()
		if(len(line)!=0):
			if line == "************************** New Article *************************":
				c+=1
				print(c)
				bagsofwords = [ collections.Counter(re.findall(r'\w+', txt)) for txt in text]

				sumbags = sum(bagsofwords, collections.Counter())

				for i in bow:
					bow_mat.write(str(sumbags[i]))
					bow_mat.write(" ")
				bow_mat.write("\n")

				bow_mat.write("\n\n")
				bow_mat.write(line)
				bow_mat.write("\n\n")

				bagsofwords = []
				text = []

			else:
				text.append(line)
			

f.close()



bow_mat.close()



