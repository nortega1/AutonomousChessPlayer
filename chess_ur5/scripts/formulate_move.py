#!/usr/bin/env python
import rospy

from std_msgs.msg import String
from pexpect import spawn

def callback(data, args):
    # Extract arguments
    game_cmd_pub = args[0]
    sf = args[1]
   
    # Send game state to stockfish and begin best move search
    sf.sendline(data.data)
    sf.sendline('go')
    
    # Use regex to get "bestmove" as returned by stockfish
    i = sf.expect('bestmove \w*')
    move = sf.after.split()[-1]

    # Publish response
    game_cmd_pub.publish(move)
    

def formulate_move():
   
    # Run stockfish executable
    # TODO: include install with package
    sf = spawn('stockfish')

    # Initialize ROS node
    rospy.init_node("formulate_move", anonymous=True)
    
    # Publish the String move to be made as returned by stockfish
    game_cmd_pub = rospy.Publisher("game_cmd", String, queue_size=10)

    # Subscribe to the String command from the user as parsed by chess_state node
    game_state_sub = rospy.Subscriber("game_state", String, callback, (game_cmd_pub, sf))
    
    # Spin ROS
    rospy.spin()

if __name__ == "__main__":
   
    try:
        formulate_move()
    except rospy.ROSInterruptException:
        pass

