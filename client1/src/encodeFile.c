#include "../inc/client.h"

int encodeFile(const char *filename, char **base64_data, size_t *base64_len) {
    int fd, ret = -1;
    struct stat sb;
    char buffer[1024];
    BIO *bio, *b64, *file;
    size_t nbytes, bytes_read = 0, total_bytes = 0;
    
    // Open file for reading
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return -1;
    }
    
    // Get file size
    if (fstat(fd, &sb) == -1) {
        perror("Error getting file size");
        return -1;
    }
    nbytes = sb.st_size;
    
    // Allocate memory for base64-encoded data
    *base64_data = malloc((nbytes*4/3)+4);
    if (*base64_data == NULL) {
        perror("Error allocating memory");
        return -1;
    }
    
    // Create base64 BIO filter
    b64 = BIO_new(BIO_f_base64());
    if (b64 == NULL) {
        perror("Error creating base64 BIO");
    }
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    
    // Create read BIO from file descriptor
    file = BIO_new_fd(fd, BIO_CLOSE);
    if (file == NULL) {
        perror("Error creating read BIO");
        return -1;
    }
    
    // Chain BIO filters and encode data
    bio = BIO_new(BIO_f_buffer());
    if (bio == NULL) {
        perror("Error creating buffer BIO");
        return -1;
    }
    bio = BIO_push(b64, bio);
    bio = BIO_push(file, bio);
    while ((bytes_read = BIO_read(bio, buffer, sizeof(buffer))) > 0) {
        memcpy(*base64_data + total_bytes, buffer, bytes_read);
        total_bytes += bytes_read;
    }
    
    // Check for errors during encoding
    if (bytes_read < 0) {
        perror("Error reading file");
        return -1;
    }
    
    // Flush the BIO
    BIO_flush(bio);
    
    // Set length of base64-encoded data
    *base64_len = total_bytes;
    
    // Return success
    ret = 0;
    
    // Cleanup resources
    BIO_free_all(bio);
    close(fd);
    
    return ret;
}
