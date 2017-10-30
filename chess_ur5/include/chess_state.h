#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <std_msgs/UInt8.h>
#include <std_msgs/String.h>
#include <math.h>
#include "Piece.h"
#include "find_move.h"

#define numPieces 32

using namespace std;

class ChessState {
	public: 
		ChessState();
	private:
		void tfCallback(const std_msgs::UInt8::ConstPtr&);
        void gameCmdCallback(const std_msgs::String::ConstPtr&);
        void readBoard();
        void copyBoard();
        void print_board();

        ros::NodeHandle node;
 		ros::Subscriber robot_return_sub;
    	ros::Publisher game_state_pub;
        ros::Subscriber game_cmd_sub;
        tf::TransformListener listener;
		
        void interpolatePos(double, double, int&, int&);
		void extractTrans(tf::StampedTransform&, double&, double&);

        static const double gridWidth;
    	static const string boardBaseId; 
        static const double base_offset_x;
        static const double base_offset_y;
        static const int numFrames;
        
        Piece (*prev_board)[8];
        Piece (*curr_board)[8];

        string command;
};

namespace chess_state_lib {
/*    
    struct pieceCoordinates{
        int tag_num;
		int x_chess_coord;
		int y_chess_coord;	
    } pieceCoords[numPieces];
*/
    //returns string of ar_marker
    string arTransformName(int i) {
        return boost::str(boost::format("/ar_marker_%d") % i );
    }

    double getMedian(vector<double> &v) {
        int size = v.size();

        sort(v.begin(), v.end());

        double median;
        if (size % 2 == 0) {
            median = (v[size/2 - 1] + v[size/2]) / 2.0;
        }
        else {
            median = v[size/2];
        }

        return median;
    }
/*
    //prints structure of artag and chess coordinates
    void printPieceCoords(){
	    for(int i = 0; i < numPieces; i++){
		    ROS_INFO("Tag:  %d  X:  %d  Y:  %d\n", pieceCoords[i].tag_num,pieceCoords[i].x_chess_coord,pieceCoords[i].y_chess_coord);
	    }
    }
  */
}

