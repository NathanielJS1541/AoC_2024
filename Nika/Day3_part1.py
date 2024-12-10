# Advent calander 2024 day 3 part 1

file = open("Day3_input.txt", "r")
content = file.read()
data=[]
inside_brackets=[]
number_pair=[]
number1=0
number2=0
sum_of_numbers=0

 

data=content.rsplit("mul(")                                                                     #check if it contains mul instruction
for a in range(len(data)):
        if (data[a].find(")")!=-1):                                                             # if it contains mulC and then C there is a chance taht between brackets is valid data
                inside_brackets=data[a].rsplit(")")
                inside_brackets.pop()                                                           # no need ofr rubbish outside brackets
                if (inside_brackets[0].find(","))!=-1:                                          # if the data are separated b , there is a chnce they aqre legit data
                        number_pair= inside_brackets[0].rsplit(",")
                        if number_pair[0].isnumeric() and number_pair[1].isnumeric():           #only legint data of they are numbers
                                
                                number1=int(number_pair[0])
                                number2=int(number_pair[1])
                                sum_of_numbers=sum_of_numbers+number1*number2
                                
                                
        
        
print(sum_of_numbers)     


        



file.close()
