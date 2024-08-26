"""
Common code between the autograder and test kit.
"""

import os
import re
import json
import shutil
import subprocess
from itertools import zip_longest

# Set test case timeout to 10 seconds
TEST_CASE_TIMEOUT = 10

VALID_TEST_MODES = ["exe"]
VALID_DIFF_TYPES = ["normal", "float", "file"]

class TextColors:
    """
    Colors for use in print.
    """
    GREEN = "\033[0;32m"
    RED = "\033[0;31m"
    END = "\033[0m"

class TestOutput:
    """
    Test case output and metadata.
    """
    def __init__(self,
                 name,
                 score,
                 max_score,
                 diff,
                 actual,
                 exit_status_non_zero,
                 exit_status_non_zero_penalty,
                 valgrind_memory_error,
                 valgrind_memory_error_penalty,
                 is_valgrind,
                 is_segfault,
                 visibility):
        self.name = name
        self.score = score
        self.max_score = max_score
        self.diff = diff
        self.actual = actual
        self.exit_status_non_zero = exit_status_non_zero
        self.exit_status_non_zero_penalty = exit_status_non_zero_penalty
        self.valgrind_memory_error = valgrind_memory_error
        self.valgrind_memory_error_penalty = valgrind_memory_error_penalty
        self.is_valgrind = is_valgrind
        self.is_segfault = is_segfault
        self.visibility = visibility

    def to_dictionary(self):
        """
        Convert test output to dictionary for json output.
        """
        output = ""
        if self.exit_status_non_zero:
            penalty = "no penalty applied"
            if self.exit_status_non_zero_penalty < 1:
                penalty = "{}% penalty applied".format((1 - self.exit_status_non_zero_penalty) * 100)
            output += "Exit status non zero! ({})\n".format(penalty)
        if self.valgrind_memory_error and self.is_valgrind:
            if self.valgrind_memory_error_penalty < 1:
                penalty = "{}% penalty applied".format((1 - self.valgrind_memory_error_penalty) * 100)
            output += "Valgrind memory error detected! ({})\n".format(penalty)

        if self.is_segfault:
            output += "Segfault detected!\n"

        if self.diff:
            output += "###### DIFF ######\n"
            output += self.diff
            output += "###### ACTUAL ######\n"
            output += self.actual

        return {
            "name": self.name,
            "score": self.score,
            "max_score": self.max_score,
            "output": output,
            "visibility": self.visibility
        }

