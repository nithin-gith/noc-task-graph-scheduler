#include <bits/stdc++.h>
#include "globals.h"
using namespace std;

#define endl '\n'
#define IOS ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

const int N = 10000;



class NoC;
class Node;
class ProcessingElement;


class Task{
    
    int id;
    // ProcessingElement allotedProcessor;
    ProcessingElement* allotedProcessor;
    // int processingElement;
    int startTime;
    int endTime;
    public:
    Task(int id){
        this->id = id;
    }
    int getTaskId(){
        return this->id;
    }
    pair<int,int> getStartEndTime(){
        return pair<int, int>(this->startTime, this->endTime);
    }
    void setStartEndTime(int startTime, int endTime){
        this->startTime = startTime;
        this->endTime = endTime;
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
    public:
    vector<Task> processingElementSchedule;

    ProcessingElement(){
        Task dummy_task = Task(0);
        vector<Task> zeroSchedule(10000, dummy_task);
        this->processingElementSchedule = zeroSchedule;
    }

    void print(){
        for(int i = 0; i<20;i++){
            cout<<this->processingElementSchedule[i].getTaskId()<<endl;
        }
    }

    int getEarliestAvailTime(int currentTime, int executionTime);
    pair<int, int> allocateProcessor(int taskId, int earliestTime, int executionTime);
};


class Node{
    // int id;
    int locationX;
    int locationY;
    Router router;
    

    friend class NoC;
    friend class MessageFlit;

    public:
    ProcessingElement processingElement;
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
    pair<int, int> getLocation(){
        return pair<int, int>(locationY, locationX);
    }
};

class NoC{
    int n;
    
    friend class MessageFlit;
    
    public: 
    vector<Node> nodes;
    NoC(int n){
        this->n = n;
        Node dummy_node = Node(0, 0);
        nodes.push_back(dummy_node);
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
        for (int i = 1; i <= n*n; i++){
            cout << nodes[i].getLocationY() << " " << nodes[i].getLocationX() << endl;
        }
    }

