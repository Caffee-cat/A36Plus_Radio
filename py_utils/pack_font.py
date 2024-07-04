# font = [chr(i) for i in range(0x20, 0x7E)]
# print(len(font))
# print(''.join(font))


import os


def merge_files(addr, target_file, files_to_merge):
    with open(target_file, 'wb') as target:
        # target.write(b'\xFF')
        # target.write(bytearray([0xFF] * 3))
        
        current_offset = addr

        target.write(bytearray([0xFF] * addr))
        for file_path in files_to_merge:
            print("file: %s, offset: 0x%02x" % (file_path, current_offset))

            with open(file_path, 'rb') as f:
                data = f.read()
                size = len(data)
                target.write(data)

            # align 16
            while ((current_offset + size) % 16) != 0:
                size += 1
                target.write(b'\xFF')

            # reserve
            size += 0x10 * 5
            target.write(bytearray([0xFF] * 5 * 0x10))

            # break
            current_offset += size



if __name__ == '__main__':
    target_file = r'output/merged_file.bin'
    files_to_merge = [
        'font/splash.bin',
        'font/font_en_8x16.fon',
        'font/font_en_8x16_blod.fon',
        'font/font_cn_16x16.fon',
        'font/font_cn_16x16_blod.fon'
    ]

    merge_files(0x7D000, target_file, files_to_merge)
    print(f'Merged files into {target_file}')
