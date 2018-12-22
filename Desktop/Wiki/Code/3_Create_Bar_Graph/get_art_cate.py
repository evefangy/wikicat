import urllib2
import re

def GetCates(art):
	url = "https://en.wikipedia.org/w/api.php?action=query&generator=categories&prop=info&format=xml&pageids=" + art
	cat = []                                        
	next = ''                                         

	req = urllib2.Request(url+next)
	response = urllib2.urlopen(req)
	response = response.read()     #web request
	cat += re.findall('<page [^>]*>', response)  
				 
	# print cat
	return cat

									
	


if __name__ == "__main__":

	start = 11426
 
	add = "/Users/eve/Desktop/art_cates_"
	add += str(start)
	add += ".txt"


	out_file = open(add, "w")


	arts = []
	cates = []
	tmp = []
	found = ''
	c = 0



	counter = start


	with open("/Users/eve/Dropbox/Wiki/two_trees/used_articles.txt",'r') as f:
		c = 0
		for line in f:
			line = line.strip('\n')
			
			if c >= start:
				arts.append(line)
			c += 1

	
	for article in arts:
		print article

		out_file.write(article)

		tmp = GetCates(article)

		for i in tmp[0:len(tmp)]:
			found = re.search('pageid="(.+?)"', i).group(1)
			out_file.write(' ')
			out_file.write(found)

		out_file.write('\n')
		tmp = []
	
		counter += 1
		print counter

	out_file.close()
	print "Finished!!!!!Finallyyyyyyyyy"



	






