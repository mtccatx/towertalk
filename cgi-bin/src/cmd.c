// Implement commands

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/stat.h>
#include <ftw.h>
#ifndef __USE_MISC
#define __USE_MISC
#endif
#include <dirent.h>
#include <auth.h>
#include <page_index.h>
#include <cmd.h>

// -------- Private Data --------

#define BUFF_SIZE           1024
#define DIR_PERMISSION      0700

// -------- Public API --------

cmd_err_t cmd__make_post(const cmd_post_t *const post_data) {
    if (!auth__validate_user(post_data->pword)) {
        return CMDERR_AUTH;
    }

    // Find a suitable id for the file
    int id = -1;
    char fname[BUFF_SIZE] = "";
    char *username = getenv("USER");
    if (!username) {
        return CMDERR_INTERNAL;
    }
    FILE *file = NULL;
    int fname_size = 0;
    do {
        if (file) {
            fclose(file);
            file = NULL;
        }
        id++;
        fname_size = sprintf(fname, "/home/%s/public/towertalk/posts/%d/TITLE.txt", username, id);
        if (fname_size < 0 || fname_size >= BUFF_SIZE) {
            return CMDERR_INTERNAL;
        }
        file = fopen(fname, "r");
    } while (file);

    // Create the containing folders if they don't exist
    struct stat st = { 0 };
    fname_size = sprintf(fname, "/home/%s/public/", username);
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    if (stat(fname, &st) == -1) {
        if (mkdir(fname, DIR_PERMISSION) == -1) {
            return CMDERR_INTERNAL;
        }
    }
    fname_size = sprintf(fname, "/home/%s/public/towertalk", username);
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    if (stat(fname, &st) == -1) {
        if (mkdir(fname, DIR_PERMISSION) == -1) {
            return CMDERR_INTERNAL;
        }
    }
    fname_size = sprintf(fname, "/home/%s/public/towertalk/posts", username);
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    if (stat(fname, &st) == -1) {
        if (mkdir(fname, DIR_PERMISSION) == -1) {
            return CMDERR_INTERNAL;
        }
    }
    fname_size = sprintf(fname, "/home/%s/public/towertalk/posts/%d", username, id);
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    if (stat(fname, &st) == -1) {
        if (mkdir(fname, DIR_PERMISSION) == -1) {
            return CMDERR_INTERNAL;
        }
    }

    // Save post title
    fname_size = sprintf(fname, "/home/%s/public/towertalk/posts/%d/TITLE.txt", username, id);
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    file = fopen(fname, "w");
    if (!file) {
        return CMDERR_INTERNAL;
    }
    fprintf(file, "%s\n", post_data->title);
    fclose(file);

    // Save post body
    fname_size = sprintf(fname, "/home/%s/public/towertalk/posts/%d/BODY.md", username, id);
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    file = fopen(fname, "w");
    if (!file) {
        return CMDERR_INTERNAL;
    }
    fprintf(file, "%s\n", post_data->body);
    fclose(file);

    // Save text display
    fname_size = sprintf(fname, "/home/%s/public/towertalk/posts/%d/index.html", username, id);
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    file = fopen(fname, "w");
    if (!file) {
        return CMDERR_INTERNAL;
    }
    fprintf(file, "%s\n", PAGE_INDEX);
    fclose(file);

    return CMDERR_NONE;
}

