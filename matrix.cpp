#include <iomanip>
#include <sstream>
#include <cstring>

#include "matrix.hpp"


matrix::matrix(int n, int m)
{
	if (n <= 0)
		throw ind_exc(n, 0);
	if (m <= 0)
		throw ind_exc(m, 0);
	//cout << "Вызыван конструктор №1 класса с двумя параметрами " << endl;
	//выделяем массив указателей, а также место для каждого указателя  
	try
	{
		this->row = n;
		this->col = m;
		this->M = new double*[n];
			for (int i=0;i<n;i++)
			{
					this->M[i] = new double [m];  
			}
			for (int i=0;i<n;i++)
			{
				for (int j=0;j<m;j++)
				{
					M[i][j] = 0.;
				}
			}
	}
	catch(...)
	{
		cerr<<"Got an exception in matrix constructor!"<<endl;
		throw matrix_exception(codes::dyn_mem_err);
	}	
}
//2) matrix( double ) – матрица 1*1 с этим элементом;
matrix::matrix(double val )
{
	try
	{
		//cout << "Вызыван конструктор №2 класса" << endl;
		this->row = 1;
		this->col = 1;
		//выделяем массив указателей, а также место для каждого указателя  
		this->M = new double*;
		this->M[0] = new double; 
		M[0][0] = val;
	}
	catch(...)
	{
		cerr<<"Got an exception in matrix constructor!"<<endl;
		throw matrix_exception(codes::dyn_mem_err);
	}
}

//3) matrix( double*, int m ) – матрица-строка из массива длины m;
matrix::matrix(double* arr, int m)
{
	if (m <= 0)
			throw ind_exc(m, 0);
	try
	{	
		//cout << "Вызван конструктор №3 создан массив-строка" << endl;
		this->row = 1;
		this->col = m;
		//выделяем память
		this->M = new double*;
		this->M[0] = new double[m]; 
		for (int i=0;i<m;i++)
		{
			M[0][i] = arr[i];
		}
	}
	catch(...)
	{
		cerr<<"Got an exception in matrix constructor!"<<endl;
		throw matrix_exception(codes::dyn_mem_err);
	}

}

//4) matrix( int n, double* ) – матрица-столбец из массива длины n;
matrix::matrix(int n, double* arr)
{
	try
	{	
		if (n <= 0)
			throw ind_exc(n, 0);
		//cout << "Вызван конструктор №4 создан массив-столбец "<< endl;
		this->row = n;
		this->col = 1;
		//выделяем память
		this->M = new double*[n];
		for (int i=0;i<n;i++){	this->M[i] = new double; }  
		for (int i=0;i<n;i++)
		{
			M[i][0] = arr[i];
		}
	}
	catch(...)
	{
		cerr<<"Got an exception in matrix constructor!"<<endl;
		throw matrix_exception(codes::dyn_mem_err);
	}

}

