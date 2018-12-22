import urllib2
import re

def GetContentSize(title):
	url = "https://en.wikipedia.org/w/api.php?action=query&prop=revisions&rvprop=size&format=xml&pageids=" + title + "&redirects"
	# next = ''                                         
	# while True:
		# req = urllib2.Request(url+next)
	req = urllib2.Request(url)
	response = urllib2.urlopen(req)
	response = response.read()     #web request 
	found = ""
	try:
		found = re.search('<rev size="(.+?)"', response).group(1)
	except AttributeError:
		found = re.findall('missing=".*"', response)
		if (len(found)!=0):
			# print("Article "+title+"is missing")
			return "-1"
		else:
			print("No page size is found for article "+title+" and the article is not missing")
			exit()
	return found    

def GetContent(title, outfile):
	url = "https://en.wikipedia.org/w/api.php?format=xml&action=query&prop=extracts&explaintext=1&exlimit=1&pageids=" + title + "&redirects"                                    
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
	start_index = 0

	counter = 0

	output_folder = "/Users/eve/Desktop/Major_Editor_5/Create_BoW_Tree"

	output_addr = output_folder+"/0_art_content.txt"
	missing_art_addr = output_folder+"/0_missing_articles.txt"
	small_art_addr = output_folder+"/0_small_articles.txt"
	valid_art_addr = output_folder+"/0_valid_articles.txt"
	outfile = open(output_addr, 'w')
	missing_art = open(missing_art_addr, 'w')
	small_art = open(small_art_addr, 'w')
	valid_art = open(valid_art_addr, 'w')


	content_size = 0
	num_small_art = 0
	num_missing_art = 0
	num_total_art = 0



	with open("/Users/eve/Desktop/Major_Editor_5/Create_BoW_Tree/used_article.txt",'r') as f:

		for line in f:
			counter = counter+1
			if counter>=start_index:
				line = line.strip('\n')
				content_size = GetContentSize(line)
				if int(content_size) < 100:
					if int(content_size == "-1"):
						print ("Article "+line+" is missing")
						num_missing_art += 1
						missing_art.write(line)
						missing_art.write('\n')
					else:
						print ("Content size for article "+line+" is less than 100B")
						num_small_art += 1
						small_art.write(line)
						valid_art.write('\n')
				else:	
					GetContent(line, outfile)
					valid_art.write(line)
					valid_art.write('\n')
					outfile.write("\r\n\r\n************************** New Article *************************\r\n\r\n") #seperate each article
			print(counter)

	outfile.close()
	missing_art.close()
	small_art.close()
	valid_art.close()

	print "Finished!!!!!"
	print ("There are "+str(num_missing_art)+" articles missing"+"\n")
	print ("There are "+str(num_small_art)+" articles<100B"+"\n")
	print ("There are "+str(num_small_art+num_missing_art)+" articles in total"+"\n")
