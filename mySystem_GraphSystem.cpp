//********************************************
// Student Name			:william
// Student ID			:114550152
// Student Email Address:william.cs14@nycu.edu.tw
//********************************************
//
//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// National Yang Ming Chiao Tung University, Taiwan
// Computer Science
// Date: 2026/02
//
//
#include <iostream>
#include "mySystem_GraphSystem.h"
#include <time.h>

using namespace std;

namespace SYS_CONSTANTS {
     int max_int = INT_MAX;
     double max_double = DBL_MAX;
};

int Param::GRAPH_MAX_NUM_NODES = 10000;
int Param::GRAPH_MAX_NUM_EDGES = 10000;

GRAPH_SYSTEM::GRAPH_SYSTEM( )
{
    mFlgAutoNodeDeletion = false;

    mFlgShowNodeDepth = false;

    //mOperation = GRAPH_OPERATION_ADD_EDGE;
    //mFlgShowNodeDepth = false;

    //mOperation = GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH;
    //mFlgShowNodeDepth = true;

    mOperation = GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH;
    mOperationName = "SHORTEST_PATH";

    mMaxNodeDepth = 0.0;

    mStartNode = nullptr;
    mDestinationNode = nullptr;

    initMemoryPool();
    createDefaultGraph();
    //
    // modify and add your code heres
    //
    mNumPoints_DoubleCircles = 24;
}

void GRAPH_SYSTEM::initMemoryPool( )
{
    mMaxNumNodes = Param::GRAPH_MAX_NUM_NODES;
    mMaxNumEdges = Param::GRAPH_MAX_NUM_EDGES;

    mNodeArr_Pool = new GRAPH_NODE[mMaxNumNodes];
    mEdgeArr_Pool = new GRAPH_EDGE[mMaxNumEdges];

    mCurNumOfActiveNodes = 0;
    mCurNumOfActiveEdges = 0;
    mActiveNodeArr = new int[mMaxNumNodes];
    mActiveEdgeArr = new int[mMaxNumEdges];

    mFreeNodeArr = new int[mMaxNumNodes];
    mFreeEdgeArr = new int[mMaxNumEdges];
    //
    for ( int i = 0; i < mMaxNumNodes; ++i ) {
        mNodeArr_Pool[ i ].id = i;
    }
    for ( int i = 0; i < mMaxNumEdges; ++i ) {
        mEdgeArr_Pool[ i ].id = i;
    }
    //
    reset( );
}

void GRAPH_SYSTEM::reset( )
{
    stopAutoNodeDeletion();

    //mNumPoints_DoubleCircles = 0;

    mCurNumOfActiveNodes = 0;
    mCurNumOfActiveEdges = 0;

    mCurNumOfFreeNodes = mMaxNumNodes;
    mCurNumOfFreeEdges = mMaxNumEdges;

    for ( int i = 0; i < mCurNumOfFreeNodes; ++i ) {
        mFreeNodeArr[ i ] = i;
    }
    for ( int i = 0; i < mCurNumOfFreeEdges; ++i ) {
        mFreeEdgeArr[ i ] = i;
    }

    mPassiveSelectedNode = 0;
    mSelectedNode = 0;
    //
    mFlgAutoNodeDeletion = false;

    mFlgShowNodeDepth = false;

    mMaxNodeDepth = 0.0;

    mStartNode = nullptr;
    mDestinationNode = nullptr;
}

GRAPH_NODE *GRAPH_SYSTEM::getFreeNode( ) 
{
    if ( mCurNumOfFreeNodes == 0 ) return 0;
    --mCurNumOfFreeNodes;
    int id = mFreeNodeArr[ mCurNumOfFreeNodes ];
    GRAPH_NODE *n = &mNodeArr_Pool[ id ];
    mActiveNodeArr[ mCurNumOfActiveNodes ] = id;
    n->dynamicID = mCurNumOfActiveNodes;
    ++mCurNumOfActiveNodes;
    return n;
}

