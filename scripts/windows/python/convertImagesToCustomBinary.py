from PIL import Image

piecesPath = "../../../assets/images/"
pieceNames = ["p", "k", "b", "r", "q", "k"]

colorLookup = {
    (0, 0, 0, 0):         0,
    (242, 242, 242, 255): 1,
    (13, 13, 13, 255):    2,
    (204, 204, 204, 255): 3
}
currentRowBinNum = 0;


for i in pieceNames:
    pieceImage = Image.open(piecesPath+i+".png")
    pieceImage = pieceImage.convert("RGBA")
    print("{\n",end='')
    for y in range(pieceImage.width):
        currentRowBinNum = 0;
        for x in range(pieceImage.height):
            currentPixelInCustomBin = colorLookup[pieceImage.getpixel((x, y))]
            currentRowBinNum |= currentPixelInCustomBin
            currentRowBinNum <<= 2 
        print(f"    0b{currentRowBinNum:064b},")
    print("},\n",end='')