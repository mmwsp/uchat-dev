#include "../inc/client.h"

int decodeFile(const char* base64_data, const char* filename) {
    int status = 0;
    BIO *b64, *bio;
    FILE* fp = NULL;
    size_t base64_len = strlen(base64_data);
    
    // Create base64 decoding filter
    b64 = BIO_new(BIO_f_base64());
    if (!b64) {
        fprintf(stderr, "Failed to create base64 filter\n");
        return 0;
    }

    // Create output file
    fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Failed to open file for writing: %s\n", filename);
        BIO_free_all(b64);
        return 0;
    }

    // Create output filter
    bio = BIO_new_fp(fp, BIO_NOCLOSE);
    if (!bio) {
        fprintf(stderr, "Failed to create output filter\n");
        fclose(fp);
        BIO_free_all(b64);
        return 0;
    }

    // Set up chain of filters to do base64 encoding
    bio = BIO_push(b64, bio);

    // Write base64 encoded data to file
    if (BIO_write(bio, base64_data, base64_len) <= 0) {
        fprintf(stderr, "Failed to write base64 data to file\n");
        status = 0;
    } else {
        status = 1;
    }

    // Free resources
    BIO_flush(bio);
    BIO_free_all(bio);
    fclose(fp);

    return status;
}
