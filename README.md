# Find

Find is a command-line utility that locates files based on some user-specified criteria and either prints the pathname of each matched object or, if another action is requested, performs that action on each matched object. The project served as a school assignment at FI MUNI. 

Learn more about application 'find' on [wikipedia](https://en.wikipedia.org/wiki/Find_(Unix)).

# Usage

```bash
# compile using gcc
gcc -o '<PROGRAM_NAME>' *.c -Wall -Werror -pedantic -O3

# run
./'<PROGRAM_NAME>'
```

# Options
Options control how should the program behave and what should find.

```bash
-n ATTR (search based on a substring in the file name)

-s f|s (sort the list of paths by full path (f) or size (s))

-m MASK (searching for a file based on access rights)

-u USER (search for a file based on username)

-f NUM (minimum distance of nodes from the root)

-t NUM (maximum distance of nodes from the root)

-a (also searches in hidden files and subdirectories)

-0 (use '\0' instead of '\n')

-h (print help)
```

# Example

```bash
gcc -o find *.c -Wall -Werror -pedantic -O3

./find -h

./find -n "sort" ./../../../

./find -m 664

./find
```
