import urllib2
import re
# import eventlet

def GetRevisions(pageTitle):
    url = "https://en.wikipedia.org/w/api.php?action=query&format=xml&prop=revisions&rvlimit=500&rvprop=user|userid|size&titles=" + pageTitle

    revisions = []                                        #list of all accumulated revisions
    next = ''                                             #information for the next request
    while True:
        response = urllib2.urlopen(url + next).read()     #web request
        revisions += re.findall('<rev [^>]*>', response)  #adds all revisions from the current request to the list

        cont = re.search('<continue rvcontinue="([^"]+)"', response)
        if not cont:                                      #break the loop if 'continue' element missing
            break

        next = "&rvcontinue=" + cont.group(1)             #gets the revision Id from which to start the next request

    return revisions;


if __name__ == "__main__":
    # text_file = open("/Users/eve/Desktop/Wiki/Life/revision_history_2.txt", "w")
    text_file = open("/home/fangy/Wiki/Life/revision_history.txt", "w")
    # with open("/Users/eve/Desktop/Wiki/Life/article.txt",'r') as f:
    with open("/home/fangy/Wiki/Life/article.txt",'r') as f:
    	counter = 0;  

        for line in f:
            counter = counter+1 
            if counter>=27070:
                # if counter < 3:


                line = line.strip('\n')
                line = line.strip('\t')
                title = line

                text_file.write(title)
                text_file.write('\n')


                revisions = GetRevisions(title)
                for i in revisions[0:len(revisions)]: #get all revisions
                    # print(i)
                    text_file.write(i)
                    text_file.write('\n')
                
                revisions = [];    
                text_file.write('\n\n')
                print(counter)
                print(title)

    text_file.close()