//5) matrix( char*) – из строкового представления (см. выше);
matrix::matrix (const char* arr)//передавать нужно как константный указатель, так как имя может совпась со стандартным классом 
{	
	try
	{
		if (!*arr) 
			throw ind_exc(0, 0);
		const char* p = arr;
		int m=0, n=0, n1=0;
		int min_fl = 0; //флаг того что число с минусом
		//R1 идем по строке пока не встретим закр.скобку или запятую
		while (*arr != '\0')
		{
				if (*arr == '{')
				{
					arr++;
					m++;
					n1 = 0;
				}
				else 
					throw str_exc('{', n, n1, m);
				if (isdigit(*arr)) 
				{
					n1++;
				}
				else if(*arr == '-')
				{
					n1++;
					arr++;
				}
				else 
				{
					throw str_exc('s', n, n1, m);
				}
					while (*arr != '}')
					//если запятая увеличиваем кол.столбцов на единицу
					{
							if (*arr == '-')
							{
								arr++;
							}
			//N если цифра увеличики значение столбцов на единицу
							//идем пока цифра идем, пока не встретим запятую или скобку 
							while(isdigit(*arr) || *arr == '.') 
							{
								arr++;
							} 
							if (*arr != ',' && *arr != '}')
							{
								throw str_exc('e', n, n1, m);					
							}
							else if (*arr == ',') 
							{
								arr++; n1++;
							}
							if (*arr == ' ')
								arr++;
					}	
					//проверка на кол.строк в пред-м и тек-м цикле
					if (n == 0) 
						n = n1;
					else if (n1 != n) 
						throw str_exc('n', n, n1, m);
					else n = n1;
					arr++;
					
					if (*arr != ',' && *arr != '}' && *arr != '\0')
					{
						throw str_exc('e', n, n1, m);					
					}
					else if (*arr == ',')
					{
						arr++;
						if (*arr == ' ')
							arr++;
					}	
					if (*arr == ' ')
						arr++;
		}
		try
		{	 
		this->row = m;
		this->col = n;
		//выделяем массив указателей, а также место для каждого указателя  
		this->M = new double*[m];
			for (int k=0;k<m;k++)
			{	
				this->M[k] = new double [n];
			 } 	
		for (int i=0;i<m;i++)
			{
				for (int j=0;j<n;j++)
				{
					this->M[i][j] = 0.;
				}
			}
		}
		catch(...)
		{
			cerr<<"Got an exception in matrix constructor!"<<endl;
			throw matrix_exception(codes::dyn_mem_err);
		}
		int i = 0, j = 0;
		double  r = 1., buf = 0.; 
		//R1 идем по строке пока не встретим закр.скобку или запятую 
		//R	 - убедиться что второй сичвол откр скобка
		while (*p != '\0')
		{
				if (*p == '{')
				{
					p++;
				}
				else 
					throw str_exc('{', j, j, i); // может сделать ('', i, j)
		//N1 пока не встретим закр.скобку
				if (!isdigit(*p) && *p != '-') 
				{
					throw str_exc('s', j, j, i);
				}
					//если запятая увеличиваем кол.столбцов на единицу
		//N если цифра увеличики значение столбцов на единицу
							//идем пока цифра идем, пока не встретим запятую или скобку 
							r = 1.;
							
							while (*p != '}')
							{
								if ( *p == '-')
									{
										p++;
										min_fl = 1;
									}
								buf *= 10.;
								buf += ((*p) - '0');
								p++;
								if (*p == '.')
								{
									p++;
									while (isdigit(*p))
									{
										r /= 10.;
										buf += (((*p) - '0')*r);
										p++;
									}
								} 
								if (min_fl)
								{
									buf *= -1.;
									min_fl = 0;
								}	
								if (*p != ',' && *p != '}' && !isdigit(*p))
								{
									throw str_exc('e', j, j, i);					
								}
								else if (*p == ',') {this->M[i][j] = buf;buf = 0.; j++; p++; r= 1.;}
								else if (*p == '}') {this->M[i][j] = buf;buf = 0.; j = 0;}
								if (*p == ' ')
									p++;
							}
					p++;
					i++;
					if (*p != ',' && *p != '}' && *p != '\0')
					{
						throw str_exc('e', j, j, i);
					}
					else if (*p == ',')
					{
						 p++;
						 if (*p == ' ')
							p++;
					}
		}
	}
	catch(...)
	{
		cerr<<"Got an exception in matrix constructor string!"<<endl;
		throw matrix_exception(codes::invalid_string);
	}
	
}

matrix matrix::identity(int n)
{
	try
	{
		if (n <= 0)
			throw ind_exc(n, 0);	
		matrix New_Arr(n, n);
		for (int i = 0; i < New_Arr.row; i++)
			for (int j = 0; j < New_Arr.col;j++)
				 New_Arr.M[i][j] = 1.;
		//cout << "Вызван метод Matrix::identity" << endl;
		return New_Arr; 
	}
	catch(...)
	{
		cerr<<"Got an exception in matrix constructor!"<<endl;
		throw matrix_exception(codes::dyn_mem_err);
	}
}
	
