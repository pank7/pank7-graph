/*
 * File:                graph.h
 * Author:              Yi Li <liyi84@gmail.com>
 * Description:         Graph structure
 * Date & Time:         Sat Jun 19 01:33:09 2010
 */

#ifndef GRAPH_H
#define GRAPH_H 1

#include        "membox.h"

#define         MAXLINKS        4096

typedef struct __node
{
  unsigned int  node;
  unsigned int  lknr;
  unsigned int  *links;
} node_struct;

void
init_node (node_struct *node, unsigned int i);

typedef struct __ugraph
{
  char          name[32];
  unsigned int  nodenr;
  unsigned int  linknr;
  membox_struct membox;
  node_struct  *nodes;
} ugraph_struct;

unsigned int *
get_ulinks (ugraph_struct *graph, unsigned int node);

typedef struct __dgraph
{
  char          name[32];
  unsigned int  nodenr;
  unsigned int  linknr;
  membox_struct membox;
  node_struct  *outnodes;
  node_struct  *innodes;
} dgraph_struct;

node_struct *
get_outnodes (dgraph_struct *graph);

node_struct *
get_innodes (dgraph_struct *graph);

unsigned int *
get_dlinks (dgraph_struct *graph, node_struct *node);

int
init_ugraph (ugraph_struct *graph);

int
init_dgraph (dgraph_struct *graph);

int
delete_ugraph (ugraph_struct *graph);

int
delete_dgraph (dgraph_struct *graph);

int
build_ugraph (ugraph_struct *graph);

int
build_dgraph (dgraph_struct *graph);

int
load_ugraph_rdonly (ugraph_struct *graph);

int
load_ugraph (ugraph_struct *graph);

int
load_dgraph_rdonly (dgraph_struct *graph);

int
load_dgraph (dgraph_struct *graph);

void
print_ugraph (ugraph_struct *graph);

void
print_dgraph (dgraph_struct *graph);

void
print_revdgraph (dgraph_struct *graph);

#endif  /* GRAPH_H */
