Если PostgreSQL работает на русском языке и русские символы на сайте отображаются криво, то переключите PostgreSQL на 
английский язык. Для этого открой файл конфигурации *postgresql.conf*, который по умолчанию расположен по пути *C:\Program Files\PostgreSQL\ __N__ \data\postgresql.conf*, где **N** - это номер версии PostgreSQL, установленного на ваш компьютер. 
В этом файле нужно заменить несколько параметров на следующие
> lc_messages = 'English_United States.1252'	# locale for system error message  
> lc_monetary = 'English_United States.1252'	# locale for monetary formatting  
> lc_numeric = 'English_United States.1252'	# locale for number formatting  
> lc_time = 'English_United States.1252'		# locale for time formatting  
> default_text_search_config = 'pg_catalog.english'
