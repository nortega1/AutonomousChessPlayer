#include "chess_state.h"

using namespace chess_state_lib;

const double ChessState::gridWidth = 0.07;
const string ChessState::boardBaseId = "/ar_marker_35";
const double ChessState::base_offset_x = 0.5 * ChessState::gridWidth;
const double ChessState::base_offset_y = 0.5 * ChessState::gridWidth;
const int ChessState::numFrames = 21;

//class constructor. subscribe to player_state and publish game_state
ChessState::ChessState() {

    robot_return_sub = node.subscribe<std_msgs::UInt8>("robot_return_code",10, &ChessState::tfCallback, this);
    game_cmd_sub = node.subscribe<std_msgs::String>("game_cmd", 10, &ChessState::gameCmdCallback, this);
    game_state_pub = node.advertise<std_msgs::String>("game_state", 10);

    command = "position startpos moves";

    prev_board = new Piece[8][8];
    curr_board = new Piece[8][8];

    readBoard();
    copyBoard();
}

//convert x/y tf translation coordinates to x/y chess coordinates
void ChessState::interpolatePos(double x_medium, double y_medium, int& x_chess, int& y_chess) {
	
    if(x_medium < 0 || y_medium < 0){
		x_chess = -1;
		y_chess = -1;
		return;
	}

   	x_chess = round((x_medium - base_offset_x) / gridWidth);
   	y_chess = round((y_medium - base_offset_y) / gridWidth);
}

//extracts x/y tf translation coordinates
void ChessState::extractTrans(tf::StampedTransform& transform, double& x_trans, double& y_trans){
	
    x_trans = transform.getOrigin().x();
	y_trans = transform.getOrigin().y();
}

void ChessState::tfCallback(const std_msgs::UInt8::ConstPtr& ret_code) {
    
    copyBoard();

    // TODO: use return code to determine action
    if (true) {
        readBoard();
    }

    // Find move from difference between previous and current state
    string human_move = find_move::getMove(prev_board, curr_board);

    // Publish command to go to Stockfish
    command += " " + human_move;    
    std_msgs::String msg;
    msg.data = command;
    game_state_pub.publish(msg);
}

void ChessState::gameCmdCallback(const std_msgs::String::ConstPtr& robot_move) {
    command += " " + robot_move->data;
}

void ChessState::copyBoard() {

    for(int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            prev_board[i][j] = curr_board[i][j];
        }
    }
}

void ChessState::readBoard() {    
	// array of structures that hold arTag name, and vectors of the x and y tf translation coordinates 
	struct tagReadings{
		int tag_num;
		vector<double> x_tf_coords;
		vector<double> y_tf_coords;	
	} tagRead[numPieces];

	
    // initialize each structure with -1.0 as coordinates
    // TODO: should be better way to initialize vectors
 	for(int i = 0; i < numPieces; i++){
		tagRead[i].tag_num = i;
      	for(int j=0; j<numFrames;j++){
			tagRead[i].x_tf_coords.push_back(-1.0);
			tagRead[i].y_tf_coords.push_back(-1.0);
		}		
	}
   
	//listen for transformation and store tf translation in array of structs
	for(int f = 0; f < numFrames; f++){
		for(int arTag = 0; arTag < numPieces; arTag++){
          	tf::StampedTransform transform;
            try {
               	listener.lookupTransform(boardBaseId, arTransformName(arTag), ros::Time(0), transform);
            }
            catch (tf::TransformException ex) {
                //ROS_INFO("Tag %d not found", arTag);
		//		ROS_INFO("transfrom exception : %s",ex.what());
                continue;
            }
            double x_trans, y_trans;
			extractTrans(transform, x_trans, y_trans);
			(tagRead[arTag].x_tf_coords)[f] = x_trans;
			(tagRead[arTag].y_tf_coords)[f] = y_trans;
        }
	}

	
	ROS_INFO("printing out struct ");
	for(int k = 0; k <numPieces; k++){
    	for(int i = 0; i < numFrames; i++){
			cout << i << endl;
			ROS_INFO("x:  %f \n", tagRead[k].x_tf_coords[i]);				ROS_INFO("y:  %f \n", tagRead[k].y_tf_coords[i]);
		}
	}

    for(int arTag=0; arTag< numPieces; arTag++){
  		vector<double> xTfCoords = tagRead[arTag].x_tf_coords;
		vector<double> yTfCoords = tagRead[arTag].y_tf_coords;
		
		double xMedianCoord = getMedian(xTfCoords);
		double yMedianCoord = getMedian(yTfCoords);

		//print out artag and medium coordinates
		ROS_INFO("ARTAG: %d   XMED: %f  YMED: %f  \n", arTag, xMedianCoord, yMedianCoord);
		
		//convert tf translation coordinates to chess coordinates
 		int xChessCoord, yChessCoord;
        interpolatePos(xMedianCoord, yMedianCoord, xChessCoord, yChessCoord);
		
		//store in board 2D array
        Piece p(arTag);
        curr_board[xChessCoord][yChessCoord] = p;
 	}
	
	//print board
    print_board();
}
 
void ChessState::print_board() {   
    cout << "--------------------\n";
    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j) {
            cout << curr_board[j][i];
        }
    cout << endl;
    }
}

int main(int argc, char ** argv) {
    ROS_INFO("Initializing chess_state node");
    ros::init(argc, argv, "chess_state");
    ChessState cs;
    ros::spin();
};
