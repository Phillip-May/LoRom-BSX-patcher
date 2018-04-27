import sys
import struct
assert sys.version_info >= (3,0)

import argparse
import codecs
import os

parser = argparse.ArgumentParser(description='BSX satelaview romhacking checksum patcher')
#parser.add_argument("filename")
parser.add_argument('filename', help='The name/directory of the file being patched')
parser.add_argument('romtype',type=int, help='0 means LoROM 1 means HiROM')


args = parser.parse_args()

name = args.filename
romtype = args.romtype
rawfile = open("%s" % name,'r+b')

#print ("your input:")
print (args.filename)

filesize = os.path.getsize(name)
print (filesize)

#This will mean it skips nothing
skipcondition = 1

#LoRom
#if (romtype == 0):
#	skipcondition = "count < 0x07FB0 or count > 0x07FDF"
	

#HiROM
#if (romtype == 1):
#	skipcondition = "count < 0x07FB0 or count > 0x07FDF"
#I tried to just pass a condition to the function but python is really slow and
# made it take twice as much as a long if statement	

#This is needed because of another bit of weird behavior in python
def twos_comp(val, bits):
    """compute the 2's complement of int value val"""
    if (val & (1 << (bits - 1))) != 0: # if sign bit is set e.g., 8bit: 128-255
        val = val - (1 << bits)        # compute negative value
    return val                         # return positive value as is

#Grab size of rom from header
#This is kind of complicated and python hates binary math sort
#just input this one manually

filesize = 0x00100000
pos = 2
count = 0
checksumtotal = 0

while (count < filesize):
	#only add numbers not part of the header
	if ( (romtype == 0 and (count < 0x07FB0 or count > 0x07FDF)) or (romtype == 1 and (count < 0x07FB0 or count > 0x07FDF)) ):
		#parameter here is what offset to seek to
		rawfile.seek(count,0)
		#parameter defines how many bytes to read
		currentvalue = rawfile.read(1)
		outputresult = 0
		outputresult = codecs.getencoder('hex_codec')(currentvalue)[0]
		checksumtotal = checksumtotal + int.from_bytes(currentvalue,byteorder='little')
		#print (outputresult)
	count = count + 1

#cast the result to an unsigned integer
struct.pack('I',checksumtotal)
#truncate to 16 bit and convert to hex string in one operation
checksumoutput = hex(checksumtotal & 0xffff)
inverse = hex(~checksumtotal & 0xffff)



checksumhighbyte = struct.pack('B',((checksumtotal & 0xff00) >> 8))
checksumelowbyte = struct.pack('B',(checksumtotal & 0x00ff))

print("Something is going wrong with this")
print(((checksumtotal & 0xff00) >> 8))
print((checksumtotal & 0x00ff))

invserverhighbyte = struct.pack('B',(((~checksumtotal & 0xffff) & 0xff00) >> 8))
invserverlowbyte = struct.pack('B',((~checksumtotal & 0xffff) & 0x00ff))


print ("Begining write cycle")
if (romtype == 0):
	print("LoROM")
	#checksum
	rawfile.seek(0x7FDF,0)
	rawfile.write(checksumhighbyte)
	rawfile.seek(0x7FDE,0)
	rawfile.write(checksumelowbyte)
	#inverse checksum
	rawfile.seek(0x7FDD,0)
	rawfile.write(invserverhighbyte)
	rawfile.seek(0x7FDC,0)
	rawfile.write(invserverlowbyte)


	
if (romtype == 1):
	print("HiROM")
	#checksum
	rawfile.seek(0xFFDF,0)
	rawfile.write(checksumhighbyte)
	rawfile.seek(0xFFDE,0)
	rawfile.write(checksumelowbyte)
	#inverse checksum
	rawfile.seek(0xFFDD,0)
	rawfile.write(invserverhighbyte)
	rawfile.seek(0xFFDC,0)
	rawfile.write(invserverlowbyte)

print ("total")
print (checksumoutput)

print ("checksum high")
print (hex((checksumtotal & 0xff00) >> 8))

print ("cehcksum low")
print (hex(checksumtotal & 0x00ff))




print ("inverse")
print (inverse)

#print (currentvalue)
#print (outputresult)









