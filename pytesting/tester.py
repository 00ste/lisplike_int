import os

# FILE PATHS
exe_path = 'C:\\code\\vs22_cpp\\lisplike_int\\x64\\Debug\\versione_0.exe'
test_path = 'C:\\code\\vs22_cpp\\lisplike_int\\pytesting\\scripts\\'
out_path = 'C:\\code\\vs22_cpp\\lisplike_int\\pytesting\\out\\'

def tester():
    # DETECT THE TEST SCRIPTS
    test_files = os.listdir(test_path)

    # DISPLAY THE TEST SCRIPT NAMES
    print('Found', len(test_files), "test scripts:")
    for filename in test_files:
        print("-", filename)
    
    # RUN THE SCRIPTS ON THE PROGRAM
    print('Beginning execution...')
    for filename in test_files:
        print('Do you want to run', filename, '? (Y/n)')
        user_input = input()
        if user_input == 'n':
            exit('Process terminated by the user.')
        if user_input == 'N':
            exit('Process terminated by the user.')
        os.system(exe_path + ' ' + filename)
    print("All scripts have been ran")
tester()