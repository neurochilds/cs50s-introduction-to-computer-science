from pyfiglet import Figlet
import sys
import random

figlet = Figlet()
fonts = figlet.getFonts()
font = fonts[random.randint(0, len(fonts))]
args = sys.argv

if len(args) > 1 and len(args) != 3:
    sys.exit(1)

if len(args) == 3:
    if args[1] not in ['-f', '--font'] or args[2] not in fonts:
        sys.exit(1)
    font = args[2]

figlet.setFont(font=font)
s = input("Your Text: ")
print(figlet.renderText(s))