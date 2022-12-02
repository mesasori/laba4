# Отчёт по лабараторной работе №4:

## Отчет аналитика: 

Принцип работы файловой системы:
Основной файл - users.txt, тут хранятся все пользователи в виде (int:id string:username int:amountOfFriends int:amountOfMessages string:registrationDate). При регистрации нового пользователя в файл users.txt добавляется этот пользователь в данном выше формате.

Второстепенные файлы #friends - friends\_id, в котором хранятся id пользователей, которые являются друзьями для пользователя с id(который в названии файла). После того, как, например, пользователь user1 c id=1 добавил в друзья пользователя user2 c id=2, в два файла с названиями: в friends\_1.txt добавится строка(2) и в friends\_2.txt строка(1). При удалении аналогичные строки удалятся из файлов.

Второстепенные файлы #messages - messagesid1\_id2, в котором во временном порядке хранятся сообщения в виде отправитель.id>>отправитель.message. Например: пользователь user1 id=1 отправил сообщение message=привет пользователю user2 id=2, тогда в файл с названием messages1_2 добавится строка 1>>привет.

Принцип работы приложения:
1) Пользователю предоставляется выбор - зарегистрироваться или войти в уже существующий аккаунт.

2) Пользователь вводит букву l/L - войти в систему(login) или s/S - зарегистрироваться(sign up), в случае введение отличных от этих букв, пользователю придется ввести букву снова.

2.l.1) При вводе l/L - пользователь должен ввести свой id и username, например 0 myusername; 
2.l.2) После ввода в консоль id и username'a система сверяет эти данные с данными всех пользователей в файле users.txt и, если такой пользователь существует, то вход произойдет успешно, иначе система предлагает вновь ввести id и username в данном выше формате.

2.s.1) При вводе s/S - пользователю предлагается ввести его будущий username.
2.s.2) После ввода username'a он сравнивается со всеми уже существующими и проверяется на уникальность. В случае уникальности пользователесь успешно регистрируется и ему выводятся id и username, чтобы он запомнил входные данные. Иначе же пользователю предлагается заново ввести username.

3) После входа в систему пользователю предоставляются следующие возможности:
	1. Show friends(Показать список друзей)
	2. Add friend(Добавить друга)
	3. Delete friend(Удалить друга)
	4. Write message(Написать сообщение)
	0. Log out(Выйти из системы)
Чтобы воспользоваться командами нужно ввести цифру 0 - 5, если будет введено что-либо другое, то приложение выведет ошибку и предложит заново ввести цифру.

3.1) Функция Show friends: обращается к файлу friends_id, и получает список с id друзей; Далее функция getUsersList() путем сравнения пользователей из users.txt и id друзей получает их username'ы а после выводит списком.

3.2) После нажатия на 2 программа предложит ввести username пользователя, которого вы хотите добавить в друзья. Username сравнивается с именами всех пользователей из вектора, который возвращает функция getUsersList() и в случае существования такого пользователя он будет добавлен в друзья и следовательно изменятся/создадутся файлы по принципу, что был описан в #friends. В случае, если пользователи уже являются друзьями ничего не изменится.

3.3) Обращается к файлу friends_id, и получает список с id друзей; Далее функция getUsersList() путем сравнения пользователей из users.txt и id друзей получает их username'ы а после выводит списком. Далее программа попросит ввод порядкового числа друга из списка. После ввода числа произойдет удаление другого пользователя из друзей и файлы изменятся по принципу, описанному в #friends.

3.4) Обращается к файлу friends_id, и получает список с id друзей; Далее функция getUsersList() путем сравнения пользователей из users.txt и id друзей получает их username'ы а после выводит списком. Далее программа попросит ввод порядкового числа друга из списка. После ввода числа пользователю будет доступно написание какого-угодно сообщения, а далее, нажав Enter, сообщение отправится по принципу, описанному в #messages.

3.0) Эта незамысловатая функция прекращает работу программы => пользователь выходит из акканута.

#### Соцсеть, запущенная с 2 разных ноутбуков должна работать исправно, а действия, выполненные на одном из устройств, должны быть синхронизированны с другим устройством: одновременное редактирование файлов, например, если зарегистрировался новый пользователь, то он должен быть добавлен в базу данных на обоих устройствах. Для выполнения данного задания использовать протокол TCP/IP.

Работа по тема №5
Нужно реализовать вход от имени администратора и дать ему возможность просматривать всю информацию о пользователе, в которую входит: количество друзей, сообщений, id, username и дата регистрации в соц.сети.

Реализация:
При входе, описанном в пункте 2.l.1, в аккаунт в случае, если пользователь введет id и username администратора, то выполнится вход под его именем. Данные для входа: 0 admin. Итак, у администратора нет функций добавлять/удалять друзей, писать сообщения, он может лишь смотреть список всех пользователей(списком выводятся username'ы всех зарегестрированных в системе пользователей) и просматривать подробную информацию о каждом пользователе.
Возможности администратора:
	1. Show users(Показать список пользователей)
	2. Check more info about user(Показать подробную информацию о пользователе)
	0. Log out(Выйти из системы)

1) Поочередно пронумерованным списком выводятся username'ы пользователей(помимо администратора) из файла users.txt

2) Поочередно пронумерованным списком выводятся username'ы пользователей(помимо администратора) из файла users.txt. После этого предлагается ввести число, под которым в списке стоит интересуемый администратора пользователь). Далее будет выведена подробная информация о выбранном пользователе в следющем виде: id username countOffriends countOfMessages registrationDate.

## Отчет программиста

Работа шла строго по разработанному вместе с аналитиком ТЗ, в процессе обдумывали и доробатывали окончательный вид соц.сети. 
Так, командой решили какие данные о пользователях будут храниться помимо id и username'a. Пришли к выводу, что достаточно хранить количество друзей, отправленных/полученных сообщений и дату регистрации. Вся информация хранится в основном файле users.txt, где, при добавлении/удалении друга меняется счетчик друзей, аналогично работае с сообщениями - при отправке/получении сообщения счетчик увеличивается.

Реализация всего по техническому заданию заняла ~5 часов. 
Около 3 из них потребовалось для написания всего функционала соцсети без использования протокола TCP/IP, то есть соцсеть работала только на одном устройстве. Следующий час был потрачен на изучение работы протокола TCP/IP, требований для успешной передачи данных между устройствами и обговаривание каким именно мы будем передавать данные с аналитиком. И на протяжении последнего часа мы с тестировщиком занимались реаилизацией самого протокола TCP/IP.

Реализация функционала соцсети:
Каких-либо проблем, помимо записи с новой строки в файлы не возникало, однако и эту проблемы удалось разрешить путем проб разных вариантов.

Реализация протокола TCP/IP:
Для изучения я прочитал статью на habr, а после пошел смотреть примеры реализации на github. Там, поискав пару неплохих, подходящих мне вариантов, я понял что делать и начал писать код. Изначально были проблемы с подключением клиента и сервера, но, засунув строчку с подключением в бесконечный цикл, я решил проблему. Ну а после осталось добавить немного кода в файлы клиента и сервера. Что касается первого, то это функции отправки сообщений на сервер о действиях пользователя - регистрация, добавление/удаление друзей, новые сообщения. А в коде сервера лишь добавить обработку этих сообщений и привязку к уже существующим функциям.


## Отчет тестировщика находится в файле tester.docx##

Тема №5<br>
Работу выполнили:<br>
Аналитик: ИУ1-12Б Исаев Тимур<br>
Программист: ИУ1-12Б Нигматуллин Руслан<br>
Тестировщик: ИУ1-12Б Фаталиев Джавид<br>


