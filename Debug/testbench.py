import os
l=[768]
o=[0,1,2,3,4,5,6,7]
n=[1]
import subprocess
for i in n:
 for ii in l:
  for iii in o:
   #os.system("./run.sh "+str(ii)+" -o"+str(iii)+" -t4")
   print ii,iii
   os.system("./avgTime.py performance_n_"+str(ii)+"_c_"+str(iii))
