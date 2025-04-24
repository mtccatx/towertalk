// Authenticate a user

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <auth.h>

// -------- Private Data --------

#define BUFF_SIZE       1024

// -------- Public API --------

bool auth__validate_user(const char *const pword) {
    char fname_buff[BUFF_SIZE] = "";
    char *username = getenv("USER");
    if (!username) {
        return false;
    }

    // Get salt
    char salt_buff[BUFF_SIZE] = "";
    int fname_size = sprintf(fname_buff, "/home/%s/.config/towertalk/password.salt", username);
    if (fname_size < 0 || fname_size > BUFF_SIZE) {
        return false;
    }
    FILE *file = fopen(fname_buff, "r");
    if (!file) {
        return false;
    }
    fseek(file, 0, SEEK_END);
    size_t file_len = ftell(file) - 1; // Ignore new line
    rewind(file);
    size_t read = fread(salt_buff, 1, file_len, file);
    if (read < file_len) {
        fclose(file);
        return false;
    }
    fclose(file);
    salt_buff[file_len] = '\0';

    // Get hash
    char hash_buff[BUFF_SIZE] = "";
    fname_size = sprintf(fname_buff, "/home/%s/.config/towertalk/password.hash", username);
    if (fname_size < 0 || fname_size > BUFF_SIZE) {
        return false;
    }
    file = fopen(fname_buff, "r");
    if (!file) {
        return false;
    }
    fseek(file, 0, SEEK_END);
    file_len = ftell(file) - 1; // Ignore new line
    rewind(file);
    read = fread(hash_buff, 1, file_len, file);
    if (read < file_len) {
        fclose(file);
        return false;
    }
    fclose(file);
    hash_buff[file_len] = '\0';

    char *crypt_str = crypt(pword, salt_buff);
    if (!crypt_str) {
        return false;
    }
    char big_buff[2 * BUFF_SIZE + 2] = "";
    fname_size = sprintf(
        big_buff, "%s$%s", (char *) salt_buff, (char *) hash_buff
    );
    if (fname_size < 0 || fname_size > BUFF_SIZE) {
        return false;
    }
    return strcmp(crypt_str, big_buff) == 0;
}