//7) static matrix matrix::diagonal( double* vals, int n ) – возвращает диагональную матрицу размера n с заданными элементами по главной диагонали;
matrix matrix::diagonal(double* vals, int n)
{
	try
	{
		if (n <= 0)
			throw ind_exc(n, 0);
		matrix New_Arr(n, n);
		for (int i = 0; i < New_Arr.row; i++)
			for (int j = 0; j < New_Arr.col;j++)
				 if (i == j) New_Arr.M[i][j] = *vals;
		//cout << "Вызван метод Matrix::identity" << endl;
		return New_Arr; 
	}
	catch(...)
	{
		cerr<<"Got an exception in matrix constructor!"<<endl;
		throw matrix_exception(codes::dyn_mem_err);
	}
}

//copy Конструктор копирования 
matrix::matrix(const matrix & m)
{
	this->row = m.rows();
	this->col = m.columns();
	//выделяем массив указателей, а также место для каждого указателя  
	try
	{
		this->M = new double*[this->row];
			for (int i=0;i< this->row;i++)
			{
				this->M[i] = new double[this->col];  
			}
			for (int i=0;i< this->row;i++)
			{
				for (int j=0;j<this->col;j++)
				{
					this->M[i][j] = m.M[i][j];
				}
			}
	}
	catch(...)
	{
		cerr<<"Got an exception in matrix constructor!"<<endl;
		throw matrix_exception(codes::dyn_mem_err);
	}		
	
}


//print matrix
ostream & operator<< (ostream & s, const matrix & m)
{
	s<<setprecision(2);
	for (int i = 0; i < m.rows(); ++i)
	{
		cout << endl;
		for (int j = 0; j < m.columns(); ++j)
			s<< ' ' << fixed << m.M[i][j];
	}
	return s;
}

const matrix matrix::operator[] (const int i)
{	
	int k = i;
	if (k >= this->row + 1 || k <= 0)
	{
		if (k >= this->col + 1 || k <= 0)
		{
			throw ind_exc(i, 0);
		}
		else
		{
			matrix new_arr1(this->row, 1);
			for (int j = 0; j < this->row;j++)
			{
				new_arr1.M[j][0] = this->M[j][k - 1];
			}
			const matrix new_arr(new_arr1);
			return new_arr;
		}
	}
	else
	{
		const matrix new_arr(this->M[k - 1], this->col);
		return new_arr;
	}
}


const matrix matrix::operator[] (const int i) const
{	
	
try
{
	int k = i;
	if (this->row > 1)
	{
		if (k >= this->row + 1 || k <= 0)
		{
			if (k >= this->col + 1 || k <= 0)
			{
				throw ind_exc(i, 0);
			}
			else
			{
				matrix new_arr1(this->row, 1);
				for (int j = 0; j < this->row;j++)
				{
					new_arr1.M[j][0] = this->M[j][k - 1];
				}
				const matrix new_arr(new_arr1);
				return new_arr;
			}
		}
		else
		{
			const matrix new_arr(this->M[k - 1], this->col);
			return new_arr;
		}
	}
	else
	{
		if (k >= this->col + 1 || k <= 0)
		{
			throw ind_exc(i, 0);
		}
		else
		{
			const matrix new_arr(this->M[0][k - 1]);
			return new_arr;
		}	
	}
}	
	catch(...)
	{
		cerr<<"Got an exception []!"<<endl;
		throw matrix_exception(codes::dyn_mem_err);
	}		
}



