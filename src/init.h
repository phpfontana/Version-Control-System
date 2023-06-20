# ifndef _INIT_H_
# define _INIT_H_

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "vcs.h"

# define INIT 'init'
# define VCS_FOLDER ".vcs"
# define COMMITS_FILE ".vcs/commits.txt"
# define CONTENTS_FILE ".vcs/contents.txt"
# define LOGS_FILE ".vcs/logs.txt"
# define METADATA_FILE ".vcs/metadata.txt"
# define STAGE_FILE ".vcs/stage.txt"

void vcs_init();

# endif