import urllib2
import re

def GetContent(title, outfile):
	url = "https://en.wikipedia.org/w/api.php?format=xml&action=query&prop=extracts&explaintext=1&exlimit=1&pageids=" + title                                     
	next = ''                                         
	while True:
		req = urllib2.Request(url+next)
		response = urllib2.urlopen(req)
		response = response.read()     #web request 
		cont = re.search('<continue excontinue="([^"]+)"', response)
		if not cont:   
			# content += response     
			outfile.write(response)                               
			break
		next = "&excontinue=" + cont.group(1)             
	#return content



if __name__ == "__main__":

	art = []
	tmp = []
	counter = 0

	output_addr = "/Users/eve/Desktop/art_content.txt"
	outfile = open(output_addr, 'w')



	with open("/Users/eve/Desktop/used_articles.txt",'r') as f:

		for line in f:
			
			# art = []
			counter = counter+1
			if counter>=1:
				line = line.strip('\n')
				GetContent(line, outfile)
				print line
				outfile.write("\r\n\r\n************************** New Article *************************\r\n\r\n") #seperate each article

				# art = line.split(', ')
				# if len(art)==2:
				# 	GetContent(art[1], outfile)
				# 	print art[1]
				# 	outfile.write("\r\n\r\n************************** New Article *************************\r\n\r\n") #seperate each article
				# else:
				# 	break
				
			print(counter)

	outfile.close()


	print "Finished!!!!!"