GRAPH_EDGE *GRAPH_SYSTEM::getFreeEdge( )
{
    if ( mCurNumOfFreeEdges == 0 ) return 0;
    --mCurNumOfFreeEdges;
    int id = mFreeEdgeArr[ mCurNumOfFreeEdges ];
    GRAPH_EDGE *e = &mEdgeArr_Pool[ id ];
    mActiveEdgeArr[ mCurNumOfActiveEdges ] = id;
    e->dynamicID = mCurNumOfActiveEdges;

    ++mCurNumOfActiveEdges;
    return e;
}

void GRAPH_SYSTEM::createDefaultGraph( )
{
    cout << "here"<< endl;
    reset( );

    float offset_x = 90.;
    float offset_z = 15.;

    int n_0 = addNode(offset_x + 0.0, 0.0, offset_z + 0.0 );
    cout << "n_0:"<< n_0 << endl;
   
    //
    // modify and add your code heres
    //
    int n_1 = addNode(offset_x + 0.0, 0.0, offset_z - 40.0);
    cout << "n_1:" << n_1 << endl;
    int n_2 = addNode(offset_x + 40.0, 0.0, offset_z - 40.0);
    cout << "n_2:" << n_2 << endl;
    addEdge( n_0, n_1 );
    addEdge( n_1, n_2 );

}

void GRAPH_SYSTEM::createRandomGraph_DoubleCircles(int n)
{
    reset( );

    //n = 36;
    float dx = 5.0;
    float dz = 5.0;
    float r = 40.0;       
    float d = 30.0;      
    float offset_x = 0.0; 
    float offset_z = 0.0; 

    if (n < 3) n = 3;
    if (n > 36) n = 36;

    std::vector<int> inner_nodes(n);
    std::vector<int> outer_nodes(n);

    for (int i = 0; i < n; ++i) {
        float theta = (2.0f * 3.14159265f * i) / n;

        float ix = offset_x + r * cos(theta);
        float iz = offset_z + r * sin(theta);
        inner_nodes[i] = addNode(ix, 0.0f, iz);

        float ox = offset_x + (r + d) * cos(theta);
        float oz = offset_z + (r + d) * sin(theta);
        outer_nodes[i] = addNode(ox, 0.0f, oz);
    }

    int max_index_distance = n / 4;
    if (max_index_distance < 1) max_index_distance = 1;

    for (int i = 0; i < n; ++i) {
        int num_edges_to_create = rand() % 2 + 1;
        for (int e = 0; e < num_edges_to_create; ++e) {
            int offset = (rand() % (2 * max_index_distance + 1)) - max_index_distance;
            int j = (i + offset + n) % n;
            addEdge(inner_nodes[i], outer_nodes[j]);
        }
    }
}

