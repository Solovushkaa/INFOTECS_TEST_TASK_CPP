# Logger

A project implementing a library for logging messages with varying severity levels and an application demonstrating the library's functionality.

User guide: <a href="#review">Documentation</a>

# Build

Make is used for the build.

The library and application build targets are located in the build directories.

Dependencies:
 - Make
 - C++17
 - STL

# Using make

All makefiles achieve the following goals:
 - all: The main purpose of the assembly.
 - test: Builds tests.
 - testrun: Run all tests.
 - clean: Deletes all temporary files.
 - testclean: Deletes all tests files.
 - distclean: Deletes all non-origin files.

# Review

The application has two startup parameters: a log file and a default logging level ('INFO', 'WARNING', 'ERROR'). If the application accepts one argument (the log file name), the logging level is set to 'INFO'.

Example:
```text
./logger <log.txt> <LogLevel>
```

The main menu of the application looks like this:

```text
+--------------------+
| To exit, press 'q' |
+--------------------+
1.Сreate a new log message
2.Change default log level
3.Clear log file
Enter to continue...
```

The meaning of menu items:
 - '1.Сreate a new log message' - сreates a new message in the log file.

After selecting an item, a menu appears where you need to enter a message for the log:
```text
Enter message: <Text>
```
After entering a message, a choice of logging level appears:
```text
Select log level:
1.INFO
2.WARNING
3.ERROR
4.Keep selected (INFO)
Enter to continue...
```

The log entry looks like this:
```text
[WARNING][2026-02-20, 00:15:00]: Text.
```

 - '2.Change default log level' - changes the default logging level.

After selecting an element, you will be able to select the default logging level:
```text
Select log level:
1.INFO
2.WARNING
3.ERROR
4.Keep selected (INFO)
Enter to continue...
```

 - '3.Clear log file' - clears the log file.

After selecting an item, a deletion confirmation appears:
```text
To clear the logging file, enter (y/Y) or enter (n/N) to cancel: <command>
```