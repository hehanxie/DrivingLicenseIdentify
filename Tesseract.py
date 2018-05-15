import pytesseract
from PIL import Image

PATH = '/Users/whstarlit/Documents/Projects/Git/DrivingLicense/exampleImage/data/'

i = 1
result = ''
while i <= 18:
    image = Image.open(PATH + str(i) +'.jpg')
    text = pytesseract.image_to_string(image, lang = 'font', config='-psm 10 digits')
    if text == '':
        text = '1'
    # print (text)
    i = i + 1
    result = result + text

print(result)

f = open('/Users/whstarlit/Desktop/out.txt', 'w+')
f.write(result)
f.close()