cmd_err_t cmd__make_comment(const cmd_comment_t *const comment_data) {
    if (!auth__validate_user(comment_data->pword)) {
        return CMDERR_AUTH;
    }

    char *username = getenv("USER");
    if (!username) {
        return CMDERR_INTERNAL;
    }
    char fname[BUFF_SIZE] = "";

    // Create the containing folders if they don't exist
    struct stat st = { 0 };
    int fname_size = sprintf(fname, "/home/%s/public/", username);
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    if (stat(fname, &st) == -1) {
        if (mkdir(fname, DIR_PERMISSION) == -1) {
            return CMDERR_INTERNAL;
        }
    }
    fname_size = sprintf(fname, "/home/%s/public/towertalk", username);
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    if (stat(fname, &st) == -1) {
        if (mkdir(fname, DIR_PERMISSION) == -1) {
            return CMDERR_INTERNAL;
        }
    }
    fname_size = sprintf(fname, "/home/%s/public/towertalk/comments", username);
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    if (stat(fname, &st) == -1) {
        if (mkdir(fname, DIR_PERMISSION) == -1) {
            return CMDERR_INTERNAL;
        }
    }
    fname_size = sprintf(
        fname, "/home/%s/public/towertalk/comments/%s", username, comment_data->post_user
    );
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    if (stat(fname, &st) == -1) {
        if (mkdir(fname, DIR_PERMISSION) == -1) {
            return CMDERR_INTERNAL;
        }
    }
    fname_size = sprintf(
        fname,
        "/home/%s/public/towertalk/comments/%s/%s",
        username, comment_data->post_user, comment_data->post_id
    );
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    if (stat(fname, &st) == -1) {
        if (mkdir(fname, DIR_PERMISSION) == -1) {
            return CMDERR_INTERNAL;
        }
    }
    fname_size = sprintf(
        fname,
        "/home/%s/public/towertalk/comments/%s/%s/",
        username, comment_data->post_user, comment_data->post_id
    );
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    if (stat(fname, &st) == -1) {
        if (mkdir(fname, DIR_PERMISSION) == -1) {
            return CMDERR_INTERNAL;
        }
    }

    // Save body
    fname_size = sprintf(
        fname,
        "/home/%s/public/towertalk/comments/%s/%s/COMMENT.%s.md",
        username, comment_data->post_user, comment_data->post_id, comment_data->cmnt_id
    );
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    FILE *file = fopen(fname, "w");
    if (!file) {
        return CMDERR_INTERNAL;
    }
    fprintf(file, "%s\n", comment_data->body);
    fclose(file);
    return CMDERR_NONE;
}

cmd_err_t cmd__get_posts(cmd_post_ls_t *ref_posts) {
    if (!ref_posts) {
        return CMDERR_INTERNAL;
    }
    char *username = getenv("USER");
    if (!username) {
        return CMDERR_INTERNAL;
    }
    char fname[BUFF_SIZE];
    size_t fname_size = sprintf(fname, "/home/%s/public/towertalk/posts/", username);
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    DIR *dir = opendir(fname);
    if (!dir) {
        // No posts; not an error, just ref_post is empty
        return CMDERR_NONE;
    }
    struct dirent *entry = NULL;
    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            cmd_post_meta_t post = { 0 };

            int index = atoi(entry->d_name);
            if (index < 0) {
                continue;
            }
            post.index = index;

            fname_size = sprintf(
                fname, "/home/%s/public/towertalk/posts/%s/TITLE.txt",
                username, entry->d_name
            );
            if (fname_size < 0 || fname_size >= BUFF_SIZE) {
                continue;
            }
            FILE *file = fopen(fname, "r");
            if (!file) {
                continue;
            }
            fseek(file, 0, SEEK_END);
            size_t file_len = ftell(file) - 1; // Ignore new line
            rewind(file);
            post.title = malloc(file_len + 1);
            if (!post.title) {
                continue;
            }
            size_t read = fread(post.title, 1, file_len, file);
            if (read < file_len) {
                free(post.title);
                fclose(file);
                return false;
            }
            fclose(file);
            post.title[file_len] = '\0';

            if (!ref_posts->posts) {
                ref_posts->posts = malloc(sizeof(cmd_post_meta_t));
                if (!ref_posts->posts) {
                    return CMDERR_INTERNAL;
                }
                ref_posts->posts[0].index = post.index;
                ref_posts->posts[0].title = post.title;
                ref_posts->posts_len = 1;
            } else {
                ref_posts->posts = realloc(
                    ref_posts->posts,
                    (ref_posts->posts_len + 1) * sizeof(cmd_post_meta_t)
                );
                if (!ref_posts->posts) {
                    return CMDERR_INTERNAL;
                }
                ref_posts->posts[ref_posts->posts_len].index = post.index;
                ref_posts->posts[ref_posts->posts_len].title = post.title;
                ref_posts->posts_len++;
            }
        }
    }
    closedir(dir);
    return CMDERR_NONE;
}

