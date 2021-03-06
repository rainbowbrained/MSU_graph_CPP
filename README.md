# MSU_graph

Матрицы и прикладные задачи на графах
=====================

В данном задании реализованы следующие классы и методы работы с ними
1. DenseMatrix - плотная матрица;
2. SparseMatrix - разреженная матрица;
3. Matrix - единый интерфейс для работы с матрицами разного типа;
4. RowIterator и ColumnIterator - интерфейс, позволяющий итерироваться по элементам матрицы; 
5. Graph - граф, который представляется в виде матрицы смежности. В зависимости от задачи и плотности графа (количества рёбер) можно использовать различные реализации матрицы (разреженная или плотная). 

В классе Graph реализуются две прикладные задачи на графах:
● вычислить диаметр графа;
● вычислить количество связных компонент графа.

###### Концепция хранения матрицы смежности

1. SparseMatrix хранится в формате CSR. Доступ к элементу через двойные скобки и 
разыменование через прокси-классы. Подробнее в ReadMeSparse.txt;
2. DenseMatrix хранится в формате вектор векторов.

Класс Graph
=====================

### Информационные поля Graph

###### Matrix * AdjMat;
	Квадратная матрица смежности. Может иметь тип SparseMatrix либо DenseMatrix. Тип
	матрицы выбирается при создании графа
	
###### bool oriented;
	Флаг, обозначающий, ориентированный ли граф или нет.

###### bool isFilled (std::vector<bool> vec);
  Утилита, используется при реализации метода double Diameter().
	
###### size_t min_unvis (std::vector<double> vec, std::vector<bool> vis);
	Утилита, используется при реализации метода double Diameter().
	
###### double max_w (std::vector<double> vec, size_t cur);
	Утилита, используется при реализации метода double Diameter().
	
### Методы Graph

###### Graph (Matrix *);
	Конструктор. Создание графа с помощью квадртаной матрицы смежности. При попытке
	создать граф с помощью не квадратной матрицы генерируется исключение. При вызове
	конструктора автоматически вызывается метод isOriented(), определяющий, 
	является ли граф ориентированным (т.е. симметрична ли матрица относительно
	главной диагонали).
	
###### Graph (const Graph &);
	Конструктор копирования. 
	
###### Graph& operator=(Graph&);
	Обычное копирование.

###### void make_not_orient ();
	Из ориентированного графа делает неориентированный с помощью отражения ненулевых 
	элементов относительно главной диагонали. Если существует путь из одной вершины в
	другую, вес которого не равен весу пути из другой вершины в первую, то 
	предпочтение отдается весу, расположенному над главной диагональю. Пример:
	 \ 0 1 2       \ 0 1 2 
	0| 0 1 0      0| 0 1 0
	1| 3 0 1  =>  1| 1 0 1
	2| 0 0 0      2| 0 1 0

###### size_t Bonded_components();
	Функция, возвращающая количество связных компонент графа.
	! В реализации метода используются итераторы RowIterator, позволяющие 
	итерироваться по ненулевым элементам матрицы смежности.

###### bool isOriented ();
	Функция, возвращающая 1 если граф ориентированный (матрица смежности не 
	симметрична относительно главной диагонали), и 0, если граф ориентированный.
	
###### double Diameter ();
	Функция, возвращающая диаметр графа. При реализации использовался алгоритм
	Дейкстры. При попытке вычислить диаметр для несвязного, ориентированного графа 
	или графа, содержащего отрицательные веса путей, генерируется исключение.
	! В реализации метода используются итераторы RowIterator, позволяющие 
	итерироваться по ненулевым элементам матрицы смежности.
	
###### void Print ();
	Функция-утилита, печатающая матрицу смежности.
	
###### void AddRib (size_t x, size_t y, double val);
	Функция, добавляющая путь из вершины x в вершину y с весом val.
  
 Класс Matrix
=====================

Интерфейс для работы с матрицами типа Sparse или Dense.

### Методы класса Matrix
  
###### 1. Метод double &at(size_t i, size_t j)
	Метод возвращает ссылку на элемент матрицы Aij.

###### 2. Геттеры и сеттеры
	double get(size_t, size_t) -- получение значения по индексам (работает и для
	константных объектов)
	void set(size_t, size_t, double) -- изменение значения по индексам
	
###### 3. Вывод размера матрицы
	size_t num_rows() -- количество строк (работает и для константных объектов)
	size_t num_columns() -- количество строк (работает и для константных объектов)
	
###### 4. Перегрузки
  - операции сравнения двух матриц (==, !=)
	- бинарная операция +, сложение разряженных матриц
	- бинарная операция *, умножение * умножение матриц (mult)
	- операция индексации [i]
	- операции для поддержки адресной арифметики 
  - операция вывода >>
  
###### 5. Итераторы 
  Принцип работы с итераторам такой же, как и в стандартной библиотеке C++
  - RowIterator iterate_rows(size_t i);        // Создание итератора по строкам в i-м столбце 
  - RowIterator end_rows(size_t i);            // Последний элемент в i-м столбце, аналог end()
  - ColumnIterator iterate_columns(size_t i);  // Создание итератора по столбцам  i-м строке
  - ColumnIterator end_columns(size_t i);      // Последний элемент в i-й строке, аналог end()
  
  
  
 Класс SparseMatrix
