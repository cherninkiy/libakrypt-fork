#!/bin/bash
compilerList="gcc musl-gcc clang tcc"
buildDir="../build.all"-`uname`

# формируем каталог для проведения экспериментов
mkdir -p $buildDir
cd $buildDir
pwd

for name in $compilerList
do
        $name --version 2>> /dev/null
        if [ $? = 0 ];
        then
                echo "--------------------------------------------------------------------------------"
                mkdir -p $name.build
                cd $name.build

                cmake -DCMAKE_C_COMPILER=$name -DCMAKE_C_FLAGS="-march=native" -DAK_STATIC_LIB=ON -DAK_SHARED_LIB=ON -DAK_EXAMPLES=ON ../../libakrypt-0.x

# выполняем сборку
                make

# выполняем тестирование
# добавляем export LD_LIB... для указания tcc, где явно располагается только что собранная библиотека
                export LD_LIBRARY_PATH=./; make test

# выполняем проверку корректности криптографических тестов
#                ./aktool test --crypto --audit 2 --audit-file stderr

# выполняем запуск тестов в окружении valgrind
#                valgrind --version >> /dev/null 2>>/dev/null
#                if [ $? = 0 ];
#                then
#                    valgrind ./aktool test --crypto
#                fi

# не выполняем очистку созданных каталогов
                cd ..
        fi
done
cd ..
