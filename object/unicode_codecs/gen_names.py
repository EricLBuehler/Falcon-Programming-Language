# -*- coding: utf-8 -*-
"""
Created on Thu Nov 24 18:32:45 2022

@author: Eric Buehler
"""
import unicodedata
import os

os.chdir("C:\\Users\\Eric Buehler\\Falcon-Programming-Language\\fpl_v5\\object\\unicode_codecs")

header = "string _cp_names[] = {\n"
header_2 = "map<string, int> _cp_names_alt = {\n"
filename = "codepoint_names.h"

l = [
"NULL",
"START OF HEADING",
"START OF TEXT",
"END OF TEXT",
"END OF TRANSMISSION",
"ENQUIRY",
"ACKNOWLEDGE",
"BELL",
"BACKSPACE",
"HORIZONTAL TABULATION",
"LINE FEED",
"VERTICAL TABULATION",
"FORM FEED",
"CARRIAGE RETURN",
"SHIFT OUT",
"SHIFT IN",
"DATA LINK ESCAPE",
"DEVICE CONTROL ONE",
"DEVICE CONTROL TWO",
"DEVICE CONTROL THREE",
"DEVICE CONTROL FOUR",
"NEGATIVE ACKNOWLEDGE",
"SYNCHRONOUS IDLE",
"END OF TRANSMISSION BLOCK",
"CANCEL",
"END OF MEDIUM",
"SUBSTITUTE",
"ESCAPE",
"FILE SEPARATOR",
"GROUP SEPARATOR",
"RECORD SEPARATOR",
"UNIT SEPARATOR",
]

for n in range(0x20, 0x1fff):
    try:
        l.append(unicodedata.name(chr(n)))
    except ValueError:
        l.append("<control>")



with open(filename, "w") as f:
    f.write(header)
    for item in l:
        f.write(f"\t\"{item}\",\n")
    f.write("};\n\n")
    
    f.write(header_2)
    for i, item in enumerate(l):
        f.write("\t{\""+item+"\", "+str(i)+"},\n")
    f.write("};")
        
        
        