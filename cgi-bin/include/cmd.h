// Module to handle server commands

#pragma once

#include <request.h>

// All the things that can go wrong with this module
typedef enum {
    CMDERR_NONE = 0,
    CMDERR_AUTH,            // Wrong password
    CMDERR_INTERNAL         // Something weird happened. It's just programmed incorrectly
} cmd_err_t;

// Represent the data needed to make a post
typedef struct {
    char *title;
    char *body;
    char *pword;
} cmd_post_t;

// Represent the data needed to make a comment
typedef struct {
    char *post_user;
    char *post_id;
    char *body;
    char *pword;
    char *cmnt_id;
} cmd_comment_t;

// Represent posts to retrieve from system
typedef struct {
    size_t index;
    char *title;
} cmd_post_meta_t;

// A list of posts
typedef struct {
    size_t posts_len;
    cmd_post_meta_t *posts;
} cmd_post_ls_t;

// A list of users
typedef struct {
    size_t users_len;
    char **users;
} user_ls_t;

// Create a new post from the request data
cmd_err_t cmd__make_post(const cmd_post_t *const post_data);

// Create a new comment on a post from the request data
cmd_err_t cmd__make_comment(const cmd_comment_t *const comment_data);

// Read posts under ~/public/towertalk/posts/*/* and fill a list
cmd_err_t cmd__get_posts(cmd_post_ls_t *ref_post);

// Read users via /etc/passwd and return an array of strings
cmd_err_t cmd__get_users(user_ls_t *ref_users);
