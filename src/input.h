#ifndef __INPUT_H
#define __INPUT_H

/************
 * INCLUDES *
 ************/

#include "window.h"

/***********
 * DEFINES *
 ***********/

#define INPUTS_NONE       (0u)
#define INPUT_LMB_PREV    (1u << INPUT_LMB_PREV_SHIFT)
#define INPUT_LMB_CURR    (1u << INPUT_LMB_CURR_SHIFT)
#define INPUT_LMB_PRESS   (1u << INPUT_LMB_PRESS_SHIFT)
#define INPUT_LMB_RELEASE (1u << INPUT_LMB_RELEASE_SHIFT)
#define INPUT_RMB_PREV    (1u << INPUT_RMB_PREV_SHIFT)
#define INPUT_RMB_CURR    (1u << INPUT_RMB_CURR_SHIFT)
#define INPUT_RMB_PRESS   (1u << INPUT_RMB_PRESS_SHIFT)
#define INPUT_RMB_RELEASE (1u << INPUT_RMB_RELEASE_SHIFT)

#define INPUT_MASK_PREV (INPUT_LMB_PREV | INPUT_RMB_PREV)
#define INPUT_MASK_CURR (INPUT_LMB_CURR | INPUT_RMB_CURR)

/*********
 * ENUMS *
 *********/

enum {
        INPUT_LMB_PREV_SHIFT = 0,
        INPUT_LMB_CURR_SHIFT,
        INPUT_LMB_PRESS_SHIFT,
        INPUT_LMB_RELEASE_SHIFT,
        INPUT_RMB_PREV_SHIFT,
        INPUT_RMB_CURR_SHIFT,
        INPUT_RMB_PRESS_SHIFT,
        INPUT_RMB_RELEASE_SHIFT,
};

/**************
 * STRUCTURES *
 **************/

struct input {
        uint8_t flags;
};

extern struct input input_init(void);
extern struct input input_poll(struct input               inp,
                               const struct window *const wnd);

#endif /* #ifndef __INPUT_H */
