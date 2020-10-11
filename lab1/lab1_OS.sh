#!/bin/bash

if [ -n "$1" ]
then
case "$1" in
-h) echo "help"
echo "________________________"
echo "AUTHORS:
Goriunova D.
Eremin T.
Dultseva I."
echo "________________________"
echo "AVAILABLE OPTIONS:
  -q: list of all network interfaces
  -u: causes the interface to be activated
  -d: causes the interface to be deactivated
 -ip: setting IP address
  -m: setting mask
 -gw: setting gateway
  -k: killing a process running on particular port
-dip: causes the interface to be deactivated using ip pattern
 -ns: displaying network statistics" 
echo "________________________"
echo "PROJECT DESCRIPTION: 
• В проекте должен обрабатываться аргумент --help|-h для вывода справки, в которой отображаются:
    ° Авторы;
    ° Все доступные аргументы;
    ° Краткое описание проекта;
    ° Примеры запуска.
• Проект обязательно должен содержать следующий функционал (на хорошо):
    ° Вывод всех сетевых интерфейсов;
    ° Включение/отключение заданных интерфейсов (в т.ч. сразу нескольких);
    ° Установка IP/Mask/Gateway для определенного интерфейса.
• Проект должен содержать два или более любых возможностей (на отлично):
    ° Убийство процесса по занимаемому порту;
    ° Отключение сетевого интерфейса по шаблону ip;
    ° Построение карты сети;
    ° Отображение сетевой статистики. "
echo "________________________"
echo "EXAMPLES OF STARTING:
  -q: ./lab1 -q
  -u: ./lab1 -u eth0
  -d: ./lab1 -d eth0 
 -ip: ./lab1 -ip eth0 172.18.52.181
  -m: ./lab1 -m eth0 255.255.0.0
 -gw: ./lab1 -gw 172.18.52.176
  -k: ./lab1 -k 8
-dip: ./lab1 -dip eth0
 -ns: ./lab1 -ns"
;;
--help) echo "HELP" ;;
-q) ifconfig -a ;;
-u)
#echo "enter interface name to activate"
while [ "$2" ]
    do sudo ifconfig "$2" up
    shift
    done
;;
-d) 
#echo "enter interface name to deactivate"
while [ "$2" ]
    do sudo ifconfig "$2" down
    shift
    done
;;
-ip) sudo ifconfig "$2" "$3" ;;
-m) sudo ifconfig "$2" netmask "$3" ;;
-gw) #172.18.0.0
sudo route add default gw "$2" ;;
-kh) ps ux ;;
-k)
while [ "$2" ]
    do kill -9 "$2"
    shift
    done
;;
-dip) echo "enter ip: "
read ip
w=$(ifconfig -a | grep -B3 "$ip" | grep ":" | cut -f 1 -d ':')
sudo ifconfig $w down
;;
-ns) netstat -l ;;
*) echo "$1 is not an option" ;;
esac
else
echo "option not found, use -h for help"
fi
