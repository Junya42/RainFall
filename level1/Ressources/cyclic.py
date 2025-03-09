#!/usr/bin/env python3
import sys
import os
import struct

def generate_cyclic_pattern(size):
    """
    Generate a cyclic pattern in the format:
    Aa0Aa1...Aa9Ab0Ab1...Ab9Ac0Ac1...Ac9...
    Each group consists of an uppercase letter, a lowercase letter, and a digit.
    The groups cycle through all two-letter combinations (lexicographically)
    with digits 0 to 9 appended.
    """
    pattern = ""
    # Iterate over uppercase letters for the first character
    for first in range(ord('A'), ord('Z') + 1):
        # For each uppercase letter, iterate over lowercase letters for the second character
        for second in range(ord('a'), ord('z') + 1):
            # For each two-letter combination, append digits 0 to 9
            for d in range(10):
                group = chr(first) + chr(second) + str(d)
                pattern += group
                if len(pattern) >= size:
                    return pattern[:size]
    return pattern[:size]

def find_offset(pattern, eip_hex):
    """
    Convert the provided EIP hex value to a 4-byte little-endian string,
    then find its offset in the cyclic pattern.
    """
    if eip_hex.startswith("0x") or eip_hex.startswith("0X"):
        eip_hex = eip_hex[2:]
    try:
        eip_val = int(eip_hex, 16)
    except ValueError:
        print("Invalid EIP hex value provided.")
        sys.exit(1)
    needle_bytes = struct.pack("<I", eip_val)
    # Use latin-1 so each byte maps 1:1 to a character.
    needle = needle_bytes.decode('latin-1')
    offset = pattern.find(needle)
    return offset, needle

def main():
    args = sys.argv[1:]
    if len(args) == 1:
        # Only pattern size provided: generate and print the pattern.
        try:
            pattern_size = int(args[0])
        except ValueError:
            print("Invalid pattern size.")
            sys.exit(1)
        pattern = generate_cyclic_pattern(pattern_size)
        os.system("echo -n '%s' | xclip -selection clipboard" % pattern)
        print(pattern)
    elif len(args) == 2:
        # Two parameters: pattern size, EIP value
        try:
            pattern_size = int(args[0])
        except ValueError:
            print("Invalid pattern size.")
            sys.exit(1)
        eip_value = args[1]
        pattern = generate_cyclic_pattern(pattern_size)
        offset, needle = find_offset(pattern, eip_value)
        if offset == -1:
            print("The needle '{}' was not found in the generated pattern.".format(needle))
            sys.exit(1)
        print("Needle '{}' found at offset: {}".format(needle, offset))
    else:
        print("Usage:")
        print("  {} <pattern_size>".format(sys.argv[0]))
        print("  {} <pattern_size> <EIP_value> <override_return_value>".format(sys.argv[0]))
        sys.exit(1)

if __name__ == "__main__":
    main()
    
    
void myfunction() {

    int a = 0;
    int b = 0;
    int c = 0;

    a += 1;
    b += 2;
    c += 3;
    
    int total = a + b + c;
    exit();
    printf("Total: %d\n", total);
}