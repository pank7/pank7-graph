/*
 * File:                graph.c
 * Author:              Yi Li <liyi84@gmail.com>
 * Description:         Graph structure implementation.
 * Date & Time:         Sat Jun 19 12:00:48 2010
 */

#include        "common.h"
#include        "graph.h"

inline void
init_node (node_struct *node, unsigned int i)
{
  node->node = i;
  node->lknr = 0;
  node->links = NULL;
}

inline unsigned int *
get_ulinks (ugraph_struct *graph, unsigned int node)
{
  unsigned int  *tmp = (unsigned int *)membox_get_addr
    (&graph->membox, (unsigned long)graph->nodes[node - 1].links);

  return tmp;
}

inline node_struct *
get_outnodes (dgraph_struct *graph)
{
  node_struct  *tmp = (node_struct *)membox_get_addr
    (&graph->membox, (unsigned long)graph->outnodes);

  return tmp;
}

inline node_struct *
get_innodes (dgraph_struct *graph)
{
  node_struct  *tmp = (node_struct *)membox_get_addr
    (&graph->membox, (unsigned long)graph->innodes);

  return tmp;
}

inline unsigned int *
get_dlinks (dgraph_struct *graph, node_struct *node)
{
  unsigned int  *links = NULL;

  links = (unsigned int *)membox_get_addr
    (&graph->membox, (unsigned long)node->links);

  return links;
}

int
init_ugraph (ugraph_struct *graph)
{
  assert (graph->nodenr > 0);
  assert (graph->linknr > 0);

  int           i = 0;
  unsigned int  nodes = graph->nodenr * sizeof (node_struct);
  unsigned int  links = graph->linknr * sizeof (unsigned int);

  strcpy (graph->membox.name, graph->name);

  membox_init (&graph->membox, nodes + links, MMAP_RWC);

  membox_stat (&graph->membox);

  graph->nodes = (node_struct *)membox_alloc (&graph->membox, nodes);

  for (i = 0; i < graph->nodenr; ++i) {
    init_node (&graph->nodes[i], i + 1);
  }
  
  return 0;
}

int
init_dgraph (dgraph_struct *graph)
{
  assert (graph->nodenr > 0);
  assert (graph->linknr > 0);

  int           i = 0;
  unsigned int  nodes = graph->nodenr * sizeof (node_struct);
  unsigned int  links = graph->linknr * sizeof (unsigned int);

  strcpy (graph->membox.name, graph->name);

  membox_init (&graph->membox, (nodes * 2) + (links * 2), MMAP_RWC);

  membox_stat (&graph->membox);

  graph->outnodes = (node_struct *)membox_alloc (&graph->membox, nodes);
  graph->innodes = (node_struct *)membox_alloc (&graph->membox, nodes);

  for (i = 0; i < graph->nodenr; ++i) {
    init_node (&graph->outnodes[i], i + 1);
    init_node (&graph->innodes[i], i + 1);
  }

  graph->outnodes = (node_struct *)membox_get_offset
    (&graph->membox, (void *)graph->outnodes);
  graph->innodes = (node_struct *)membox_get_offset
    (&graph->membox, (void *)graph->innodes);
  
  return 0;
}

int
delete_ugraph (ugraph_struct *graph)
{
  membox_free (&graph->membox);

  return 0;
}

int
delete_dgraph (dgraph_struct *graph)
{
  membox_free (&graph->membox);

  return 0;
}

int
build_ugraph (ugraph_struct *graph)
{
  int           fin = -1, i = 0, j = 0;
  FILE          *in = NULL;
  ssize_t       ret = -1;
  unsigned int  from, lknr, to;
  char          name[64];

  strcpy (name, graph->name);
  strcat (name, ".ugraph.txt");

  fin = open (name, O_RDONLY);
  assert (fin > 0);
  in = fdopen (fin, "r");

  for (i = 0; i < graph->nodenr; ++i) {
    ret = fscanf (in, "%u %u", &from, &lknr);
    if (ret == EOF) {
      break;
    }
    assert (graph->nodes[from - 1].node == from);
    graph->nodes[from - 1].lknr = lknr;
    graph->nodes[from - 1].links =
      (unsigned int *)membox_alloc (&graph->membox,
                                    sizeof (unsigned int) * lknr);
    for (j = 0; j < lknr; ++j) {
      fscanf (in, "%u", &to);
      graph->nodes[from - 1].links[j] = to;
    }

    unsigned long       offset = membox_get_offset
      (&graph->membox, (void *)graph->nodes[from - 1].links);
    graph->nodes[from - 1].links = (unsigned int *)offset;
  }

  close (fin);

  membox_stat (&graph->membox);

  return 0;
}

