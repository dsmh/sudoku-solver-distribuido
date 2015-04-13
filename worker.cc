
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <czmq.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>


using namespace std;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

void sendMsg(void* channel, vector<string> parts) {
  zmsg_t* msg = zmsg_new();
  for (const string& s : parts) {
    zmsg_addstr(msg, s.c_str());
  }
  zmsg_send(&msg, channel);
}

string intToS(int n)
{
  std::ostringstream sin;
  sin << n;
  std::string val = sin.str();
  return val;
}

int main()
{
    zctx_t* context = zctx_new();
    void* client = zsocket_new(context, ZMQ_DEALER);
  //zsocket_connect(client, "tcp://192.168.8.214:5555");
    zsocket_connect(client, "tcp://localhost:5555");
    string myName="NetWorker";
    zmq_pollitem_t items[] = {{client, 0, ZMQ_POLLIN, 0}};
    sendMsg(client, {"join", myName});       


    while(true){
        int st = zmq_poll(items, 1, 10 * ZMQ_POLL_MSEC);
        if (st == -1) {
      // Handles termination by ^C
            break;
        }
    

        if (items[0].revents & ZMQ_POLLIN) {
      // This is executed if there is data in the client socket that corresponds
      // to items[0]
      //cout << "PLAYER1: " << endl;
            zmsg_t* msg = zmsg_recv(client);
      
      //zmsg_print(msg);

            zframe_t* quemado = zmsg_pop(msg);
      
            if(zframe_streq(quemado,"opponentMove"))
		    {
			    char* datos = zmsg_popstr(msg);
			    char* datos2 = zmsg_popstr(msg);
			    char* datos3 = zmsg_popstr(msg);
			
			  //cout << "quemado "<< quemado << endl;
			  //cout << "datos1 "<< datos << endl;
			  //cout << "datos2 "<< datos2 << endl;
			  //cout << "datos3 "<< datos3 << endl;
		
			    do_enemy_move(datos, leftPaddle, rightPaddle, upperPaddle, downPaddle, atoi(datos2), atoi(datos3));

		    }else if(zframe_streq(quemado,"fin"))
			{
				isPlaying = false;
			}

      
      zmsg_destroy(&msg);
	}
}//fin while true
    
    
    return 0;
}
