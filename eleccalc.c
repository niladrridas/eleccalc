#include <stdio.h>
#include <math.h>

#define MAX_COMPONENTS 100
#define MAX_NODES 100

typedef struct {
    int type;   
    double value;  
    int node1;  
    int node2;  
} Component;

typedef struct {
    int num_components;        
    int num_nodes;             
    Component components[MAX_COMPONENTS];  
    double node_values[MAX_NODES];  
} Circuit;

void initializeCircuit(Circuit *circuit);
void solveCircuit(Circuit *circuit);
void printResults(Circuit *circuit);

int main() {
    Circuit circuit;
    
    initializeCircuit(&circuit);
    solveCircuit(&circuit);
    printResults(&circuit);
    
    return 0;
}

void initializeCircuit(Circuit *circuit) {
    circuit->num_components = 3;  // Example: 3 resistors
    circuit->num_nodes = 3;        // Example: 3 nodes
    
    // Define components
    circuit->components[0].type = 0;  // Resistor
    circuit->components[0].value = 1000; // 1000 ohms
    circuit->components[0].node1 = 1;  // Connected to node 1
    circuit->components[0].node2 = 2;  // Connected to node 2
    
    circuit->components[1].type = 0;  
    circuit->components[1].value = 1500; 
    circuit->components[1].node1 = 2;  
    circuit->components[1].node2 = 3;  
    
    circuit->components[2].type = 0;  
    circuit->components[2].value = 2000; 
    circuit->components[2].node1 = 1;  
    circuit->components[2].node2 = 3;  
    
    // Set initial node voltages (if known)
    circuit->node_values[0] = 0.0;  // Ground node
    circuit->node_values[1] = 5.0;  // Example: Node 1 voltage
    circuit->node_values[2] = 0.0;  // Example: Node 2 voltage
    circuit->node_values[3] = 0.0;  // Example: Node 3 voltage
}

void solveCircuit(Circuit *circuit) {
    // Nodal analysis algorithm
    double conductance[MAX_NODES][MAX_NODES] = {0}; // Initialize conductance matrix
    double current_sources[MAX_NODES] = {0};       // Initialize current source vector
    
    // Build conductance matrix and current source vector
    for (int i = 0; i < circuit->num_components; i++) {
        int node1 = circuit->components[i].node1;
        int node2 = circuit->components[i].node2;
        double value = circuit->components[i].value;
        
        // Add conductance to matrix
        if (node1 != 0)
            conductance[node1][node1] += 1.0 / value;
        if (node2 != 0)
            conductance[node2][node2] += 1.0 / value;
        if (node1 != 0 && node2 != 0) {
            conductance[node1][node2] -= 1.0 / value;
            conductance[node2][node1] -= 1.0 / value;
        }
    }
    
    // Solve for node voltages
    for (int i = 1; i <= circuit->num_nodes; i++) {
        if (i == 1) continue;  // Skip ground node
        
        double sum = 0.0;
        for (int j = 1; j <= circuit->num_nodes; j++) {
            sum += conductance[i][j] * circuit->node_values[j];
        }
        circuit->node_values[i] = sum;
    }
}

void printResults(Circuit *circuit) {
    printf("Node Voltages:\n");
    for (int i = 1; i <= circuit->num_nodes; i++) {
        printf("Node %d: %.2f V\n", i, circuit->node_values[i]);
    }
}