int
build_dgraph (dgraph_struct *graph)
{
  int           fin = -1, i = 0, j = 0;
  FILE          *in = NULL;
  ssize_t       ret = -1;
  unsigned int  from, lknr, to;
  char          name[64];

  strcpy (name, graph->name);
  strcat (name, ".dgraph.txt");

  fin = open (name, O_RDONLY);
  assert (fin > 0);
  in = fdopen (fin, "r");

  node_struct  *outnodes = (node_struct *)membox_get_addr
    (&graph->membox, (unsigned long)graph->outnodes);
  for (i = 0; i < graph->nodenr; ++i) {
    ret = fscanf (in, "%u %u", &from, &lknr);
    if (ret == EOF) {
      break;
    }
    outnodes[from - 1].node = from;
    outnodes[from - 1].lknr = lknr;
    outnodes[from - 1].links =
      (unsigned int *)membox_alloc (&graph->membox,
                                    sizeof (unsigned int) * lknr);
    for (j = 0; j < lknr; ++j) {
      fscanf (in, "%u", &to);
      outnodes[from - 1].links[j] = to;
    }
    outnodes[from - 1].links = (unsigned int *)membox_get_offset
      (&graph->membox, (void *)outnodes[from - 1].links);
  }

  close (fin);

  strcpy (name, graph->name);
  strcat (name, ".revdgraph.txt");

  fin = open (name, O_RDONLY);
  assert (fin > 0);
  in = fdopen (fin, "r");

  node_struct  *innodes = (node_struct *)membox_get_addr
    (&graph->membox, (unsigned long)graph->innodes);
  for (i = 0; i < graph->nodenr; ++i) {
    ret = fscanf (in, "%u %u", &to, &lknr);
    if (ret == EOF) {
      break;
    }
    innodes[to - 1].node = to;
    innodes[to - 1].lknr = lknr;
    innodes[to - 1].links =
      (unsigned int *)membox_alloc (&graph->membox,
                                    sizeof (unsigned int) * lknr);
    for (j = 0; j < lknr; ++j) {
      fscanf (in, "%u", &from);
      innodes[to - 1].links[j] = from;
    }
    innodes[to - 1].links = (unsigned int *)membox_get_offset
      (&graph->membox, (void *)innodes[to - 1].links);
  }

  close (fin);

  membox_stat (&graph->membox);

  return 0;
}

void
print_ugraph (ugraph_struct *graph)
{
  int   i = 0, j = 0;

  for (i = 0; i < graph->nodenr; ++i) {
    fprintf (stdout, "%u %u", graph->nodes[i].node, graph->nodes[i].lknr);
    unsigned int        *links = get_ulinks (graph, i + 1);
    for (j = 0; j < graph->nodes[i].lknr; ++j) {
      fprintf (stdout, " %u", links[j]);
    }
    fprintf (stdout, "\n");
  }

  return;
}

void
print_dgraph (dgraph_struct *graph)
{
  int   i = 0, j = 0;

  node_struct  *outnodes = get_outnodes (graph);
  for (i = 0; i < graph->nodenr; ++i) {
    fprintf (stdout, "%u %u", outnodes[i].node, outnodes[i].lknr);
    unsigned int        *links = get_dlinks (graph, &outnodes[i]);
    for (j = 0; j < outnodes[i].lknr; ++j) {
      fprintf (stdout, " %u", links[j]);
    }
    fprintf (stdout, "\n");
  }

  return;
}

void
print_revdgraph (dgraph_struct *graph)
{
  int   i = 0, j = 0;

  node_struct  *innodes = get_innodes (graph);
  for (i = 0; i < graph->nodenr; ++i) {
    fprintf (stdout, "%u %u", innodes[i].node, innodes[i].lknr);
    unsigned int        *links = get_dlinks (graph, &innodes[i]);
    for (j = 0; j < innodes[i].lknr; ++j) {
      fprintf (stdout, " %u", links[j]);
    }
    fprintf (stdout, "\n");
  }

  return;
}

int
load_ugraph_rdonly (ugraph_struct *graph)
{
  assert (graph->nodenr > 0);
  assert (graph->linknr > 0);

  unsigned int  nodes = graph->nodenr * sizeof (node_struct);
  unsigned int  links = graph->linknr * sizeof (unsigned int);

  strcpy (graph->membox.name, graph->name);

  membox_init (&graph->membox, nodes + links, MMAP_RD);

  graph->nodes = (node_struct *)graph->membox.pool;

  return 0;
}

int
load_ugraph (ugraph_struct *graph)
{
  assert (graph->nodenr > 0);
  assert (graph->linknr > 0);

  unsigned int  nodes = graph->nodenr * sizeof (node_struct);
  unsigned int  links = graph->linknr * sizeof (unsigned int);

  strcpy (graph->membox.name, graph->name);

  membox_init (&graph->membox, nodes + links, MMAP_WR);

  graph->nodes = (node_struct *)graph->membox.pool;

  return 0;
}

int
load_dgraph_rdonly (dgraph_struct *graph)
{
  assert (graph->nodenr > 0);
  assert (graph->linknr > 0);

  unsigned int  nodes = graph->nodenr * sizeof (node_struct);
  unsigned int  links = graph->linknr * sizeof (unsigned int);

  strcpy (graph->membox.name, graph->name);

  membox_init (&graph->membox, nodes + (links * 2), MMAP_RD);

  graph->outnodes = (node_struct *)graph->membox.pool;
  graph->innodes = graph->outnodes + graph->nodenr;

  graph->outnodes = (node_struct *)membox_get_offset
    (&graph->membox, (void *)graph->outnodes);
  graph->innodes = (node_struct *)membox_get_offset
    (&graph->membox, (void *)graph->innodes);

  return 0;
}

int
load_dgraph (dgraph_struct *graph)
{
  assert (graph->nodenr > 0);
  assert (graph->linknr > 0);

  unsigned int  nodes = graph->nodenr * sizeof (node_struct);
  unsigned int  links = graph->linknr * sizeof (unsigned int);

  strcpy (graph->membox.name, graph->name);

  membox_init (&graph->membox, nodes + (links * 2), MMAP_WR);

  graph->outnodes = (node_struct *)graph->membox.pool;
  graph->innodes = graph->outnodes + graph->nodenr;

  graph->outnodes = (node_struct *)membox_get_offset
    (&graph->membox, (void *)graph->outnodes);
  graph->innodes = (node_struct *)membox_get_offset
    (&graph->membox, (void *)graph->innodes);

  return 0;
}
