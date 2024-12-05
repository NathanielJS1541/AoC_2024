file = open("Day2_input.txt", "r")
content = file.readlines()

report=[]
ints=[]
safe=False
count = 0


for i in range(len(content)):
    report=content[i].split()       #each line is in separate list
    for a in range(len(report)):    #change yo intigers
        report[a]=int(report[a])

    if report==sorted(report) or report==sorted(report, reverse=True):      #check if it is in decending or assending order
        checks=len(report)-1        #has to be minus one to allow for all iteration and not be out of range
        for n in range(checks):
            if abs(report[n]-report[n+1])>=1 and abs(report[n]-report[n+1])<=3 :    #compare teh difference between adjecant list elements
                safe=True
            else:
                safe=False
                break               #if at least one element is not up to a standart stop checking

    if safe:
        count=count+1
               

print(count)


file.close()
