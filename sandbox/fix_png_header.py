with open(filename, 'rb') as f:
    data = f.read()
data = bytearray(data)
png_header = [137, 80, 78, 71, 13, 10, 26, 10]
for i in range(8):
    data[i] = png_header[i]
with open(filename, 'wb') as f:
    f.write(data)
