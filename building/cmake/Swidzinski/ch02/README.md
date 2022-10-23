# The CMake Language

## Comments

[Bracket Argument](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html):

An opening bracket is written `'['` followed by zero or more `'='` followed by `'[`'. The corresponding closing bracket is written `']'` followed by the same number of '`=`' followed by '`]`'.

```
bracket_argument ::= bracket_open bracket_content bracket_close
bracket_open     ::= '[' '='* '['
bracket_content  ::= <any text not containing a bracket_close with
                        the same number of '=' as the bracket_open>
bracket_close    ::= ']' '='* ']'
```

[Bracket Comment](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html):

A `#` immediately followed by a `bracket_open` forms a *bracket comment* consisting of the entire bracket enclosure:

```
bracket_comment  ::= '#' bracket_argument
```

For example:

```
#[=[
bracket comment
    #[[
        nested bracket comment
    #]]
#]=]
```

## Command invocations

To execute a command, you must provide its name, followed by parentheses, in which you may enclose a whitespace-separated list of command arguments.

Command names aren't case-sensitive, but there is a convention in the CMake community to use `snake_case` in command names.

```
message("1st arg" 2nd_arg 3rd)
```

[Command Invocations](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html):

A command invocation is a name followed by paren-enclosed arguments separated by whitespace:

```
command_invocation  ::=  space* identifier space* '(' arguments ')'
identifier          ::=  <match '[A-Za-z_][A-Za-z0-9_]*'>
arguments           ::=  argument? separated_arguments*
separated_arguments ::=  separation+ argument? |
                         separation* '(' arguments ')'
separation          ::=  space | line_ending
```

Command names are case-insensitive. Nested unquoted parentheses in the arguments must balance. Each `'('` or `')'` is given to the command invocation as a literal Unquoted Argument. This may be used in calls to the `if()` command to enclose conditions. For example:

```
if(FALSE AND (FALSE OR TRUE)) # evaluates to FALSE
```

## Command arguments

[Command Arguments](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html):

There are three types of arguments within Command Invocations:

```
argument ::=  bracket_argument  |
              quoted_argument   |
              unquoted_argument
```

The only data type recognized by CMake is a *string*. This is why every command expects zero or more strings for its arguments.

CMake will evaluate every argument to a static string and then pass them into the command. *Evaluating* means *string interpolation*, or substituting parts of a string with another value. This can mean

- replacing the escape sequences,
- expanding the *variable references* (also called *variable interpolation*), and
- unpacking lists.

These kinds of *bracket arguments* have limited use – typically, to contain longer blocks of text.

```
message([[multiline
    bracket
    argument
]])
message([==[
    because we used two equal-signs "=="
    following is still a single argument:
    { "petsArray" = [["mouse","cat"],["dog"]] }
]==])
```

```bash
$ cmake -P chapter02/01-arguments/bracket.cmake
multiline
bracket
argument
    because we used two equal-signs "=="
    following is still a single argument:
    { "petsArray" = [["mouse","cat"],["dog"]] }
```

In most cases, we'll need something more dynamic, such as *quoted arguments*.

```
message("1. escape sequence: \" \n in a quoted argument")
message("2. multi...
    line")
message("3. and a variable reference: ${CMAKE_VERSION}")
```

```bash
$ cmake -P chapter02/01-arguments/quoted.cmake
1. escape sequence: "
in a quoted argument
2. multi...
line
3. and a variable reference: 3.16.3
```

*Unquoted arguments* evaluate both escape sequences and variable references. However, be careful with semicolons (;), as in CMake, this is treated as a delimiter. CMake will split the argument containing it into multiple arguments.

```
message(a\ single\ argument)
message(two arguments)
message(three;separated;arguments)
message(${CMAKE_VERSION})  # a variable reference
message(()()())            # matching parentheses
```

```bash
$ cmake -P chapter02/01-arguments/unquoted.cmake
a single argument
twoarguments
threeseparatedarguments
3.16.3
()()()
```

