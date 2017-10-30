import numpy as np
import robot_fcns as rf
from math import pi
a = np.array([[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,16]])
x = np.array([[4],[5],[6]])
w_hat = np.array([[0,-3,2,],[3,0,-1],[-2,1,0]])
xi_hat = np.concatenate((w_hat,x), axis = 1)

xi_hat = np.concatenate((xi_hat,np.array([[0,0,0,0]])), axis=0)
xi = rf.wedge4(xi_hat)

F = rf.RTtoHomog(rf.rotz(pi/4),x)
#print F

twist = rf.logm(F)

#print twist

inv_F = rf.inv_trans(F)
#print inv_F

