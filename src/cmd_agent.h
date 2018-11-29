#ifndef _CMD_AGENT_H_
#define _CMD_AGENT_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "tcp.h"

#define DEBUG(x, a...) printf("%s: " x "\n", __func__, ## a) 
#define BREAK(x, a...) printf("==## file [%s] func [%s] line [%d] " x "\n", __FILE__, __func__, __LINE__, ## a)

#define AGENT_TYPE_LINUX_NVIDIA     "linux-claymore-nvidia"
#define AGENT_TYPE_UX               "ux"

#endif
