import tester

GENERAL_ERROR = "[eE]rror:[\s\S]*"

t = tester.HomeworkTester()

"""Modify this file with your tests.

The test is already filled out with some basic tests.

Basically, your main usage is:

    t.add_test("command to execute 1", "expected output as a regex string")
    t.add_test("command to execute 2", "expected output as a regex string")
    ...
    t.add_test("command to execute 3", "expected output as a regex string")
    t.run()
    t.print_results()
    t.reset()
"""
##################### Basic Executables #########################
# ls should not be found
t.add_test("ls", GENERAL_ERROR)

# But /bin/echo should work
t.add_test("/bin/echo hello world", "hello world")

# Check if /bin/date works
t.add_test("/bin/date", "[A-Za-z]{3} [A-Za-z]{3} [ \d]{2} [\d]{2}:[\d]{2}:[\d]{2} [A-Za-z]{3} [\d]{4}")

# Check if /bin/ls (with full path) is found


t.run()
t.print_results()
t.reset()

############################# Builtins ##########################
# Test that cd works
t.add_test("cd /tmp", "")
t.add_test("/bin/pwd", "/tmp")
t.add_test("cd /var", "")
t.add_test("/bin/pwd", "/var")
t.add_test("cd", GENERAL_ERROR)

# Test that history works


# Test that exit works with a specific message
t.add_test("exit", "")

t.run()
t.print_results()
t.reset()

# Additional tests for piping
t.add_test("/bin/echo hello | /usr/bin/wc -w", "1")
t.add_test("/bin/ls | /usr/bin/wc -l", ".+")

t.run()
t.print_results()
t.reset()

