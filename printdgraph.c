#include        "common.h"
#include        "graph.h"

void
help (char *cmd);

int
main (int argc, char **argv)
{
  int           opt;
  unsigned int  nodenr = 0, linknr = 0;
  dgraph_struct g;
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

  load_dgraph_rdonly (&g);

  fprintf (stdout, "dgraph\n");
  print_dgraph (&g);
  fprintf (stdout, "revdgraph\n");
  print_revdgraph (&g);

  delete_dgraph (&g);

  return 0;
}

void
help (char *cmd)
{
  fprintf (stderr, "usage:\n");
  fprintf (stderr, "\t%s -N <name> -n <nodenr> -l <linknr>\n", cmd);

  return;
}
