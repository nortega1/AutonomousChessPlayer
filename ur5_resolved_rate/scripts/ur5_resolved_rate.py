#!/usr/bin/env python
import roslib; roslib.load_manifest('ur_driver')
import rospy
import actionlib
import tf
from control_msgs.msg import *
from trajectory_msgs.msg import *
from sensor_msgs.msg import JointState
from math import pi
import time
import numpy as np
from numpy import linalg as la
import robot_fcns as rf
import ur5_kinematics as fk

JOINT_NAMES = ['shoulder_pan_joint', 'shoulder_lift_joint', 'elbow_joint',
               'wrist_1_joint', 'wrist_2_joint', 'wrist_3_joint']
#Q1 = [2.2,0,-1.57,0,0,0]
#Q1 = [0,-1.57, 0, 0, 0, 0]
#Q2 = [1.5,0,-1.57,0,0,0]
#Q3 = [1.5,-0.2,-1.57,0,0,0]
    
client = None

def resolved_rate(base_goal):

    # Initialize algorithm variables
    t = 0.0
    dt = 0.05
    error = float("inf")

    # Read joint angles from robot
    joint_states = rospy.wait_for_message("joint_states", JointState)
    joints_curr = np.transpose([np.array(joint_states.position)])

    # Initialize new trajectory object
    g = FollowJointTrajectoryGoal()
    g.trajectory = JointTrajectory()
    g.trajectory.joint_names = JOINT_NAMES
    g.trajectory.points = [JointTrajectoryPoint(positions=joints_curr, velocities=[0]*6, time_from_start=rospy.Duration(0.0))]
 
    while (error > 0.1):
        # Use forward kinematics to get transform from base to ee
        base_ee = fk.forwardKinematics(joints_curr)
 
        # Use matrix algebra to get transfrom from goal to ee
        goal_ee = np.matmul(rf.inv_trans(base_goal), base_ee)

        # Calculate desired twist to get from ee to goal
        body_vel = rf.logm(goal_ee)
        
        # Calculate inverse of the body Jacobian from current joints
        J = fk.jacobian(joints_curr)
        J_inv = np.linalg.inv(J)

        # Get joint velocity to achieve desired twist
        joints_vel = np.matmul(J_inv, -body_vel)
        
        # Use joint velocity to calculate updated joint angles
        joints_updated = joints_curr + dt*joints_vel
        
        # Add updated joints to trajectory
        t = t + 5*dt
        g.trajectory.points.append(JointTrajectoryPoint(positions=joints_updated, velocities=[0]*6, time_from_start=rospy.Duration(t)))

        # Update error and prepare for next iteraion
        error = la.norm(body_vel)
        joints_curr = joints_updated

    client.send_goal(g)
    client.wait_for_result()

'''   
def move(joints_curr, joints_updated, dt):

    # Initialize new trajectory object
    g = FollowJointTrajectoryGoal()
    g.trajectory = JointTrajectory()
    g.trajectory.joint_names = JOINT_NAMES

    try:
        # Add start and end points to trajectory          
        g.trajectory.points = [JointTrajectoryPoint(positions=joints_curr, velocities=[0]*6, time_from_start=rospy.Duration(0.0))]
        g.trajectory.points.append(JointTrajectoryPoint(positions=joints_updated, velocities=[0]*6, time_from_start=rospy.Duration(dt)))

        # Send trajectory to robot
        # TODO: perhaps don't wait for result with resolved rate
        client.send_goal(g)
        client.wait_for_result()

    except KeyboardInterrupt:
        client.cancel_goal()
        raise
    except:
        raise

def callback(data, args):

    listener = args

    try:
        (t,quat) = listener.lookupTransform('/base', '/goal', rospy.Time(0))
    except (tf.LookupException, tf.ConnetivityException, tf.ExtrapolationException):
        continue

    R = quat2rotm(quat)
    base_goal = RTtoHomog(R,t)

    resolved_rate(base_goal)
'''
  
def main():
    global client
    try:
        
        rospy.init_node("test_move", anonymous=True, disable_signals=True)
        client = actionlib.SimpleActionClient('follow_joint_trajectory', FollowJointTrajectoryAction)
        print "Waiting for server..."
        client.wait_for_server()
        print "Connected to server"
         
        #listener = tf.TransformListener()
        #go_sub = rospy.Subscriber("robot_turn", String, callback, (listener))

        a = fk.forwardKinematics(np.array([[ 0.67080277], [-2.15875942],[-1.2023843 ],[-0.07104522],[-0.66597492],[-0.45564396]]))

        #a = rf.RTtoHomog(np.eye(3), np.array([[.25],[0.25],[0.25]]))
        resolved_rate(a)
        
        #b = RTtoHomog(rotz(pi/4), np.array([[0.3],[0.3],[0.3]]))
        #resolved_rate(b)
    except KeyboardInterrupt:
        rospy.signal_shutdown("KeyboardInterrupt")
        raise

if __name__ == '__main__': main()