void GRAPH_SYSTEM::createNet_Circular( int n, int num_layers )
{
    reset( );

    float dx = 5.0;
    float dz = 5.0;
    float r = 20.0;       
    float d = 15.0;      
    float offset_x = 0.0; 
    float offset_z = 0.0; 

    if (num_layers < 3) num_layers = 3;
    int total_layers = num_layers + 1;

    std::vector<std::vector<int>> node_grid(total_layers, std::vector<int>(n));

    for (int layer = 0; layer < total_layers; ++layer) {
        float current_r = r + layer * d;
        for (int i = 0; i < n; ++i) {
            float theta = (2.0f * 3.14159265f * i) / n;
            float x = offset_x + current_r * cos(theta);
            float z = offset_z + current_r * sin(theta);
            node_grid[layer][i] = addNode(x, 0.0f, z);
        }
    }

    for (int layer = 0; layer < total_layers; ++layer) {
        for (int i = 0; i < n; ++i) {
            if (layer < total_layers - 1) {
                int next_i = (i + 1) % n;
                addEdge(node_grid[layer][i], node_grid[layer][next_i]);
            }
            if (layer < total_layers - 1) {
                addEdge(node_grid[layer][i], node_grid[layer + 1][i]);
            }
        }
    }
}
void GRAPH_SYSTEM::createNet_Square( int n, int num_layers )
{
    reset( );

    float dx = 15.0;     
    float dz = 15.0;      
    float r = 5;
    float d = 5;
    float offset_x = 0.0; 
    float offset_z = 0.0; 
    if (n < 3) n = 3;
    if (num_layers < 1) num_layers = 1;
    int total_side = n + (num_layers * 2);
    int** node_grid = new int* [total_side];
    for (int i = 0; i < total_side; ++i) {
        node_grid[i] = new int[total_side];
        for (int j = 0; j < total_side; ++j) {
            node_grid[i][j] = -1;
        }
    }
    float start_x = offset_x - ((total_side - 1) * dx) / 2.0f;
    float start_z = offset_z - ((total_side - 1) * dz) / 2.0f;
    for (int r_idx = 0; r_idx < total_side; ++r_idx) {
        for (int c_idx = 0; c_idx < total_side; ++c_idx) {
            if (r_idx >= num_layers && r_idx < num_layers + n &&
                c_idx >= num_layers && c_idx < num_layers + n) {
                continue;
            }
            float x = start_x + c_idx * dx;
            float z = start_z + r_idx * dz;
            node_grid[r_idx][c_idx] = addNode(x, 0.0f, z);
        }
    }
    for (int r_idx = 0; r_idx < total_side; ++r_idx) {
        for (int c_idx = 0; c_idx < total_side; ++c_idx) {
            int current_id = node_grid[r_idx][c_idx];
            if (current_id == -1) continue;
            if (c_idx + 1 < total_side) {
                int right_id = node_grid[r_idx][c_idx + 1];
                if (right_id != -1) {
                    addEdge(current_id, right_id);
                }
            }
            if (r_idx + 1 < total_side) {
                int down_id = node_grid[r_idx + 1][c_idx];
                if (down_id != -1) {
                    addEdge(current_id, down_id);
                }
            }
        }
    }
    for (int i = 0; i < total_side; ++i) {
        delete[] node_grid[i];
    }
    delete[] node_grid;
}
void GRAPH_SYSTEM::createNet_RadicalCircular( int n ) {

    reset( );

    float offset_x = 0.0;    
    float offset_z = 0.0;    
    float r = 50.0;  
    if (n < 20) n = 20;
    int center_node_id = addNode(offset_x, 0.0f, offset_z);
    int num_outer_nodes = n - 1;
    for (int i = 0; i < num_outer_nodes; ++i) {
        float theta = (2.0f * 3.14159265f * i) / num_outer_nodes;
        float x = offset_x + r * cos(theta);
        float z = offset_z + r * sin(theta);
        int outer_node_id = addNode(x, 0.0f, z);
        addEdge(center_node_id, outer_node_id);
    }

}

//
// return the node id
// return -1: no free node
//
int GRAPH_SYSTEM::addNode( float x, float y, float z, float r )
{
    GRAPH_NODE* n = getFreeNode();
    if (n == nullptr) return -1;
    n->p.x = x;
    n->p.y = y;
    n->p.z = z;
    n->depth = 0;
    n->visited = false;
    n->edgeID.clear();
    //mActiveNodeArr[mCurNumOfActiveNodes] = n->id;
    //mCurNumOfActiveNodes++; 
    return n->id;
}

//
// return the edge id
// return -1: no free edge
//
int GRAPH_SYSTEM::addEdge( int nodeID_0, int nodeID_1 )
{
    GRAPH_EDGE* e = getFreeEdge();
    if (e == nullptr) return -1;

    e->nodeID[0] = nodeID_0;
    e->nodeID[1] = nodeID_1;
    GRAPH_NODE* n0 = &mNodeArr_Pool[nodeID_0];
    GRAPH_NODE* n1 = &mNodeArr_Pool[nodeID_1];

    if (n0 != nullptr && n1 != nullptr) {
        n0->edgeID.push_back(e->id);
        n1->edgeID.push_back(e->id);
    }

    return e->id;
}

