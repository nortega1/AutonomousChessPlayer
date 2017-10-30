import numpy as np
from math import acos, sin, cos

def logm(m):
    R = m[0:3,0:3]
    t = np.transpose(np.array([m[:,3][0:3]]))

    angle = acos((np.trace(R) - 1) / 2.0)

    w_hat = (R - np.transpose(R)) * angle / (2*sin(angle))

    if angle is 0:
        V=np.identity(3)
    else:
        A = sin(angle)/angle
        B = (1 - cos(angle)) / angle**2
        V = np.identity(3) - 0.5*w_hat + (1/angle**2)*(1-(A/(2*B)))*np.matmul(w_hat,w_hat)
    u = np.matmul(V, t)

    return np.concatenate((u,wedge3(w_hat)))

def wedge4(xi_hat):
    w_hat = xi_hat[0:3,0:3]

    v = np.transpose(np.array([xi_hat[:,3][0:3]]))
    w = wedge3(w_hat)
    return np.concatenate((v, w), axis=0)

def wedge3(w_hat):
    return np.array([[w_hat[2,1]],
                     [w_hat[0,2]],
                     [w_hat[1,0]]])

def RTtoHomog(R,t):
    F = np.concatenate((R,t), axis=1)
    return np.concatenate((F,np.array([[0,0,0,1]])), axis=0)

def inv_trans(F):
    R = F[0:3,0:3]
    t = np.transpose(np.array([F[:,3][0:3]]))
    inv_R = np.transpose(R)
    inv_t = -np.matmul(inv_R, t)
    return RTtoHomog(inv_R, inv_t)

def rotx(ang):
    return np.array([[1,       0,          0],
                     [0, cos(ang), -sin(ang)],
                     [0, sin(ang),  cos(ang)]])

def roty(ang):
    return np.array([[ cos(ang), 0, sin(ang)],
                     [        0, 1,        0],
                     [-sin(ang), 0, cos(ang)]])

def rotz(ang):
    return np.array([[ cos(ang), -sin(ang), 0],
                     [ sin(ang),  cos(ang), 0],
                     [        0,         0, 1]])

def quat2rotm(q):
    qx = q[0]
    qy = q[1]
    qz = q[2]
    qw = q[3]
    
    R = np.zero((3,3))

    R[0][0] = 1 - 2*pow(qy,2) - 2*pow(qz,2)
    R[0][1] = 2*qx*qy - 2*qz*qw
    R[0][2] = 2*qx*qz + 2*qy*qw
    
    R[1][0] = 2*qx*qy + 2*qz*qw
    R[1][1] = 1 - 2*pow(qx,2) - 2*pow(qz,2)
    R[1][2] = 2*qy*qz - 2*qx*qw

    R[2][0] = 2*qx*qz - 2*qy*qw
    R[2][1] = 2*qy*qz + 2*qx*qw
    R[2][2] = 1 - 2*pow(qx,2) - 2*pow(qy,2)

    return R
