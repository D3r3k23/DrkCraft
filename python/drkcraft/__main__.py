from .main import main
import sys

argv = [ 'drkcraft' ] + sys.argv[1:]
sys.exit(main(argv))
