from math import floor, log10


def MyFunc(goal, remainingNumbers, depth):
  print(" |"*depth, "Can you get",goal, "from", remainingNumbers)

  lastInArray = remainingNumbers[-1]

  if len(remainingNumbers) == 1:
        print(" |"*depth, "Yes. They are equal" if lastInArray == goal else "No")
        return lastInArray == goal
  else:
      if MyFunc(goal-lastInArray, remainingNumbers[:-1], depth+1):
        print(" |"*depth, "Yes, by +")
        return True
      elif MyFunc(goal/lastInArray, remainingNumbers[:-1], depth+1):
        print(" |"*depth, "Yes, by *")
        return True
      else:
        divisor = pow(10, floor(log10(lastInArray)) + 1)
        remainder = goal % divisor

        if remainder == lastInArray:
           if MyFunc(int(goal/divisor), remainingNumbers[:-1], depth+1):
              print(" |"*depth, "Yes, by ||")
              return True
           else:
              print(" |"*depth, "No, +/*/|| all don't work")
              return False
        else:
           print(" |"*depth, "No, +/* does not work and cannot deconcat")
           return False

def MyFuncSlim(goal, remainingNumbers):

  lastInArray = remainingNumbers[-1]

  if len(remainingNumbers) == 1:
        return lastInArray == goal
  else:
      if MyFuncSlim(goal-lastInArray, remainingNumbers[:-1]) or MyFuncSlim(goal/lastInArray, remainingNumbers[:-1]):
        return True
      else:
        divisor = pow(10, floor(log10(lastInArray)) + 1)
        remainder = goal % divisor

        if remainder == lastInArray:
           return MyFuncSlim(int(goal/divisor), remainingNumbers[:-1])
        else:
           return False

caseToUse = 2
match(caseToUse):
  case 0:
    #17388841797561: 978 2 889 1 7 97 561
    Target = 17388841797561
    Numbers = [978, 2, 889, 1, 7, 97, 561]

  case 1:
    #21289332681989: 615 5 1 6 7 989 189 9 9
    Target = 21289332681989
    Numbers = [615, 5, 1, 6, 7, 989, 189, 9, 9]

  case 2:
    #312553360488386: 781 3 834 5 8 4 8 8 386
    Target = 312553360488386
    Numbers = [781, 3, 834, 5, 8, 4, 8, 8, 386]


print(MyFunc(Target, Numbers, 0))
