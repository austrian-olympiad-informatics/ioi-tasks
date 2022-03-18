#include "stations.h"
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

static FILE *fin;
static FILE *fout;

int main(int argc, char **argv) {
	if (argc != 4) {
		fprintf(stderr, "grader error\n");
		fprintf(stderr, "number of arguments isn't 4\n");
		exit(0);
	}
	
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "a");
	
	if (string(argv[3]) == "0") { // grader A
		int r;
		if (fscanf(fin, "%d", &r) != 1) {
			fprintf(stderr, "grader A error\n");
			fprintf(stderr, "could not read 'r'\n");
			exit(0);
		}
		for (int i = 0; i < r; i++) {
			int n, k;
			if (fscanf(fin, "%d%d", &n, &k) != 2) {
				fprintf(stderr, "grader A error\n");
				fprintf(stderr, "could not read 'n' and 'k'\n");
				exit(0);
			}
			if (n < 0 || k < 0) { // error in labels
				exit(0);
			}
			vector<int> u(n - 1), v(n - 1);
			for (int j = 0; j < n - 1; j++) {
				if (fscanf(fin, "%d%d", &u[j], &v[j]) != 2) {
					fprintf(stderr, "grader A error\n");
					fprintf(stderr, "could not read edges\n");
					exit(0);
				}
			}
			vector<int> labels = label(n, k, u, v);
			fprintf(fout, "%d\n", (int)labels.size());
			if ((int)labels.size() != n) {
				fflush(fout);
				exit(0);
			}
			for (int j : labels) {
				fprintf(fout, "%d\n", j);
			}
			fflush(fout);
		}
	} else if (string(argv[3]) == "1") { 
		int q;
		if (fscanf(fin, "%d", &q) != 1) {
			fprintf(stderr, "grader B error\n");
			fprintf(stderr, "could not read 'q'\n");
			exit(0);
		}
		if (q < 0) { // error in labels
			exit(0);
		}
		for (int i = 0; i < q; i++) {
			int s, t, l;
			if (fscanf(fin, "%d%d%d", &s, &t, &l) != 3) {
				fprintf(stderr, "grader B error\n");
				fprintf(stderr, "could not read query\n");
				exit(0);
			}
			if (s < 0 || t < 0 || l < 0) { // error in answer
				exit(0);
			}
			vector<int> c(l);
			for (int j = 0; j < l; j++) {
				if (fscanf(fin, "%d", &c[j]) != 1) {
					fprintf(stderr, "grader B error\n");
					fprintf(stderr, "could not read adjacency list\n");
					exit(0);
				}
			}
			fprintf(fout, "%d\n", find_next_station(s, t, c));
			fflush(fout);
		}
	} else {
		fprintf(stderr, "grader error\n");
		fprintf(stderr, "unknown grader type %s\n", argv[3]);
		exit(0);
	}
	return 0;
}
