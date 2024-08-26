# Debug lab

## Objectives

- Learn how to use `gdb`, a powerful debugging tool. We will use this tool
  extensively later on in the semester, so it's vital for you to get somewhat
  used to using it now.

---

### Overview

This directory contains a program called `debug1.c` and the corresponding
`Makefile`. There are four functions defined in `debug1.c`. They are creatively
named: `f1()`, `f2()`, `f3()`, and `f4()`. There is also a global array, again
creatively named `array1`. You are more than welcome to look through this file
-- it's not very interesting...

In this lab, we use `gdb` to examine the state of the program at different
points during its execution. For this lab, we  use only the command-line
version of `gdb`! This version has the ability to display source code to track
where the program is in its execution using the `-tui` option. For you to be
able to see source code within `gdb`, programs that you are running inside `gdb`
need to have been compiled with the`gcc` `-g` option. The debugger still works
without this, and don't worry -- you will get the chance soon to explore this as
well!

---

### `debug1.c`

Here is a list of steps you can follow to inspect what is going on in this
little program... You should follow all the steps as best as you can! Note that
for all of the commands involving the prompt of the debugger, it is assumed that
you will type the given command into the prompt of the debugger, and then follow
that with an `Enter` key or a `Return` key. Also, if you see weird output in the
`tui` window, you should just execute `tui disable` and then `tui enable` in the
prompt of the debugger. More about this later... Also, if you need to *exit* the
debugger at some point, just type `q` into the prompt and follow that up with an
`Enter` or a `Return` key.

1. First, build the target `debug1` by executing `make` in the command line.

2. Start `gdb` with the `-tui` option (`gdb -tui debug1`). 

   **NOTE:** The `tui` window of the debugger often becomes messed up during the
   course of execution. It might show that you're executing on multiple lines or
   you might see duplicate lines of code erroneously. To fix this, simply
   restart the window by typing `tui disable` and then `tui enable` into the
   prompt of the debugger.


3. Set a *breakpoint* at `main()`. You can do this by typing the following:
   ```gdb
   break main
   ```
   and following that with an `Enter` or `Return`. Again, this is assumed for
   all the debugger commands written here.

4. Begin the execution of the program by typing `r` into the prompt of the
   debugger. `r` stands for `run`, and you can type that too.

5. At this point, the program execution should have stopped. Your `tui` window
   should show the first line of the `main()` function and highlight it. We can
   now inspect the state of our program. Go ahead and type `print array1` into
   the `gdb` prompt, following that with an `Enter` key or the `Return` key. You
   should see the array printed out.

6. Now set a breakpoint at function `f1()`. You can again do this by typing
   `break f1` into the debugger prompt. Continue execution of the program until
   you reach the function `f1()`. In order to continue, you should type `c` into
   the prompt. Note that the command `r` will not work now, since the program is
   already running.

7. Use `s` (step) to execute the individual lines of code in `f1()` one by one
   until the function returns to `main()`.

8. Use `s` to step into the call to the function `f2()`, and through `f2()`
   until it returns to `main()`. This command and the previous should only be a
   couple of clicks...

9. Now use `n` (next) to step over the call to `printf()`. Note that although
   commands `n` and `s` seem to do the same thing in `gdb` (that is, execute one
   line of code), there is a big difference between them... Can you figure out
   what it is? *Hint*: Think about how much work is done by one line when using
   `s`, versus how much work is done by one line when using `n`.

   **Answer:** As you probably figured out yourself, the key difference between
   `s` and `n` is about the way in which they handle lines that call other
   functions. The command `s` will step *into* the function that is being called
   (which is what happened when the function `f2()` was called above), while the
   command `n` will treat the line that calls the function, as a *single line*.
   Meaning, all the execution of the function is done "behind the scenes". In
   the case of `printf()` executions, `n` is basically always the way to go,
   because there is usually little reason for you to look into the
   implementation of standard-library functions.

10. At this point, what usually happens is that the `tui` window gets a bit
    messed up. Now is a good time to follow this up with executing `tui disable`
    and `tui enable`. If the window is "back to normal", you can proceed. You
    can tell that it's back to normal by looking at how many lines are
    highlighted -- if it is only one, the window is OK!

    Now step *into* the function `f3()` (which command should
    you use now?... `s` or `n`?), and step for a few lines of the execution.
    Also print `array1` to see how it is changing. *Hint*: It should not be
    changing...

    This will probably be quite a few clicks, so be patient... Just before you
    leave `f3()` look at instruction 11.

11. Set a breakpoint at the next `printf()` -- use `b printf`. This will
    introduce a breakpoint at each call to `printf()` Note that you could also
    just insert a break point at an individual line, by executing `break
    <line_number>`. At this point, if you followed instruction 10 correctly, you
    should *still* be in function `f3()`.

11. Use `c` (continue) to continue executing until the next breakpoint. You
    should now be in `printf()` function, and you should basically not see
    anything in the `tui` window. Type `bt` to show your stack trace. 

    There should be *two* entries in the stack trace. Notice that the top entry
    shows that you're in `printf()`, which was called from `main()` (this is the
    lower entry). You can execute until the completion of the call using
    `finish`. Now you're back in `main()` function, on line `49`.

    At this point, the `tui` window likely got messed up again. So restart it...

12. Now go ahead and print `array1` to display its values. If you followed all
    the steps above, it should be unchanged.

13. You can now step into the function `f4()`, by using the `s` command.

14. Step a few lines of code in `f4()`, examining `array1` to see how it is
    changing. *Hint*: It *should* be changing now...

15. You can use `finish` to skip through until `f4()` returns. Or just execute
    `s` a couple more times if that is more fun to you...

16. Now you should be back in `main()`, on line `50`. Go ahead and print
    `array1` to see its final values.

17. Congrats! You are done, so you can just type `c` to finish the execution and
    exit the debugger.

If you mess up at some point of this tutorial, it's OK! You can just start at
the beginning by (presumably first exiting the debugger and then) executing the
`gdb` command at the terminal prompt.

You can either try to exactly execute the commands from above or use breakpoints
to stop at the function where you left off. Experiment with different
breakpoints and examining variables.

If you ever need to enable/disable any of your breakpoints, you might find the
following commands helpful:

* `info b` - lists the current breakpoints you have.

* `enable <breakpoint number>` enable the breakpoint with that ID number (you
  can find ID numbers by executing `info b`).

* `disable <breakpoint number>` disables the breakpoint with that ID number.

The `gdb`'s `info` command is sometimes useful for exploring data structures and
binaries. For example, right after you start `gdb` you can type `info functions`
and it will display the declared functions. You can ignore the items after the
"*Non-debugging symbols:*" output line.

---

### `debug2`

Now you need to find variables and values in an executable that you do not have
the source code for. The file `debug2` is a binary with debugging symbols
included. You can read this as the binary being compiled using the `-g` option
to the compiler. Like `debug1.c` this program has the same four function names
and a global variable named `array1`. However, some things in the program are
different. You need to use `gdb` to answer the following questions, and enter
the answers into Gradescope. The following questions are also replicated on
Gradescope, for your convenience.

1. What value is the argument `a` and return value of the function `f1()`?

2. What values are passed in `a` and `b` to the function `f2()`, and what is its
   return value?

3. What are the initial values in `array1`, and what is the return value of
   `f3()`?

4. What argument is passed to `f4()`?

5. What are the final values in `array1`?

---

### Local Testing and Submission

1. There is no local testing available (or needed) for this lab.

2. On Gradescope, you can enter your solutions for each question, press *save
   answer*, and it should tell you if your answer is correct or not.

3. You also do not have to push anything to GitLab for this lab.