void matrix::set(const int i, const int j, const double val)
{
	//проверяем удовлетворяет ли матрица размерам
	if (i <= 0)
		throw ind_exc(i, 0);
	if (j <= 0)
		throw ind_exc(j, 0);
	else if (i > this->row + 1) 
		throw ind_exc(i, this->row);
	else if (j > this->col)
		throw ind_exc(j, this->col);
	else
		this->M[i - 1][j - 1] = val;

}

//destructor
matrix::~matrix()
{
	//cout << "Вызван деструктор" << endl;
	for (int i=0; i < this->row; i++)
		{
			//удаляем строки
			delete[] M[i];
		}
		//удаляем массив указателей на строки
		delete[] M;
}

//перезагрузка оператора присваивания 
matrix matrix::operator = (const matrix& arr)
{	
	if(this != &arr)
        {
		for (int i=0; i < this->row; i++)
			{
				//удаляем строки
				delete[] this->M[i];
			}
		//удаляем массив указателей на строки
			delete[] this->M;
			this->row = arr.rows();
			this->col = arr.columns();
            this->M = new double*[this->row];
			for (int i=0;i<this->row;i++)
			{
					this->M[i] = new double [this->col];  
			}
			for (int i=0;i < this->row;i++)
			{
				for (int j=0;j<this->col;j++)
				{
					this->M[i][j] = arr.M[i][j];
				}
			}
			return *this;
        }
	//удаляем старое
	return *this; 
	
	
}

//13) matrix * scalar и matrix*=scalar – умножение матрицы на скаляр;
matrix matrix::operator* (const double scalar)
{ 
	if (scalar == 0.)
		return matrix(this->row,  this->col);
	matrix arr_cp(this->row, this->col);
	for (int i=0;i< this->row;i++)
	{
		for (int j=0;j< this->col;j++)
		{
			arr_cp.M[i][j] = this->M[i][j] * scalar;
		}
	}
	return arr_cp;
}    


matrix matrix::operator*= (const double scalar)
{	if (scalar == 0)
		return matrix(this->row,  this->col);
	for (int i=0;i< this->row;i++)
	{
		for (int j=0;j< this->col;j++)
		{
			this->M[i][j] *= scalar;
		}
	}
	return *this;
}    

//15)matrix + matrix ;
matrix matrix::operator+(const matrix& arr) const
{
	//Проверка на размерность...
	if (this->row != arr.rows())
		throw incomp_exc('+', 'r', this->row, arr.rows());
	else if (this->col != arr.columns())
		throw incomp_exc('+', 'c', this->col, arr.columns());
	else
	{
		matrix New_Arr(arr.rows(), arr.columns());
		for (int i=0;i< arr.rows();i++)
		{
			for (int j=0;j< arr.columns();j++)
			{
				New_Arr.M[i][j] = this->M[i][j] + arr.M[i][j];
			}
		}
		return New_Arr;
	}
	
}

//16)matrix += matrix ; 
matrix matrix::operator +=(const matrix& arr)
{
	if (this->row != arr.rows())
		throw incomp_exc('+', 'r', this->row, arr.rows());
	else if (this->col != arr.columns())
		throw incomp_exc('+', 'c', this->col, arr.columns());
	else
	{		
		for (int i=0;i< arr.row;i++)
		{
			for (int j=0;j< arr.col;j++)
			{
				this->M[i][j] += arr.M[i][j];
			}
		}
		return *this;
	}	
}

//17)matrix - matrix ;
matrix matrix::operator -(const matrix& arr) const
{
	if (this->row != arr.rows())
		throw incomp_exc('-', 'r', this->row, arr.rows());
	else if (this->col != arr.columns())
		throw incomp_exc('-', 'c', this->col, arr.columns());
	else
	{		
		//cout << "Вычетание матриц +" << endl;
		matrix New_Arr(arr.rows(), arr.columns());
		for (int i=0;i< arr.row;i++)
		{
			for (int j=0;j< arr.col;j++)
			{
				New_Arr.M[i][j] = this->M[i][j] - arr.M[i][j] ;
			}
		}
		return New_Arr;
	}
}

