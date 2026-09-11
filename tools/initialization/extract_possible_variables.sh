#!/bin/sh
grep "4byte" $1 | grep 0x03 | cut -d' ' -f3 | sort | uniq | cut -d'x' -f2 | xargs printf '. = 0x%1$s; gUnknown_%1$s = .;\n' > sym_iwram.txt
sed -i 's/0x03/0x00/' sym_iwram.txt
sed -E -i 's/(4byte |@ =)0x03([0-9A-F]+)/\1gUnknown_03\2/i' $1