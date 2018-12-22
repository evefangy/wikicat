# The code process the articles and removes all the stopwords, non-English words and unicode words. 
#It also tokenize all the words in the file. All the words left are in lower case

from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
from many_stop_words import get_stop_words
import string 
import enchant

english_dict = enchant.Dict("en_US")
filtered_file = open("/Users/eve/Desktop/w_o_stop_words_enhanced.txt", "w")
unsaved_uni = open("/Users/eve/Desktop/unsaved_unicode_words.txt", "w")

content_addr = "/Users/eve/Desktop/content_w_o_xml.txt"


example_sent = ""
line = ""


stop_words = list(get_stop_words('en'))         #About 900 stopwords
nltk_words = list(stopwords.words('english')) #About 150 stopwords
stop_words.extend(nltk_words)
tmp = []


with open(content_addr,'r') as f:
        num_art = 0
        c = 0

        for line in f:
            line = line.strip()

            if(len(line)!=0):
            	filtered_sentence = []
            	if(line == "************************** New Article *************************"):
            		filtered_file.write("\n\n")
            		filtered_file.write(line)
            		filtered_file.write("\n\n")
            		num_art+=1
            		print(num_art)
            	else:
            		# print(line)
            		tmp = []
            		line = line.lower()
            		tmp = line.split(" ")
            		line = ""
            		for i in tmp:
            			if (len(i)==0):
            				continue
            			if isinstance(i, unicode) == False:
            				if english_dict.check(i)==True:
            					if is_ascii(i):
            						line = line+i+" "
            					else:
            						unsaved_uni.write(i)
            						unsaved_uni.write("\n")

            		# print line
            		if(len(line)==0):
            			continue
	            	example_sent = line

	            	word_tokens = word_tokenize(example_sent)

	            	# filtered_sentence = [w for w in word_tokens if not w in stop_words]
	            	filtered_sentence = []
	            	for w in word_tokens:
	            		if w not in stop_words:
	            			filtered_sentence.append(w)

	            	for i in filtered_sentence:
	            		c+=1
	            		if i not in string.punctuation:
	            			i=i.strip("/")
	            			i=i.strip("'")
	            			i=i.strip("=")
	            			i=i.strip("`")
	            			if len(i)!=0:
	            				filtered_file.write(i)
	            				if c!=len(filtered_sentence):
	            					filtered_file.write(', ')
	            		c = 0


			
f.close()



 

 

 

 

 