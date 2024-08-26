Your task is to complete the simple direct-mapped cache simulator in the file casim.c.

---
**casim.c**

You must complete two functions, get_tag and get_index, and complete the body of the
while loop that performs a lookup in the cache structure to determine if the
access is a hit or miss.  Your loop body should increment the value **hits** or **misses**
based on the result of the lookup. See the TODO comments in casim.c for where to put your code.

You can use the function log2(x) to get the log base 2 of x.
You should not have to write many lines of code for this lab.
 
A helpful debugging method is to use a block size of 16 and a cache size of 256.
This makes it very easy to use printf to see index and tag 
values in hex (I will let you think about why that is...)

---
**Testing Individual Tests**

Run make to compile `casim.c`

If there are any errors with compilation, please fix them and run make again.
```bash
make
```

Check the `settings.json` file for all of the tests arguments. The strings in "args" is all of the command line arguments in order.

Example line in `settings.json`:
```json
... "args": ["16", "1024", "col_trace.txt" ] ...
```

Run the simulator on the command line as follows:
```bash
./casim [blocksize] [number of sets] [tracefile]

./casim 16 1024 col_trace.txt
```
All three arguments are required. A 'trace file' is just a file containing a list of memory
accesses. We provide two trace files for you to use: row_trace.txt and col_trace.txt.

---

**Local Testing and Submission**

Run the Python tester to check your solution in a variety of configurations
using the given tracefiles:
```bash
python3 test_kit.py ALL
```

Submit your completed casim.c to gradescope.
