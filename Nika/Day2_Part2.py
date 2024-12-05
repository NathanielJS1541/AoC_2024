# day 2 part 2 of Advent Calender Challange 2024


file = open("Day2_input.txt", "r")
content = file.readlines()

report=[]
report_unchanged=[]
p=0

safe=False
safe_secondary=False
count = 0
count_standard=0


for i in range(len(content)):
    report=content[i].split()       #each line is in separate list
    for b in range(len(report)):    #change to intigers
        report[b]=int(report[b])

    
    if report==sorted(report) or report==sorted(report, reverse=True):      #check if it is in decending or assending order
        checks=len(report)-1        #has to be minus one to allow for all iteration and not be out of range
        for n in range(checks):
            if abs(report[n]-report[n+1])>=1 and abs(report[n]-report[n+1])<=3 :    #compare teh difference between adjecant list elements
                safe=True
            else:
                safe=False
                break               #if at least one element is not up to a standart stop checking
    if safe:
        count_standard=count_standard+1
        
    else:
        for a in range(len(report)):

            removed_value=report.pop(a)
                    
            if (report==sorted(report) or report==sorted(report, reverse=True)):
            
                    for m in range(len(report)-1):
                        if abs(report[m]-report[m+1])>=1 and abs(report[m]-report[m+1])<=3 :    #compare the difference between adjecant list elements
                            p=p+1
                            
                            if p ==len(report)-1:
                                safe_secondary=True
                                count=count+1
                                p=0
                                break
                                
                        else:
                            safe_secondary=False
                            p=0
            
            report.insert(a, removed_value)
            if safe_secondary:
                print(report)
                safe_secondary=False
                break
               
                
            
            p=0
            
        
print(count+count_standard)            
               


file.close()
