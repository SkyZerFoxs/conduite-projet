#!/bin/bash

mkdir -p bin/
echo -e "\n------------------------  Executing test_map   ------------------------------"
./bin/test_map | tee -a bin/result_test.txt
echo -e "\n------------------------  Executing test_objet -----------------------------"
./bin/test_objet | tee -a bin/result_test.txt
echo -e "\n------------------------  Executing test_perso ----------------------------"
./bin/test_perso | tee -a bin/result_test.txt
echo -e "\n------------------------ Executing test_sprite ----------------------------"
./bin/test_sprite | tee -a bin/result_test.txt
echo -e "\n------------------------------------------------------"
