#include <bits/stdc++.h>
using namespace std;

#define endl '\n'
#define IOS ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

const int N = 10000;

class NoC;
class Node;

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

    // route the flit to the destination using xy routing and update the ports and schedule
    NoC routeXY(NoC noc, MessageFlit flit, Node sourceNode, Node destinationNode, int startTime);
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

    MessageFlit getFlit(int i){
        return flits[i-1];
    }


};

enum Direction { 
    NORTH, SOUTH, EAST, WEST, NONE
};

class UnitPortSlot{
    MessageFlit messageFlit;
    Direction direction;

    friend class Port;
    friend class MessageFlit;
    
    public:
    UnitPortSlot() : messageFlit(0, 0), direction(NONE){
        messageFlit = MessageFlit(0, 0);
        direction = NONE;
    }
};


class Port{
    vector<UnitPortSlot> portSchedule;

    friend class Router;
    friend class MessageFlit;
    
    public:
    Port() : portSchedule(10000) {
    }

    int updateSchedule (int time, MessageFlit flit, Direction direction){
        int updatedTime = time;
        if (portSchedule[updatedTime].direction == NONE){// slot is empty
            portSchedule[updatedTime].messageFlit = flit;
            portSchedule[updatedTime].direction = direction;
        }else{
            while(true){
                updatedTime++;
                if (time > 10000){
                    cout<<"Error: Port is full"<<endl;
                    break;
                }
                if (portSchedule[updatedTime].direction == NONE){
                    portSchedule[updatedTime].messageFlit = flit;
                    portSchedule[updatedTime].direction = direction;
                    break;
                }
            }
        }
        return updatedTime - time + 1;   
    }
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
    Port localPort;

    friend class Node;
    friend class MessageFlit;
    
    public:
    Router(){
        northPort = Port();
        southPort = Port();
        eastPort = Port();
        westPort = Port();
    }
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

    friend class NoC;
    friend class MessageFlit;

    public:
    Node(int y, int x){
        this->locationX = x;
        this->locationY = y;
        router = Router();
    }
    int getLocationX(){
        return locationX;
    }
    int getLocationY(){
        return locationY;
    }
};

class NoC{
    int n;
    vector<Node> nodes;
    friend class MessageFlit;
    
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
    Node getNode(int i){
        return nodes[i];
    }
    int getN(){
        return n;
    }

    void print(){
        for (int i = 0; i < n*n; i++){
            cout << nodes[i].getLocationX() << " " << nodes[i].getLocationY() << endl;
        }
    }
};


NoC MessageFlit::routeXY(NoC noc, MessageFlit flit, Node sourceNode, Node destinationNode, int startTime){
    int sourceNodeX = sourceNode.getLocationX();
    int sourceNodeY = sourceNode.getLocationY();
    int currentX = sourceNodeX;
    int currentY = sourceNodeY;
    int destinationNodeX = destinationNode.getLocationX();
    int destinationNodeY = destinationNode.getLocationY();
    int time = startTime;
    cout<<"ID : m_"<<flit.message_id<<"_"<<flit.id<<endl;
    cout<<"PATH : ";
    cout<<"("<<currentY<<","<<currentX<<") ";
    while(true){    
        
        if (currentX == destinationNodeX && currentY == destinationNodeY){
            cout<<endl;
            cout<<"TIME : "<<time<<endl;
            cout<<"---------------------------------"<<endl;
            return noc;
        }
        else if(currentX < destinationNodeX){
            if (currentX == sourceNodeX){
                time += noc.nodes[(currentY-1) * noc.getN() + currentX - 1].router.localPort.updateSchedule(time, flit, EAST);
            }
            else{
                time += noc.nodes[(currentY-1) * noc.getN() + currentX - 1].router.westPort.updateSchedule(time, flit, EAST);
            }
            currentX++;
        }else if (currentX > destinationNodeX){
            if (currentX == sourceNodeX){
                time += noc.nodes[(currentY-1) * noc.getN() + currentX - 1].router.localPort.updateSchedule(time, flit, WEST);
            }
            else{
                time += noc.nodes[(currentY-1) * noc.getN() + currentX - 1].router.eastPort.updateSchedule(time, flit, WEST);
            }
            currentX--;
        }else if (currentX == destinationNodeX){
            if (currentY < destinationNodeY){
                if (currentY == sourceNodeY){
                    time += noc.nodes[(currentY-1) * noc.getN() + currentX - 1].router.localPort.updateSchedule(time, flit, SOUTH);
                }
                else{
                    time += noc.nodes[(currentY-1) * noc.getN() + currentX - 1].router.northPort.updateSchedule(time, flit, SOUTH);
                }
                currentY++;
            }
            else if (currentY > destinationNodeY){
                if (currentY == sourceNodeY){
                    time += noc.nodes[(currentY-1) * noc.getN() + currentX - 1].router.localPort.updateSchedule(time, flit, NORTH);
                }
                else{
                    time += noc.nodes[(currentY-1) * noc.getN() + currentX - 1].router.southPort.updateSchedule(time, flit, NORTH);
                }
                currentY--;
            }
        }
        cout<<"-> ("<<currentY<<","<<currentX<<") ";
    }
    return noc;
}