class Grader():
    """
    Grade code and circuits.
    """
    def __init__(self, test_suite, settings_file):
        self.test_suite = test_suite

        with open(settings_file, "r") as sfile:
            setting = json.load(sfile)

        self.test_dir = setting["test_dir"]

        self.mode = setting["mode"]
        assert self.mode in VALID_TEST_MODES, "Invalid test mode."

        try:
            self.force_suite_filename = setting["force_suite_filename"]
        except KeyError:
            self.force_suite_filename = ""

        self.test_suite_names = setting["test_suite_names"]
        self.non_zero_exit_status_penalty = setting["non_zero_exit_status_penalty"]
        self.memory_penalty = setting["memory_penalty"]
        self.test_suites = setting["test_suites"]

    def run(self):
        """
        Run the autograder.
        """
        self.clean()

        test_outputs = []
        total_score = 0

        if self.test_suite == "ALL":
            for test_suite_name in self.test_suite_names:
                test_outputs, total_score = self.run_test_suite(test_suite_name, test_outputs, total_score)
        elif self.test_suite == "CLEAN":
            self.clean()
        elif self.test_suite in self.test_suite_names:
            test_outputs, total_score = self.run_test_suite(self.test_suite, test_outputs, total_score)
        else:
            raise Exception("Invalid test provided: {}".format(self.test_suite))

        return test_outputs, total_score

    def run_test_suite(self, test_suite_name, test_outputs, total_score):
        """
        Run a test suite.
        """
        print("Running tests for {}...".format(test_suite_name))

        for test_num, test_case in enumerate(self.test_suites[test_suite_name]):
            test_output, points = self.run_test_case(test_suite_name, test_case, test_num)
            test_outputs.append(test_output)
            total_score += points

        print("Done running tests for {}.\n".format(test_suite_name))

        return test_outputs, total_score

    def run_test_case(self, test_suite_name, test_case, test_num):
        """
        Run a specific test case.
        """
        description = test_case["desc"]
        args = test_case["args"]
        is_valgrind = test_case["valgrind"]
        diff_type = test_case["diff"]
        infile = test_case["infile"]
        outfile = test_case["outfile"]

        max_points = 0
        display_points = False
        if "points" in test_case:
            max_points = test_case["points"]
            display_points = True

        visibility = "visible"
        if "visibility" in test_case:
            visibility = test_case["visibility"]

        try:
            is_pass, exit_status_non_zero, memory_error, diff, actual, is_segfault = self.execute_test(test_suite_name,
                                                                                                   test_num, args,
                                                                                                   is_valgrind,
                                                                                                   diff_type,
                                                                                                   infile,
                                                                                                   outfile)
        except Exception:
            is_pass = False
            exit_status_non_zero = False
            memory_error = False
            diff = "Test Execution Crash. Please report this problem to the course staff"
            actual = "Test Execution Crash. Please report this problem to the course staff"
            is_segfault = False

        points = max_points
        if exit_status_non_zero:
            points = points * self.non_zero_exit_status_penalty
        if memory_error:
            points = points * self.memory_penalty

        test_output = TestOutput("%s test %d: %s" % (test_suite_name, test_num, description),
                                 points if is_pass else 0,
                                 max_points,
                                 diff,
                                 actual,
                                 exit_status_non_zero,
                                 self.non_zero_exit_status_penalty,
                                 memory_error,
                                 self.memory_penalty,
                                 is_valgrind,
                                 is_segfault,
                                 visibility)

        def print_result(test_id, description, status, errors, score=None):
            if score:
                print("%-10s %-50s %-20s %-15s %-45s" % (test_id, description, status, score, errors))
            else:
                print("%-10s %-50s %-20s %-45s" % (test_id, description, status, errors))

        status = TextColors.RED + "Failed" + TextColors.END
        if is_pass:
            status = TextColors.GREEN + "Pass" + TextColors.END

        score = None
        if display_points:
            score = "%0.2f/%0.2f" % ((points if is_pass else 0), max_points)

        errors = ""
        if exit_status_non_zero:
            errors += TextColors.RED + "exit_status_non_zero" + TextColors.END + "  "
        if memory_error:
            errors += TextColors.RED + "valgrind_memory_error" + TextColors.END

        print_result("Test %d " % test_num,
                     "(%s):" % description,
                     status,
                     errors,
                     score)

        return test_output, points if is_pass else 0

    def execute_test(self, test_suite_name, test_num, args, is_valgrind, diff_type, infile, outfile):
        """
        Execute a test, get output and calculate score.
        """
        executable_file_name = None
        if self.force_suite_filename:
            executable_file_name = self.force_suite_filename
        elif self.mode == "exe":
            executable_file_name = test_suite_name 

        expected_output_filename = os.path.join(self.test_dir,
                                                "%s_expected_%d.txt" % (test_suite_name, test_num))
        actual_output_filename = os.path.join(self.test_dir,
                                              "%s_actual_%d.txt" % (test_suite_name, test_num))
        diff_filename = os.path.join(self.test_dir,
                                     "%s_diff_%d.txt" % (test_suite_name, test_num))

        input_file = None
        if self.mode == "exe":
            command = "./%s" % executable_file_name
            arguments = args
            if infile != "":
                input_file = open(infile,"r")

        output_file = open(actual_output_filename, "w+") 
        exit_status = self.run_process(command, arguments, output_file, input_file)
        exit_status_non_zero = exit_status != 0
        is_segfault = exit_status == -11

        if diff_type == "normal":
            is_pass = self.normal_diff(expected_output_filename, actual_output_filename, diff_filename)
        elif diff_type == "file":
            is_pass = self.file_diff(expected_output_filename, outfile, diff_filename)
        elif diff_type == "float":
            is_pass = self.float_diff(expected_output_filename, actual_output_filename, diff_filename)
        else:
            raise Exception("Invalid diff_type")

        memory_error = False
        if is_valgrind:
            command = "valgrind"
            arguments = [
                "-q",
                "--error-exitcode=88",
                "--show-reachable=yes",
                "--leak-check=full",
                "./%s" % executable_file_name,
            ] + args
            exit_status = self.run_process(command, arguments)
            if exit_status == 88:
                memory_error = True

        try:
            with open(diff_filename, 'r') as diff_file:
                diff = diff_file.read()
        except Exception:
            diff = 'Reading diff file failed. This may be because the diff file included non UTF-8 characters.'

        try:
            with open(actual_output_filename, 'r') as actual_file:
                actual = actual_file.read()
        except Exception:
            actual = 'Reading actual file failed.'

        return is_pass, exit_status_non_zero, memory_error, diff, actual, is_segfault

    def normal_diff(self, expected_output_filename, actual_output_filename, diff_filename):
        """
        Simple diff.
        """
        command = "diff"
        arguments = ["-bwB", expected_output_filename, actual_output_filename]
        output_file = open(diff_filename, "w+")
        exit_status = self.run_process(command, arguments, output_file)
        return exit_status == 0

    def file_diff(self, expected_output_filename, actual_output_filename, diff_filename):
        """
        Simple diff.
        """
        command = "diff"
        arguments = ["-bwB", expected_output_filename, actual_output_filename]
        output_file = open(diff_filename, "w+")
        exit_status = self.run_process(command, arguments, output_file)
        return exit_status == 0

    def float_diff(self, expected_output_filename, actual_output_filename, diff_filename, frac_delta=0.001):
        """
        Float diff with tolerance.
        """
        expected_output = open(expected_output_filename, "r")
        actual_output = open(actual_output_filename, "r")
        diff = open(diff_filename, "w")
        is_pass = True

        def line_match(line1, line2):
            if line1 == line2:
                return True
            match1 = re.match(r"(\w+)\s+([.\d]+)$", line1)
            if not match1:
                return False
            match2 = re.match(r"(\w+)\s+([.\d]+)$", line2)
            if not match2:
                return False
            key1 = match1.group(1)
            value1 = float(match1.group(2))
            key2 = match2.group(1)
            value2 = float(match2.group(2))
            if key1 != key2:
                return False
            if frac_delta is not None and abs(frac_difference(value1, value2)) > frac_delta:
                return False
            return True

        # Fraction difference (i.e. percent difference but not x100) between two numbers
        # Special cases: if a=b=0, returns 0.0, if b=0 & a!=b, returns 1.0
        def frac_difference(num1, num2):
            if num2 == 0:
                if num1 == 0:
                    return 0.0
                else:
                    return 1.0
            return num1/num2 - 1.0

        for line1, line2 in zip_longest(expected_output, actual_output, fillvalue=""):
            line1 = line1.rstrip()
            line2 = line2.rstrip()
            if not line_match(line1, line2):
                diff.write("< %s\n> %s\n" % (line1, line2))
                is_pass = False

        expected_output.close()
        actual_output.close()
        diff.close()

        return is_pass

    def run_process(self, command, arguments, output_file=None, input_file=None):
        """
        Execute a shell command and return exit code.
        command: string
        arguments: list of strings
        output_file: file to write output
        """
        try:
            if output_file is not None:
                return subprocess.check_call([command] + arguments,
                                             stdout=output_file,
                                             stderr=output_file,
                                             stdin=input_file,
                                             timeout=TEST_CASE_TIMEOUT,
                                             shell=False)
            return subprocess.check_call([command] + arguments,
                                         stdout=subprocess.DEVNULL,
                                         stderr=output_file,
                                         stdin=input_file,
                                         timeout=TEST_CASE_TIMEOUT,
                                         shell=False)
        except subprocess.CalledProcessError as exception:
            return exception.returncode
        except Exception as exception:
            return -1

    # Remove spim headers and colon-terminated prompts
    def spim_clean(self, filename):
        """
        Clean SPIM result to use with diff.
        """
        def apply_file_filter(custom_filter, filename, backup_filename=None):
            """
            Apply a file filter.
            """
            if backup_filename:
                shutil.copy(filename, backup_filename)
            fp_src = open(backup_filename, "r")
            fp_dst = open(filename, "w")
            for line in custom_filter(fp_src):
                fp_dst.write(line)
            fp_src.close()
            fp_dst.close()

    def clean(self):
        """
        Remove the output of running tests.
        """
        os.system("rm -f " +
                  os.path.join(self.test_dir, "*_actual_*.txt* ") +
                  os.path.join(self.test_dir, "*_diff_*.txt*"))
