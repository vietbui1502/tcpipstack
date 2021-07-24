#ifndef __GRAPH__
#define __GRAPH__

#include "gluethread/glthread.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define IF_NAME_SIZE 32
#define NODE_NAME_SIZE 32
#define MAX_ITF_PER_NODE 10
#define TOPO_NAME_SIZE 64

//forward declaration
typedef struct node_ node_t;
typedef struct link_ link_t;

typedef struct interface_ {
	char if_name[IF_NAME_SIZE];
	struct node_ *att_node;
	struct link_ *link;
}interface_t;

struct link_ {
	interface_t if1;
	interface_t if2;
	unsigned int cost;
};

struct node_ {
	char node_name[NODE_NAME_SIZE];
	interface_t *itf[MAX_ITF_PER_NODE];
	glthread_t graph_glue; // check later?
};
GLTHREAD_TO_STRUCT(graph_glue_to_node, node_t, graph_glue);

typedef struct graph_ {
	char topology_name[TOPO_NAME_SIZE];
	glthread_t node_list;
} graph_t;



graph_t *create_new_topo(char * topo_name);
node_t *create_new_node(graph_t *topo, char *node_name);
int insert_link_between_two_node(node_t *node1, node_t *node2, char *if_name1, char *if_name2, unsigned int cost);
void dump_graph_topo(graph_t *topo);

static inline node_t *
get_nbr_node(interface_t *interface) {
	if (interface->att_node == NULL || interface->link == NULL) {
		printf("Error\n");
		return NULL;
	}

	link_t *link = interface->link;

	if (&link->if1 == interface){
		return link->if2.att_node;
	}else {
		return link->if1.att_node;
	}
}

static inline int
get_node_intf_available_slot(node_t *node) {
	int slot = 0;
	for (slot = 0; slot < MAX_ITF_PER_NODE; slot++) {
		if (node->itf[slot] == NULL) {
			return slot;
		}
	}
	return -1; // no availble interface slot for this node
}

static inline interface_t *
get_node_if_by_name(node_t *node, char *if_name) {
	int i;

	if (node == NULL || if_name == NULL) {
		printf("Check null failed !!!\n");
		return NULL;
	}

	for (i = 0; i < MAX_ITF_PER_NODE; i++) {
		if (node->itf[i] != NULL) {
			if (strcmp(node->itf[i], if_name) == 0) {
				return node->itf[i];
			}
		}
	}

	printf("Could not found ifname [%s] !!!\n", if_name);
	return NULL;
}

static inline node_t *
get_node_by_node_name(graph_t *topo, char *node_name){
	node_t *node;
    glthread_t *curr;    

    ITERATE_GLTHREAD_BEGIN(&topo->node_list, curr){

        node = graph_glue_to_node(curr);
        if(strncmp(node->node_name, node_name, strlen(node_name)) == 0)
            return node;
    } ITERATE_GLTHREAD_END(&topo->node_list, curr);
    return NULL;
}



#endif // __GRAPH__