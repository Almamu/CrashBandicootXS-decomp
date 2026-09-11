<?php
    $contents = explode ("\n", file_get_contents ('addresses.txt'));
    $left = 0x7C4F3C;
    $start = 0x803B0C4;

    // parse numbers to integers
    for ($i = 0; $i < count ($contents); $i ++) {
        $contents[$i] = intval($contents[$i], 16);
    }
    // add the starting value if there's a gap
    if ($contents[0] - $start > 0) {
        array_unshift ($contents, $start);
    }
    // now get the length and store it in the array
    for ($i = 0; $i < count ($contents) - 1; $i ++) {
        $contents[$i] = [
            'start' => $contents[$i],
            'length' => $contents[$i + 1] - $contents[$i]
        ];
        $left -= $contents[$i]['length'];
    }
    // take the last one and calculate it based off the total length
    $last = count ($contents) - 1;
    $contents[$last] = [
        'start' => $contents[$last],
        'length' => $left
    ];
    
    // generate the output file now
    $fp = fopen ('data.s', 'w');

    fputs ($fp, ".section .rodata\n\n");
    
    foreach ($contents as $entry) {
        $strval = sprintf ('%08X', $entry ['start']);
        $file_address = sprintf ('%08X', $entry ['start'] - 0x08000000);
        $length = sprintf ('%08X', $entry ['length']);

        fprintf ($fp, ".global gStaticData_%s\n", $strval);
        fprintf ($fp, "gStaticData_%s:\n", $strval);
        fprintf ($fp, "\t.incbin \"baserom.gba\", 0x%s, 0x%s\n\n", $file_address, $length);
    }

    fclose ($fp);