void GRAPH_SYSTEM::askForInput( )
{
    //
    // modify and add your code heres
    //
    cout << "GRAPH_SYSTEM" << endl;
    cout << "Key usage:" << endl;
    cout << "1: create a default graph" << endl;
    cout << "2: create a circular net ( spider web )" << endl;
    cout << "3: create a square net ( hollow square )" << endl;
    cout << "4: create a radial circular graph" << endl;
    cout << "5: create a random graph with double circles" << endl;
    cout << "<: decrease the number of nodes for double circles" << endl;
    cout << ">: increase the number of nodes for double circles" << endl;
    cout << "d, D: Toggle automatic deletion of nodes" << endl;
    cout << "Delete: delete a selected node and all the edges incident to it" << endl;
    cout << "Spacebar: Change operation" << endl;
    cout << " " << endl;
    cout << "Use the mouse to select nodes and add edges" << endl;
    cout << "Click the left button to select/unselect or create an edge" << endl;
    cout << " " << endl;
    cout << "A selected node is highlighted as red." << endl;

}


GRAPH_NODE *GRAPH_SYSTEM::findNearestNode( double x, double z, double &cur_distance2 ) const
{
    GRAPH_NODE* n = nullptr;
    double min_dist2 = 1e30;
    int num_nodes = getNumOfNodes();

    for (int i = 0; i < num_nodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* g = &mNodeArr_Pool[nodeID];

        if (g == nullptr) continue;

        double dx = g->p.x - x;
        double dz = g->p.z - z;
        double dist2 = dx * dx + dz * dz;

        if (dist2 < min_dist2) {
            min_dist2 = dist2;
            n = g;
        }
    }

    if (n != nullptr) {
        cur_distance2 = min_dist2;
    }
    else {
        cur_distance2 = -1.0;
    }

    return n;
}

void GRAPH_SYSTEM::moveTo(double x, double y )
{
    if (!mSelectedNode) return;
    mSelectedNode->p.x = x;
    mSelectedNode->p.z = y;
}

//
// handle the mouse click event at position (x, z).
//
void GRAPH_SYSTEM::clickAt(double x, double z)
{
    double cur_d2;
    GRAPH_NODE *nearest_node = findNearestNode( x, z, cur_d2 );
    if (nearest_node == 0 ) {
        mSelectedNode = 0;
        return;
    }
    if ( cur_d2 > nearest_node->r* nearest_node->r ) {
        mSelectedNode = 0;
        return;

    }
    if ( mSelectedNode == nearest_node) {
        mSelectedNode = 0;
        return;
    }

    performOperation(nearest_node);

}

void GRAPH_SYSTEM::performOperation(GRAPH_NODE* node) {
    switch (mOperation) {
    case GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH:
        mSelectedNode = node;
        mFlgShowNodeDepth = true;
        computeDepthOfAllNodesFromSelectedNode();
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE:
        if (mSelectedNode != 0) {
            addEdge(mSelectedNode->id, node->id);
            mSelectedNode = 0;
            return;
        }
        mSelectedNode = node;
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH:
        if (mSelectedNode != 0) {
            mStartNode = mSelectedNode;
            mDestinationNode = node;
            computeShortestPath();
            mSelectedNode = 0;
            return;
        }
        mSelectedNode = node;
        break;
    }
}

void GRAPH_SYSTEM::deleteEdge( int edgeID )
{
    GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID];
    if (e == nullptr) return;

    int u_id = e->nodeID[0];
    int v_id = e->nodeID[1];
    GRAPH_NODE* u_node = &mNodeArr_Pool[u_id];
    GRAPH_NODE* v_node = &mNodeArr_Pool[v_id];

    if (u_node != nullptr) {
        for (auto it = u_node->edgeID.begin(); it != u_node->edgeID.end(); ++it) {
            if (*it == edgeID) {
                u_node->edgeID.erase(it);
                break;
            }
        }
    }

    if (v_node != nullptr) {
        for (auto it = v_node->edgeID.begin(); it != v_node->edgeID.end(); ++it) {
            if (*it == edgeID) {
                v_node->edgeID.erase(it);
                break;
            }
        }
    }
}

