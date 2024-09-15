from PIL import Image
import os

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565."""
    r = (r >> 3) & 0x1F
    g = (g >> 2) & 0x3F
    b = (b >> 3) & 0x1F
    return (r << 11) | (g << 5) | b

def split_rgb565(rgb565_value):
    """Split RGB565 value into two uint8_t values."""
    high_byte = (rgb565_value >> 8) & 0xFF
    low_byte = rgb565_value & 0xFF
    return high_byte, low_byte

def image_to_rgb565_array(image_path):
    """Convert image to RGB565 array split into a single uint8_t array."""
    with Image.open(image_path) as img:
        img = img.convert('RGB')
        width, height = img.size
        rgb565_bytes = []
        for y in range(height):
            for x in range(width):
                r, g, b = img.getpixel((x, y))
                rgb565_value = rgb888_to_rgb565(r, g, b)
                high_byte, low_byte = split_rgb565(rgb565_value)
                rgb565_bytes.append(f'0x{high_byte:02X}')
                rgb565_bytes.append(f'0x{low_byte:02X}')
        return width, height, rgb565_bytes

def save_c_array(filename, width, height, rgb565_bytes):
    """Save RGB565 bytes to a C file as a single uint8_t array."""
    with open(filename, 'w') as f:
        f.write('#include <stdint.h>\n\n')
        f.write(f'const uint8_t image_data[{len(rgb565_bytes)}] = {{\n')
        for i in range(0, len(rgb565_bytes), 16):
            f.write('    ' + ', '.join(rgb565_bytes[i:i+16]) + ',\n')
        f.write('};\n')

def main():
    for file_name in os.listdir('.'):
        if file_name.lower().endswith('.jpg'):
            print(f'Processing {file_name}...')
            width, height, rgb565_bytes = image_to_rgb565_array(file_name)
            c_filename = f'{os.path.splitext(file_name)[0]}.c'
            save_c_array(c_filename, width, height, rgb565_bytes)
            print(f'Saved {c_filename}')

if __name__ == '__main__':
    main()
