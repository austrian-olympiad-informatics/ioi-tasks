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

std::istream judge_in(std::cin.rdbuf());
std::ifstream author_out;
std::ofstream author_in;

void vreport_feedback(double score,
                      const char* msg,
                      va_list pvar) {
    printf("%f", score);
    vfprintf(stderr, msg, pvar);
    fprintf(stderr, "\n");
    exit(0);
}

void report_feedback(double score, const char* msg, ...) {
    va_list pvar;
    va_start(pvar, msg);
    vreport_feedback(score, msg, pvar);
}

void judge_message(double scorevalue, const char* msg, ...) {
    va_list pvar;
    va_start(pvar, msg);
    vreport_feedback(scorevalue, msg, pvar);
}

void wrong_answer(const char* msg, ...) {
    va_list pvar;
    va_start(pvar, msg);
    vreport_feedback(0.0, msg, pvar);
    exit(0);
}

void accept_with_score(double scorevalue) {
    report_feedback(scorevalue, "translate:success");
    exit(0);
}


bool is_directory(const char *path) {
    struct stat entry;
    return stat(path, &entry) == 0 && S_ISDIR(entry.st_mode);
}

void init_io(int argc, char **argv) {
    author_in.open(argv[2], std::ios_base::out);
    author_out.open(argv[1], std::ios_base::in);
    judge_in.rdbuf(std::cin.rdbuf());
}