void GRAPH_SYSTEM::removeEdgeFromNode( const GRAPH_EDGE *e, int nodeID )
{
    GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
    if (n == nullptr || e == nullptr) return;
    int target_edge_id = e->id;
    for (auto it = n->edgeID.begin(); it != n->edgeID.end(); ++it) {
        if (*it == target_edge_id) {
            n->edgeID.erase(it);
            break; 
        }
    }

}
void GRAPH_SYSTEM::deleteEdgesOfNode( int nodeID )
{
    GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
    if (n == nullptr) return;
    while (!n->edgeID.empty()) {
        int target_edge_id = n->edgeID.back();
        deleteEdge(target_edge_id);
        for (int i = 0; i < mCurNumOfActiveEdges; ++i) {
            if (mActiveEdgeArr[i] == target_edge_id) {
                mActiveEdgeArr[i] = mActiveEdgeArr[mCurNumOfActiveEdges - 1];
                mCurNumOfActiveEdges--; 
                break;
            }
        }
    }
}

void GRAPH_SYSTEM::deleteNode( int nodeID ) {
    GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
    if (n == nullptr) return;
    deleteEdgesOfNode(nodeID);
    if (mCurNumOfActiveNodes > 0) {
        int target_idx = n->dynamicID; 
        int last_node_id = mActiveNodeArr[mCurNumOfActiveNodes - 1];
        mActiveNodeArr[target_idx] = last_node_id;
        mNodeArr_Pool[last_node_id].dynamicID = target_idx;
        mCurNumOfActiveNodes--;
    }
    if (mSelectedNode == n) {
        mSelectedNode = nullptr;
    }
    if (mPassiveSelectedNode == n) {
        mPassiveSelectedNode = nullptr;
    }
}

void GRAPH_SYSTEM::deleteSelectedNode(  ) {
    if (mSelectedNode == 0) return;
    int targetID = mSelectedNode->id;
    mSelectedNode = 0;
    mPassiveSelectedNode = 0;
    deleteNode(targetID);
}

bool GRAPH_SYSTEM::isSelectedNode( ) const
{
    return mSelectedNode != 0;
}

void GRAPH_SYSTEM::getInfoOfSelectedPoint( double &r, vector3 &p ) const
{
    if ( isSelectedNode( ) == false ) return;
    r = mSelectedNode->r;
    p = mSelectedNode->p;

}

void GRAPH_SYSTEM::changeOperation()
{
    switch (mOperation) {
    case GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE;
        mOperationName = "ADD_EDGE";
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH;
        mOperationName = "SHORTEST_PATH";
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH;
        mOperationName = "NODE_DEPTH";

        break;
    }
}

void GRAPH_SYSTEM::resetDepthOfAllNodes()
{
    mMaxNodeDepth = 0;
    //
    // modify and add your code heres
    // 

    int numNodes = getNumOfNodes(); 
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
        if (n == nullptr) continue;

        //
        // modify and add your code heres
        // 
        n->depth = 0;
        n->visited = false;
        n->path_cost = 0.0;
        n->path_parent = nullptr;
    }
}



/*
* k := computeDepthOfAllNodesFromSelectedNode

* void k( Node *n, int depth ) {
	if n is null, return
	set n’s depth to depth
	for each adjacent node m of n				; note m should not be n
		if (m’s depth < depth + 1) k(m, depth+1)
}

void computeDepthOfAllNodesFromSelectedNode( ) {
	for each node n, set its depth to FLT_MAX
	k(selected_node, 0)
	update mMaxNodeDepth
}
*/

