#include "../inc/server.h"

char* format_time(long int timestamp) {
    char* buffer = malloc(20 * sizeof(char)); // allocate space for the formatted string
    time_t raw_time = timestamp;
    struct tm* time_info = localtime(&raw_time);
    strftime(buffer, 20, "%Y.%m.%d %H:%M", time_info);
    return buffer;
}

long int get_current_time(void) {
    time_t current_time;
    time(&current_time);
    return (long int)current_time;
}
