print("@subtask samples")
print("manual sample-01.in");
print("");

# Line subtask
print("@subtask line");

print("gencode 10", end = '')
print(" inc_line 2 1000 10000", end = '')
print(" inc_line 3 1000 10000", end = '')
print(" inc_line 4 1000 10000", end = '')
print(" inc_line 1000 1000 10000", end = '')
print(" random_line 10 1000 10000", end = '')
print(" random_line 100 1000 10000", end = '')
for i in range(4):
  print(" random_line %d 1000 10000" % (1000 - i), end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line %d 1000 10000" % (1000 - i), end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 2 1000 10000", end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 3 1000 10000", end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 4 1000 10000", end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 1000 1000 10000", end = '')
print(" 0")

print("gencode 1 random_line 1000 1000 100000 0")
for i in [4, 6, 3]:
  print("gen-short-line %d 1000 0" % (i))
print("")

# Binary tree subtask
print("@subtask binary-tree")

print("gencode 10", end = '')
for i in range(10):
  print(" binary_tree %d 1000 10000" % (1000 - i), end = '')
print(" 0")

n = [2, 3, 4, 31, 32, 127, 128, 511, 512, 1000]
print("gencode 10", end = '')
for i in range(10):
  print(" binary_tree %d 1000 10000" % (n[i]), end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" binary_tree 2 1000 10000", end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" binary_tree 3 1000 10000", end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" binary_tree 4 1000 10000", end = '')
print(" 0")

print("gencode 1 binary_tree 1000 1000 100000 0")
print("")

# Star subtask
print("@subtask star")

print("gencode 10", end = '')
print(" inc_line 2 1000000 10000", end = '')
print(" inc_line 3 1000000 10000", end = '')
print(" inc_line 4 1000000 10000", end = '')
print(" inc_line 1000 1000000 10000", end = '')
print(" random_line 10 1000000 10000", end = '')
print(" random_line 100 1000000 10000", end = '')
for i in range(4):
  print(" random_line %d 1000000 10000" % (1000 - i), end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line %d 1000000 10000" % (1000 - i), end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 2 1000000 10000", end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 3 1000000 10000", end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 4 1000000 10000", end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 1000 1000000 10000", end = '')
print(" 0")

print("gencode 1 random_line 1000 1000000 100000 0")
for i in [4, 6, 3]:
  print("gen-short-line %d 1000000 0" % (i))

legs = [2, 2, 2, 2, 2, 3, 3, 3, 3, 3]
print("gencode 10", end = '')
for i in range(10):
  print(" random_star 4 1000000 10000 0 %d" % (legs[i]), end = '')
print(" 0")

w = [0, 0, 10, 25, 0, 0, 5, 25, 125]
legs = [1, 2, 3, 5, 10, 50, 100, 200, 500]
print("gencode 10 random_star 2 1000000 1000 0 1", end = '')
for i in range(9):
  print(" random_star 1000 1000000 11000 %d %d" % (w[i], legs[i]), end = '')
print(" 0")

w = [0, 0, 5, 100, 0, 5, 100, 0, 5, 100]
legs = [999, 200, 150, 100, 40, 30, 20, 3, 3, 3]
print("gencode 10", end = '')
for i in range(10):
  print(" random_star 1000 1000000 10000 %d %d" % (w[i], legs[i]), end = '')
print(" 0")
  
print("gencode 1 random_star 1000 1000000 100000 0 30 0")
a = [96, 541, 1000]
b = [7685, 321, 71415]
for i in range(3):
  print("gen-special 10 1000000 0 %d %d 0" % (a[i], b[i]))
print("")

# Exponential subtask
print("@subtask exponential")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 2 1000000000 10000", end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 3 1000000000 10000", end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 4 1000000000 10000", end = '')
print(" 0")

for i in [4, 6, 3]:
  print("gen-short-line %d 1000000000 0" % (i))

legs = [2, 2, 2, 2, 2, 3, 3, 3, 3, 3]
print("gencode 10", end = '')
for i in range(10):
  print(" random_star 4 1000000000 10000 0 %d" % (legs[i]), end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" binary_tree 2 1000000000 10000", end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" binary_tree 3 1000000000 10000", end = '')
print(" 0")

print("gencode 10", end = '')
for i in range(10):
  print(" binary_tree 4 1000000000 10000", end = '')
print(" 0")

a = [0, 10, 20, 30, 40]
b = [10, 20, 30, 40, 46]
for i in range(5):
  print("gen-all-iso %d %d 0" % (a[i], b[i]))
  
print("gencode 10", end = '')
print(" inc_line 8 1000000000 10000", end = '')
print(" random_line 8 1000000000 10000", end = '')
print(" binary_tree 8 1000000000 10000", end = '')
print(" random_star 8 1000000000 10000 10 3", end = '')
print(" random_star 8 1000000000 10000 5 5", end = '')
print(" random_star 8 1000000000 10000 0 7", end = '')
print(" random_tree 8 1000000000 10000 -50", end = '')
print(" random_tree 8 1000000000 10000 -5", end = '')
print(" random_tree 8 1000000000 10000 5", end = '')
print(" random_tree 8 1000000000 10000 50", end = '')
print(" 0")

print("gencode 10", end = '')
print(" random_line 8 1000000000 10000", end = '')
print(" random_line 8 1000000000 10000", end = '')
print(" random_star 8 1000000000 10000 0 3", end = '')
print(" random_star 8 1000000000 10000 5 4", end = '')
print(" random_star 8 1000000000 10000 2 5", end = '')
print(" random_star 8 1000000000 10000 10 6", end = '')
print(" random_star 8 1000000000 10000 0 7", end = '')
print(" random_tree 8 1000000000 10000 -10", end = '')
print(" random_tree 8 1000000000 10000 0", end = '')
print(" random_tree 8 1000000000 10000 50", end = '')
print(" 0")

for _ in range(3):
  w = [-125, -25, -5, -3, -1, 1, 3, 5, 25, 125]
  print("gencode 10", end = '')
  for i in range(10):
    print(" random_tree 8 1000000000 10000 %d" % (w[i]), end = '')
  print(" 0")
print("")

# Partial scoring subtask
print("@subtask partial")

print("gencode 10", end = '')
print(" inc_line 2 1000000000 10000", end = '')
print(" inc_line 3 1000000000 10000", end = '')
print(" inc_line 4 1000000000 10000", end = '')
print(" inc_line 1000 1000000000 10000", end = '')
print(" random_line 10 1000000000 10000", end = '')
print(" random_line 100 1000000000 10000", end = '')
for i in range(4):
  print(" random_line %d 1000000000 10000" % (1000 - i), end = '')
print(" 1")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line %d 1000000000 10000" % (1000 - i), end = '')
print(" 1")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 2 1000000000 10000", end = '')
print(" 1")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 3 1000000000 10000", end = '')
print(" 1")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 4 1000000000 10000", end = '')
print(" 1")

print("gencode 10", end = '')
for i in range(10):
  print(" random_line 1000 1000000000 10000", end = '')
print(" 1")

print("gencode 1 random_line 1000 1000000000 100000 1")
for i in [4, 6, 3]:
  print("gen-short-line %d 1000000000 1" % (i))

print("gencode 10", end = '')
for i in range(10):
  print(" binary_tree %d 1000000000 10000" % (1000 - i), end = '')
print(" 1")

n = [2, 3, 4, 31, 32, 127, 128, 511, 512, 1000]
print("gencode 10", end = '')
for i in range(10):
  print(" binary_tree %d 1000000000 10000" % (n[i]), end = '')
print(" 1")

print("gencode 10", end = '')
for i in range(10):
  print(" binary_tree 2 1000000000 10000", end = '')
print(" 1")

print("gencode 10", end = '')
for i in range(10):
  print(" binary_tree 3 1000000000 10000", end = '')
print(" 1")

print("gencode 10", end = '')
for i in range(10):
  print(" binary_tree 4 1000000000 10000", end = '')
print(" 1")

print("gencode 1 binary_tree 1000 1000000000 100000 1")

legs = [2, 2, 2, 2, 2, 3, 3, 3, 3, 3]
print("gencode 10", end = '')
for i in range(10):
  print(" random_star 4 1000000000 10000 0 %d" % (legs[i]), end = '')
print(" 1")

w = [0, 0, 10, 25, 0, 0, 5, 25, 125]
legs = [1, 2, 3, 5, 10, 50, 100, 200, 500]
print("gencode 10 random_star 2 1000000000 1000 0 1", end = '')
for i in range(9):
  print(" random_star 1000 1000000000 11000 %d %d" % (w[i], legs[i]), end = '')
print(" 1")

w = [0, 0, 5, 100, 0, 5, 100, 0, 5, 100]
legs = [999, 200, 150, 100, 40, 30, 20, 3, 3, 3]
print("gencode 10", end = '')
for i in range(10):
  print(" random_star 1000 1000000000 10000 %d %d" % (w[i], legs[i]), end = '')
print(" 1")
  
print("gencode 1 random_star 1000 1000000000 100000 0 30 1")
a = [96, 541, 1000]
b = [7685, 321, 71415]
for i in range(3):
  print("gen-special 10 1000000000 0 %d %d 1" % (a[i], b[i]))

a = [0, 10, 20, 30, 40]
b = [10, 20, 30, 40, 46]
for i in range(5):
  print("gen-all-iso %d %d 1" % (a[i], b[i]))
  
print("gencode 10", end = '')
print(" inc_line 8 1000000000 10000", end = '')
print(" random_line 8 1000000000 10000", end = '')
print(" binary_tree 8 1000000000 10000", end = '')
print(" random_star 8 1000000000 10000 10 3", end = '')
print(" random_star 8 1000000000 10000 5 5", end = '')
print(" random_star 8 1000000000 10000 0 7", end = '')
print(" random_tree 8 1000000000 10000 -50", end = '')
print(" random_tree 8 1000000000 10000 -5", end = '')
print(" random_tree 8 1000000000 10000 5", end = '')
print(" random_tree 8 1000000000 10000 50", end = '')
print(" 1")

print("gencode 10", end = '')
print(" random_line 8 1000000000 10000", end = '')
print(" random_line 8 1000000000 10000", end = '')
print(" random_star 8 1000000000 10000 0 3", end = '')
print(" random_star 8 1000000000 10000 5 4", end = '')
print(" random_star 8 1000000000 10000 2 5", end = '')
print(" random_star 8 1000000000 10000 10 6", end = '')
print(" random_star 8 1000000000 10000 0 7", end = '')
print(" random_tree 8 1000000000 10000 -10", end = '')
print(" random_tree 8 1000000000 10000 0", end = '')
print(" random_tree 8 1000000000 10000 50", end = '')
print(" 1")

for _ in range(3):
  w = [-125, -25, -5, -3, -1, 1, 3, 5, 25, 125]
  print("gencode 10", end = '')
  for i in range(10):
    print(" random_tree 8 1000000000 10000 %d" % (w[i]), end = '')
  print(" 1")

print("small-tree-broken-line")
print("gencode 10", end = '')
print(" inc_line 1000 1000000000 10000", end = '')
print(" random_line 1000 1000000000 10000", end = '')
print(" binary_tree 1000 1000000000 10000", end = '')
print(" random_star 1000 1000000000 10000 25 3", end = '')
print(" random_star 1000 1000000000 10000 5 30", end = '')
print(" random_star 1000 1000000000 10000 0 999", end = '')
print(" random_tree 1000 1000000000 10000 -50", end = '')
print(" random_tree 1000 1000000000 10000 -5", end = '')
print(" random_tree 1000 1000000000 10000 5", end = '')
print(" random_tree 1000 1000000000 10000 50", end = '')
print(" 1")

print("gencode 10", end = '')
print(" random_line 990 1000000000 10000", end = '')
print(" random_line 991 1000000000 10000", end = '')
print(" random_star 992 1000000000 10000 0 3", end = '')
print(" random_star 993 1000000000 10000 10 10", end = '')
print(" random_star 994 1000000000 10000 25 30", end = '')
print(" random_star 995 1000000000 10000 5 100", end = '')
print(" random_star 996 1000000000 10000 100 200", end = '')
print(" random_tree 997 1000000000 10000 -10", end = '')
print(" random_tree 998 1000000000 10000 0", end = '')
print(" random_tree 999 1000000000 10000 50", end = '')
print(" 1")

for _ in range(2):
  w = [-125, -25, -5, -3, -1, 1, 3, 5, 25, 125]
  print("gencode 10", end = '')
  for i in range(10):
    print(" random_tree %d 1000000000 10000 %d" % (1000-i, w[i]), end = '')
  print(" 1")

for _ in range(2):
  w = [-125, -25, -5, -3, -1, 1, 3, 5, 25, 125]
  print("gencode 10", end = '')
  for i in range(10):
    print(" random_tree %d 1000000000 10000 %d" % (1000-10+i, w[i]), end = '')
  print(" 1")

for _ in range(1):
  w = [-125, -25, -5, -3, -1, 1, 3, 5, 25, 125]
  print("gencode 10", end = '')
  for i in range(10):
    print(" random_tree 1000 1000000000 10000 %d" % (w[i]), end = '')
  print(" 1")
  
a = [0, 0, 1, 1, 2, 2, 3, 3]
b = [731, 1000, 892, 1000, 712, 1000, 905, 1000]
c = [213321, 321312, 42131, 12402139, 8786, 4444, 331, 321712]
for i in range(8):
  print("gen-special 10 1000000000 %d %d %d 1" % (a[i], b[i], c[i]))
  