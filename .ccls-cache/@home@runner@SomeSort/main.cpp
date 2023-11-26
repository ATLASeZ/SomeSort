#include <iostream>
#include <chrono>

using namespace std;

class Array 
{
private:
  int* m_array;															// Массив
  int m_arraySize;														// Размерность массива

public:
  Array(int lenght = 1, int type = 1, int range = 10);					// Конструктор (с параметрами)
  Array(int* values, int lenght);											// Конструктор (с параметрами)			
  Array(Array& other);													// Конструктор копирования

  ~Array();																// Деструктор

  bool IsOrderedArray();													// Проверка упорядоченности массива

  Array& operator=(const Array& other);									// Оператор присваивания
  bool operator==(Array other);											// Оператор сравнения равенства

  int& operator[](int key);												// Оператор доступа к элементу

  friend istream& operator>>(istream& in, Array& other);					// Оператор потокового ввода
  friend ostream& operator<<(ostream& out, Array& other);					// Оператор потокового вывода

  void ShellSort();														// Сортировка Шелла
  void HeapifyProcedure(int, int);										// Процедура построения пирамиды
  void HeapSort();														// Пирамидальная сортировка
  void HoareSort();														// Сортировка Хоара
  int ProvokeHoareSort(int* hoareArray, int left, int right);				// Вспомогательная функция алгоритма сортировки Хоара
};

/* Конструктор (с параметрами) */
Array::Array(int lenght, int type, int range)
{
  /* Выделяем память под массив */
  m_array = new int[lenght];
  /* Устанавливаем длину */
  m_arraySize = lenght;

  /* Функция для генерации рандомных значений */
  srand((unsigned)time(nullptr));

  /* Генерируем рандомные значения в диапазоне range */
  if (type == 1)
  {
    for (int i = 0; i < m_arraySize; i++)
    {
      m_array[i] = rand() % range;
    }
  }
  /* Формируем массив по возрастанию */
  else if (type == 2)
  {
    for (int i = 0; i < m_arraySize; i++)
    {
      m_array[i] = i;
    }
  }
  /* Формируем массив по убыванию */
  else if (type == 3)
  {
    for (int i = m_arraySize - 1; i >= 0; i--)
    {
      m_array[i] = m_arraySize - 1 - i;
    }
  }
  /* Иначе завершаем работу программы */
  else
  {
    exit(1);
  }
};

/* Конструктор (с параметрами) */
Array::Array(int* values, int lenght)
{
  /* Выделяем память под массив */
  m_array = new int[lenght];
  /* Устанавливаем длину */
  m_arraySize = lenght;

  /* Заполняем массив, копируя значения */
  for (int i = 0; i < m_arraySize; i++)
  {
    m_array[i] = values[i];
  }
};

/* Конструктор копирования */
Array::Array(Array& other)
{
  /* Выделяем память под массив */
  m_array = new int[other.m_arraySize];
  /* Устанавливаем размерность массива (такая же, как и у other) */
  m_arraySize = other.m_arraySize;

  /* Копируем значения */
  for (int i = 0; i < m_arraySize; i++)
  {
    m_array[i] = other.m_array[i];
  }
};

/* Деструктор */
Array::~Array()
{
  /* Проверяем, что m_array является не пустым указателем */
  if (m_array)
  {
    /* Освобождаем память, выделенную для m_array */
    delete[] m_array;
  }

  /* Устанавливаем указатель m_array в значение nullptr */
  m_array = nullptr;
}

/* Проверка упорядоченности массива */
bool Array::IsOrderedArray()
{
  /* Проверяем массив на возрастание */
  for (int i = 1; i < m_arraySize; i++)
  {
    /* Если найдутся элементы, нарушающие данный вид упорядоченности */
    if (m_array[i] < m_array[i - 1])
    {
      /* Это значит, что массив неупорядочен, возвращаем false */
      return false;
    }
  }

  /* Иначе массив упорядочен, возвращаем true */
  return true;
}

/* Оператор присваивания */
Array& Array::operator=(const Array& other)
{
  /* Проверяем, что объект не является самим собой  */
  if (this == &other)
  {
    /* Возвращаем ссылку на текущий объект */
    return *this;
  }

  /* Освобождаем память */
  delete[] m_array;

  /* Задаём новую длину массива */
  m_arraySize = other.m_arraySize;
  /* Выделяем память */
  m_array = new int[m_arraySize];

  /* Присваиваем (переписываем) элементы */
  for (int i = 0; i < m_arraySize; i++)
  {
    m_array[i] = other.m_array[i];
  }

  /* Возвращаем ссылку на текущий объект */
  return *this;
};

