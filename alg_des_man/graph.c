#define MAXV 1000 // max number of vertices

typedef struct {
  int y;                 // adjacency info
  int weight;            // edge weight, if any
  struct edgenode *next; // next edge in list
} edgenode;

typedef struct {
  edgenode *edges[MAXV+1]; // adjaceny info
  int degree[MAXV+1];      // outdegree of each vertex
  int nvertices;           // number of vertices in graph
  int nedges;              // number of edges in graph
  bool directed;           // is the graph directed?
} graph;