void GRAPH_SYSTEM::computeDepthOfAllNodesFromSelectedNode(GRAPH_NODE* node, int depth)
{
    if (node == 0) return;
    node->depth = depth;
    node->visited = true;
    //
    // modify and add your code heres
    //
    int total_nodes = getNumOfNodes();
    GRAPH_NODE** my_queue = new GRAPH_NODE * [total_nodes];
    int head = 0;
    int tail = 0;
    my_queue[tail] = node;
    tail++;
    while (head < tail) {
        GRAPH_NODE* cur_node = my_queue[head];
        head++;
        if (cur_node->depth > mMaxNodeDepth) {
            mMaxNodeDepth = cur_node->depth;
        }
        for (int i = 0; i < cur_node->edgeID.size(); ++i) {
            int edgeID = cur_node->edgeID[i];
            GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID];
            if (e == nullptr) continue;
            GRAPH_NODE* n0 = &mNodeArr_Pool[e->nodeID[0]];
            GRAPH_NODE* n1 = &mNodeArr_Pool[e->nodeID[1]];
            if (n0 == nullptr || n1 == nullptr) continue;
            GRAPH_NODE* next_node = nullptr;
            if (n0 == cur_node) {
                next_node = n1;
            }
            else {
                next_node = n0;
            }
            if (!next_node->visited) {
                next_node->visited = true;
                next_node->depth = cur_node->depth + 1;
                my_queue[tail] = next_node;
                tail++;
            }
        }
    }
    delete[] my_queue;
}

void GRAPH_SYSTEM::computeDepthOfAllNodesFromSelectedNode()
{
    resetDepthOfAllNodes();
    if (mSelectedNode == nullptr) return;
    mMaxNodeDepth = 0;
    mSelectedNode->depth = 0;
    computeDepthOfAllNodesFromSelectedNode(mSelectedNode, 0);
    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        //
        // modify and add your code heres
        //
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
        if (n == nullptr) continue;
        if (n->visited && n->depth > mMaxNodeDepth) {
            mMaxNodeDepth = n->depth; 
        }
    }
}

// This member function is not used.
float GRAPH_SYSTEM::getNodeDepthFromSelectedNode(int nodeIndex) const
{
    float d = 0.0;
    if (nodeIndex < 0 || nodeIndex >= getNumOfNodes()) {
        return d;
    }
    int nodeID = mActiveNodeArr[nodeIndex];
    const GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
    if (n != nullptr) {
        d = static_cast<float>(n->depth);
    }

    return d; 
}

void GRAPH_SYSTEM::resetPathInformationOfAllNodes()
{
    //
    // modify and add your code heres
    // 

    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
        if (n == nullptr) continue;

        //
        // modify and add your code heres
        //

        n->path_cost = 1e30; 
        n->path_parent = nullptr;
        n->visited = false;
        n->depth = 0;
    }
}

void GRAPH_SYSTEM::computeShortestPath(GRAPH_NODE *node)
{
    if (node == 0) return;
    //
    // modify and add your code heres
    //    


    // for each edge incident to node, node 
    // DO
    for (int i = 0; i < node->edgeID.size(); ++i) {
        int edgeID = node->edgeID[i];
        GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID];
        if (e == nullptr) continue;
        GRAPH_NODE* n0 = &mNodeArr_Pool[e->nodeID[0]];
        GRAPH_NODE* n1 = &mNodeArr_Pool[e->nodeID[1]];

        if (n0 == nullptr || n1 == nullptr) continue;
        GRAPH_NODE* next_node = nullptr;
        if (n0 == node) {
            next_node = n1;
        }
        else {
            next_node = n0;
        }
        double dx = node->p.x - next_node->p.x;
        double dz = node->p.z - next_node->p.z;
        double d = sqrt(dx * dx + dz * dz);
        double new_path_cost = node->path_cost + d;
        if (new_path_cost >= next_node->path_cost) {
            continue;
        }
        next_node->path_cost = new_path_cost;   
        next_node->path_parent = node;
        computeShortestPath(next_node);
    }
}

