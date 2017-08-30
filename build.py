from subprocess import call
from termcolor import colored

print colored("\nClang-Format ...\n", 'blue')
call('clang-format-4.0 -i src/*.cc src/*.h', shell=True)

print colored("\nCppCheck ...\n", 'blue')
call(['cppcheck', '--enable=all', '--inconclusive', '--suppress=missingIncludeSystem', '--inline-suppr', '--template=gcc', '--quiet', 'src'])

print colored("\nMake clean ...\n", 'blue')
call(['make', 'clean'])

print colored("Make ...\n", 'blue')
call(['make'])

print colored("Make run ...\n", 'blue')
call(['make', 'run'])
