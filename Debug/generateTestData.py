#!/usr/bin/python
import numpy as np
import sys

n=int(sys.argv[1])
fn=str(n)
r=9999
a=np.random.randint(-r,r,(n,n))
b=np.random.randint(-r,r,(n,n))
result = np.dot(a,b)

arr=[[a,'_a.matrix'],[b,'_b.matrix'],[result,'_result.matrix']]

for ar in arr:
    f = open('data_'+fn+ar[1],'w')
    f.write(str(n)+'\n')
    for x in ar[0]:
        for i,y in enumerate(x):
            if i==0:
                f.write(str(y))
            else:
                f.write(" "+str(y))
        #f.write(str(x).replace(',','').replace('[','').replace(']',''))
        f.write('\n')
    f.close()

#f = open('script_'+str(n)+'.sh','w')
#f.write('#!/bin/bash\n')
#f.write('./matrixmultiply data_'+str(n)+'_a.matrix data_'+str(n)+'_b.matrix data_'+str(n)+'_result.matrix $1 $2')
