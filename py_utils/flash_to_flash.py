'''
* @file flash_to_flash.py
 * @author Jamiexu (doxm@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2024-06-28
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
 

import serial
import struct
import tkinter

USART_FLASH_HEADER = 0xAD1991DA
USART_FLASH_END = 0xDA9119AD

USART_FLASH_CMD = 0x00
USART_FLASH_FLAG = 0x01
USART_FLASH_DATA = 0xFF

USART_FLASH_SHAKE_1 = 0xDF
USART_FLASH_SHAKE_2 = 0xFD
USART_FLASH_SHAKE_3 = 0x19
USART_FLASH_ACK = 0x00
USART_FLASH_NACK = 0xFF


USART_FLASH_CMD_FLASH_DATA = 0x01  # // Flash data request
USART_FLASH_CMD_PACK_DATA = 0x02  # // data package
USART_FLASH_CMD_READ_DATA = 0x03  # // read data from flash


W25Q16JV_PAGE_SIZE = 256
W25Q16JV_SECTOR_SIZE = 4096

ser = serial.Serial("COM7", 115200, timeout=20)
handshake_count = 1

# handshake


def send_cmd(cmd, *args, handshake=0, wait_ack=True):
    try:
        data = struct.pack("<IB", USART_FLASH_HEADER, USART_FLASH_CMD)
        if len(args) != 0:
            l = 1
            for i in args:
                l += len(i)
            data += struct.pack("<IB", l, cmd)
            for i in args:
                data += i
        else:
            data += struct.pack("<IB", 0x01, cmd)
        data += struct.pack("<I", USART_FLASH_END)
        print("send cmd:", data)
        ser.write(data)
        if not wait_ack:
            return None
        data2 = struct.unpack("<IBIBI", ser.read(14))
        # print("rev: ", data2)
        return data2[0] == USART_FLASH_HEADER and ((data2[1] == USART_FLASH_CMD) or (data2[1] == USART_FLASH_FLAG)) and data2[2] == 0x01 and data2[3] == (USART_FLASH_ACK if handshake == 0 else cmd) and data2[4] == USART_FLASH_END
    except Exception as e:
        print(e)
        return False


def send_flag(flag):
    data = struct.pack("<IBIBI", USART_FLASH_HEADER,
                       USART_FLASH_FLAG, 0x01, flag, USART_FLASH_END)
    ser.write(data)


def send_data(data: bytes, size):
    try:
        data2 = struct.pack("<IBI", USART_FLASH_HEADER, USART_FLASH_DATA,
                            size)
        data2 += data
        data2 += struct.pack("<I", USART_FLASH_END)
        # print("send data:", data)
        ser.write(data2)
        data3 = struct.unpack("<IBIBI", ser.read(14))
        # print("rev: ", data3)
        return data3[0] == USART_FLASH_HEADER and data3[1] == USART_FLASH_FLAG and data3[2] == 0x01 and data3[3] == USART_FLASH_ACK and data3[4] == USART_FLASH_END
    except Exception as e:
        print(e)
        return False


def write_file(file_path: str, addr: int):
    if addr % W25Q16JV_PAGE_SIZE != 0:
        print("The address must be aligned W25Q16JV_PAGE_SIZE")
        exit(-1)

    print("Erasing flash...")
    if not send_cmd(USART_FLASH_CMD_FLASH_DATA, struct.pack("<I", addr)):
        print("Flash command failed...")
        exit(-1)

    with open(file_path, 'rb') as f:
        data = f.read()

    print("Writing file...")
    print("File size: %d" % len(data))
    for i in range(0, len(data), 256):
        end = i + 256
        if end > len(data):
            end = len(data)
        print("Flash data[%d:%d] to SPI Flash..." % (i, end))
        if not send_data(data[i:end], 256):
            print("Flash fatal..")
            exit(-1)

    send_flag(USART_FLASH_NACK)


def read_to_file(file_path: str, addr: int, size: int):
    if addr % W25Q16JV_SECTOR_SIZE != 0:
        print("The address must be aligned W25Q16JV_SECTOR_SIZE")
        exit(-1)

    if not send_cmd(USART_FLASH_CMD_READ_DATA, struct.pack("<II", addr, size)):
        print("Read command failed...")
        exit(-1)

    file = open(file_path, "wb")
    # if not send_data(struct.pack("<II", addr, size), 8):
    #     print("Read fatal...")
    #     exit(-1)

    send_data(struct.pack("<II", addr, size), 8)  # dummy data
    # print(ser.read())
    while True:
        data = ser.read(9)
        print(data)
        # header = struct.unpack("<IBI", data[:9])
        # # end = struct.unpack("<I", data[-4:])
        # print([hex(i) for i in header], )
        break


if __name__ == "__main__":
    print("Start handshake...")
    while handshake_count < 4:
        if not send_cmd(USART_FLASH_SHAKE_1, handshake=1):
            print("Excepiton: Handshake 1, try %d handshake..." %
                  handshake_count)
            handshake_count += 1
            continue

        if not send_cmd(USART_FLASH_SHAKE_2, handshake=1):
            print("Excepiton: Handshake 2, try %d handshake..." %
                  handshake_count)
            handshake_count += 1
            continue

        if not send_cmd(USART_FLASH_SHAKE_3, handshake=1):
            print("Excepiton: Handshake 3, try %d handshake..." %
                  handshake_count)
            handshake_count += 1
            continue
        else:
            break

    if (handshake_count >= 4):
        print("Handshake failed...")
        exit(-1)

    print("Handshake successful...")
    print()
    print()

    write_file(r"output/merged_file.bin", 0x00)
    # read_to_file(
    #     r"C:\Users\Administrator\Desktop\A36Plus\Talkpod_read.bin", 0x00, 32768)

    print("Done!")
