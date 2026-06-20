/**
 * @file calendar.c
 * @brief A modern unix utility to parse and display calendar events.
 */

/* Feature Test Macro for POSIX extensions */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#ifndef CALENDAR_VERSION
#define CALENDAR_VERSION "0.4.0"
#endif

typedef struct {
    int day;
    int month;
    int year;
    int wday; /* Day of the week: 0 = Sunday, 1 = Monday, ..., 6 = Saturday */
} DateContext;

/* Forward Declarations */
bool parse_calendar_line(char *line_buffer, const DateContext *today, bool debug_mode);
bool is_date_match(const char *date_token, const DateContext *today);
void lowercase_string(char *str);

/**
 * @brief Converts a string to lowercase in-place.
 */
void lowercase_string(char *str)
{
    if (!str) return;
    for (int i = 0; str[i]; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

/**
 * @brief Evaluates an isolated date token against the current system date.
 * Handles: mm/dd/yyyy, mm/dd, "mm *" (entire month), dd (specific day), "daily", "weekday", "weekend", and days of the week.
 */
bool is_date_match(const char *date_token, const DateContext *today)
{
    // Bug 0002: Check for the "daily" string variant
    if (strcmp(date_token, "daily") == 0) {
        return true;
    }

    // Bug 0003: Check for "weekday" (Monday through Friday: 1 to 5)
    if (strcmp(date_token, "weekday") == 0) {
        return (today->wday >= 1 && today->wday <= 5);
    }

    // Bug 0003: Check for "weekend" (Sunday or Saturday: 0 or 6)
    if (strcmp(date_token, "weekend") == 0) {
        return (today->wday == 0 || today->wday == 6);
    }

    // Bug 0004 & 0005: Match the first 3 letters of named days of the week
    const char *weekdays[] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
    for (int i = 0; i < 7; i++) {
        if (strncmp(date_token, weekdays[i], 3) == 0) {
            return (today->wday == i);
        }
    }

    int item1 = 0, item2 = 0, item3 = 0;
    int tokens_found = sscanf(date_token, "%d/%d/%d", &item1, &item2, &item3);

    if (tokens_found == 3) {
        // Format: mm/dd/yyyy
        return (item1 == today->month && item2 == today->day && item3 == today->year);
    } 
    
    if (tokens_found == 2) {
        // Format: mm/dd
        return (item1 == today->month && item2 == today->day);
    } 
    
    if (tokens_found == 1) {
        // Check if this is a month wildcard (e.g., "06 *") or a day wildcard ("18")
        if (strchr(date_token, '*') != NULL) {
            // Found an asterisk, so item1 represents an entire month
            return (item1 == today->month);
        }
        
        // If it's a completely alphabetic string, sscanf returns 0, so we only handle numeric fallback here
        if (isdigit((unsigned char)date_token[0])) {
            return (item1 == today->day);
        }
    }

    return false;
}

bool parse_calendar_line(char *line_buffer, const DateContext *today, bool debug_mode)
{
    if (line_buffer == NULL) return false;
    line_buffer[strcspn(line_buffer, "\r\n")] = '\0';

    // Destructively tokenize the line using the tab character
    char *date_token = strtok(line_buffer, "\t");
    char *desc_token = strtok(NULL, "\t");

    if (date_token == NULL || desc_token == NULL) {
        return false;
    }

    // Bug 0005: Create a copy of the date token and make it lowercase for easy comparison
    char *lc_date_token = strdup(date_token);
    if (lc_date_token == NULL) return false;
    lowercase_string(lc_date_token);

    if (is_date_match(lc_date_token, today)) {
        if (debug_mode) {
            printf("Date: %-10s | Event: %s\n", date_token, desc_token);
        } else {
            printf("%s\n", desc_token);
        }
        free(lc_date_token);
        return true;
    }

    free(lc_date_token);
    return false;
}

int main(int argc, char *argv[])
{
    bool debug_mode = false;

    if (argc > 1) {
        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
            printf("calendar utility version %s\n", CALENDAR_VERSION);
            return EXIT_SUCCESS;
        } else if (strcmp(argv[1], "-d") == 0) {
            debug_mode = true;
        }
    }

    time_t raw_time = time(NULL);
    struct tm *time_info = localtime(&raw_time);
    if (time_info == NULL) {
        fprintf(stderr, "Fatal: Could not retrieve local system time.\n");
        return EXIT_FAILURE;
    }

    DateContext today = {
        .day   = time_info->tm_mday,
        .month = time_info->tm_mon + 1,
        .year  = time_info->tm_year + 1900,
        .wday  = time_info->tm_wday // Saved to support weekday/weekend tracking
    };

    const char *home_dir = getenv("HOME");
    if (home_dir == NULL) {
        fprintf(stderr, "Fatal: HOME environment variable is undefined.\n");
        return EXIT_FAILURE;
    }

    const char *target_suffix = "/.calendar";
    int path_length = snprintf(NULL, 0, "%s%s", home_dir, target_suffix);
    if (path_length < 0) return EXIT_FAILURE;

    char *file_path = malloc((size_t)path_length + 1);
    if (file_path == NULL) return EXIT_FAILURE;
    snprintf(file_path, (size_t)path_length + 1, "%s%s", home_dir, target_suffix);

    FILE *stream_ptr = fopen(file_path, "r");    
    if (stream_ptr == NULL) {
        perror("Fatal: Unable to open target calendar stream");
        free(file_path);
        return EXIT_FAILURE;
    }

    char *current_line = NULL;
    size_t allocated_capacity = 0;
    
    while (getline(&current_line, &allocated_capacity, stream_ptr) != -1) {
        parse_calendar_line(current_line, &today, debug_mode);
    }

    free(current_line);
    free(file_path);
    fclose(stream_ptr);

    return EXIT_SUCCESS;
}
