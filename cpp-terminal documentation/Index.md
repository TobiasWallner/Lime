
This is an evolveing documentation of functions their usecases and behaviour that we discover while working with the library.
We did not create the library.

## namespace Therm
------------------

### <tty.hpp>
-------------

```C++
// Returns true if the standard input is attached to a terminal
bool is_stdin_a_tty();

// Returns true if the standard output is attached to a terminal
bool is_stdout_a_tty();

// Returns true if the standard error is attached to a terminal
bool is_stderr_a_tty(); 
```

 