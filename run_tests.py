import os
import sys
import difflib

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
]

TEST_OUTPUT_PATH = os.path.join("build", "test", "out")
TEST_REF_PATH = os.path.join("test", "ref")
os.makedirs(TEST_OUTPUT_PATH, exist_ok=True)

for test in TESTS:
    print(test)
    bindfile = f"{test}.cpp"
    bindfile_path = os.path.join("test", bindfile)
    exe_path = os.path.join(".", "build", "bbl", "bbl-translate")
    out_cpp_path = os.path.join(TEST_OUTPUT_PATH, f"{test}.cpp")
    out_h_path = os.path.join("build", "test", "out", f"{test}.h")

    os.system(f"{exe_path} {bindfile_path} -- --std=c++17 -Ibbl/include -- {out_cpp_path} {out_h_path}")

failed_tests = []

for test in TESTS:
    tst_c_p = os.path.join(TEST_OUTPUT_PATH, f"{test}.cpp")
    tst_h_p = os.path.join(TEST_OUTPUT_PATH, f"{test}.cpp")
    ref_c_p = os.path.join(TEST_REF_PATH, f"{test}.cpp")
    ref_h_p = os.path.join(TEST_REF_PATH, f"{test}.cpp")

    tst_c_f = open(tst_c_p)
    tst_h_f = open(tst_h_p)
    ref_c_f = open(ref_c_p)
    ref_h_f = open(ref_h_p)

    tst_c = tst_c_f.readlines()
    tst_h = tst_h_f.readlines()
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
