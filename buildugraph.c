#include        "common.h"
#include        "graph.h"

void
help (char *cmd);

int
main (int argc, char **argv)
{
  int           opt;
  unsigned int  nodenr = 0, linknr = 0;
  ugraph_struct g;
  char          name[32];

  /* fprintf (stdout, "long int: %lu\n", sizeof (long int)); */

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
      exit (-1);
    }
  }

  g.nodenr = nodenr;
  g.linknr = linknr;
  strcpy (g.name, name);

  fprintf (stderr, "Memeory usage:\n");
  fprintf (stderr, "\tnode_struct: %lu\n", sizeof (node_struct));
  fprintf (stderr, "\tunsigned int: %lu\n", sizeof (unsigned int));

  unsigned int  nodes = nodenr * sizeof (node_struct);
  unsigned int  links = linknr * sizeof (unsigned int);

  fprintf (stderr, "\tnodes: %u nodes, %u bytes (%u MB)\n", nodenr, nodes, nodes / (1024 * 1024));
  fprintf (stderr, "\tlinks: %u links, %u bytes (%u MB)\n", linknr, links, links / (1024 * 1024));
  fprintf (stderr, "\ttotal: %u bytes (%u MB)\n", nodes + links, (nodes + links) / (1024 * 1024));

  init_ugraph (&g);

  build_ugraph (&g);

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