## Variables

Some key facts about variables in CMake:

- Variable names are case-sensitive and can include almost any character.
- All variables are stored internally as strings, even if some commands can interpret them as values of other data types (even lists!).

To set a variable, we simply call `set()`, providing its name and the value:

```
set(MyString1 "Text1")
set([[My String2]] "Text2")
set("My String 3" "Text3")

message(${MyString1})
message(${My\ String2})
message(${My\ String\ 3})
```

As you can see, the use of brackets and quoted arguments allows for spaces to be included in the variable name. However, when referencing it later, we have to escape the whitespace with a backslash `'\'`. For that reason, it is recommended to use only alphanumeric characters, dashes `'-'`, and underscores `'_'` in variable names.

The `set()` command accepts a plain text variable name as its first argument, but the `message()` command uses a *variable reference* wrapped in the `${}` syntax.

## Variable references

To create a reference to a defined variable, we need to use the `${}` syntax.

On evaluation, CMake will traverse the scope stack and replace `${MyString1}` with a value, or an empty string if no variable is found. This process is called *variable evaluation*, *expansion*, or *interpolation*.

| syntax     | cvariable ategory
|------------|-------------------
| `${}`      | normal or cache
| `$ENV{}`   | environment
| `$CACHE{}` | cache

To modify or create an environment variable, use the `set()` command:

```
# set(ENV{<variable>} <value>)
set(ENV{CXX}) "clang++")
```

*Cache variables* can be referenced with the `$CACHE{<name>}` syntax. To set a cache variable, use `set()` with the following syntax:

```
set(<variable> <value> CACHE <type> <docstring> [FORCE])
```

For example,

```
set(FOO "BAR" CACHE STRING "interesting value")
```

The above call has no permanent effect if the variable exists in the cache. However, if the value doesn't exist in cache or an optional `FORCE` argument is specified, the value will be persisted:

```
set(FOO "BAR" CACHE STRING "interesting value" FORCE)
```

## The variable scope in CMake

CMake has two *variable scopes*:

- *Function* scope: For when custom functions defined with `function()` are executed
- *Directory* scope: For when a `CMakeLists.txt` listfile in a nested directory is executed from the `add_subdirectory()` command

Whenever we try to access the normal variable, CMake will search for the variables from the current scope, and if the variable with such a name is defined, it will return its value. So far, so good. However, when CMake can't find a variable with that name, it will search through the cache variables and return a value from there if a match is found.

## Lists

To store a list, CMake concatenates all elements into a string, using a semicolon `';'` as a delimiter: `a;list;of;5;elements`.

To create a list, we can use the `set()` command. Because of how lists are stored, the following commands will have exactly the same effect:

```
set(myList a list of five elements)
set(myList a list "of;five;elements")
set(myList "a;list;of;five;elements")
```

CMake automatically unpacks lists in unquoted arguments. By passing an unquoted `myList` reference, we effectively send more arguments to the command:

```
message("the list is:" ${myList})
```

The `message()` command will receive here six arguments: `"the list is:"`, `"a"`, `"list"`, `"of"`, `"five"`, `"elements"`. This may have unintended consequences, as the output will be printed without any additional spaces between the arguments:

```
the list is:alistoffiveelements
```