//18)matrix -= matrix ;
//Может ли объект быть const ? 
matrix matrix::operator -=(const matrix& arr)
{
	
	if (this->row != arr.rows())
		throw incomp_exc('-', 'r', this->row, arr.rows());
	else if (this->col != arr.columns())
		throw incomp_exc('-', 'c', this->col, arr.columns());
	else
	{		
		for (int i=0;i< arr.row;i++)
		{
			for (int j=0;j< arr.col;j++)
			{
				this->M[i][j] -= arr.M[i][j] ;
			}
		}
		return *this;
	}
}

//19)matrix * matrix ;
matrix matrix::operator *(const matrix& arr) const
{
	
	if (this->col != arr.rows())
		throw incomp_exc('*', 'm', this->col, arr.rows());	
	else
	{		
		matrix New_Arr(this->row, arr.columns());
		for (int i=0;i< this->rows();i++)
		{
			for (int j=0;j< arr.columns();j++)
			{
				for (int k=0;k < arr.rows();k++)
					New_Arr.M[i][j] += this->M[i][k] * arr.M[k][j] ;
			}
		}
		return New_Arr;
	}
}

//20)matrix *= matrix ;
matrix matrix::operator *=(const matrix& arr)
{
	if (this->col != arr.rows())
		throw incomp_exc('*', 'm', this->col, arr.rows());	
	else
	{		
		matrix New_Arr(this->row, arr.columns());
		for (int i=0;i< this->rows();i++)
		{
			for (int j=0;j< arr.columns();j++)
			{
				for (int k=0;k < arr.rows();k++)
					New_Arr.M[i][j] += this->M[i][k] * arr.M[k][j] ;
			}
		}
		return New_Arr;
	}
}

//21)-matrix – унарный минус, применить ко всем элементам ;
//тут всмысле поменять знак или уменьшить на 1 ? 
matrix matrix::operator -()
{
	cout << "Вызван унарный минус " << endl;
	for (int i=0;i< this->rows();i++)
	{
		for (int j=0;j< this->columns();j++)
		{
			(this->M[i][j]) = -(this->M[i][j]);
		}
	}
	return *this;
}

//22)matrix == matrix – точность сравнения задана статической константой matrix::EPS ;
bool matrix::operator == (const matrix& arr) const
{
	//Проверка вызова матрицы самой себя 
	//Проверка размерностей матрцы
	if (this == &arr) { return 1;}	
	if (this->row != arr.rows())
		throw incomp_exc('=', 'r', this->row, arr.rows());
	else if (this->col != arr.columns())
		throw incomp_exc('=', 'c', this->col, arr.columns());
	else
	{		
	double num;
	for (int i=0;i< this->rows();i++)	
	{
		for (int j=0;j< this->columns();j++)
		{
			
			num = this->M[i][j] - arr.M[i][j]; 
			if (num <= EPS && num >= -EPS)  continue;
			else return 0 ;
		}
	}
	}
	return 1;
	
}

	//23)matrix != matrix ;
bool matrix::operator != (const matrix& arr) const
{
	//Проверка вызова матрицы самой себя 
	if (this == &arr) {cout << "Матрица не равна сама себе " << endl; return 1;}
	//Проверка размерностей матрцы
	if (this->row != arr.rows())
		throw incomp_exc('!', 'r', this->row, arr.rows());
	else if (this->col != arr.columns())
		throw incomp_exc('!', 'c', this->col, arr.columns());
	else
	{		
	cout << "Вызвано сравнение != " << endl;
	double num;
	for (int i=0;i< this->rows();i++)	
	{
		for (int j=0;j< this->columns();j++)
		{
			num = abs(this->M[i][j] - arr.M[i][j]); 
			if (num >= EPS || num <= -EPS)  return 1;
		}
	}
	}
	return 0;
}