cmd_err_t cmd__get_users(user_ls_t *ref_users) {
    if (!ref_users) {
        return CMDERR_INTERNAL;
    }
    FILE *passwd = fopen("/etc/passwd", "r");
    if (!passwd) {
        return CMDERR_INTERNAL;
    }
    char line[BUFF_SIZE] = "";
    char username[BUFF_SIZE] = "";
    size_t id = 0;
    while (fgets(line, sizeof(line), passwd)) {
        char *first_x = strchr(line, ':');
        if (!first_x) {
            continue;
        }
        strncpy(username, line, (size_t) (first_x - line));
        username[(size_t) (first_x - line)] = '\0';
        size_t found_items = sscanf(first_x, ":x:%lu:", &id);
        if (found_items == 1 && id >= 1000 && id < 2000) {
            if (!ref_users->users) {
                ref_users->users = malloc(sizeof(char *));
                if (!ref_users->users) {
                    return CMDERR_INTERNAL;
                }
                ref_users->users_len = 1;
            } else {
                ref_users->users = realloc(
                    ref_users->users,
                    sizeof(char *) * (ref_users->users_len + 1)
                );
                if (!ref_users->users) {
                    return CMDERR_INTERNAL;
                }
                ref_users->users_len++;
            }
            size_t name_len = strlen(username);
            ref_users->users[ref_users->users_len - 1] = malloc(name_len + 1);
            strcpy(ref_users->users[ref_users->users_len - 1], username);
            ref_users->users[ref_users->users_len - 1][name_len] = '\0';
        }
    }
    fclose(passwd);
    return CMDERR_NONE;
}

cmd_err_t cmd__get_comments(cmnt_ls_t *ref_cmnts, const cmd_cmnt_lookup_t *const cmnt_lookup_data) {
    if (!ref_cmnts || !cmnt_lookup_data) {
        return CMDERR_INTERNAL;
    }
    char *username = getenv("USER");
    if (!username) {
        return CMDERR_INTERNAL;
    }
    char fname[BUFF_SIZE];
    size_t fname_size = sprintf(
        fname,
        "/home/%s/public/towertalk/comments/%s/%s",
        username, cmnt_lookup_data->post_user, cmnt_lookup_data->post_id
    );
    if (fname_size < 0 || fname_size >= BUFF_SIZE) {
        return CMDERR_INTERNAL;
    }
    DIR *dir = opendir(fname);
    if (!dir) {
        // No posts; not an error, just ref_post is empty
        return CMDERR_NONE;
    }
    struct dirent *entry = NULL;
    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_REG) {
            // Get comment index
            size_t index = 0;
            if (sscanf(entry->d_name, "COMMENT.%lu.md", &index) != 1) {
                continue;
            }

            // Add to list
            if (!ref_cmnts->cmnt_ids) {
                ref_cmnts->cmnt_ids = malloc(sizeof(size_t));
                if (!ref_cmnts->cmnt_ids) {
                    return CMDERR_INTERNAL;
                }
                ref_cmnts->cmnt_ids[0] = index;
                ref_cmnts->cmnts_len = 1;
            } else {
                ref_cmnts->cmnt_ids = realloc(
                    ref_cmnts->cmnt_ids,
                    sizeof(size_t) * (ref_cmnts->cmnts_len + 1)
                );
                if (!ref_cmnts->cmnt_ids) {
                    return CMDERR_INTERNAL;
                }
                ref_cmnts->cmnt_ids[ref_cmnts->cmnts_len] = index;
                ref_cmnts->cmnts_len++;
            }
        }
    }
    closedir(dir);
    return CMDERR_NONE;
}
