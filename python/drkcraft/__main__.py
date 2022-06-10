import sys
from drkcraft.main import main

argv = [ 'drkcraft' ] + sys.argv[1:]
sys.exit(main(argv))