//add to right
matrix matrix::operator | (const matrix& arr) const
{
	if (this->row != arr.rows())
		throw incomp_exc('|', 'r', this->row, arr.rows());
	else
	{
		matrix New_Arr(this->row, this->col + arr.columns());
		int j1= 0;
		for (int i = 0; i < this->row;i++)
		{
			j1 = 0;
			for (int j=0;j < this->col + arr.columns();j++)
			{
					New_Arr.M[i][j] = j < this->col ? this->M[i][j]: arr.M[i][j1++];
			}
		}
		return New_Arr;
	}
}
		
//25)matrix / matrix – конкатенировать (приписать) матрицы горизонтально (вторую под первой).
matrix matrix::operator / (const matrix& arr) const
{
	if (this->col != arr.columns())
		throw incomp_exc('/', 'c', this->col, arr.columns());
	else
	{ 
		matrix New_Arr(this->row + arr.rows(), this->columns());
		int i1 = 0;
		for (int i = 0; i < this->row + arr.rows() ;i++)
		{
			for (int j=0;j < this->col;j++)
			{
				New_Arr.M[i][j] = i < this->row ? this->M[i][j]: arr.M[i1][j];
			}
			if (i >= this->row)
				i1++;
		}
		return New_Arr;
	}
}

//26) ~matrix – обратная матрица;
matrix matrix::Transpone ()
{
  if (this->row != this->col)
	throw incomp_exc('*', 'm', this->row, this->col);
  matrix Tr_Mat(this->row, this->col);
  for (int i = 0; i < this->col; i++)
  {
	for (int j = 0; j < this->row; j++)
		Tr_Mat.M[i][j] = this->M[j][i];
  }
  return Tr_Mat;
}
// Обратная матрица
matrix matrix::invers()
{
  if (this->row != this->col)
		throw incomp_exc('*', 'm', this->row, this->columns());
  //обратная матрица
  matrix Rev_Mat(this->row, this->col);
  double det = determinant(); // находим определитель исходной матрицы
  for (int i = 0; i < this->row; i++)
  {
	for (int j = 0; j < this->col; j++)
	{
	  Rev_Mat.M[i][j] = GetMatr(i, j).determinant();
	  if ((i + j) % 2 == 1)       // если сумма индексов строки и столбца нечетная
		Rev_Mat.M[i][j] = -Rev_Mat.M[i][j];    // меняем знак минора
	  Rev_Mat.M[i][j] = Rev_Mat.M[i][j] / det;
	}
  }
  return Rev_Mat.Transpone();
}

	//27) double matrix::determinant() – определитель;
double matrix::determinant() const
{
		if (this->row != this->col)
			throw incomp_exc('*', 'm', this->row, this->col);
		//вычисление определителя осуществляется при помощи приведения матицы 
		//к нижне треугольной матрице с использование метода Гаусса
		//таким образом определитель будет равен произведению эл-в на главной диагонале 
		//проверяе что матрицы квадратная 
		double det = 1.0;
		int n = this->row;
		//создаем верхне и нижне-треугольные матрицы
		matrix A = *this;
		matrix U (this->row, this->col);
		matrix L(this->row, this->col);
		
			for (int i = 0; i < n; i++) {
				// Calculate upper triangular matrix (U)
				for (int k = i; k < n; k++) {
					double sum = 0.;
					for (int j = 0; j < i; j++)
						sum += L.M[i][j] * U.M[j][k];
					U.M[i][k] = A.M[i][k] - sum;
				}

				// Calculate lower triangular matrix (L)
				for (int k = i; k < n; k++) 
				{
					if (i == k)
						L.M[i][i] = 1.;
					else 
					{
						double sum = 0.;
						for (int j = 0; j < i; j++)
							sum += L.M[k][j] * U.M[j][i];
						if
							(U.M[i][i] == 0) return 0.;
						else
							L.M[k][i] = (A.M[k][i] - sum) / U.M[i][i];
					}
				}
			}
		/*
		cout << ".........L........." << endl;
		cout << L;
		cout << endl << ".........U........." << endl;
		cout << U;
		*/
		det*= -1;
		for (int i = 0;i < U.rows();i++)
		{
			for (int j = 0;j < U.columns();j++)
				{
				if (i == j) {det *= -1 * U.M[i][j];}
				}
		}
		return det;
}

