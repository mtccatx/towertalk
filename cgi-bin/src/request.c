// Implement request processing

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <cmd.h>
#include <request.h>

// -------- Public Data --------

size_t REQCMD_NUM_ARGS[REQCMD_COUNT] = {
    3,
    5,
    0,
    0,
    2,
    0
};

// -------- Private Data --------

// Map the response code enum to the text a response header should send
static char *RESPCODE_TXTS[RESPCODE_COUNT] = {
    "200 OK",
    "400 Bad Request",
    "401 Unauthorized",
    "500 Internal Server Error"
};

// -------- Private Fn Protos --------

// Try to parse an {arg} starting at a given index. Returns 0 if it fails else the length of the arg
static size_t get_next_arg(char *buffer, size_t len, size_t start_ind);

// Add an argument from a section of buffer to a request
static req_err_t append_arg(req_t *ref_req, char *buffer, size_t start_ind, size_t arg_len);

// -------- Public API --------

req_err_t request__make_from_env(req_t *ref_req) {
    if (!ref_req) {
        return REQERR_NULL_ARG;
    }
    if (ref_req->args || ref_req->args_len != 0) {
        return REQERR_INITIALIZED;
    }

    // Read in the request content
    char *req_method = getenv("REQUEST_METHOD");
    if (!req_method || strcmp(req_method, "POST") != 0) {
        return REQERR_WRONG_METHOD;
    }
    char *content_len_str = getenv("CONTENT_LENGTH");
    if (!content_len_str) {
        return REQERR_CONTENT_READ;
    }
    size_t content_len = atoll(content_len_str);
    if (content_len < 1) {
        return REQERR_CONTENT_MALFORMED;
    }
    char *buffer = malloc(content_len + 1);
    buffer[content_len] = '\0';
    size_t bytes_read = fread(buffer, 1, content_len, stdin);
    if (bytes_read != content_len) {
        free(buffer);
        return REQERR_CONTENT_READ;
    }

    // Parse it into a request
    ref_req->cmd = buffer[0] - '0';
    if (ref_req->cmd >= REQCMD_COUNT) {
        free(buffer);
        return REQERR_UNKNOWN_CMD;
    }
    size_t num_args = REQCMD_NUM_ARGS[ref_req->cmd];
    size_t buff_ind = 1;
    for (int i = 0; i < num_args; i++) {
        size_t arg_len = get_next_arg(buffer, content_len, buff_ind);
        if (!arg_len) {
            request__cleanup(ref_req);
            free(buffer);
            return REQERR_CONTENT_MALFORMED;
        }
        req_err_t err = append_arg(ref_req, buffer, buff_ind, arg_len);
        if (err != REQERR_NONE) {
            request__cleanup(ref_req);
            free(buffer);
            return err;
        }
        buff_ind += arg_len;
    }
    free(buffer);
    return REQERR_NONE;
}

void request__cleanup(req_t *ref_req) {
    if (!ref_req) {
        return;
    }
    if (!ref_req->args) {
        return;
    }
    for (size_t arg_i = 0; arg_i < ref_req->args_len; arg_i++) {
        if (!ref_req->args[arg_i]) {
            continue;
        }
        free(ref_req->args[arg_i]);
        ref_req->args[arg_i] = NULL;
    }
    free(ref_req->args);
    ref_req->args = NULL;
    ref_req->args_len = 0;
}

void request__respond(resp_code_t status_code, char *body) {
    printf("Status: %s\nContent-Type: text/plain\n\n\n%s\n", RESPCODE_TXTS[status_code], body);
}

// -------- Private Fns --------

static size_t get_next_arg(char *buffer, size_t len, size_t start_ind) {
    if (start_ind >= len) {
        return 0;
    }
    if (buffer[start_ind] != '{') {
        return 0;
    }
    size_t index = start_ind + 1;
    size_t arg_len = 1;
    size_t depth = 0;
    while (index < len && (buffer[index] != '}' || depth != 0)) {
        if (buffer[index] == '{') {
            depth++;
        }
        if (buffer[index] == '}') {
            depth--;
        }
        index++;
        arg_len++;
    }
    if (index >= len || buffer[index] != '}') {
        return 0;
    }
    arg_len++;
    return arg_len;
}

static req_err_t append_arg(req_t *ref_req, char *buffer, size_t start_ind, size_t arg_len) {
    if (!ref_req || !buffer) {
        return REQERR_NULL_ARG;
    }
    if (!ref_req->args) {
        ref_req->args = malloc(sizeof(char *));
        if (!ref_req->args) {
            return REQERR_OUT_OF_MEM;
        }
        ref_req->args_len = 1;
    } else {
        ref_req->args = realloc(ref_req->args, sizeof(char *) * (ref_req->args_len + 1));
        if (!ref_req->args) {
            return REQERR_OUT_OF_MEM;
        }
        ref_req->args_len++;
    }
    ref_req->args[ref_req->args_len - 1] = malloc(arg_len - 2 + 1);
    if (!ref_req->args[ref_req->args_len - 1]) {
        return REQERR_OUT_OF_MEM;
    }
    memcpy(ref_req->args[ref_req->args_len - 1], buffer + start_ind + 1, arg_len - 2);
    ref_req->args[ref_req->args_len - 1][arg_len - 2] = '\0';
    return REQERR_NONE;
}
