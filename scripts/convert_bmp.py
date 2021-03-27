from PIL import Image
import numpy as np

def add_opcodes_and_seq_and_checksum_and_line_endings(values):
    ret_string=''
    for ii,i in enumerate(np.array(values).reshape((-1,32))): # Split up into 32 character strings for export.
        row=''
        row+='1000' # Insert OpCode
        row+=f'{ii<<12:04X}'# Insert the sequence value
        for j in i: # Loop through the passed in data
            row+=f'{j:X}' # Add each hex value one by one (each is a nibble or half byte)
        row+=f'{calc_checksum(row):02X}' # Add in the checksum (1 byte)
        row=':'+row # Add in the weird colon at the start of the row
        row+='\r\n' # Add the required CR/LF
        ret_string+=row
    return ret_string

def calc_checksum(seq):
    seq=np.array(list(seq))
    x=seq.reshape((-1,2)) # Reshape to pairs
    z=[]
    for i in x:
        z.append(int(''.join(i),16)) # Convert each pair into hex values
    return (256 - (sum(z) % 256))%256 # Sum all those values, subtract from 256

def print_array(array):
    for i in array:
        for j in i:
            if j:
                print('0', end='')
            else:
                print('.',end='')
        print()

def load_bmp_as_array(file_location, invert_zeros_ones=True):
    image_matrix = np.array(Image.open(file_location), dtype=bool)
    if invert_zeros_ones is True:
        image_matrix = np.invert(image_matrix)
    return image_matrix

def generate_sign_code(image_array):
    assert (image_array.shape == (16, 120)), "Image supplied is not the correct shape. Must be 120x16" # Make sure we have the right dimentions
    image_array=np.rot90(image_array,3) # Rotate so it's in the format we need.
    image_array=np.roll(image_array,8,1) # Roll along 1st axis to match format sign wants (This is nifty)
    image_array=image_array.reshape((-1,4)) # Reshape array to single hex symbol on first axis
    flat=[] #Flat list to track each int that represents 4 bits / LEDs
    header="01120000" # Start with the header that starts the data.
    for i in header:
        flat.append(int(i, 16)) # add in the header
    for i in image_array:
        flat.append(sum(a<<j for j,a in enumerate(np.flip(i)))) # Convert bin false/true to a digit. the "np.flip" is to switch to LSB from MSB
    footer = "FFFFFFFFFFFFFFFFFFFFFFFF"
    for i in footer:
        flat.append(int(i, 16)) # add in the footer (really just filling out the rest of the data, could probably be anything)
    sign_code = add_opcodes_and_seq_and_checksum_and_line_endings(flat) # Convert into format for sing, adding opcode, sequence, and checksums
    return sign_code

if __name__ == "__main__":
    import sys
    assert (len(sys.argv) == 2), "Must specify bitmap file to convert, and only that" 
    image_array = load_bmp_as_array(sys.argv[1])
    print(generate_sign_code(image_array))