matrix matrix::solve()
{
	if (this->row + 1 != this->col)
		throw slv_equ_exc('s');
	matrix x(this->row, 1), y(this->row, 1), y1(this->row, 1);
	double max;
	int k = 0, index;
	for (int i = 0; i < this->row; i++)
	{
	  y.M[i][0] = this->M[i][this->col - 1];
	  y1.M[i][0] = this->M[i][this->col - 1];
	}
	
	matrix A(this->row, this->row);
	
	for (int i=0; i < this->row;i++)
	{
	   for (int j=0; j < this->row;j++)
		{
			A.M[i][j] = this->M[i][j];
		}
	}
	while (k < this->row)
	{
	// Поиск строки с максимальным a[i][k]
	max = abs(this->M[k][k]);
	index = k;
	for (int i = k + 1; i < this->row; i++)
	{
	  if (abs(this->M[i][k]) > max)
	  {
		max = abs(this->M[i][k]);
		index = i;
	  }
	}
	// Перестановка строк
	if (max < EPS)
	{
	  // нет ненулевых диагональных элементов
	  throw slv_equ_exc('n');
	}

	for (int j = 0; j < this->row; j++)
	{
		 double temp = this->M[k][j];
		  this->M[k][j] = this->M[index][j];
		  this->M[index][j] = temp;
	}
	double temp = y.M[k][0];
	y.M[k][0] = y.M[index][0];
	y.M[index][0] = temp;
	// Нормализация уравнений
	for (int i = k; i < this->row; i++)
	{
	  double temp = this->M[i][k];
	  if (abs(temp) < EPS) continue; // для нулевого коэффициента пропустить
	  for (int j = k; j < this->row; j++)
		this->M[i][j] = this->M[i][j] / temp;
	  y.M[i][0] = y.M[i][0] / temp;
	  if (i == k)  continue; // уравнение не вычитать само из себя
	  for (int j = 0; j < this->row; j++)
		this->M[i][j] = this->M[i][j] - this->M[k][j];
	  y.M[i][0] = y.M[i][0] - y.M[k][0];
	}
	
	k++;
	}
	// обратная подстановка
	for (k = this->row - 1; k >= 0; k--)
	{
		x.M[k][0] = y.M[k][0];
		for (int i = 0; i < k; i++)
		  y.M[i][0] = y.M[i][0] - this->M[i][k] * x.M[k][0];
	}
	if ((A*x) == y1)
		return x;
	else 
		throw slv_equ_exc('!');
}


matrix matrix::GetMatr(int row_i, int col_j)
 {
	int di = 0, dj;
	matrix New_Mat(this->row - 1, this->col - 1);
	for (int i = 0; i < this->row - 1; i++) { // проверка индекса строки
	if (i == row_i)  // строка совпала с вычеркиваемой
	  di = 1;    // - дальше индексы на 1 больше
	dj = 0;
	for (int j = 0; j < this->col - 1; j++) { // проверка индекса столбца
	  if (j == col)  // столбец совпал с вычеркиваемым
		dj = 1;    // - дальше индексы на 1 больше
	  New_Mat.M[i][j] =	this->M[i + di][j + dj];
	}
	}
	return New_Mat;
}
	