/* Оператор сравнения равенства */
bool Array::operator==(Array other)
{
  /* Сравниваем длины массивов */
  if (m_arraySize != other.m_arraySize)
  {
    /* Если длины не равны, возвращаем false */
    return false;
  }

  /* Длина массива */
  int lenght = m_arraySize;

  /* Идём по массиву */
  for (int i = 0; i < m_arraySize; i++)
  {
    /* Создаём флаг */
    bool flag = false;

    /* Проходим по всем элементам */
    for (int k = 0; k < lenght; k++)
    {
      /* Ищем равные элементы */
      if (m_array[i] == other.m_array[k])
      {
        /* Меняем значение флага */
        flag = true;
        /* Помещаем найденный элемент в конец */
        other[k] = other[lenght - 1];
        /* Уменьшаем диапазон поиска */
        lenght--;

        /* Выходим из цикла */
        break;
      }
    }

    /* Если флаг равен false */
    if (flag == false)
    {
      /* Массивы не равны */
      return false;
    }
  }

  /* Массивы равны */
  return true;
}

/* Оператор доступа к элементу */
int& Array::operator[](int key)
{
  /* Проверяем, что значение key находится в допустимых границах */
  if (key < 0 || key >= m_arraySize)
  {
    /* Если значение key не соответствует допустимым границам, вызываем функцию exit для завершения работы программы */
    exit(-1);
  }

  /* Возвращаем ссылку на символ по индексу key */
  return m_array[key];
}

/* Оператор потокового ввода */
istream& operator>>(istream& in, Array& other)
{
  /* Запрашиваем длину массива у пользователя */
  int lenght;
  cout << "Введите длину: ";
  in >> lenght;

  /* Объявляем массив */
  int* arrayName;
  /* Выделяем память */
  arrayName = new int[lenght];

  cout << "Массив: ";

  /* Проходимся по массиву */
  for (int i = 0; i < lenght; i++) 
  {
    /* Выводим элементы */
    in >> arrayName[i];
  }

  /* Копируем элементы в массив */
  other = Array(arrayName, lenght);

  /* Освобождаем память */
  delete[] arrayName;

  /* Возвращаем поток */
  return in;
}

/* Оператор потокового вывода */
ostream& operator<<(ostream& out, Array& other) 
{
  /* Проходим по массиву */
  for (int i = 0; i < other.m_arraySize; i++) 
  {
    /* Выводи его элементы на консоль */
    out << other[i] << " ";
  }

  /* Возвращаем поток */
  return out;
}

/* Сортировка Шелла */
void Array::ShellSort() 
{
  /* Разбиваем на шаги */
  for (int step = m_arraySize / 2; step > 0; step /= 2)
  {
    /* Сначала рассматриваем первый элемент подмассива */
    for (int first = 0; first < step; first++) 
    {
      /* Прходимся по другим элементам массива (и сортируем их) */
      for (int currentIndex = first + step; currentIndex < m_arraySize; currentIndex += step) 
      {
        /* Текущее значение элемента */
        int currentValue = m_array[currentIndex];
        /* Индекс следующего элемента */
        int previousIndex;

        /* Смотрим на предыущие элементы и сравниваем их с текущим значением */
        for (previousIndex = currentIndex - step; previousIndex >= 0 && m_array[previousIndex] > currentValue; previousIndex -= step) 
        {
          m_array[previousIndex + step] = m_array[previousIndex];
        }

        /* Записываем текущее значение элемента в его правильное место в упорядоченной части массива */
        m_array[previousIndex + step] = currentValue;
      }
    }
  }
}

/* Процедура построения пирамиды */
void Array::HeapifyProcedure(int heapSize, int currentIndex)
{
  /* Левый потомок */
  int leftChild = 2 * currentIndex + 1;
  /* Временное значение */
  int temporaryValue = m_array[currentIndex];
  /* Устанавливаем флаг (для отслеживания удовлетворённости свойства кучи) */
  bool flag = true;

  while (leftChild < heapSize && flag)
  { 	
    /* Пока не вышли за границу, а также есть потомок больше предка */
    if (leftChild + 1 < heapSize && m_array[leftChild + 1] > m_array[leftChild])
    {
      /* Выбираем максимальное значение потомка */
      leftChild++;
    }

    /* Если нашли потомка, который больше предка */
    if (m_array[leftChild] > temporaryValue)
    {
      /* В предка записываем потомка */
      m_array[currentIndex] = m_array[leftChild];
      /* Рассматриваем место потомка как нового предка */
      currentIndex = leftChild;
      leftChild = 2 * currentIndex + 1;
    }
    /* Иначе свойство кучи не выполнились */
    else
    {
      /* Сбрасываем флаг */
      flag = false;
    }
  }

  /* В текущий элемент помещаем временное значение */
  m_array[currentIndex] = temporaryValue;
}

/* Пирамидальная сортировка */
void Array::HeapSort()
{
  /* Формируем пирамиду с максимальным значением в её вершине */
  for (int i = m_arraySize / 2 - 1; i >= 0; i--)
  {
    (*this).HeapifyProcedure(m_arraySize, i);
  }

  /* Прохожимся по пирамиде */
  for (int i = m_arraySize - 1; i > 0; i--)
  {
    /* Сортируем массив */
    swap(m_array[0], m_array[i]);
    (*this).HeapifyProcedure(i, 0);
  }
}

