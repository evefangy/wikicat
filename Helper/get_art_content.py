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

    output_addr = "/Users/eve/Desktop/art_content_2.txt"
    outfile = open(output_addr, 'w')

    with open("/Users/eve/Desktop/Two_Trees/artname.txt",'r') as f:

        for line in f:
        	
            art = []
            counter = counter+1
            if counter>=13909:
	            line = line.strip('\n')
	            art = line.split(', ')
	            if len(art)==2:
	                GetContent(art[0], outfile)
	                outfile.write("\n\r\n\r************************** New Article *************************\n\r\n\r") #seperate each article
	            else:
	            	break
              	
            print(counter)

    outfile.close()


    print "Finished!!!!!"
