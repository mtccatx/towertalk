// Module to process an HTTP request over cgi-bin into something we can use/understand

#pragma once

#include <stddef.h>
#include <cmd.h>

// Things that can go wrong when processing a request
typedef enum {
    REQERR_NONE = 0,
    REQERR_NULL_ARG,            // A function passed in NULL for ref_req
    REQERR_INITIALIZED,         // Trying to create a new req in a req that exists is a mem leak
    REQERR_WRONG_METHOD,        // We only use post requests
    REQERR_CONTENT_READ,        // CONTENT_LENGTH != length of content or other bad read
    REQERR_UNKNOWN_CMD,         // Something outside of req_cmd_t at the start.
    REQERR_CONTENT_MALFORMED,   // Couldn't deserialize content
    REQERR_OUT_OF_MEM           // Tried to malloc and failed
} req_err_t;

// Types of response code. This isn't all of them
typedef enum {
    RESPCODE_OK = 0,
    RESPCODE_CLNTERR_BAD_REQ,
    RESPCODE_CLNTERR_UNAUTHORIZED,
    RESPCODE_SVRERR_INTERNAL,
    RESPCODE_COUNT
} resp_code_t;

// Possible commands a user can request
typedef enum {
    REQCMD_MAKE_POST,
    REQCMD_MAKE_COMMENT,
    REQCMD_POSTS,
    REQCMD_COUNT
} req_cmd_t;

// Represent a command request made to us
typedef struct {
    req_cmd_t cmd;
    size_t args_len;
    char **args;
} req_t;

// How many arguments to expect
extern size_t REQCMD_NUM_ARGS[REQCMD_COUNT];

// Fill in a req_t with data from the cgi-bin environment variables
req_err_t request__make_from_env(req_t *ref_req);

// Clean up a created request (args are stored on heap)
void request__cleanup(req_t *ref_req);

// Send an arbitrary response back to the user
void request__respond(resp_code_t status_code, char *body);
