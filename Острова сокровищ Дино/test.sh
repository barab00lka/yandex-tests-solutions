#!/bin/bash

for i in {1..9}; do
	 echo -n "Пример '$i', ответ: "
	 sed -n '/### Пример '$i'/,/\*\*Вывод\*\*/p' README.md | grep -v '\*\*' | grep -v '###' | ./a.out
	 echo 
done
