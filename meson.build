project('turing-machine', 'c')

incdir = include_directories('include')
c_files = run_command('c_src.sh', check: true).stdout().strip().split('\n')
executable('turing', c_files, include_directories: incdir)