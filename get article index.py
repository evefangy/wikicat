
import urllib2
import re
if __name__ == "__main__":
    # text_file = open("/Users/eve/Desktop/Wiki/revision_history_2.txt", "w")
    myline = "$20_bill_U.S."
    with open("/Users/eve/Desktop/Wiki/article_title.txt",'r') as f:
    	counter = 0;  

        # print "here"
        for line in f:
            counter = counter+1 
            line = line.strip('\n')
            line = line.strip('\t')
            title = line
            # print(title)

            if(myline == title.strip()):
                print(counter)	
                print(title)
                break
            

    		   	

