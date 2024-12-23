def MyFunc(goal, remainingNumbers):
  lastInArray = remainingNumbers[-1] 
  if len(remainingNumbers) == 1:
        return lastInArray == goal
  else:
      possible = MyFunc(goal-lastInArray, remainingNumbers[:-1]) or MyFunc(goal/lastInArray, remainingNumbers[:-1])
      return possible

Target = 292
Numbers = [11, 6, 16, 20]

print(MyFunc(Target, Numbers))
