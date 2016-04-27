#!/bin/sh
# Copyright © 2016 George Georgiev. All rights reserved.

#
# Check if copyright statements include the current year
#

files=`git diff --cached --name-only`
year=`date +"%Y"`

for f in $files; do
    head -10 $f | grep -i copyright 2>&1 1>/dev/null || continue

    if ! grep -i -e "copyright.*$year" $f 2>&1 1>/dev/null; then
        missing_copyright_files="$missing_copyright_files $f"
    fi
done

if [ -n "$missing_copyright_files" ]; then
    echo "$year is missing in the copyright notice of the following files:"
    for f in $missing_copyright_files; do
        echo "    $f"
    done
    exit 1
fi
