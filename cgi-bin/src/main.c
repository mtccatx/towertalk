// Entry point for Tower Talk cgi-bin control file

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <version.h>
#include <cmd.h>
#include <request.h>

// -------- Private Fn Protos --------

// Call into the request module to parse environment data and do error handling
static int get_req(req_t *ref_req);

// Convert a request into cmd module data and call appropriate run fns then error handle
static int run_cmd(const req_t *const ref_req);

// -------- Public API --------

int main(int argc, char **argv) {
    req_t req = { 0 };
    if (get_req(&req)) {
        return 1;
    }
    if (run_cmd(&req)) {
        request__cleanup(&req);
        return 1;
    }
    request__respond(RESPCODE_OK, "Completed");
    request__cleanup(&req);
    return 0;
}

// -------- Private Fns --------

static int get_req(req_t *ref_req) {
    req_err_t req_err = request__make_from_env(ref_req);
    if (req_err != REQERR_NONE) {
        switch (req_err) {
            case REQERR_INITIALIZED:
                request__respond(RESPCODE_SVRERR_INTERNAL, "Bad code on our part. Sorry");
                break;
            case REQERR_WRONG_METHOD:
                request__respond(RESPCODE_CLNTERR_BAD_REQ, "You must submit as a POST request");
                break;
                break;
            case REQERR_CONTENT_READ:
            case REQERR_CONTENT_MALFORMED:
                request__respond(RESPCODE_CLNTERR_BAD_REQ, "Failed to understand request");
                break;
            case REQERR_UNKNOWN_CMD:
                request__respond(RESPCODE_CLNTERR_BAD_REQ, "Invalid command requested.");
                break;
            default:
                request__respond(RESPCODE_SVRERR_INTERNAL, "An unknown error occurred");
                break;
        }
        return 1;
    }
    return 0;
}

