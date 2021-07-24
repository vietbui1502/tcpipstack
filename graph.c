#include "graph.h"

graph_t *
create_new_topo(char * topo_name) {
	graph_t *graph = (graph_t*)calloc(1, sizeof(graph_t));

	strncpy (graph->topology_name, topo_name, TOPO_NAME_SIZE);
	init_glthread(&graph->node_list);

	return graph;
}

node_t *
create_new_node(graph_t *topo, char *node_name) {
	node_t *node = (node_t*)calloc(1, sizeof(node_t));
	strncpy (node->node_name, node_name, NODE_NAME_SIZE);
	//node->itf
	init_glthread(&node->graph_glue);
	glthread_add_next(&topo->node_list, &node->graph_glue);
	return node;
}

int 
insert_link_between_two_node(node_t *node1, node_t *node2, char *if_name1, char *if_name2, unsigned int cost){
	
	int available_slot = -1;

	if (!node1 || !node2 || !if_name1 || !if_name2) {
		printf("Check NULL failed!!!\n");
		return -1;
	}

	if (cost == 0) {
		printf("Cost must be > 0 !!!\n");
		return -1;
	}

	link_t *link = (link_t*)calloc(1, sizeof(link_t));

	strncpy(link->if1.if_name, if_name1, IF_NAME_SIZE);
	link->if1.att_node = node1;
	link->if1.link = link;

	strncpy(link->if2.if_name, if_name2, IF_NAME_SIZE);
	link->if2.att_node = node2;
	link->if2.link = link;

	available_slot = get_node_intf_available_slot(node1);

	if(available_slot == -1) {
		printf("Interface slot is full !!!\n");
		return -1;
	}

	node1->itf[available_slot] = &link->if1;

	available_slot = get_node_intf_available_slot(node2);

	if(available_slot == -1) {
		printf("Interface slot is full !!!\n");
		return -1;
	}

	node2->itf[available_slot] = &link->if2;

	return 0;
}

void
dump_graph_topo(graph_t *topo){

	printf("Topology_name is : %s\n", topo->topology_name);

	//topo->node_list;
	node_t *node;
	glthread_t *curr_glthread;

	ITERATE_GLTHREAD_BEGIN(&topo->node_list, curr_glthread){
		node = graph_glue_to_node(curr_glthread);
		dump_node(node);
	} ITERATE_GLTHREAD_END(&graph->node_list, curr_glthread);

}

void
dump_node(node_t *node) {

	printf("Node name is: %s\n", node->node_name);

	int i;
	interface_t *itf;

	for (i = 0; i < MAX_ITF_PER_NODE; i++) {
		itf = node->itf[i];
		if(!itf) {
			break;
		}
		dump_interface(itf);
	}
}

void
dump_interface(interface_t *itf) {

	link_t *link = itf->link;
	node_t *nbr_node = get_nbr_node(itf);

	printf("Interface name is : %s\n", itf->if_name);
	printf("Connect to node: %s, Local node: %s, cost: %d\n", 
		nbr_node->node_name, 
		itf->att_node->node_name, 
		link->cost);
	return;
}