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
t.run()
t.print_results()
t.reset()

# newadd: test cat
t.add_test("/bin/echo hello world | /bin/cat | /bin/cat | /bin/cat", "hello world")
t.run()
t.print_results()
t.reset()

# newadd: test history
history = []
history_count = 0
for i in range(20):
    numberOfLines = i + 1
    test_command = "history {} | /bin/cat".format(numberOfLines)
    if history_count == 0 or test_command != history[history_count - 1]:
        history.append("{}".format(test_command))
        history_count += 1
    answer = ""
    if numberOfLines > 10:
        numberOfLines = 10
    start = max(0, history_count - numberOfLines)
    for i in range(start, history_count):
        if i != start:
            answer += "{:5d}  {}".format(i + 1, history[i]) 
        else:
            answer += "{}  {}".format(i + 1, history[i])
        if i != history_count - 1:
            answer += "\n"
            
    #print(answer)
    t.add_test(test_command, answer)
t.run()
t.print_results()
t.reset()

# newadd: errors in pipeline
t.add_test("/bin/echo hi | /bin/cat | history aaa | /bin/echo hi | /bin/cat", GENERAL_ERROR)
t.run()
t.print_results()
t.reset()


############################# CaiCai ##########################
# test for error
t.add_test("cd", GENERAL_ERROR)
t.add_test("cd haha", GENERAL_ERROR)
t.add_test("history -a", GENERAL_ERROR)
t.add_test("history -9527", GENERAL_ERROR)
t.add_test("history 66a", GENERAL_ERROR)
t.add_test("haha", GENERAL_ERROR)
t.add_test("history -c", "")
t.add_test("history 66", "{:5d}  {}".format(1, "history 66"))
t.run()
t.print_results()
t.reset()

test_command = "/bin/echo haha"
for j in range(20):
    test_command += " | /bin/cat"
t.add_test(test_command, "haha")

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
t.run()
t.print_results()
t.reset()

t.run()
t.print_results()
t.reset()

