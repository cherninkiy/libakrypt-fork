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
#
##########################################################################
echo "Тест первый: фомируем базу и проверяем контрольные суммы"
run "i -r *.sh --database db.streebog256"
echo
run "i -v --database db.streebog256 --dont-show-stat"
rm -f db.streebog256

#
##########################################################################
echo "Тест второй: используем явно указанный алгоритм хеширования"
run "i -r *.sh -a streebog512 --format linux"
run "i -v -a streebog512 --format linux"
run "i --clean"
echo
run "i -r *.sh -a crc64 --tag"
run "i -v -a crc64 --format bsd --dont-show-stat"
run "i --clean"

##########################################################################
echo "Удаляем временные файлы"
rm cat/*.sh
rmdir -f cat
echo "Тест пройден"
exit
#
echo; echo "Тестируем алгоритмы hmac"
run "k -nt hmac-streebog256 -o hmac256.key --outpass 132a"
echo;
run "i --key hmac256.key --inpass 132a . -d result.hmac-streebog256"
run "i --list -d result.hmac-streebog256"
run "i -v result.hmac-streebog256 --key hmac256.key --inpass 132a"
echo
exit

#
#
run "k -nt hmac-streebog512 -o hmac512.key --outpass 132a"
echo;
run "i --key hmac512.key --inpass 132a * -o result.hmac-streebog512"
cat result.hmac-streebog512
run "i -c result.hmac-streebog512 --key hmac512.key --inpass 132a"
echo;
#
#
echo; echo "Тестируем алгоритмы выработки имитовставки с использованием шифра Магма"
run "k -nt magma -o magma.key --outpass 123"
run "i --key magma.key -m cmac-magma --inpass 123 * -o result.magma"
echo
cat result.magma
run "i -c result.magma --key magma.key -m cmac-magma --inpass 123"
echo
#
#
echo; echo "Тестируем алгоритмы выработки имитовставки с использованием шифра Кузнечик"
run "k -nt kuznechik -o kuznechik.key --outpass 123"
run "i --key kuznechik.key -m cmac-kuznechik --inpass 123 --tag * -o result.kuznechik"
echo
cat result.kuznechik
run "i -c result.kuznechik --key kuznechik.key -m cmac-kuznechik --inpass 123 --dont-show-stat"
#
#
rm -f magma.key kuznechik.key hmac256.key hmac512.key
rm -f result.*
echo "Тест пройден"
