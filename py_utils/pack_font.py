
'''
* @file pack_font.py
 * @author Jamiexu (doxm@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2024-07-04
 *
 * @copyright MIT License

Copyright (c) 2024 (Jamiexu or Jamie793)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 *
'''

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
            size += 0x10 * 20
            target.write(bytearray([0xFF] * 20 * 0x10))

            # break
            current_offset += size
        target.seek(0, 2)

        print("pack ok...  file size: %d" % target.tell())


if __name__ == '__main__':
    target_file = r'output/merged_file.bin'
    files_to_merge = [
        'font/jamiexu_splash.bin',
        'font/font_en_8x16.fon',
        'font/font_en_8x16_blod.fon',
        'font/font_en_16x32.fon',
        'font/font_cn_16x16.fon',
    ]

    merge_files(0x7D000, target_file, files_to_merge)
    print(f'Merged files into {target_file}')
