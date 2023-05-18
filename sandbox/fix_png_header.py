with open('./static/img/corrupt-chris.png', 'rb') as f:
    data = f.read()
data = bytearray(data)
png_header = [137, 80, 78, 71, 13, 10, 26, 10]
for i in range(8):
    data[i] = png_header[i]
with open('./static/img/user_output.png', 'wb') as f:
    f.write(data)
