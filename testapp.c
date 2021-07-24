#include "graph.h"
#include <stdio.h>

extern graph_t *build_first_topo();

int main() {

	graph_t *topo = build_first_topo();
	dump_graph_topo(topo);

	return 0;
}