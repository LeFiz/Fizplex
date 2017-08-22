from subprocess import call
from termcolor import colored

print colored("\nCppCheck ...\n", 'blue')
call(['cppcheck', '--enable=all', '--inconclusive', 'src'])

print colored("\nMake clean ...\n", 'blue')
call(['make', 'clean'])

print colored("Make ...\n", 'blue')
call(['make'])

print colored("Make run ...\n", 'blue')
call(['make', 'run'])
