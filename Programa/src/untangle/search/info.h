#pragma once

#include <stdint.h>

/** Contains the information regarding the best version of a state */
struct search_state_info
{
	/** Maximum cost allowed when this state was found */
	uint8_t bound;
	/** Depth at which this state was found */
	uint8_t depth;
};

/** Contains the information regarding the best version of a state */
typedef struct search_state_info Info;
