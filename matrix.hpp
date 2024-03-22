#include <iostream>

using namespace std;

enum class codes
{
	dyn_mem_err,
	index_out_bounds,
	invalid_string,
	incomp_sizes,
	invalid_solve,
	unknown
};

class matrix_exception
{
	codes code;
public:
	matrix_exception(codes c = codes::unknown) {code = c;}
	friend ostream & operator<< (ostream & s, const matrix_exception & e);
};

//Invalid index exception subclass
class ind_exc: public matrix_exception
{
	int ind, valid;
public:
	ind_exc(int i = -1, int v = -1): matrix_exception(codes::index_out_bounds) {ind = i; valid = v;}
	friend ostream & operator<< (ostream & s, const ind_exc & e);
};

class str_exc: public matrix_exception
{
	int n, n1, m;
	char category; //'{' for '{', 's' for err_simbol, 'n' for compare n and n1, 'e' error end needs '}' or ','
public:
	str_exc(char cat = '\0', int _n = 0, int _n1 = 0,int _m = 0):
		matrix_exception(codes::invalid_string) {category = cat; n =_n; n1 = _n1; m = _m;}
	friend ostream & operator<< (ostream & s, const str_exc & e);
};
//Incompatible matrices exception subclass

class incomp_exc: public matrix_exception
{
	char operation; //'+' / '-' / '*' / '|' / '/' / '=' / '!'
	char category; //'r' for rows, 'c' for columns, 'm' for multiplication
	int first, second;
public:
	incomp_exc(char oper = '\0', char cat = '\0', int fst = -1, int snd = -1):
		matrix_exception(codes::incomp_sizes) {operation = oper; category = cat; first = fst; second = snd;}
	friend ostream & operator<< (ostream & s, const incomp_exc & e);
};

class slv_equ_exc: public matrix_exception
{
	char category; //'!' if Ax != y,  'n' if max_el < EPS 
public:
	slv_equ_exc(char cat = '\0'):
		matrix_exception(codes::invalid_solve) {category = cat;}
	friend ostream & operator<< (ostream & s, const slv_equ_exc & e);
};


class matrix
{
	
	int row, col; //rows and columns
	double** M; //content; array of rows
	matrix GetMatr(int row_i, int col_j);
public:
	
	static double EPS;
	
	//accuracy in comparing
	matrix(int n, int m); //zero matrix nxm+
	matrix(double val = 0); //1x1 matrix with val element; includes default+
	matrix(double* arr, int m); //1xm row-matrix+
	matrix(int n, double* arr); //nx1 column-matrix+
	matrix(const char* arr); //from string+
	static matrix identity(int n); //identity matrix nxn+
	static matrix diagonal(double* vals, int n); //diagonal matrix nxn+
	matrix(const matrix & m); //copy 
	~matrix(); //destructor+
	int rows() const {return row;} //rows number+
	int columns() const {return col;}  //columns number+
	void set(const int i, const int j, const double val); //set matrix[i][j]=val
	const matrix operator [](const int i);//+
	const matrix operator [] (const int i) const;
	matrix operator*= (const double scalar); //matrix x scalar
	matrix operator* (const double scalar); //matrix x scalar
	friend ostream & operator<< (ostream & s, const matrix & m); //print matrix
	matrix operator = (const matrix& arr);
	matrix operator+= (const matrix & arr);
	matrix operator+ (const matrix & arr) const;
	matrix operator-= (const matrix & arr);
	matrix operator- (const matrix & arr) const;
	matrix operator*= (const matrix & arr);
	matrix operator* (const matrix & arr) const;
	matrix operator- (void);
	bool operator== (const matrix & arr) const;
	bool operator!= (const matrix & arr) const;
	matrix operator| (const matrix & arr) const;
	matrix operator/ (const matrix & arr) const; 
	matrix Transpone ();
	matrix invers();
	double determinant() const;
	matrix solve();
};
