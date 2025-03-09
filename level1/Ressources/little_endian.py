#!/usr/bin/env python3
import sys
import struct
import os

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 script.py <hex_address>")
        sys.exit(1)

    addr_str = sys.argv[1]
    
    # Remove '0x' or '0X' prefix if present.
    if addr_str.startswith("0x") or addr_str.startswith("0X"):
        addr_str = addr_str[2:]
    
    # Ensure the address is 8 hex characters (4 bytes)
    if len(addr_str) != 8:
        print("Address must be exactly 8 hex digits.")
        sys.exit(1)
    
    try:
        addr_val = int(addr_str, 16)
    except ValueError:
        print("Invalid hexadecimal address.")
        sys.exit(1)
    
    # Pack the integer in little-endian format (4 bytes)
    packed = struct.pack("<I", addr_val)
    
    # Format each byte as an escaped hex string
    output = "".join("\\x{:02x}".format(b) for b in packed)
    os.system("echo -n '%s' | xclip -selection clipboard" % output)
    print(output)

if __name__ == "__main__":
    main()
