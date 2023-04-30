#include "../inc/uchat.h"

// TODO: Don't forget about json_object_put() for free() memory!!

//Check simple validation of password.
// return 0 if correct!
// return -1 if incorrect lenth of password
// return -2 if password didn't contain ALPHABET characters
// return -3 if password didn't contain DIGITS characters
// return -4 if password didn't contain LOWERCASE characters
// return -5 if password didn't contain UPPERCASE characters
int check_password(char *password) {
    if (strlen(password) < 16 && (int)strlen(password) > 24) {
        //perror("Error: length of password must be > 16 but < 24 characters!");
        return -1;
    }

    int alpa_counter = 0;
    int dig_counter = 0;
    int lower_counter = 0;
    int upper_counter = 0;

    for (int i = 0; i < (int)strlen(password); i++) {
        if (isalpha(password[i])) {
            alpa_counter++;
        }

        if (isdigit(password[i])) {
            dig_counter++;
        }

        if (isupper(password[i])) {
            upper_counter++;
        }

        if (islower(password[i])) {
            lower_counter++;
        }
    }

    if (alpa_counter < 1) {
        //perror("Error: password must contain > 1 ALPHABETIC character!");
        return -2;
    }

    if (dig_counter < 1) {
        //perror("Error: password must contain > 1 DIGIT character!");
        return -3;
    }

    if (lower_counter < 1) {
        //perror("Error: password must contain > 1 LOWERCASE character!");
        return -4;
    }
    
    if (upper_counter < 1) {
        //perror("Error: password must contain > 1 UPPERCASE character!");
        return -5;
    }

    return 0;
}

//Check simple validation of login.
// return 0 if correct!
// return -1 if incorrect length of login
// return -2 if login didn't contain alphabet, digits, '_' and '-' symbols
int check_login(char *login) {
    if ((int)strlen(login) < 2) {
        perror("Error: the length of login must be longer than 2 symbols!");
        return -1;
    }

    for (int i = 0; i < (int)strlen(login); i++) {
        if (isdigit(login[i] || isalpha(login[i])) || login[i] == '_' || login[i] == '-') {
            continue;
        }
        else {
            perror("Error: login must contain ALPHABETIC, DIGITS, '_' and '-' only!");
            return -2;
        }
    }

    return 0;
}

// Count words in string.
// return - count of words
int mx_count_words(const char *str, char c) {
    int count = 0;

    if (str == NULL) {
        return -1;
    }

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != c
            && (i == 0
            || str[i - 1] == c)) {
            count++;
        }
    }

    return count;
}

// Finding word.
// return - word
const char *find_word(const char *s, char c) {
    while (*s == c && *s != '\0') {
        s++;
    }

    if (*s == '\0') {
        return 0;
    }

    return s;
}

// Count lenth of string.
// return - length
int word_length(const char *s, char c) {
    int length = 0;
    while (s[length] != c && s[length] != '\0') {
        length++;
    }
    return length;
}

char *strnew(const int size) {
    char *str;
    if (size < 0) {
        return NULL;
    }

    str = malloc(size + 1);
    if (str == NULL) {
        return NULL;
    }

    for (int i = 0; i < size + 1; i++) {
        str[i] = '\0';
    }

    return str;
}

char *strjoin(char const *s1, char const *s2) {
    char *str;

    if (s1 == 0 && s2 == 0) {
        return 0;
    }
    if (s1 == 0 && s2 != 0) {
        return strdup(s2);
    }
    if (s1 != 0 && s2 == 0) {
        return strdup(s1);
    }

    str = strnew(strlen(s1) + strlen(s2));
    strcat(str, s1);
    strcat(str, s2);
    return str;
}

// To split string.
// return - array of words (string)
char **strsplit(const char *s, char c) {
    int count;
    char **arr;
    const char *word;

    if (s == NULL) {
        return NULL;
    }

    count = mx_count_words(s, c);
    arr = malloc((count + 1) * sizeof(char*));
    arr[count] = NULL;

    word = find_word(s, c);
    for (int i = 0; word != 0; i++) {
        int len = word_length(word, c);

        arr[i] = strnew(len);
        strncpy(arr[i], word, len);
        word = find_word(word + len, c);
    }

    return arr;
}

// To get formated date.
// return - date (string)
char *get_formated_date(char *full_date) {
    char **formated = strsplit(full_date, ' ');
    char *result = "";

    for (int i = 1; i < 4; i++) {
        result = strjoin(result, formated[i]);
        result = strjoin(result, " ");
    }

    return result;
}

char* format_time(long int timestamp) {
    char* buffer = malloc(20 * sizeof(char)); // allocate space for the formatted string
    time_t raw_time = timestamp;
    struct tm* time_info = localtime(&raw_time);
    strftime(buffer, 20, "%Y.%m.%d %H:%M", time_info);
    return buffer;
}

// To be continued..

