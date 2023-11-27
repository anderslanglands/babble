import os
import argparse
import sys
import difflib
import shutil
import platform

parser = argparse.ArgumentParser()
parser.add_argument('-v', '--verbose', action='store_true', help='increase verbosity')
args = parser.parse_args()

TESTS = [
    "test001",
    "test002",
    "test003",
    "test004",
    "test005",
    "test006",
    "test007",
    "test008",
    "test009",
    "test011",
    "test012",
    "test013",
    "test014",
    "test015",
    "test016",
    "test017",
    "test018",
    "test019",
    "test021",
    "test022",
    "test023",
    "test024",
    # "test025a",
    # "test025b",
    "test026",
    "test027",
    "test028",
    "test029",
    "test030",
    "test031",
]

TEST_OUTPUT_PATH = os.path.join("build", "test", "out")

if os.name == "nt":
    TEST_REF_PATH = os.path.join("test", "ref", "windows")
else:
    TEST_REF_PATH = os.path.join("test", "ref", "linux")

if os.path.isdir(TEST_OUTPUT_PATH):
    shutil.rmtree(TEST_OUTPUT_PATH)
os.makedirs(TEST_OUTPUT_PATH)

for test in TESTS:
    print(test)
    bindfile = f"{test}.cpp"
    bindfile_path = os.path.join("test", bindfile)
    exe_path = os.path.join(".", "build", "bbl", "bbl-translate")
    out_cpp_path = os.path.join(TEST_OUTPUT_PATH, f"{test}.cpp")
    out_h_path = os.path.join("build", "test", "out", f"{test}.h")

    cmd = f"{exe_path} {bindfile_path} -- --std=c++17 -Ibbl/include -idirafter build/include -- {test} -o {TEST_OUTPUT_PATH}"
    if args.verbose:
        print(f"# {cmd}")
    os.system(cmd)

failed_tests = []

for test in TESTS:
    tst_c_p = os.path.join(TEST_OUTPUT_PATH, f"{test}-c.cpp")
    tst_h_p = os.path.join(TEST_OUTPUT_PATH, f"{test}-c.cpp")
    ref_c_p = os.path.join(TEST_REF_PATH, f"{test}-c.cpp")
    ref_h_p = os.path.join(TEST_REF_PATH, f"{test}-c.cpp")

    try:
        tst_c_f = open(tst_c_p)
        tst_h_f = open(tst_h_p)
    except FileNotFoundError:
        failed_tests.append(test)
        continue
    tst_c = tst_c_f.readlines()
    tst_h = tst_h_f.readlines()

    try:
        ref_c_f = open(ref_c_p)
        ref_h_f = open(ref_h_p)
    except FileNotFoundError:
        print(f"no ref found for {test}:\n")
        sys.stdout.writelines(tst_c)
        print()
        sys.stdout.writelines(tst_h)
        print()
        tst_c_f.close()
        tst_h_f.close()
        continue

    ref_c = ref_c_f.readlines()
    ref_h = ref_h_f.readlines()

    diff_c = "".join(difflib.unified_diff(ref_c, tst_c, ref_c_p, tst_c_p))
    diff_h = "".join(difflib.unified_diff(ref_h, tst_h, ref_h_p, tst_h_p))

    if diff_c or diff_h:
        print(f"FAILED {test}")
        print(f"{len(diff_c)}")
        print(f"{len(diff_h)}")
        sys.stderr.writelines(diff_c)
        sys.stderr.writelines(diff_h)

        failed_tests.append(test)

    tst_c_f.close()
    tst_h_f.close()
    ref_c_f.close()
    ref_h_f.close()

print()
if len(failed_tests) == 0:
    print("PASS")
else:
    print("FAIL")
    print(f"{len(failed_tests)} failed tests out of {len(TESTS)}")
    for ft in failed_tests:
        print(f"    {ft}")
