import urllib2
import re

def GetUserId(username):
    url = "https://en.wikipedia.org/w/api.php?action=query&list=users&format=xml&ususers=" + str(username)
    subcat = []                                        
    next = ''                                         
    while True:
        req = urllib2.Request(url+next)
        response = urllib2.urlopen(req)
        response = response.read()     #web request
        subcat += re.findall('<user [^>]*>', response)  
        cont = re.search('<continue uscontinue="([^"]+)"', response)
        if not cont:                                      #break the loop if 'continue' element missing
            break
        next = "&cmcontinue=" + cont.group(1)             
    return subcat

# def GetCateId(cate_name):
#     url = "https://en.wikipedia.org/w/api.php?action=query&format=xml&titles=Category:" + cate_name
#     subcat = [] 
#     found = ''
#     # while True:
#     req = urllib2.Request(url)
#     response = urllib2.urlopen(req)
#     response = response.read()     #web request
#     subcat += re.findall('<page [^>]*>', response)  
#     # cont = re.search('<continue cmcontinue="([^"]+)"', response)
#     # if not cont:                                      #break the loop if 'continue' element missing
#     #     break   
#     if len(subcat)>1:
#         printf("\n\rGet multiple ids for the category")
#         return
#     try:
#         found = re.search('pageid="(.+?)"',subcat[0]).group(1)
#     except AttributeError:
#         found = ''
#         printf("No pageid is found for the category")
#     return found                                     
    


if __name__ == "__main__":
    id_file = open("/Users/eve/Desktop/WikiBotId.txt", "w")
    tmp = []
    found = ''


    with open("/Users/eve/Desktop/WikiBot.txt",'r') as f:
        for line in f:
            line = line.strip('\n')
            line = line.replace(" ","_")
            # print(line)
            tmp = []
            tmp = GetUserId(line)
            # print(tmp)
            match = re.search('userid="(.+?)"', tmp[0])
            if(match is None):
                print line
            else:
                found = match.group(1)
            # print(found)
            id_file.write(found)
            id_file.write("\n")

    id_file.close()
    print "Finished!!!!!Finallyyyyyyyyy"



    






