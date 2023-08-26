/* Utility functions for writing output validators for the Kattis
 * problem format.
 *
 * The primary functions and variables available are the following.
 * In many cases, the only functions needed are "init_io",
 * "wrong_answer", and "accept".
 *
 * - init_io(argc, argv):
 *        initialization
 *
 * - judge_in, judge_ans, author_out:
 *        std::istream objects for judge input file, judge answer
 *        file, and submission output file.
 *
 * - accept():
 *        exit and give Accepted!
 *
 * - accept_with_score(double score):
 *        exit with Accepted and give a score (for scoring problems)
 *
 * - judge_message(std::string msg, ...):
 *        printf-style function for emitting a judge message (a
 *        message that gets displayed to a privileged user with access
 *        to secret data etc).
 *
 * - wrong_answer(std::string msg, ...):
 *        printf-style function for exitting and giving Wrong Answer,
 *        and emitting a judge message (which would typically explain
 *        the cause of the Wrong Answer)
 *
 * - judge_error(std::string msg, ...):
 *        printf-style function for exitting and giving Judge Error,
 *        and emitting a judge message (which would typically explain
 *        the cause of the Judge Error)
 *
 * - author_message(std::string msg, ...):
 *        printf-style function for emitting an author message (a
 *        message that gets displayed to the author of the
 *        submission).  (Use with caution, and be careful not to let
 *        it leak information!)
 *
 */
#pragma once

#include <sys/stat.h>
#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

typedef void (*feedback_function)(const char*, ...);

#define USAGE "%s: judge_in judge_ans feedback_dir < author_out\n"

std::ifstream judge_in, author_out;

char *feedbackdir = NULL;

void vreport_feedback(float score,
                      const char* msg,
                      va_list pvar) {
    printf("%f", score);
    vfprintf(stderr, msg, pvar);
    fprintf(stderr, "\n");
    exit(0);
}

void report_feedback(float score, const char* msg, ...) {
    va_list pvar;
    va_start(pvar, msg);
    vreport_feedback(score, msg, pvar);
}

void author_message(const char* msg, ...) {
    va_list pvar;
    va_start(pvar, msg);
    vreport_feedback(0.0, msg, pvar);
}

void judge_message(const char* msg, ...) {
    va_list pvar;
    va_start(pvar, msg);
    vreport_feedback(0.0, msg, pvar);
}

void wrong_answer(const char* msg, ...) {
    va_list pvar;
    va_start(pvar, msg);
    vreport_feedback(0.0, msg, pvar);
    exit(0);
}

void judge_error(const char* msg, ...) {
    va_list pvar;
    va_start(pvar, msg);
    vreport_feedback(0.0, msg, pvar);
    assert(0);
}

void accept() {
    report_feedback(1.0, "translate:success");
}

void accept_with_score(double scorevalue) {
    report_feedback(scorevalue, "translate:partial");
    exit(0);
}


bool is_directory(const char *path) {
    struct stat entry;
    return stat(path, &entry) == 0 && S_ISDIR(entry.st_mode);
}

void init_io(int argc, char **argv) {
    if(argc < 4) {
        fprintf(stderr, USAGE, argv[0]);
        judge_error("Usage: %s judgein judgeans feedbackdir [opts] < userout", argv[0]);
    }

    judge_in.open(argv[1], std::ios_base::in);
    if (judge_in.fail()) {
        judge_error("%s: failed to open %s\n", argv[0], argv[1]);
    }

    author_out.open(argv[3], std::ios_base::in);
    if (author_out.fail()) {
        judge_error("%s: failed to open %s\n", argv[0], argv[3]);
    }
}
