#!/usr/bin/env python

import sys

def print_out(key, line_number, key_len, inp_list):
    if (inp_list[line_number].lower().split(' ').count(key) > 1):
        y = '*'
    else:
        y = ''
    print(key.upper() + (key_len+2-len(key)) * ' ' +  inp_list[line_number] + " (%d%s)" % (line_number+1, y))

def get_longest_key(key_list, key_len):
    for key in key_list:
        if key_len < len(key):
            key_len = len(key)
    return key_len

def final_output(key_list, inp_list, key_len):
    for key in key_list:
        line_number = 0
        for line in inp_list:
            if (key in line.lower().split(' ')): # line.lower().split(' ') == []
                print_out(key, line_number, key_len, inp_list)
            line_number += 1

def get_key(key_list, exc_list):
    new_key_list = []
    for line in key_list:
        for word in (line.split(' ')):
            if((word.lower() not in exc_list) and (word.lower() not in new_key_list) and word != ''):  # if (true) -> j is the key word
                new_key_list.append(word.lower())
            else:
                continue
    new_key_list.sort()
    return new_key_list

def copy_file(inp_file, inp_list):
    for line in inp_file:
        new_line = ''
        new_line = line[:-1]
        inp_list.append(new_line)


def main():
    inp_file_name = ''
    inp_list = []
    key_list = []
    exc_list = []
    key_len = 0
    if ('-e' in sys.argv):
        exc_file_name = sys.argv[sys.argv.index('-e') + 1]
        exc_file = open(exc_file_name, 'r')
        for i in sys.argv:          # i is type str
            if (i[0:2] == 'in'):
                inp_file_name = i
        inp_file = open(inp_file_name, 'r')
        copy_file(inp_file, inp_list)
        copy_file(exc_file, exc_list)
        inp_file.close()
        exc_file.close()
        key_list = inp_list[:]
        key_list = get_key(key_list, exc_list)
        key_len = get_longest_key(key_list, key_len)
        final_output(key_list, inp_list, key_len)

    
    else:
        for i in sys.argv:          # i is type str
            if (i[0:2] == 'in'):
                inp_file_name = i
        inp_file = open(inp_file_name, 'r')
        inp_list.append(inp_file.read())
        inp_file.close()
        if (inp_list[0] == ''):
            return 0
        inp_list[0] = inp_list[0][:-1]
        print_out(inp_list[0], 0, len(inp_list[0]), inp_list)


if __name__ == "__main__":
    main()
