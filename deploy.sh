#!/bin/bash
#ssh-keyscan -H 172.24.116.8 >> ~/.ssh/known_hosts
scp ./cat/s21_cat johannec@172.24.116.8:/usr/local/bin
scp ./grep/s21_grep johannec@172.24.116.8:/usr/local/bin

