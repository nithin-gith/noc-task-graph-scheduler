#include <bits/stdc++.h>
#include "globals.h"
using namespace std;

#define endl '\n'
#define IOS ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

const int N = 10000;

int getNoOfFlits(int messageSize);

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

    

    public:
    int id;
    string message_id;
    MessageFlit(int id, string message_id){
        this->id = id;
        this->message_id = message_id;
    }

    // route the flit to the destination using xy routing and update the ports and schedule
    NoC routeXY(NoC noc, MessageFlit flit, Node sourceNode, Node destinationNode, int startTime);
};

class Message{

    public:

    string id;
    int sourceTaskId;
    int destinationTaskID;
    int messageSize;
    vector<MessageFlit> flits;

    // intialising the message 
    Message(int sourceTaskId, int destinationTaskID, int messageSize){
        // message id is 100 * source task id + dest task id 
        // ex : message from task 1 to task 2 is m_1002 
        this->id = to_string(sourceTaskId * 100)  + to_string(destinationTaskID);
        this->sourceTaskId = sourceTaskId;
        this->destinationTaskID = destinationTaskID;
        this->messageSize = messageSize;
        MessageFlit dummy_flit = MessageFlit(0, "0");
        flits.push_back(dummy_flit);
        int no_of_flits = getNoOfFlits(messageSize);
        for (int i = 1; i <= no_of_flits; i++){
            MessageFlit new_flit = MessageFlit(i, id);
            flits.push_back(new_flit);
        }
    }

