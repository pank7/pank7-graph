#include        "common.h"
#include        "graph.h"
#include        "list.h"
#include        <time.h>

void
help (char *cmd);

void
ugbfs (ugraph_struct *graph, unsigned int node, int *d,
       membox_struct *memboxq);

int
ugand (ugraph_struct *graph);

int
main (int argc, char **argv)
{
  int           opt;
  unsigned int  nodenr = 0, linknr = 0;
  ugraph_struct g;
  char          name[32];

  if (argc <= 1) {
    help (argv[0]);
    exit (-1);
  }

  while ((opt = getopt (argc, argv, "N:n:l:h")) != -1) {
    switch (opt) {
    case 'N':
      strcpy (name, optarg);
      break;
    case 'n':
      nodenr = atoi (optarg);
      break;
    case 'l':
      linknr = atoi (optarg);
      break;
    case 'h':
    default:
      help (argv[0]);
    }
  }

  g.nodenr = nodenr;
  g.linknr = linknr;
  strcpy (g.name, name);

  load_ugraph (&g);

  ugand (&g);

  delete_ugraph (&g);

  return 0;
}

void
help (char *cmd)
{
  fprintf (stderr, "usage:\n");
  fprintf (stderr, "\t%s -N <name> -n <nodenr> -l <linknr>\n", cmd);

  return;
}

typedef struct __qnode
{
  list_head           hook;
  unsigned int        node;
} qnode;

int
ugand (ugraph_struct *graph)
{
  membox_struct         memboxd, memboxq;
  unsigned int          and = -1;
  int                   *d = NULL;
  int                   i = -1, j = -1;
  clock_t               c;

  membox_init (&memboxq, sizeof (qnode) * graph->nodenr, MALLOC);
  membox_init (&memboxd, graph->nodenr * sizeof (int), MALLOC);

  for (i = 0; i < graph->nodenr; ++i) {
    sprintf (memboxd.name, "d/d-%d.bin", i + 1);
    d = (int *)membox_alloc (&memboxd, graph->nodenr * sizeof (int));
    c = clock ();
    ugbfs (graph, i + 1, d, &memboxq);
    fprintf (stderr, "%ld\n", (clock () - c) / CLOCKS_PER_SEC);
    for (j = 0; j < graph->nodenr; ++j) {
      ;
    }
    membox_reset (&memboxd);
    membox_reset (&memboxq);
  } /* for (i... */

  membox_free (&memboxd);
  membox_free (&memboxq);

  return and;
}

void
ugbfs (ugraph_struct *graph, unsigned int node, int *d,
       membox_struct *memboxq)
{
  LIST_HEAD(queue);  /* The visit queue. */
  list_head             *ptr;
  int                   i = -1;
  qnode                 *qn = NULL, *pos = NULL;
  unsigned int          *links = NULL;
  int                   dnow = 0;

  for (i = 0; i < graph->nodenr; ++i) {
    d[i] = -1;
  }
  d[node - 1] = dnow;
  /* Add first node. */
  qn = (qnode *)membox_alloc (memboxq, sizeof (qnode));
  qn->node = node;
  INIT_LIST_HEAD (&qn->hook);
  list_add_tail (&qn->hook, &queue);

  while (!list_empty (&queue)) {
    ptr = queue.next;
    list_del (ptr);
    pos = list_entry(ptr, qnode, hook);
    links = get_ulinks (graph, pos->node);
    for (i = 0; i < graph->nodes[pos->node - 1].lknr; ++i) {
      if (d[links[i] - 1] < 0) {
        d[links[i] - 1] = d[pos->node - 1] + 1;
        qn = (qnode *)membox_alloc (memboxq, sizeof (qnode));
        qn->node = links[i];
        INIT_LIST_HEAD (&qn->hook);
        list_add_tail (&qn->hook, &queue);
      } /* if */
    } /* for */
  } /* while */

  return;
}