static int run_cmd(const req_t *const ref_req) {
    if (ref_req->args_len != REQCMD_NUM_ARGS[ref_req->cmd]) {
        request__respond(
            RESPCODE_CLNTERR_BAD_REQ,
            "Wrong number of args to command"
        );
        return 1;
    }
    cmd_post_t post = { 0 };
    cmd_comment_t comment = { 0 };
    cmd_err_t cmd_err = CMDERR_NONE;
    cmd_post_ls_t posts = { 0 };
    char *resp = NULL;
    size_t resp_len = 0;
    char int_buff[1024] = "";
    size_t append_size = 0;
    user_ls_t users = { 0 };
    cmd_cmnt_lookup_t cmnt_lookup = { 0 };
    cmnt_ls_t cmnts = { 0 };
    switch (ref_req->cmd) {
        case REQCMD_MAKE_POST:
            post.title = ref_req->args[0];
            post.body = ref_req->args[1];
            post.pword = ref_req->args[2];
            cmd_err = cmd__make_post(&post);
            break;
        case REQCMD_MAKE_COMMENT:
            comment.post_user = ref_req->args[0];
            comment.post_id = ref_req->args[1];
            comment.body = ref_req->args[2];
            comment.pword = ref_req->args[3];
            comment.cmnt_id = ref_req->args[4];
            cmd_err = cmd__make_comment(&comment);
            break;
        case REQCMD_POSTS:
            cmd_err = cmd__get_posts(&posts);
            if (cmd_err == CMDERR_NONE) {
                resp_len = 1;
                resp = malloc(resp_len + 1);
                resp[0] = '[';
                for (size_t i = 0; i < posts.posts_len; i++) {
                    sprintf(int_buff, "%lu", posts.posts[i].index);
                    append_size =
                        strlen(posts.posts[i].title) + strlen(int_buff)
                            + strlen("{\"index\":,\"title\":\"\"}")
                            + (i == 0 ? 0 : 1);
                    resp = realloc(resp, resp_len + 1 + append_size);
                    if (i == 0) {
                        sprintf(
                            resp + resp_len,
                            "{\"index\":%s,\"title\":\"%s\"}",
                            (char *) int_buff, posts.posts[i].title
                        );
                    } else {
                        sprintf(
                            resp + resp_len,
                            ",{\"index\":%s,\"title\":\"%s\"}",
                            int_buff, posts.posts[i].title
                        );
                    }
                    resp_len += append_size;
                }
                resp = realloc(resp, resp_len + 1 + 1);
                resp_len++;
                resp[resp_len - 1] = ']';
                resp[resp_len] = '\0';
                request__respond(RESPCODE_OK, resp);
                free(resp);
                if (posts.posts_len > 0) {
                    for (size_t i = 0; i < posts.posts_len; i++) {
                        free(posts.posts[i].title);
                    }
                    free(posts.posts);
                }
                exit(0); // Return success ourselves bc we've already sent the response
            }
            break;
        case REQCMD_USERS:
            cmd_err = cmd__get_users(&users);
            if (cmd_err == CMDERR_NONE) {
                resp_len = 1;
                resp = malloc(resp_len + 1);
                resp[0] = '[';
                for (size_t i = 0; i < users.users_len; i++) {
                    append_size = strlen(users.users[i]) + strlen("\"\"") + (i == 0 ? 0 : 1);
                    resp = realloc(resp, resp_len + 1 + append_size);
                    if (i == 0) {
                        sprintf(resp + resp_len, "\"%s\"", users.users[i]);
                    } else {
                        sprintf(resp + resp_len, ",\"%s\"", users.users[i]);
                    }
                    resp_len += append_size;
                }
                resp = realloc(resp, resp_len + 1 + 1);
                resp_len++;
                resp[resp_len - 1] = ']';
                resp[resp_len] = '\0';
                request__respond(RESPCODE_OK, resp);
                free(resp);
                if (users.users_len > 0) {
                    for (size_t i = 0; i < users.users_len; i++) {
                        free(users.users[i]);
                    }
                    free(users.users);
                }
                exit(0); // Return success ourselves bc we've already sent the response
            }
            break;
        case REQCMD_COMMENTS:
            cmnt_lookup.post_user = ref_req->args[0];
            cmnt_lookup.post_id = ref_req->args[1];
            cmd_err = cmd__get_comments(&cmnts, &cmnt_lookup);
            if (cmd_err == CMDERR_NONE) {
                resp_len = 1;
                resp = malloc(resp_len + 1);
                resp[0] = '[';
                for (size_t i = 0; i < cmnts.cmnts_len; i++) {
                    sprintf(int_buff, "%lu", cmnts.cmnt_ids[i]);
                    append_size = strlen(int_buff) + (i == 0 ? 0 : 1);
                    resp = realloc(resp, resp_len + 1 + append_size);
                    if (i == 0) {
                        sprintf(resp + resp_len, "%s", int_buff);
                    } else {
                        sprintf(resp + resp_len, ",%s", int_buff);
                    }
                    resp_len += append_size;
                }
                resp = realloc(resp, resp_len + 1 + 1);
                resp_len++;
                resp[resp_len - 1] = ']';
                resp[resp_len] = '\0';
                request__respond(RESPCODE_OK, resp);
                free(resp);
                if (cmnts.cmnts_len > 0) {
                    free(cmnts.cmnt_ids);
                }
                exit(0);
            }
            break;
        case REQCMD_VERSION:
            request__respond(RESPCODE_OK, VERSION);
            exit(0);
            break;
        default:
            request__respond(RESPCODE_CLNTERR_BAD_REQ, "Invalid command requested.");
            return 1;
    }
    if (cmd_err != CMDERR_NONE) {
        switch (cmd_err) {
            case CMDERR_AUTH:
                request__respond(
                    RESPCODE_CLNTERR_UNAUTHORIZED,
                    "Incorrect username or password"
                );
                break;
            default:
                request__respond(
                    RESPCODE_SVRERR_INTERNAL,
                    "Unknown error occurred while processing command."
                );
                break;
        }
        return 1;
    }
    return 0;
}