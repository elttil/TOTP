#!/bin/sh
file="./test_cases.txt"
i=0
while IFS= read -r line
do
	key="$(echo "$line" | awk -F ' ' '{print $1}')"
	time="$(echo "$line" | awk -F ' ' '{print $2}')"
	real_code="$(echo "$line" | awk -F ' ' '{print $3}')"

    digits=$(echo $real_code | wc -c)
    digits=$(($digits-1))

	output_code_arg="$(echo $key | ./totp -t $time -d $digits)"

	test $output_code_arg != $real_code && echo "ERROR: TOTP \
output does not match test case ($i)." && echo "TOTP Output: $output_code_arg" && exit
    i=$((i+1))
done <"$file"
echo "All $i tests passed"