    MessageFlit getFlit(int i){
        return flits[i];
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
            UnitPortSlot() : messageFlit(0, "0"), direction(NONE){
        messageFlit = MessageFlit(0, "0");
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

int updateSchedule(int time, MessageFlit flit, Direction direction, int message_size) {
        int updatedTime = time;
        int consecutiveEmptySlots = 0; 
        int startSlot = -1; 

        while (updatedTime < 10000) { 
            if (portSchedule[updatedTime].direction == NONE) { 
                if (consecutiveEmptySlots == 0) {
                    startSlot = updatedTime; 
                }
                consecutiveEmptySlots++;
                if (consecutiveEmptySlots == message_size) {
                   
                    for (int i = startSlot; i < startSlot + message_size; i++) {
                        portSchedule[i].messageFlit = flit;
                        portSchedule[i].direction = direction;
                    }
                    return updatedTime - time + 1; 
                }
            } else {
                consecutiveEmptySlots = 0; 
                startSlot = -1; 
            }
            updatedTime++;
        }

        cout << "Error: Port is full" << endl;
        return -1; 
    }

    void print(int t){
        for (int i = 0 ;i < t; i++){
            cout<<portSchedule[i].messageFlit.message_id<<"_"<<portSchedule[i].messageFlit.id<<":"<<portSchedule[i].direction<<endl;
        }
    }
};

class Router{
    // int id;
    // int location_x;
    // int location_y;

    friend class Node;
    friend class MessageFlit;
    
    public:
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
        localPort = Port();
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
    
    

    friend class NoC;
    friend class MessageFlit;

    public:
    Router router;
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
    // intialising the noc architecture
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
    vector<Message> getMessagePriorityList(int taskId, int tentProcessorId, int task_graph[1000][1000], map<int, int> task_processor_mappings, map<int, pair<int, int>> tasks_start_end_times);
};



double getTaskRank(int task_graph[1000][1000], int execution_time_matrix[1000][1000], map<int, double> task_ranks, int task_id){
    // this fucntion returns the task rank for a given task
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
        // recursively calling this function to calculate the ranks of all the tasks
        for (int i = 1; i<= no_of_tasks; i++){
            if (task_graph[task_id][i]!=0) max_succ_task_rank = max(max_succ_task_rank, getTaskRank(task_graph, execution_time_matrix, task_ranks, i) + task_graph[task_id][i]);
        }
        task_ranks[task_id] = avg_exec_time + max_succ_task_rank;
        return task_ranks[task_id];
    }
    return 0.0;
}

bool sortByDecreasingValue(const std::pair<int, int>& a, const std::pair<int, int>& b) {
    return a.second > b.second; // Sort in descending order of ranks
}

int getManhattanDistance(int sourceProcessorId, int destProcessorId){
    int source_x = (sourceProcessorId - 1) % n + 1;
    int source_y = (sourceProcessorId - 1) / n + 1;
    int dest_x = (destProcessorId - 1) % n + 1;
    int dest_y = (destProcessorId - 1) / n + 1;

    return abs(source_x - dest_x) + abs(source_y - dest_y);
}

double getPCMValue(int task_graph[1000][1000], int execution_time_matrix[1000][1000], int pcm[300][300], int task_id, int processor_id){
    
    bool sink_node = true;
    if (pcm[task_id][processor_id] != 0)return pcm[task_id][processor_id];

    for (int i = 1; i <= no_of_tasks; i++){
        if (task_graph[task_id][i] != 0){
            sink_node = false; 
            break;
        }
    }

    double max_succ_task_pcm = 0;
    if (sink_node) return execution_time_matrix[task_id][processor_id];
    else{
        for (int i = 1; i<= no_of_tasks; i++){
            double min_task_pcm = INT_MAX*1.0;
            if (task_graph[task_id][i]!=0) {
                for (int j = 1; j <= n*n; j++){
                    int comm_dist = getManhattanDistance(processor_id, j);
                    pcm[i][j] = getPCMValue(task_graph, execution_time_matrix, pcm, i, j);
                    min_task_pcm = min(min_task_pcm, pcm[i][j] + execution_time_matrix[i][j] + execution_time_matrix[task_id][j] + comm_dist * 1.0);   
                }
                max_succ_task_pcm = max(max_succ_task_pcm, min_task_pcm);
            }
            
        }
    }
    return max_succ_task_pcm;
}

vector<int> generateTaskPriorityList(int task_graph[1000][1000], int execution_time_matrix[1000][1000]){
    
    map<int, double> task_ranks;
    vector<int> task_priority_list;
    int pcm[300][300];

    for(int i = 1; i <= no_of_tasks; i++){
        for (int j = 1; j<=n*n; j++){
            pcm[i][j] = getPCMValue(task_graph, execution_time_matrix, pcm, i, j);
        }
    }

    for(int i = 1; i <= no_of_tasks; i++){
        double sum_pcm = 0;
        for (int j = 1; j<=n*n; j++){
           sum_pcm += pcm[i][j];
        }
        task_ranks[i] = sum_pcm / (n*n);
    }


    // sorting the tasks based on decreasing order of their ranks
    vector<pair<int, double> > taskid_rank_pairs(task_ranks.begin(), task_ranks.end());
    sort(taskid_rank_pairs.begin(), taskid_rank_pairs.end(), sortByDecreasingValue);

    for (auto taskid_rank_pair : taskid_rank_pairs){
        task_priority_list.push_back(taskid_rank_pair.first);
    }
    
    return task_priority_list;
}





int ProcessingElement::getEarliestAvailTime(int currentTime, int executionTime){
    // this function returns the earlisest avail time for an processor
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
    return earliestAvailTime;
}

pair<int,int> ProcessingElement::allocateProcessor(int taskId, int earliestTime, int executionTime){
    Task allotedTask = Task(taskId);
    int startTime = 0;
    int endTime = 0;
    int count = 0;
    // this function allocates the task to processor after checking vacancy and allocates at earliest avail time
    for (int i = earliestTime; i < 10000; i++) {
        if (this->processingElementSchedule[i].getTaskId() == 0) {
            count++;
            if (count == executionTime) {
                for (int j = i - executionTime + 1; j <= i; ++j) {
                    this->processingElementSchedule[j] = allotedTask;
                }
                startTime = i - executionTime + 1;
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
    // this function returns the predecessor task ids of a given task in task graph
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

int getNoOfFlits(int messageSize){
    // this is function to calucate no of flits for given message size
    if (messageSize%b_w == 0){
        return messageSize/b_w;
    }
    return messageSize/b_w + 1;
}

vector<Message> NoC::getMessagePriorityList(int taskId, int tentProcessorId, int task_graph[1000][1000], map<int, int> task_processor_mappings, map<int, pair<int, int>> tasks_start_end_times){
    
    vector<Message> message_priority_list;
    vector<int> pred_task_ids;
    pred_task_ids = getPredTaskIds(taskId, task_graph);

    // earliest pred task completion
    for (auto pred_task_id : pred_task_ids){
        Message message = Message(pred_task_id, taskId, task_graph[pred_task_id][taskId]);
        message_priority_list.push_back(message);
    }
    return message_priority_list;
}

vector<vector<int>> generateShortestPath(int source, int dest) {

    // grid is just a 2d array from 1 to n^2
    vector<vector<int>> grid(n, vector<int>(n));
    int count = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            grid[i][j] = count++;
        }
    }

    // this (dx, dy) pairs denotes the neighbors
    vector<int> dx = {-1, 1, 0, 0};
    vector<int> dy = {0, 0, -1, 1};

    // maintaining queue for bfs
    queue<vector<int>> q;


    unordered_map<int, vector<vector<int>>> shortest_path_from_source;


    q.push({source});
    shortest_path_from_source[source] = {{source}};


    while (!q.empty()) {
        // using first route of the queue and last node of that route
        vector<int> curr = q.front();
        q.pop();
        int last_node = curr.back();

        if (last_node == dest) continue; 

        int x = (last_node - 1) / n;
        int y = (last_node - 1) % n;

        // exploring different possiblities and routes between nodes and updating shortest paths
        for (int k = 0; k < 4; ++k) {
            int nx = x + dx[k];
            int ny = y + dy[k];
            if (nx >= 0 && nx < n && ny >= 0 && ny < n) {
                int neighbor_node = grid[nx][ny];
                if (shortest_path_from_source.find(neighbor_node) == shortest_path_from_source.end() ||
                    shortest_path_from_source[neighbor_node][0].size() == curr.size() + 1) {
                    vector<int> new_path = curr;
                    new_path.push_back(neighbor_node);
                    shortest_path_from_source[neighbor_node].push_back(new_path);
                    q.push(new_path);
                }
            }
        }
    }

    // Return all shortest paths from source to destination
    return shortest_path_from_source[dest];
}

map<string, vector<vector<int> > > generateShortestPaths(){
    // generation shortest paths from all processors to all other processors
    map<string, vector<vector<int> > > shortest_paths_map;
    for(int i = 1 ; i<=n * n ;i++){
        for(int j = 1 ; j<=n * n;j++){
            if (i == j)continue;
            else{
                string route_id = to_string(i * 100) + to_string(j);
                shortest_paths_map[route_id] = generateShortestPath(i, j);
            }
        }
    }

    return shortest_paths_map;
}

Direction getDirection(int sourceNodeId, int neighborNodeId){
    // to get the direction from source to neighbor in n x n architecture
    if (sourceNodeId + 1 == neighborNodeId) return EAST;
    else if(sourceNodeId - 1 == neighborNodeId) return WEST;
    else if(sourceNodeId + n == neighborNodeId) return SOUTH;
    else if(sourceNodeId - n == neighborNodeId) return NORTH;
    return NONE;
}

int routeMessageXY(Message m_ij, NoC &noc, int sourceProcessorId, int destProcessorId, int startTime){
    
    int no_of_flits = getNoOfFlits(m_ij.messageSize);
    int transmission_time_message = 0;


    for (int i = 1; i <= no_of_flits; i++){
        int transmission_time_flit = 0;
        int source_x = (sourceProcessorId - 1) % n + 1;
        int source_y = (sourceProcessorId - 1) / n + 1;
        int dest_x = (destProcessorId - 1) % n + 1;
        int dest_y = (destProcessorId - 1) / n + 1; 
        int cur_x = source_x;
        int cur_y = source_y;
        bool dir_change = false;
        
        while(!(cur_x == dest_x && cur_y == dest_y)){
            bool is_source_node = false;
            int node_id = (cur_y - 1) * n + cur_x;

            if (cur_x == source_x && cur_y == source_y) is_source_node = true;
            if(cur_x > dest_x){
                if (is_source_node){
                    transmission_time_flit = noc.nodes[node_id].router.localPort.updateSchedule(startTime, m_ij.flits[i], WEST, m_ij.messageSize * 1.0);
                }else{
                    transmission_time_flit += noc.nodes[node_id].router.eastPort.updateSchedule(startTime + transmission_time_flit, m_ij.flits[i], WEST, m_ij.messageSize * 1.0);
                }
                cur_x--;
                if (cur_x == dest_x) dir_change = true;
            }else if (cur_x < dest_x){
                if (is_source_node){
                    transmission_time_flit = noc.nodes[node_id].router.localPort.updateSchedule(startTime, m_ij.flits[i], EAST, m_ij.messageSize * 1.0);
                }else{
                    transmission_time_flit += noc.nodes[node_id].router.eastPort.updateSchedule(startTime + transmission_time_flit, m_ij.flits[i], EAST, m_ij.messageSize * 1.0);
                }
                cur_x++;
                if (cur_x == dest_x) {dir_change = true;}
            }else{
                if(cur_y > dest_y){
                    if (is_source_node){
                        transmission_time_flit = noc.nodes[node_id].router.localPort.updateSchedule(startTime, m_ij.flits[i], NORTH, m_ij.messageSize * 1.0);
                    }else if(dir_change){
                        if (source_x < dest_x) transmission_time_flit += noc.nodes[node_id].router.westPort.updateSchedule(startTime + transmission_time_flit, m_ij.flits[i], NORTH, m_ij.messageSize * 1.0);
                        if (source_x > dest_x) transmission_time_flit += noc.nodes[node_id].router.eastPort.updateSchedule(startTime + transmission_time_flit, m_ij.flits[i], NORTH, m_ij.messageSize * 1.0);
                        dir_change = false;
                    }else{
                        transmission_time_flit += noc.nodes[node_id].router.northPort.updateSchedule(startTime + transmission_time_flit, m_ij.flits[i], NORTH, m_ij.messageSize * 1.0);
                    }
                    cur_y--;
                }else if(cur_y < dest_y){
                    if (is_source_node){
                        transmission_time_flit = noc.nodes[node_id].router.localPort.updateSchedule(startTime, m_ij.flits[i], SOUTH, m_ij.messageSize * 1.0);
                    }else if(dir_change){
                        if (source_x < dest_x) transmission_time_flit += noc.nodes[node_id].router.westPort.updateSchedule(startTime + transmission_time_flit, m_ij.flits[i], SOUTH, m_ij.messageSize * 1.0);
                        if (source_x > dest_x) transmission_time_flit += noc.nodes[node_id].router.eastPort.updateSchedule(startTime + transmission_time_flit, m_ij.flits[i], SOUTH, m_ij.messageSize * 1.0);
                        dir_change = false;
                    }else{
                        transmission_time_flit += noc.nodes[node_id].router.southPort.updateSchedule(startTime + transmission_time_flit, m_ij.flits[i], SOUTH, m_ij.messageSize * 1.0);
                    }
                    cur_y++;
                }
            }
        }

        transmission_time_message = max(transmission_time_message, transmission_time_flit);
    }
    return startTime + transmission_time_message; 
}


int main() {
    IOS
    #ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif


    srand(time(0));
    
    // intialising variables to store neccessary information
    int task_graph[1000][1000];
    int execution_time_matrix[1000][1000];
    vector<int> task_priority_list;
    map<int, int> task_processor_mappings;

    

    // cout<<"Enter the size of n x n mesh NoC:";
    cin>>n;

    // cout<<"Enter number of tasks :";
    cin>>no_of_tasks;


    // cout<<"Input the adjancy matrix :";
    for (int i = 1; i <= no_of_tasks; i++) {
        for (int j = 1; j <= no_of_tasks; j++) {
            cin >> task_graph[i][j];
            // if (task_graph[i][j] == 1){
            //     task_graph[i][j] = rand() % 21 + 10;
            // }
        }
    }

    cout<<"Task Graph"<<endl;
     for (int i = 1; i <= no_of_tasks; i++) {
        for (int j = 1; j <= no_of_tasks; j++) {
            cout<<task_graph[i][j]<<" ";
        }
        cout<<endl;
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
    //     for (int j = 1; j <= n * n ; j++) {
    //         execution_time_matrix[i][j] = rand() % 21 + 10;
    //     }
    // }

    cout<<"Execution time matrix"<<endl;

    for (int i = 1; i <= no_of_tasks; i++) {
        for (int j = 1; j <= n * n ; j++) {
            cout<<execution_time_matrix[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;


    // creating task priority list


    task_priority_list = generateTaskPriorityList(task_graph, execution_time_matrix);
    for (auto a : task_priority_list){
        cout<<a<<" ";
    }
    cout<<endl;
    // return 0;
    
    // computing all possible shortest paths and storing them in a map
    map<string, vector<vector<int>>> all_shortest_paths = generateShortestPaths();
    map<int, pair<int, int>> tasks_start_end_times;


    NoC noc = NoC(n);
    for(int i = 0; i<task_priority_list.size();i++){
        if (i==0){// if task is sink task
            int min_exec_time_proccesor_id = 0;
            int min_exec_time = INT_MAX;
            for (int j = 1; j<=n*n ; j++){
                if (min_exec_time > execution_time_matrix[task_priority_list[i]][j]){
                    min_exec_time_proccesor_id = j;
                    min_exec_time = execution_time_matrix[task_priority_list[i]][j];
                }
            }
            tasks_start_end_times[task_priority_list[i]] =  noc.nodes[min_exec_time_proccesor_id].processingElement.allocateProcessor(task_priority_list[i], 0, execution_time_matrix[task_priority_list[i]][min_exec_time_proccesor_id]);
            task_processor_mappings[task_priority_list[i]] = min_exec_time_proccesor_id;
        }else{
            vector<int> possible_processors;
            vector<int> pred_task_ids;
            
            // getting predecessor tasks ids and and processors and their neighbors
            pred_task_ids = getPredTaskIds(task_priority_list[i], task_graph);
            for (auto pred_task_id : pred_task_ids){
                possible_processors.push_back(task_processor_mappings[pred_task_id]);
                vector<int> neighbors;
                neighbors = noc.getNeighbors(task_processor_mappings[pred_task_id]);
                for (auto neighbor :neighbors)possible_processors.push_back(neighbor);
            }
            set<int> unique_processors(possible_processors.begin(), possible_processors.end());
            possible_processors.assign(unique_processors.begin(), unique_processors.end());

            int min_eft = INT_MAX;
            int min_eft_possible_processor = 0;

            for (auto possible_processor : possible_processors){
                // tentiative allocation to find optimal processor
                int est = 0;
                int actual_est = 0;
                int eft = 0;
                NoC noc_1 = noc; // dummy noc to check possibility
                vector<Message> msg_priority_list = noc.getMessagePriorityList(task_priority_list[i], possible_processor, task_graph, task_processor_mappings, tasks_start_end_times);
                for (auto msg : msg_priority_list){
                    int source_task_id = msg.sourceTaskId;
                    est = max(est, routeMessageXY(msg, noc_1, task_processor_mappings[source_task_id], possible_processor, tasks_start_end_times[source_task_id].second));
                }
                pair<int, int>start_end_times = noc_1.nodes[possible_processor].processingElement.allocateProcessor(task_priority_list[i], est, execution_time_matrix[task_priority_list[i]][possible_processor]);
                actual_est = start_end_times.first;
                eft = start_end_times.second;
                cout<<"task : "<<task_priority_list[i]<<", processor :  "<<possible_processor<<", eft : "<<eft<<endl;
                if (eft < min_eft){
                    min_eft = eft;
                    min_eft_possible_processor = possible_processor;
                }
            }
            cout<<endl;
            // permanent allocation
            int est = 0;
            int actual_est = 0;
            
            vector<Message> msg_priority_list = noc.getMessagePriorityList(task_priority_list[i], min_eft_possible_processor, task_graph, task_processor_mappings, tasks_start_end_times);
            for (auto msg : msg_priority_list){
                int source_task_id = msg.sourceTaskId;
                est = max(est, routeMessageXY(msg, noc, task_processor_mappings[source_task_id], min_eft_possible_processor, tasks_start_end_times[source_task_id].second));
            }
            task_processor_mappings[task_priority_list[i]] = min_eft_possible_processor;
            tasks_start_end_times[task_priority_list[i]] =  noc.nodes[min_eft_possible_processor].processingElement.allocateProcessor(task_priority_list[i], est, execution_time_matrix[task_priority_list[i]][min_eft_possible_processor]);
        }

    }

    cout<<"FINAL ALLOCATION, START AND END TIMES"<<endl;
    cout<<endl;
    for(auto task : task_priority_list){
        cout<<"TASK - "<<task<<"(processor : "<<task_processor_mappings[task]<<")"<<endl;
        cout<<tasks_start_end_times[task].first<<" "<<tasks_start_end_times[task].second<<endl;
    }

    double speedup = 0.0;
    int min_sum_exec_times = INT_MAX;
    for (int j =1; j<=n*n;j++){
        int sum_exec_times = 0;
        for (int i = 1; i<=no_of_tasks; i++ ){
            sum_exec_times += execution_time_matrix[i][j];
        }
        min_sum_exec_times = min(min_sum_exec_times, sum_exec_times);
    }
    int makespan = 0;
    for (auto task : task_priority_list){
        makespan = max(makespan, tasks_start_end_times[task].second);
    }

    speedup = (min_sum_exec_times *1.0) / (makespan * 1.0);
    cout<<"Makespan : "<<makespan<<endl;
    cout<<"Speedup : "<<speedup<<endl;



    #ifndef ONLINE_JUDGE
    cerr << "\ntime taken : " << (float)clock() / CLOCKS_PER_SEC << " secs " << endl;
    #endif 
    return 0;
}