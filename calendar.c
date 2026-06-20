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
#include <time.h>

#ifndef CALENDAR_VERSION
#define CALENDAR_VERSION "0.5.0"
#endif

typedef struct {
    int day;
    int month;
    int year;
} DateContext;

/* Forward Declarations */
bool parse_calendar_line(char *line_buffer, const DateContext *today, bool debug_mode);
bool is_date_match(const char *date_token, const DateContext *today);

/**
 * @brief Evaluates an isolated date token against the current system date.
 * Handles: mm/dd/yyyy, mm/dd, "mm *" (entire month), and dd (specific day)
 */
bool is_date_match(const char *date_token, const DateContext *today)
{
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
        // I need to check if this is a month wildcard (e.g., "06 *") or a day wildcard ("18")
        if (strchr(date_token, '*') != NULL) {
            // Found an asterisk, so item1 represents an entire month
            return (item1 == today->month);
        }
        
        // No asterisk, so it defaults to matching the specific day of the month
        return (item1 == today->day);
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

    if (is_date_match(date_token, today)) {
        if (debug_mode) {
            printf("Date: %-10s | Event: %s\n", date_token, desc_token);
        } else {
            printf("%s\n", desc_token);
        }
        return true;
    }

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
        .year  = time_info->tm_year + 1900
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
