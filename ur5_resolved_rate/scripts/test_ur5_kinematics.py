import ur5_kinematics as u
def main():
    print 'I am in the function'
    thetas = [0,0,0,0,0,10]
    Tbt = u.forwardKinematics(thetas)
    print 'Forward Kinematics:\n'
    print Tbt
    jac = u.jacobian(thetas)
    print 'Jacobian:\n'
    print jac

if __name__ == '__main__':
  main()