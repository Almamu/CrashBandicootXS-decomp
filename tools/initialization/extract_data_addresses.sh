#!/bin/sh
grep "0x08" $1 | grep 4byte | cut -d' ' -f3 | sort | uniq > addresses.txt
php generate_data_addresses.php
sed -E -i 's/(4byte |@ =)0x08([0-9A-F]+)/\1gStaticData_08\2/i' $1