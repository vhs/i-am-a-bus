from PIL import Image, ImageFont, ImageDraw
import numpy as np
import convert_bmp
import sys
assert (len(sys.argv) == 2), "Must specify text in quotes, and only that" 

font='VCR_OSD_MONO.ttf'
text=sys.argv[1]

font = ImageFont.truetype(font, 16) #load the font
size = font.getsize(text)  #calc the size of text in pixels
image = Image.new("1", (120, 16), 1)#create a b/w image
draw = ImageDraw.Draw(image)
draw.text(((120-size[0])/2-3,0),text,font=font)
print(convert_bmp.generate_sign_code(np.invert(np.array(image))),end='')