    vector<int> getNeighbors(int nodeId);
};



double getTaskRank(int task_graph[1000][1000], int execution_time_matrix[1000][1000], map<int, double> task_ranks, int task_id){
    bool sink_node = true;

    if (task_ranks[task_id] != 0) return task_ranks[task_id];

    for (int i = 1; i <= no_of_tasks; i++){
        if (task_graph[task_id][i] != 0){
            sink_node = false; 
            break;
        }
    }

    double sum_exec_time = 0;
    for (int i = 1; i <= n*n ; i++ ){
        sum_exec_time += execution_time_matrix[task_id][i];
    }
    double avg_exec_time = sum_exec_time / (n * n * 1.0);

    if (sink_node) return avg_exec_time;
    else{
        // max_succ_task_rank = P(T_j) + CC_i,j Tj is successor of Ti
        double max_succ_task_rank = 0;
        for (int i = 1; i<= no_of_tasks; i++){
            if (task_graph[task_id][i]!=0) max_succ_task_rank = max(max_succ_task_rank, getTaskRank(task_graph, execution_time_matrix, task_ranks, i) + task_graph[task_id][i]);
        }
        task_ranks[task_id] = avg_exec_time + max_succ_task_rank;
        return task_ranks[task_id];
    }
    return 0.0;
}

bool sortByValue(const std::pair<int, int>& a, const std::pair<int, int>& b) {
    return a.second > b.second; // Sort in descending order of ranks
}

vector<int> generateTaskPriorityList(int task_graph[1000][1000], int execution_time_matrix[1000][1000]){
    
    map<int, double> task_ranks;
    vector<int> task_priority_list;
    map<int, double> avg_exec_times;
    
    for (int i = 1; i<= no_of_tasks; i++){
        task_ranks[i] = 0;
    }
    for (int i = 1; i<=no_of_tasks ; i++){
        task_ranks[i] = getTaskRank(task_graph, execution_time_matrix,task_ranks, i);
    }
    vector<pair<int, double> > taskid_rank_pairs(task_ranks.begin(), task_ranks.end());
    sort(taskid_rank_pairs.begin(), taskid_rank_pairs.end(), sortByValue);

    for (auto taskid_rank_pair : taskid_rank_pairs){
        task_priority_list.push_back(taskid_rank_pair.first);
    }
    
    return task_priority_list;
}


int ProcessingElement::getEarliestAvailTime(int currentTime, int executionTime){
    int count = 0;
    int earliestAvailTime = 0;
    for (int i = currentTime; i < 10000; i++) {
        if (this->processingElementSchedule[i].getTaskId() == 0) {
            count++;
            if (count == executionTime) {
                earliestAvailTime = i - executionTime + 1;
                return earliestAvailTime;
            }
        } else {
            count = 0;
        }
    }
}

pair<int,int> ProcessingElement::allocateProcessor(int taskId, int earliestTime, int executionTime){
    Task allotedTask = Task(taskId);
    int startTime;
    int endTime;
    int count = 0;
    for (int i = earliestTime; i < 10000; i++) {
        if (this->processingElementSchedule[i].getTaskId() == 0) {
            count++;
            if (count == executionTime) {
                for (int j = i - executionTime + 1; j <= i; ++j) {
                    this->processingElementSchedule[j] = allotedTask;
                    startTime = j;
                }
                endTime = startTime + executionTime;
                break;
            }
        } else {
            count = 0;
        }
    }
    return pair<int,int> (startTime, endTime);
}


vector<int> getPredTaskIds(int taskId, int task_graph[1000][1000]){
    vector<int> pred_task_ids;
    for (int i = 1; i <= no_of_tasks; i++) {
        if (task_graph[i][taskId]!=0)pred_task_ids.push_back(i);
    }
    return pred_task_ids;
}

vector<int> NoC::getNeighbors(int nodeId){
    vector<int> neighbors;
     int row = (nodeId - 1) / n;
    int col = (nodeId - 1) % n;

    if (row > 0)  // North neighbor
        neighbors.push_back(nodeId - n);
    if (row < n - 1)  // South neighbor
        neighbors.push_back(nodeId + n);
    if (col > 0)  // West neighbor
        neighbors.push_back(nodeId - 1);
    if (col < n - 1)  // East neighbor
        neighbors.push_back(nodeId + 1);

    return neighbors;
}



int main() {
    IOS
    #ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif


    srand(time(0));
    
    
    int task_graph[1000][1000];
    int execution_time_matrix[1000][1000];
    vector<int> task_priority_list(1000, 0);
    map<int, int> task_processor_mappings;

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
    for (int i = 1; i <= no_of_tasks; i++) {
       for (int j = 1; j <= n * n ; j++) {
            cin >> execution_time_matrix[i][j];
        }
    }

                // OR

    // generate random execution times
    // for (int i = 1; i <= no_of_tasks; i++) {
    //     for (int j = 0; j < n * n ; j++) {
    //         execution_time_matrix[i][j] = rand() % 21 + 10;
    //     }
    // }

    // cout<<"Execution time matrix"<<endl;

    // for (int i = 1; i <= no_of_tasks; i++) {
    //     for (int j = 1; j <= n * n ; j++) {
    //         cout<<execution_time_matrix[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    // cout<<endl;

    task_priority_list = generateTaskPriorityList(task_graph, execution_time_matrix);



    NoC noc = NoC(n);
    for(int i = 0; i<=task_priority_list.size();i++){
        if (i==0){
            int min_exec_time_proccesor_id = 0;
            int min_exec_time = INT_MAX;
            for (int j = 1; j<=n*n ; j++){
                if (min_exec_time > execution_time_matrix[task_priority_list[i]][j]){
                    min_exec_time_proccesor_id = j;
                    min_exec_time = execution_time_matrix[task_priority_list[i]][j];
                }
            }
            noc.nodes[min_exec_time_proccesor_id].processingElement.allocateProcessor(task_priority_list[i], 0, execution_time_matrix[task_priority_list[i]][min_exec_time_proccesor_id]);
            task_processor_mappings[task_priority_list[i]] = min_exec_time_proccesor_id;
            cout<<"task "<<task_priority_list[i]<<" has been alloted to "<<min_exec_time_proccesor_id<<endl;
        }else{
            vector<int> possible_processors;
            vector<int> pred_task_ids;

            pred_task_ids = getPredTaskIds(task_priority_list[i], task_graph);
            for (auto pred_task_id : pred_task_ids){
                possible_processors.push_back(task_processor_mappings[pred_task_id]);
                vector<int> neighbors;
                neighbors = noc.getNeighbors(task_processor_mappings[pred_task_id]);
                for (auto neighbor :neighbors)possible_processors.push_back(neighbor);
            }
            set<int> unique_processors(possible_processors.begin(), possible_processors.end());
            possible_processors.assign(unique_processors.begin(), unique_processors.end());
        }

        // cout<<i+1<<"th task possiblities"<<endl;
        // for (auto possible_node : possible_nodes){
        //     cout<<possible_node.getLocation().first<<" "<<possible_node.getLocation().second<<endl;
        // }
    }






    // // noc.print();
    // Message m_12 = Message(1, 2, 3);
    // MessageFlit m_12_1 = m_12.getFlit(1);
    // MessageFlit m_12_2 = m_12.getFlit(2);
    // MessageFlit m_12_3 = m_12.getFlit(3);
    // Node sourceNode = noc.getNode(0);
    // Node destinationNode = noc.getNode(15);
    // noc = m_12_1.routeXY(noc, m_12_1, sourceNode, destinationNode, 0);
    // noc = m_12_2.routeXY(noc, m_12_2, sourceNode, destinationNode, 0);
    // noc = m_12_3.routeXY(noc, m_12_3, sourceNode, destinationNode, 0);

    

    #ifndef ONLINE_JUDGE
    cerr << "\ntime taken : " << (float)clock() / CLOCKS_PER_SEC << " secs " << endl;
    #endif 
    return 0;
}