ostream & operator<< (ostream & s, const matrix_exception & e)
{
	switch (e.code)
	{
	case codes::dyn_mem_err:
		s<<"<exc> A dynamic memory error occured!";
	break;
	case codes::invalid_string:
		s<<"<exc> Invalid string used in constructor!";
	break;
	case codes::invalid_solve:
		s<<"<exc> An unknown error occured in a olve system!";
	break;
	case codes::unknown:
		s<<"<exc> An unknown error occured in a matrix method!";
	break;
	default:
		s<<"<exc> An exception throwed but caught inproperly!";
	break;


	}
	return s;
}

ostream & operator<< (ostream & s, const ind_exc & e)
{
	s<<"<exc> Invalid index used!";
	if (e.ind >= 0)
	{
		s<<" (";
		if (e.valid <= e.ind)
			s<<e.ind;
		else
			s<<e.ind+1;
		if (e.valid >= 0)
		{
			s<<" used, while ";
			if (e.valid < e.ind)
				s<<"maximum possible is "<<e.valid;
			else
				s<<"minimum possible is "<<e.valid+1;
		}
		s<<')';
	}
	return s;
}
ostream & operator<< (ostream & s, const str_exc & e)
{
	s<<"<exc> String constructor ";
	if (e.category != '\0')
		s<<e.category<<' ';
	s<<"incorrectly entered string!";
	switch(e.category)
	{
	case '{':
		s<<endl<<"(Error bracket: ";
		if (e.category == '}')
			s<<"right bracket '}' " << "row = " << e.m << " col = " << e.n << endl;
	break;
	case 's':
		s<<endl<<"(Error input number: ";
		s<<"The symbol can be '0'-'9'   "<< "row = " << e.m << " col = " << e.n << endl;
		s<<')';
	break;
	case 'n':
		s<<endl<< "(Error number columns";
		s << "must be: " << e.n << "Instead: " << e.n1 << ")" << endl;   
	break;
	case 'e':
		s<< endl << "(Error invalid string character";
		s << "must be: '}' or ',' " <<  "row = " << e.m << " col = " << e.n1 << ")" << endl;
	break;
	
	}
	return s;
}

ostream & operator<< (ostream & s, const incomp_exc & e)
{
	s<<"<exc> Arithmetical operation ";
	if (e.operation != '\0')
		s<<e.operation<<' ';
	s<<"operands are incompatible!";
	switch(e.category)
	{
	case 'r': case 'c':
		s<<endl<<"      (Different ";
		if (e.category == 'r')
			s<<"rows";
		else
			s<<"columns";
		s<<" number";
		if (e.first >= 0)
			s<<": the first operand's is "<<e.first;
		if (e.second >= 0)
			s<<", while the second operand's is "<<e.second;
		s<<')';
	break;
	case 'm':
		if (e.first >= 0)
			s<<endl<<"      (First operand has " <<e.first<<" columns";
		if (e.second >= 0)
			s<<" while the second one has "<<e.second<<" rows";
		s<<')';
	break;
	case '|': case '/':
			s<<endl<<"      (Different ";
		if (e.category == 'r')
			s<<"rows";
		else
			s<<"columns";
		s<<" number";
		if (e.first >= 0)
			s<<": the first operand's is "<<e.first;
		if (e.second >= 0)
			s<<", while the second operand's is "<<e.second;
		s<<')';
	break;
	}
	return s;
}

ostream & operator<< (ostream & s, const slv_equ_exc & e)
{
	s<<"<exc> Invalid solve sistem!";
	if (e.category != '\0')
		s<<e.category<<' ';
	s<<"incorrectly entered string!";
	switch(e.category)
	{
	case '!':
		s<<endl<<"(Error solve ";
		s<<"The solution not satisfy condition Ax = y)" << endl;
	break;
	case 'n':
		s<<endl<<"(Error solve ";
		s<<"The maximum element of the diagonal < EPS)" << endl;
	break;
	case 's':
		s<<endl<<"(Error solve ";
		s<<"The wrong size of the system must be n x n + 1)" << endl;
	break;
	}
	return s;
}