=====================
  
Хранение матрицы в формате CSR:
 	В составе класса 3 динамических массива:
 	- data - массив значений. Хранит значения ненулевых элементов, взятые подряд из 
 	первой непустой строки, затем идут значения из следующей непустой строки и т.д.;
 	- col - массив индексов столбцов. Размер равен размеру массива data, хранит 
 	номера столбцов, соответствующих элементов из массива значений.
 	- row массив индексации строк - массив размера n + 1, где n - кол-во строк в 
 	матрице. Для индекса i хранит количество ненулевых элементов в строках до i − 1 
 	включительно. Последний элемент массива индексации строк совпадает с кол-вом 
 	ненулевых элементов (размером массива data и col) а первый всегда равен 0.
 	
 	Для того, чтобы посмотреть содержимое matrix[i][j] (индексация с 0), нужно:
 	0. Если row[i+1] - row[i] = 0, то в строке нет ненулевых элементов.
 	1. row[i] - индекс первого ненулевого элемента i-й строки в масиве data. То есть
 	matrix[i][col[row[i]]]- первый ненулевой элемент iй строки, а data[row[i]] - его
 	значение.
 	2. Итак, в i-й строке всего row[i+1] - row[i] ненулевых элементов. Смотрим
 	содержимое массива col и ищем значение j. Если в диапазоне от col[row[i]] до 
 	col[row[i+1]] такого значения нет, то matrix[i][j] = 0, иначе 
 	matrix[i][j] = data[k], где k: col[k] = j и row[i] <= k < row[i+1]
 	
 	Примеры:
		data {1, 2, 3, 4, 5}       |1      |
		col {0, 0, 2, 1, 3}        |2   3  |
		row {0, 1, 3, 5}           |  4   5|
		
		data {1, 6!, 2, 3, 4, 5}   |1   6  |
		col {0, 2!, 0, 2, 1, 3}    |2   3  |
		row {0, 1+1, 3+1, 5+1}     |  4   5|
		
		data {1, 2, 6!, 3, 4, 5}   |1   6  |
		col {0, 0, 1!, 2, 1, 3}    |2 6 3  |
		row {0, 1, 3+1, 5+1}       |  4   5|

		The number of elements in a column №r equals row[r] - row[r-1]
		
	В составе класса также есть 2 подкласса Row и constRow, которые, в свою очередь,
	имеют свои подклассы subRow и constsubRow. Данные классы используются для 
	перегрузки операторов разыменования и индексации.

### Информационные поля класса SparseMatrix

1. int num_columns; // кол-во столбцов в матрице
2. int num_rows;    // кол-во строк в матрице
3. double *data;    // массив значений, динамический
4. int *row;        // массив индексации строк, динамический
5. int *col;        // массив индексации столбцов
6. int non_zero;    // размер динамических массивов data и col

### Методы класса SparseMatrix

###### 1. Конструктор.
 	При создании матрица инициализирована нулями. Начальная длина динамических 
 	массивов (rows*columns + 10) / 5. По умолчанию количество строк и столбцов равно 
 	константному числу N.
 	
###### 2. Метод double* GetElem(int r, int c) 
  	Метод получает на вход строку и столбец искомого элемента, при его отсутсвии 
  	создает ячейку в массиве data и col, и возвращает адрес ячейки в массиве data.
  	Алгоритм:
  	1. Look for a col[i] == c in range from row[r] to row[r+1] - 1
	2. If there is nothing and i == row[r+1], shift everything right
	3. If col[i-1] < c < col[i], shift everythinf right 
	4. return pointer to the element in data array 
	
###### 3. Метод void SparseMatrix::CleanZero()
	Метод убирает нулевые значения (точнее значения по модулю меньшие эпсилон) из
	массива значений data, сдвигает остальные элементы массива влева. Аналогичым
	образом меняется массив col. В массиве row нужные элементы уменьшаются на 0.
	Ситуация возникновения нулей в массиве может быть обусловлена выражениями типа:
	A[i][j] = 0;
	*(*(A + i) + j) = 0;
	т.к. операция присваивания не перегружена, и пользователь можнт по ошибке 
	добавить в матрицу ноль.
  

 Класс DenseMatrix
=====================

  Хранение матрицы в виде вектора из векторов.

###### Информационные поля класса DenseMatrix

1. int num_columns;                            // кол-во столбцов в матрице
2. int num_rows;                              // кол-во строк в матрице
3. std::vector<std::vector<double>> *data;    // массив значений
 -- количество строк (работает и для константных объектов)
	size_t num_columns() -- количество строк (работает и для константных объектов)
	
  
ФАЙЛЫ ПРОЕКТА
=====================

1. Makefile           - файл-сборщик
2. graph.hpp          - библиотека для работы c графами
   graph.cpp                                                   
3. matrix.hpp         - библиотека для работы с матрицами
4. densematrix.cpp    - библиотека для работы с плотными матрицами (подкласс матриц)
   densematrix.hpp
5. sparsematrix.hpp   - библиотека для работы с разреженными матрицами (подкласс матриц)
   sparsematrix.cpp
6. iterator.hpp       - библиотека для работы с итераторами
   iterator.cpp      
7. main.cpp          - основная программа, содержит пример использования графов и матриц

