Работа с сертификатами открытых ключей
=====================================

.. contents:: Содержание
   :depth: 3

Структуры, используемые при работе с сертификатами
-------------------------------------------------

Структура запроса на сертификат открытого ключа `ak_request` содержит следующие поля:

* `struct verifykey vkey` - открытый ключ

* `struct request_opts opts` - параметры запроса на сертификат

Структура сертификата открытого ключа `ak_certificate` содержит следующие поля:

* `struct verifykey vkey` - открытый ключ

* `struct certificate_opts opts` - параметры сертификата


Структура секретного ключа алгоритма выработки электронной подписи ГОСТ Р 34.10-2012 `ak_signkey` содержит следующие поля:

* `struct skey key` - структура секретного ключа

* `struct hash ctx` - структура бесключевой функции хеширования

* `ak_uint8 verifykey_number[32]` - номер открытого ключа, выработанного из данного секретного ключа


Создание сертификата из запроса на сертификат
---------------------------------------------

Функция:
::

  static ak_certificate aktool_key_verify_ca(ak_certificate ca_cert, const char *filename)

Данная функция позволяет считать сертификат ключа УЦ. Параметры функции:

* `ak_certificate ca_cert` - структура сертификата открытого ключа

* `const char *filename` - имя файла с сертификатом

Возвращаемое значение: структура сертификата открытого ключа `ak_certificate` или `NULL`.

Функция:
::

  int ak_request_import_from_file(ak_request req, const char *filename)

Данная функция позволяет импортировать запрос на сертификат. Параметры функции:

* `ak_request req` - структура запроса на сертификат открытого ключа

* `const char *filename` - имя файла с сертификатом

Возвращаемое значение: код ошибки типа `int`. Если функуция отработала успешно возвращается `ak_error_ok`.

Функция:
::

  int ak_certificate_export_to_file(ak_certificate subject_cert, ak_signkey issuer_skey, ak_certificate issuer_cert, ak_random generator, char *filename, const size_t size, export_format_t format)

Данная функция позволяет подписать и сохранить сертификат. Параметры функции:

* `ak_certificate subject_cert` - структура создаваемого сертификата открытого ключа

* `ak_signkey issuer_skey` - структура секретного ключа, с помощью которого подписывается создаваемый сертификат

* `ak_certificate issuer_cert` - структура сертификата открытого ключа, соответствующая секретному ключу подписи

* `ak_random generator` - структура генератора псевдо-случайных чисел, используемый для подписи сертификата

* `char *filename` - имя файла, в который будет экспортирован ключ

* `const size_t size` - размер области памяти, в которую будет помещено имя файла

* `export_format_t format` - формат, в котором сохраняются данные (`asn1_der_format` или `asn1_pem_format`)

Возвращаемое значение: код ошибки типа `int`. Если функция отработала успешно возвращается `ak_error_ok`.

Пример
~~~~~~

::

    struct certificate ca_cert;
    ak_certificate ca_cert_ptr = NULL;
    ak_request req;
    struct signkey issuer_skey;
    ak_random generator;
    ca_cert_ptr = aktool_key_verify_ca(&ca_cert, "example.crt") // считываем сертификат ключа УЦ
    ak_request_import_from_file(&req, "example2.crt") //импорт запроса на сертификат
    ...
    ak_certificate_export_to_file(&ca_cert, &issuer_skey, ca_cert_ptr, generator, // подпись запроса и сохранение сертификата
        "example3", 8, asn1_pem_format);
    ak_certificate_destroy( &ca_cert );

Создание p7b контейнера
-----------------------

Функция:
::

  ak_asn1 ak_certificate_new_p7b_skeleton( ak_asn1 *sequence )

Данная функция позволяет создать новый (пустой) p7b контейнер. Параметры функции:

* `ak_asn1 *sequence` - указатель, в который помещается ссылка на прелполагаемый список сертификатов

Возвращаемое значение: В случае успеха возвращается указатель на вершину созданного asn1 дерева или `NULL`.

Функция:
::

  int ak_asn1_import_from_file( ak_asn1 asn, const char *filename, export_format_t *format )

Данная функция позволяет импортировать ASN.1 дерево из файла, содержащего der-последовательность.

* `ak_asn1 asn` - уровень ASN.1, в который помещается считываемое значение

* `const char *filename` - имя файла, в котором содержится der-последовательность

* `export_format_t *format` - если указатель не равен NULL, то по даному адресу размещается формат считанных данных

Возвращаемое значение: код ошибки типа `int`. Если функция отработала успешно возвращается `ak_error_ok`.

Функция:
::

  int ak_certificate_import_from_asn1( ak_certificate subject_cert, ak_certificate issuer_cert, ak_asn1 root )

Данная функция позволяет импортировать открытый ключ асимметричного преобразования из сертификата открытого ключа, представленного в виде asn1 дерева. Параметры функции:

* `ak_certificate subject_cert` - контекст сертификата открытого ключа

* `ak_certificate issuer_cert` - контекст сертификата ключа проверки подписи

* `ak_asn1 root` - указатель на вершину asn1 дерева

Возвращаемое значение: 

Вспомогательные функции
-----------------------

Функция:
::

  int ak_certificate_destroy(ak_certificate cert)

Данная функция позволяет очистить поля структуры сертификата. Параметры функции:

* `ak_certificate cert` - структура сертификата открытого ключа

Возвращаемое значение: код ошибки типа `int`. Если функуция отработала успешно возвращается `ak_error_ok`.