// int get_task_rank(int[1000][1000] task_graph, int[1000][1000] execution_time_matrix, int no_of_tasks, int task_id){
//     //checking sink node or not
//     bool sink_node = true;
//     for (int i = 1; i<= no_of_tasks; i++){
//         if (task_graph[task_id][i] != 0) sink_node = false;
//     }
//     cout<<sink_node<<endl;
// }


int main() {
    IOS
    #ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif


    srand(time(0));
    
    int n;
    int no_of_tasks;
    int task_graph[1000][1000];
    int execution_time_matrix[1000][1000];
    int rank [1000];

    // cout<<"Enter the size of n x n mesh NoC:";
    cin>>n;

    // cout<<"Enter number of tasks :";
    cin>>no_of_tasks;


    // cout<<"Input the adjancy matrix :";
    for (int i = 1; i <= no_of_tasks; i++) {
        for (int j = 1; j <= no_of_tasks; j++) {
            cin >> task_graph[i][j];
        }
    }


    // input execution times
    // for (int i = 1; i <= no_of_tasks; i++) {
    //    for (int j = 0; j < n * n ; j++) {
    //         cin >> execution_time_matrix[i][j];
    //     }
    // }

                // OR

    // generate random execution times
    for (int i = 1; i <= no_of_tasks; i++) {
        for (int j = 0; j < n * n ; j++) {
            execution_time_matrix[i][j] = rand() % 21 + 10;;
        }
    }

    cout<<"Execution time matrix"<<endl;

    for (int i = 1; i <= no_of_tasks; i++) {
        for (int j = 0; j < n * n ; j++) {
            cout<<execution_time_matrix[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;


    // int rank_6 = get_task_rank(task_graph, execution_time_matrix, no_of_tasks, 6);




    NoC noc = NoC(n);
    // noc.print();
    Message m_12 = Message(1, 2, 3);
    MessageFlit m_12_1 = m_12.getFlit(1);
    MessageFlit m_12_2 = m_12.getFlit(2);
    MessageFlit m_12_3 = m_12.getFlit(3);
    Node sourceNode = noc.getNode(0);
    Node destinationNode = noc.getNode(8);
    noc = m_12_1.routeXY(noc, m_12_1, sourceNode, destinationNode, 0);
    noc = m_12_2.routeXY(noc, m_12_2, sourceNode, destinationNode, 0);
    noc = m_12_3.routeXY(noc, m_12_3, sourceNode, destinationNode, 0);

    

    #ifndef ONLINE_JUDGE
    cerr << "\ntime taken : " << (float)clock() / CLOCKS_PER_SEC << " secs " << endl;
    #endif 
    return 0;
}




