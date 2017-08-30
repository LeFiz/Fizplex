from subprocess import call
from time import time
from termcolor import colored

print colored("\nClang-Format ...\n", 'blue')
call('clang-format-4.0 -i src/*.cc src/*.h', shell=True)

print colored("CppCheck ...\n", 'blue')
call(['cppcheck', '--enable=all', '--inconclusive', '--suppress=missingIncludeSystem', '--inline-suppr', '--template=gcc', '--quiet', 'src'])

print colored("Make clean ...\n", 'blue')
call(['make', 'clean'])

start=time();
print colored("Make ...", 'blue')
call(['make'])
print "Make completed in ", time() - start, "s\n"

print colored("Make run ...\n", 'blue')
call(['make', 'run'])
