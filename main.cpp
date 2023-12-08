#include <bits/stdc++.h>
using namespace std;

#define endl '\n'
#define IOS ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

const int N = 10000;


class Task{
    
    int id;
    
    public:
    Task(int id){
        this->id = id;
    }
};

class MessageFlit{

    int id;
    int message_id;

    public:
    MessageFlit(int id, int message_id){
        this->id = id;
        this->message_id = message_id;
    }
};

class Message{

    int id;
    int sourceTaskId;
    int destinationTaskID;
    int messageSize;
    vector<MessageFlit> flits;

    public:
    Message(int sourceTaskId, int destinationTaskID, int messageSize){
        this->id = 10 * sourceTaskId + destinationTaskID;
        this->sourceTaskId = sourceTaskId;
        this->destinationTaskID = destinationTaskID;
        this->messageSize = messageSize;
        for (int i = 1; i <= messageSize ; i++){
            MessageFlit new_flit = MessageFlit(i, id);
            flits.push_back(new_flit);
        }
    }
};

enum Direction { 
    NORTH, SOUTH, EAST, WEST
};

class UnitPortSlot{
    Message message;
    Direction direction;
};


class Port{
    vector<UnitPortSlot> portSchedule;
};

class Router{
    // int id;
    // int location_x;
    // int location_y;
    
    Router *northNeighbour;
    Router *southNeighbour; 
    Router *eastNeighbour;
    Router *westNeighbour;

    Port northPort;
    Port southPort;
    Port eastPort;
    Port westPort;
};

class ProcessingElement{
    // int id;
    // int location_x;
    // int location_y;
    vector<Task> processingElementSchedule;
};

class Node{
    // int id;
    int locationX;
    int locationY;
    Router router;
    ProcessingElement processingElement;
    public:
    Node(int x, int y){
        this->locationX = x;
        this->location_y = y;
    }
    int getLocationX(){
        return locationX;
    }
    int getLocationY(){
        return locationX;
    }
};

class NoC{
    int n;
    vector<Node> nodes;
    
    public: 
    NoC(int n){
        this->n = n;
        for (int i = 1; i <= n; i++){
            for (int j = 1; j <= n ; j++){
                Node new_node = Node(i, j);
                nodes.push_back(new_node);
            }
        }
    }
    
    void print(){
        for (int i = 0; i < n*n; i++){
            cout << nodes[i].getLocationX() << " " << nodes[i].getLocationY() << endl;
        }
    }
};


int main() {
    IOS
    #ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif



    NoC noc = NoC(4);




    #ifndef ONLINE_JUDGE
    cerr << "\ntime taken : " << (float)clock() / CLOCKS_PER_SEC << " secs " << endl;
    #endif 
    return 0;
}