[List Command](https://cmake.org/cmake/help/latest/command/list.html):

```
list(<subcommand> <list> <parameters> <output_variable>)
```

Reading and search subcommands:

| subcommand  | description
|-------------|-------------
| LENGTH      | Returns the list's length.
| GET         | Returns another list of elements specified by indices from the list.
| JOIN        | Returns a string joining all list's elements using the glue string.
| SUBLIST     | Returns another list of elements specified by the begin index and length.
| FIND        | Returns the index of the element specified in the list.

Modification subcommands:

| subcommand  | description
|-------------|-------------
| APPEND      | Appends elements to the list.
| FILTER      | Includes or removes items from the list that match the REGEX pattern.
| INSERT      | Inserts elements to the list to the specified index.
| POP_BACK    | Remove the last N values from the list.
| POP_FRONT   | Remove the first N values from the list.
| PREPEND     | Insert elements to the 0th position in the list.
| REMOVE_ITEM | Removes all instances of the given items from the list.
| REMOVE_AT   | Removes items at given indices from the list.
| REMOVE_DUPLICATES | Removes duplicated items in the list.

`TRANSFORM` subcommand: Transforms the list by applying an action to all or selected elements of the list.

```
list(TRANSFORM <list> <ACTION> [<SELECTOR>]
                      [OUTPUT_VARIABLE <output variable>])
```

| action      | description
|-------------|-----
| APPEND      | Append specified value to each element of the list.
| PREPEND     | Prepend specified value to each element of the list.
| TOLOWER     | Convert each element of the list to upper characters.
| TOUPPER     | Convert each element of the list to lower characters.
| STRIP       | Remove leading and trailing spaces from each element of the list.
| GENEX_STRIP | Strip any *generator expressions* from each element of the list.
| REPLACE     | Substitute the replacement expression for the REGEX match for each element of the list.

| selector | description
|----------|-------
| AT       | Specify a list of indexes.
| FOR      | Iterate over the range.
| REGEX    | Only matched elements will be transformed.

Ordering subcommands:

| subcommand  | description
|-------------|-------------
| RESERVE     | Reverses the contents of the list in-place.
| SORT        | Sorts the list in-place alphabetically.

For the subcommand `ORDER`, three keywords for sorting are available:

- `COMPARE`: selects the comparison method for sorting.
  - `STRING`: sorts a list of strings alphabetically.
  - `NATURAL`: sorts a list of strings using natural order.
  - `FILE_BASENAME`: sorts a list of pathnames of files by their basenames.
- `CASE`: selects a case sensitive or insensitive mode.
  - `SENSITIVE`
  - `INSENSITIVE`
- `ORDER`: controls the sort order.
  - `ASCENDING`
  - `DESCENDING`

## Conditional blocks

```
if(<condition>)
    <commands>
elseif(<condition>) # optional block, can be repeated
    <commands>
else()              # optional block
    <commands>
endif()
```

The `if()` conditions support the logical operators:

```
NOT <condition>
<condition> AND <condition>
<condition> OR <condition>
```

Also, the nesting of conditions is possible with matching pairs of parentheses (`()`).

## The evaluation of a string and a variable

[Condition Syntax, Basic Expressions](https://cmake.org/cmake/help/latest/command/if.html):

`if(<boolean-constant>)`:

- True if the constant is `1`, `ON`, `YES`, `TRUE`, `Y`, or a non-zero number.
- False if the constant is `0`, `OFF`, `NO`, `FALSE`, `N`, `IGNORE`, `NOTFOUND`, the empty string, or ends in the suffix `-NOTFOUND`.
- Named boolean constants are case-insensitive.
- If the argument is not one of these specific constants, it is treated as a variable or string, and
- one of the following two forms applies.

`if(<variable>)`:

- True if given a variable that is defined to a value that is not a false constant.
- False otherwise, including if the variable is undefined.

`if(<string>)`:

- A quoted string always evaluates to false,
- unless the string's value is one of the true constants.

[Condition Syntax, Variable Expansion](https://cmake.org/cmake/help/latest/command/if.html):

The if command was written very early in CMake's history, predating the `${}` variable evaluation syntax, and for convenience evaluates variables named by its arguments as shown in the above signatures. Note that normal variable evaluation with `${}` applies before the if command even receives the arguments. Therefore code like

```
set(var1 OFF)
set(var2 "var1")
if(${var2})
```

appears to the `if` command as

```
if(var1)
```

and is evaluated according to the `if(<variable>)` case. The result is `OFF` which is false.

However, if we remove the `${}` from the example then the command sees

```
if(var2)
```

which is true because `var2` is defined to `"var1"` which is not a false constant.

