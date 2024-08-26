# Disassembler lab

## Objectives

* Peek at assembly language (facileASM) programs and how they run under Facile.
* Work with C `printf` with `switch` statements and `enum` enumerated types to format output.
* Practice with command line (shell).
* Understand standard input and standard output, and EOF (end of file, `ctrl-d`).

### Preliminaries

For this lab, first follow the same steps as for all C coding labs.  

1. Fork this repository and mark it private.

2. Clone your newly forked repository into your development directory (folder) for your Linux container.

3. Read the Facile document `facile.pdf` up to the start of Section 3: the first three pages, plus Figure 2 and Table 1 on pg. 4.   The terms and concepts there will help you understand this lab description.    Section 3.3 describes the mandated output format for facileASM.

4. Get started with building and running facile as described below.

### Get started

Open a command shell in your cloned directory, and just type `make`.

```
make
```

It builds a single executable binary program, called facile.   You can run it by typing `./facile` to your command shell.
The prefix `./` tells the shell to look for the named program in the current directory rather than in standard commands.

```
./facile
Usage: ./facile [-x or -d] <program file> (provide input on stdin)
```

You use the `-x` (execute) option to run facile programs.  The repo has a few programs in `.fac` files. For example, try:

```
./facile -x infinite.fac
Don't leave me hanging!  Type inputs and then ctrl-d.
```

That `infinite.fac` program is a simple infinite loop.   But nothing happens yet because facile wants input before launch.
It reminds you that it is hanging
waiting for input.  Since infinite ignores its input, just type `ctrl-d` (`control` key and `d` key together) to launch.
Ctrl-d means "end of file": it says you are done entering input.

After a few times around the loop, facileCPU raises a fault to say the program ran on too long. That was fun!

Now try:

```
./facile -x count.fac
```

Count reads one number as input.  Type a non-negative integer, not too big.  Maybe 42. Type `return`.  Then `ctrl-d`.
Count counts to that number you entered,  i.e., it loops and writes the counts to its output, then halts (fin).  Facile prints
the program's output after it terminates.

*Thought Question*: what is the largest number `count.fac` can count to?   What limits it?  Can you find it in the code?

### Input and output, featuring echo and cat

Now try a program that takes multiple inputs:

```
./facile -x echo.fac
```

Echo reads a sequence of input numbers---non-negative integers, not too many.  You can type them in one per line, typing a return after
each number, then type `ctrl-d` as before.

Typing a bunch of numbers on the keyboard gets tedious.  Use the standard `cat` command to look at the text file `numbers.txt`:

```
cat numbers.txt
```

You should see some good numbers.   We can pass them as input to echo like this:

```
./facile -x echo.fac <numbers.txt
```

That says: run echo as before, but this time read the input from the file numbers instead of from the keyboard.
The character `<` means: redirect the standard input to the named file instead of the keyboard.   *I/O redirection*
is a nice feature of the Unix/Linux command line.

If you don't like those numbers, there are others.   You can make your own file of numbers to use.   Of course you
can use a text editor.   Or, for quick you can use `cat`.

```
cat >mynumbers.txt
```

That cat might appear to be asleep.    In fact it is waiting for input on its standard input: the keyboard.
`Cat` waits silently for input, rather than reminding
you to provide it like facile does.   But `cat` takes input the same way.
Type your numbers (or whatever you want), 
in lines followed by `return` key, then after return from the last line type `ctrl-d`.

Like echo, `cat` simply copies its input to its output.  The `>` means: redirect the standard output
to the named file instead of the terminal.  You can see if you did it right by viewing the `mynumbers.txt` file:

```
cat mynumbers.txt
```

That `cat` should type your numbers back to you.   Now you can use them easily for inputs to your facile programs.
Try it with the echo program:

```
./facile -x echo.fac <mynumbers.txt
```

You can also use the `cat` command to look at any fac program, including the examples above.   You can even use
`cat >prog.fac` to write your own facileASM programs.   Like all programming tools, it works best if you get the code right the first time.
Otherwise you will want to edit your facileASM code with a proper text editor.

### Disassembler option

Another way to look at a program is to use facile's `-d` option: disassemble.

```
./facile -d echo.fac
```

Facile reads the echo program assembles it as before.   But it does not
execute the binary (`-x`).   Instead it disassembles the binary: it
regenerates some ASM code that produces the same binary. 

If you try the `-d` option, you see that it does not work: it just prints the numbers for the executable binary.  This lab asks you to write the code for the `-d` option.


### Writing the disassembler

This lab asks you to write a simple disassembler for the Facile machine architecture and its assembly language (facileASM).  You don't need to understand (yet) much at all about how Facile works or what the facileASM code means.   

You are to fill in C code for one function in the file `dis.c`.   That is the only code to modify.

`int facile_asm_showline(opcode_t op, int src, int dest)`

This function `facile_asm_showline` is called exactly once for each instruction of a facileASM program when you run facile with the `-d` option.   Its function is to print out the instruction in proper facileASM syntax.

The arguments to `facile_asm_showline` are the fields of the executable binary representation of the instruction.   It is defined in the type instruction_t in the C header file `facile.h`.

The objective of your code is to use C `printf` to print the instruction in one line to the standard output.   Print it so it looks exactly as described in Section 3.3.   That section gives some hints about how to find the mnemonic for the opcode, and how to format its operands in compliance with the mandated source and destination operand modes for that opcode.   

You do not need to understand what the operand modes mean, but if you are curious you can find it in Section 3 of the Facile document.    We will discuss it later in the semester.

The disassembled output from the `-d` option should look like the facileASM code in the various `.fac` files, but without the comments.   It looks like the code on the left side of Figure 2, but without the line numbers.

To do it, you will need to use C `switch` statements on certain enumerated types defined in `facile.h`.  As a model, you can look at theassembler code in `asm.c`.    It is simple and instructive, though more involved than the code you need to write.

### Testing: disassembler games

Run `make` to rebuild facile after making you changes. Changing your program's C code has no
effect until you save your changes and recompile/rebuild.
   
If you get it right, the disassembled output from the `-d` option should work if you pass it back to facile as a program to assemble and execute.   That property is useful to test your disassembler on the sample fac programs. 

Try your disassembler with a program of your choice,  e.g.: echo.

```
./facile -d echo.fac
```

Use `cat` to compare the output to the original `echo.fac`.   Are there any differences?   Note that comments are lost.   Why?
It is because the assembler ignores comments, so they are not in the
binary, and so the disassembler does not know about them.

Is it the same program?   At least the output of `-d` should itself be legal facileASM code with the same behavior.   Of course we can
write it into a file and try to pass it off as a program to facile.   Try it out with `count.fac`:

```
./facile -d count.fac >discount.fac
cat discount.fac
./facile -x discount.fac
```

Type an input number and see if it counts.

### Submission

Add, commit, and push `dis.c` to your git repo.

Also create, add, and commit a text file called `statement.txt` disclosing any sources of assistance for the lab.


   ```bash
   git add dis.c statement.txt
   git commit -m "finished disassembler"
   git push
   ```

   In future labs, you will be responsible for typing the commands to add,
   commit, and push your files.
   
   **Note**: The commit message, which comes after `-m`, can be whatever you
   want. It is a short string describing the changes in this commit.  

Finally, submit your completed lab to Gradescope via the GitLab submission button.

