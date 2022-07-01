#!/usr/bin/env python

import sys
import re


class Concordance:

    def __init__(self, filename, exceptions):
        self.filename = filename
        self.exception_file = exceptions

    def _get_key(self, filelist, exception_list):
        key_list = []   # key_list contains all valid key words.
        for line in filelist:
            # .findall() obtained from "https://www.w3schools.com/python/python_regex.asp"
            # The following instruction splits a line into words and stores them in a list.
            new_line = re.findall(r"[a-z'-]+", line, re.IGNORECASE)
            for word in new_line:
                # check if the word is a valid keyword and not in key_list.
                if ((word.lower() not in exception_list) and (word.lower() not in key_list) and word != ''):
                    key_list.append(word.lower())
        key_list.sort()
        return key_list


    def _copy_file(self, file):
        file_list = []
        for line in file:
            new_line = ''
            new_line = line[:-1] # remove '\n' from the end of every line
            file_list.append(new_line)
        return file_list

    def _max_len(self, keywords_list): # _max_len() finds the length of the longest key word
        key_len = 0
        for key in keywords_list:
            if key_len < len(key):
                key_len = len(key)
        return key_len

    def _key_freq(self, line, key):
        freq = line.count(key)
        return freq

    def full_text(self):
        # copy exception_file to exception_list
        if (self.exception_file == None):
            exception_list = []
        else:
            excep_file = open(self.exception_file, 'r')
            exception_list = self._copy_file(excep_file)
            excep_file.close()

        # copy filename to filelist
        words_file = open(self.filename, 'r')
        filelist = self._copy_file(words_file)
        words_file.close()

        # extract keyword and save them to keywords_list
        keywords_list = self._get_key(filelist, exception_list)

        # get length of the longest key
        key_len = self._max_len(keywords_list)

        # print out final version text
        final_text = []
        for key in keywords_list:
            line_number = 0

            for line in filelist:
                # new_line contains all words that are going to be compared with the keyword.
                new_line = re.findall(r"[a-z'-]+", line, re.IGNORECASE)
                new_line = [x.lower() for x in new_line] # make every word in lower case

                if(key in new_line):
                    final_line = ''
                    if (self._key_freq(new_line, key) > 1): # if the keyword appears twice,
                                                            # the output will have symbol '*'.
                        y = '*'
                    else:
                        y = ''
                    final_line = key.upper() + (key_len+2-len(key)) * ' ' +  filelist[line_number] + " (%d%s)" % (line_number+1, y)
                    final_text.append(final_line)
                line_number += 1

        return final_text
