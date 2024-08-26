Your task is to complete the virtual memory simulation in vasim.c.

The system has a 24-bit virtual address space with 4KiB pages, the physical memory has a total
of only 64 pages.  The pagetable is a flat page table that uses a first-in-first-out (FIFO) replacement
policy.

---

**vasim.c**

You must complete two functions: get_vpn and allocate_phys_page.

**get_vpn**: returns the virtual page number of the provided address

**allocate_phys_page**: This function is called only when a mapping is invalid (a page fault). It is 
passed a pointer to the page table and the virtual page number of the access. It updates the 
page table with a new physical page for the virtual page provided.  For simplicity we assume
that no physical pages are used initially, and when all pages are used the FIFO policy is to be used for replacement.
Initial mappings from virtual to physical pages should start with physical page 0, then 1, etc. until
all 64 pages are allocated. Thereafter, the FIFO algorithm should be used.

- If the verbose value is 1 (or True) and the valid of the pagetable entry is True, then you should call the provided function `print_replacement()`

The program has three command line arguments:
-  -f _filename_, 
- -n _access_limit_, and
- -v for verbose output.

the -f argument is required, the others are optional. The -n flag stops simulation after _access_limit_ accesses are processed
from the tracefile. 

We provide two trace files: row_trace.txt and col_trace.txt.

Run the simulator on the command line:
```bash
./vasim -f [tracefile] -n 1024
```

**Example output:**
```
./vasim -f row_trace.txt -n 1024
Running for 1024 accesses

Accesses 1024: Hits: 1015 Faults: 9
```

---

**Local Testing and Submission**

```bash
python3 test_kit.py ALL
```

Submit your completed vasim.c to gradescope.
