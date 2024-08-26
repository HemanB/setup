"""
Test kit for executable, SPIM, and Logisim files.
Uses provided settings file to test assignment.
"""

import argparse
from common import Grader

def main():
    """
    Parse arguments and run autograder.
    """
    parser = argparse.ArgumentParser(description="Run autograder.")
    parser.add_argument("test_suite", type=str, default="ALL",
                        help="ALL, CLEAN, or TEST_SUITE_NAME")
    parser.add_argument("--settings", type=str, default="settings.json",
                        help="settings file to use for grading (default=settings.json)")

    args = parser.parse_args()

    grader = Grader(args.test_suite, args.settings)
    _ = grader.run()

if __name__ == "__main__":
    main()