void GRAPH_SYSTEM::computeShortestPath()
{
    //
    // modify and add your code heres
    //
    resetPathInformationOfAllNodes();
    if (mStartNode == nullptr || mDestinationNode == nullptr) {
        return;
    }
    mStartNode->path_cost = 0.0;
    computeShortestPath(mStartNode);
}

void GRAPH_SYSTEM::handleKeyPressedEvent( unsigned char key )
{
    
    switch( key ) {
    case 127: // delete
        mFlgAutoNodeDeletion = false;
        deleteSelectedNode( );
        glutPostRedisplay();
        break;
    case '1':
        mFlgAutoNodeDeletion = false;
        createDefaultGraph( );
        mSelectedNode = 0;
        break;
    case '2':
        mFlgAutoNodeDeletion = false;
        createNet_Circular(12, 6);
        mSelectedNode = 0;

        break;
    case '3':
        mFlgAutoNodeDeletion = false;
        createNet_Square(15, 4);
        mSelectedNode = 0;

        break;
    case '4':
        mFlgAutoNodeDeletion = false;
        createNet_RadicalCircular(24);
        mSelectedNode = 0;

        break;
    case '5':
        mNumPoints_DoubleCircles = 24;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;
    case '<':
        mNumPoints_DoubleCircles--;
        if (mNumPoints_DoubleCircles<3) 
            mNumPoints_DoubleCircles = 3;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;
    case '>':
        mNumPoints_DoubleCircles++;
        if (mNumPoints_DoubleCircles> 36) 
            mNumPoints_DoubleCircles = 36;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;

    case 'r':
        mFlgAutoNodeDeletion = false;
        mSelectedNode = 0;
        break;
    case ' ':
        changeOperation();
        break;
    case 'd':
    case 'D':
        mFlgAutoNodeDeletion = !mFlgAutoNodeDeletion;
        break;
    }
}

void GRAPH_SYSTEM::handlePassiveMouseEvent( double x, double z )
{
    double cur_d2;
    GRAPH_NODE *n = findNearestNode( x, z, cur_d2 );
    if ( n == 0 ) return;
    if ( cur_d2 > n->r*n->r ) {
        mPassiveSelectedNode = 0;
        return;
    }
    mPassiveSelectedNode = n;
}

int GRAPH_SYSTEM::getNumOfNodes( ) const
{
    return mCurNumOfActiveNodes;
}

void GRAPH_SYSTEM::getNodeInfo( int nodeIndex, double &r, vector3 &p ) const
{
    int nodeID = mActiveNodeArr[ nodeIndex ];
    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];
    r = n->r;
    p = n->p;
}

void GRAPH_SYSTEM::getNodeInfo(int nodeIndex, double& r, vector3& p, float& depth) const
{
    int nodeID = mActiveNodeArr[nodeIndex];
    GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
    r = n->r;
    p = n->p;
    depth = n->depth;
}

int GRAPH_SYSTEM::getNumOfEdges( ) const
{
    return mCurNumOfActiveEdges;
}

vector3 GRAPH_SYSTEM::getNodePositionOfEdge( int edgeIndex, int nodeIndex ) const
{
    int edgeID = mActiveEdgeArr[ edgeIndex ];
    GRAPH_EDGE *e = &mEdgeArr_Pool[ edgeID ];
    return mNodeArr_Pool[ e->nodeID[ nodeIndex ] ].p;
}

void GRAPH_SYSTEM::stopAutoNodeDeletion()
{
    mFlgAutoNodeDeletion = false;
}

//
// For every frame, update( ) function is called.
//
// 
void GRAPH_SYSTEM::update( )
{
    if (!mFlgAutoNodeDeletion) {

        return;
    }
    if (mCurNumOfActiveNodes <= 0) {
        mFlgAutoNodeDeletion = false;
        return;
    }
    Sleep(250);

    //
    // modify and add your code
    //
    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* target_node = &mNodeArr_Pool[nodeID];
        if (target_node != nullptr) {
            deleteNode(nodeID);
            break;
        }
    }
    mSelectedNode = 0;
    mPassiveSelectedNode = 0;
}