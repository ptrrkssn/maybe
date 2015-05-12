#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#define PATH_RANDOM "/dev/random"


int replies = -1;
int silent = 0;


int
main(int argc,
     const char *argv[])
{
    int fd, i, j, rc = 1;
    unsigned long seed = time(NULL)^getpid();
    int len;
    const char *ynv[] = { "y", "n",  NULL };
    const char **ap;
    
    
    for (i = 1; i < argc && argv[i][0] == '-'; i++)
    {
	for (j = 1; argv[i][j]; j++)
	    switch (argv[i][j])
	    {
	      case 'S':
		if (strcmp(argv[i]+j+1, "-") == 0)
		{
		    fd = open(PATH_RANDOM, O_RDONLY);
		    if (fd < 0)
		    {
			fprintf(stderr, "%s: %s: Unable to open for reading\n",
				argv[0], PATH_RANDOM);
			exit(1);
		    }
		    if (read(fd, &seed, sizeof(seed)) != sizeof(seed)) {
			fprintf(stderr, "%s: %s: Unable to read seed number\n",
				argv[0], PATH_RANDOM);
			exit(1);
		    }
		    close(fd);
		}
		else
		{
		    if (sscanf(argv[i]+j+1, "%lu", &seed) < 1)
		    {
			fprintf(stderr, "%s: %s: Invalid random seed number\n",
				argv[0], argv[i]+j+1);
			exit(1);
		    }
		}
		goto NextArg;
		
	      case 'n':
		sscanf(argv[i]+j+1, "%u", &replies);
		goto NextArg;

	      case 's':
		++silent;
		break;
		
	      case 'h':
		printf("Usage: %s [<options>] [<answer-1> ... <answer-N>]\n", argv[0]);
		printf("Options:\n");
		printf("   -S<seed>     Force random seed value.\n");
		printf("   -n<count>    Return <count> results (implies -v).\n");
		printf("   -h           Display this text.\n");
		printf("   -s           Silent - only return a random exit code.\n");
		exit(0);
		
	      default:
		fprintf(stderr, "%s: Invalid option: %s\n", argv[0], argv[i]);
		exit(1);
	    }
      NextArg:;
    }

    srand(seed);
    
    len = argc-i;
    if (len > 0)
	ap = argv+i;
    else
    {
	ap = ynv;
	len = 2;
    }
    
    rc = (rand()%len);
    while (!silent && replies) {
	fputs(ap[rc], stdout);
	putc('\n', stdout);
	if (replies > 0)
	    --replies;
	rc = (rand()%len);
    };

    return rc;
}
