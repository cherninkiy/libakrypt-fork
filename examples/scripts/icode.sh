##########################################################################
#! /bin/bash
# проверка различных  способов выработки и проверки контрольных сумм и имитовставок
#
akt=aktool
if [ -n "$1" ]
then
akt=$1
fi
#
run() {
$akt $1
if [[ $? -ne 0 ]]
then 
  echo "$akt не может выполнить $1"; exit;
fi
}

##########################################################################
# подготовительный этап
mkdir -p cat
cp *.sh cat
$akt --version
if [[ $? -ne 0 ]]
then 
  echo "используйте icode.sh <полнфй путь к aktool>"; exit;
fi
#
##########################################################################
echo
echo "Тест первый: фомируем базу и проверяем контрольные суммы"
run "i -r . --database db.streebog256"
echo
run "i -v --database db.streebog256 --dont-show-stat"
rm -f db.streebog256
#
#
##########################################################################
echo
echo "Тест второй: используем явно указанный алгоритм хеширования"
run "i -r . -a streebog512 --format linux"
run "i -v -a streebog512"
run "i --clean"
echo
run "i -r . -a crc64 --tag"
run "i -v -a crc64 --dont-show-stat"
run "i --clean"
#
#
##########################################################################
echo 
echo "Тест третий: используем алгоритмы hmac"
run "k -nt hmac-streebog256 -o hmac256.key --outpass 132a"
echo
run "i --key hmac256.key --inpass 132a . --dont-show-stat"
echo
run "i --list"
echo
run "i -v --key hmac256.key --inpass 132a --verbose"
rm hmac256.key
#
#
##########################################################################
echo
echo "Тест четвертый: используем hmac512"
run "k -nt hmac-streebog512 -o hmac512.key --outpass 132a"
echo
run "i --key hmac512.key --inpass 132a . --dont-show-stat --format bsd"
echo
run "i -v --key hmac512.key --inpass 132a --verbose"
rm hmac512.key
#
#
##########################################################################
echo
echo "Тест пятый: используем имитовставку ГОСТ Р 34.13-2015 с блочным шифром Магма"
run "k -nt magma -o magma.key --outpass 132x"
echo
run "i --key magma.key --inpass 132x . --dont-show-stat"
echo
run "i -v --key magma.key --inpass 132x"
rm magma.key
#
#
##########################################################################
echo
echo "Тест шестой: используем имитовставку ГОСТ Р 34.13-2015 с блочным шифром Кузнечик"
run "k -nt kuznechik -o kuznechik.key --outpass 132x"
echo
run "i --key kuznechik.key --inpass 132x . --dont-show-stat"
echo
run "i -v --key kuznechik.key --inpass 132x"
rm kuznechik.key
#
#
##########################################################################
echo
echo "Удаляем временные файлы"
rm cat/*.sh
rmdir --ignore-fail-on-non-empty cat
echo "Тест пройден"
exit

