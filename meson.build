project('turing-machine', 'c')

incdir = include_directories('include')
# c_files = run_command('c_src.sh', check: true).stdout().strip().split('\n')
c_files = [
    'src/instruction.c',
    'src/main.c',
    'src/tape.c',
    'src/turing.c',
    'src/utils.c',
    'src/state.c'
]
executable('turing', c_files, include_directories: incdir)