/* Сортировка Хоара */
void Array::HoareSort()
{
  /* Вызываем вспомогательную функцию для работы алгоритма сортировки Хоара */
  ProvokeHoareSort(m_array, 0, m_arraySize - 1);
}

/* Вспомогательная функция сортировки Хоара */
int Array::ProvokeHoareSort(int* hoareArray, int left, int right) 
{
  /* если левая граница больше или равна правой */
  if (left >= right) 
  { 				
    return 0;
  }

  /* Нижняя граница */
  int low = left; 	
  /* Верхняя граница */
  int high = right;
  /* Центральный (опорный) элемент */
  int pivot = hoareArray[(left + right) / 2];

  /* Пока левая граница не больше правой */
  while (low <= high) 
  {
    /* Пока элемент из нижней границы (то есть левой части) массива меньше центрального элемента */
    while (hoareArray[low] < pivot)
    {
      /* Сдвигаем нижнюю границу на 1 вправо (увеличиваем) */
      low++;
    }
    /* Пока элемент из верхней границы (то есть правой части) массива больше центрального элемента */
    while (hoareArray[high] > pivot) 
    {
      /* Сдвигаем верхнюю границу на 1 влево (уменьшаем) */
      high--;
    }

    /* Если левая граница не больше правой границы */
    if (low <= high) 
    {
      /* Меняем эти элементы местами */
      swap(hoareArray[low], hoareArray[high]);

      /* Сдвигаем границы - нижнюю на 1 вправо (увеличиваем), а верхнюю границу на 1 влево (уменьшаем) */
      low++;
      high--;
    }
  }						

  /* Разбиваем массив на две части, каждую из которых рекурсивно сортируем */
  ProvokeHoareSort(hoareArray, left, high);
  ProvokeHoareSort(hoareArray, low, right);

  return 0;
}

/* Все сортировки */
void AllSorts() 
{
  using std::chrono::high_resolution_clock;
  using std::chrono::duration;
  using std::chrono::milliseconds;

  Array testArray(100000, 1);

  Array shellArray = testArray;
  Array heapArray = testArray;
  Array hoareArray = testArray;

  auto shellStart = high_resolution_clock::now();
  shellArray.ShellSort();
  auto shellEnd = high_resolution_clock::now();
  duration<float, std::milli> shellTime = shellEnd - shellStart;
  cout << "Время сортировки Шелла: " << shellTime.count() << " мс" << endl;
  cout << "Отсортировано Шеллом по возрастанию? " << shellArray.IsOrderedArray() << endl;

  auto heapStart = high_resolution_clock::now();
  heapArray.HeapSort();
  auto heapEnd = high_resolution_clock::now();
  duration<float, std::milli> heapTime = heapEnd - heapStart;
  cout << "Время пирамидальной сортировки: " << heapTime.count() << " мс" << endl;
  cout << "Отсортировано пирамидой по возрастанию? " << heapArray.IsOrderedArray() << endl;

  auto hoareStart = high_resolution_clock::now();
  hoareArray.HoareSort();
  auto hoareEnd = high_resolution_clock::now();
  duration<float, std::milli> hoareTime = hoareEnd - hoareStart;
  cout << "Время сортировки Хоара: " << hoareTime.count() << " мс" << endl;
  cout << "Отсортировано Хоаром по возрастанию? " << hoareArray.IsOrderedArray() << endl;
}

int main()
{
	setlocale(LC_ALL, "Rus");

	cout << "Проверка работы конструкторов класса: " << endl;
	Array firstArray(10, 1, 30);
	Array secondArray(15, 2);
	Array thirdArray(15, 3);

	cout << "firstArray: " << firstArray << endl;
	cout << "secondArray: " << secondArray << endl;
	cout << "thirdArray: " << thirdArray << endl;

	cout << endl;

	cout << "Проверка работы методов класса: " << endl;
  secondArray = thirdArray;
	cout << "secondArray: " << secondArray << endl;

	cout << "firstArray[3]: " << firstArray[3] << endl;

	cout << "Задайте параметры массива secondArray: " << endl;
	cin >> secondArray;
	cout << "secondArray: " << secondArray << endl;

//	cout << endl;
//
//	cout << "Проверка работы алгоритмов сортировок: " << endl;
//	Array sortAlgorithmArray = firstArray;
//	cout << "До алгоритмов сортировки: " << sortAlgorithmArray << endl;
//	sortAlgorithmArray.ShellSort();
//	cout << "После алгоритма Шелла: " << sortAlgorithmArray << endl;
//
//	sortAlgorithmArray = firstArray;
//	sortAlgorithmArray.HeapSort();
//	cout << "После пирамидального алгоритма: " << sortAlgorithmArray << endl;
//
//	sortAlgorithmArray = firstArray;
//	sortAlgorithmArray.HoareSort();
//	cout << "После алгоритма Хоара: " << sortAlgorithmArray << endl;
//
//	AllSorts();
//
//	return 0;
//}