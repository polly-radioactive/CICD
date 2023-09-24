#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"s test_0_grep.txt VAR"
)

declare -a extra=(
"-n for test_1_grep.txt test_2_grep.txt"
"-n for test_1_grep.txt"
"-n -e ^\} test_1_grep.txt"
"-ce ^int test_1_grep.txt test_2_grep.txt"
"-e"
"-ie INT test_5_grep.txt"
"-iv int test_5_grep.txt"
"-in int test_5_grep.txt"
"-ne ) test_5_grep.txt"
"-l for test_1_grep.txt test_2_grep.txt"
"-e int test_4_grep.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "$SUCCESS SUCCESS grep $t"
    else
      (( FAIL++ ))
      echo "$FAIL FAIL grep $t"
    fi
    rm test_s21_grep.log test_sys_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c l n h
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# # 2 параметра
# for var1 in v c l n h
# do
#     for var2 in v c l n h
#     do
#         if [ $var1 != $var2 ]
#         then
#             for i in "${tests[@]}"
#             do
#                 var="-$var1 -$var2"
#                 testing $i
#             done
#         fi
#     done
# done

# # 3 параметра
# for var1 in v c l n h
# do
#     for var2 in v c l n h
#     do
#         for var3 in v c l n h
#         do
#             if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
#             then
#                 for i in "${tests[@]}"
#                 do
#                     var="-$var1 -$var2 -$var3"
#                     testing $i
#                 done
#             fi
#         done
#     done
# done

# # 2 сдвоенных параметра
# for var1 in v c l n h
# do
#     for var2 in v c l n h
#     do
#         if [ $var1 != $var2 ]
#         then
#             for i in "${tests[@]}"
#             do
#                 var="-$var1$var2"
#                 testing $i
#             done
#         fi
#     done
# done

# # 3 строенных параметра
# for var1 in v c l n h
# do
#     for var2 in v c l n h
#     do
#         for var3 in v c l n h
#         do
#             if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
#             then
#                 for i in "${tests[@]}"
#                 do
#                     var="-$var1$var2$var3"
#                     testing $i
#                 done
#             fi
#         done
#     done
# done

echo "FAIL: $FAIL"
echo "SUCCESS: $SUCCESS"
echo "ALL: $COUNTER"
