#ifndef ACCEPT_ARROW_INPUT
#define ACCEPT_ARROW_INPUT

#define RIGHT_ARROW_KEY 77
#define LEFT_ARROW_KEY 75
#define ENTER_KEY 13

// (SPECIAL_ARROW_KEY 224) and (SPECIAL_KEY_IF_STANETEMT 0) will be used 
// because the left and right arrows are special symbols
// and if not included the getch() would also catch 'M' and 'K' chars.
// 224 works is the special symbol for my computer but on others it
// may be 0 so it is also included in the if statement

#define SPECIAL_ARROW_KEY 224       
#define SPECIAL_KEY_IF_STANETEMT 0

#include "dynamic_graph_creation.h"

int accept_arrow_input(struct Graph* graph);

#endif
