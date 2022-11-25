#include <iostream>
#include <limits>

using namespace std;

template <typename T>
class image;

template <typename T>
ostream& operator << (ostream& out, const image<T>& a);

template <typename T>
image<T> operator + (T val, const image<T>& a);

template <typename T>
image<T> operator * (T val, const image<T>& a);

template <typename T>
class image
{
private:
	T** _a;
	size_t _size;

	void allocate()
	{
		_a = new T* [_size];
		for (int i = 0; i < _size; i++)
		{
			_a[i] = new T[_size];
		}
	}

	void clear()
	{
		for (size_t i = 0; i < _size; i++)
		{
			delete[] _a[i];
		}
		delete[] _a;
		_a = nullptr;
	}

public:
	image()
	{
		_a = nullptr;
		_size = 0;
	}

	image(size_t size)
	{
		_size = size;
		allocate();
	}

	image(const image& a)
	{
		_size = a._size;
		allocate();
		for (int j = 0; j < _size; j++)
		{
			for (int k = 0; k < _size; k++)
			{
				_a[j][k] = a._a[j][k];
			}
		}
	}

	~image()
	{
		clear();
	}

	image& operator = (const image& a)
	{
		for (int j = 0; j < _size; j++)
		{
			for (int k = 0; k < _size; k++)
			{
				_a[j][k] = a._a[j][k];
			}
		}
		return (*this);
	}

	T& operator () (int row, int col)
	{
		if (row < 0 || col < 0) throw("Invalid index");
		return _a[row][col];
	}

	image& operator ! ()
	{
		for (int j = 0; j < _size; j++)
		{
			for (int k = 0; k < _size; k++)
			{
				_a[j][k] = ~_a[j][k];
			}
		}
		return (*this);
	}

	image& operator += (const image& a)
	{
		if (_size != a._size) throw("Different dimensions");
		for (int j = 0; j < _size; j++)
		{
			for (int k = 0; k < _size; k++)
			{
				_a[j][k] += a._a[j][k];
			}
		}
		return (*this);
	}

	image& operator += (T val)
	{
		for (int j = 0; j < _size; j++)
		{
			for (int k = 0; k < _size; k++)
			{
				_a[j][k] = (_a[j][k] != 0);
				val = (val != 0);
				_a[j][k] += val;
			}
		}
		return (*this);
	}

	image operator + (T val) const
	{
		image tmp(*this);
		tmp += val;
		return tmp;
	}

	image operator + (const image& a) const
	{
		if (a._size != _size) throw("Different dimensions");
		image tmp(*this);
		tmp += a;
		return tmp;
	}

	friend image operator + (T val, const image<T>& a)
	{
		image tmp(a);
		tmp += val;
		return tmp;
	}

	image& operator *= (const image& a)
	{
		if (a._size != _size) throw("Different dimensions");
		for (int j = 0; j < _size; j++)
		{
			for (int k = 0; k < _size; k++)
			{
				_a[j][k] *= a._a[j][k];
			}
		}
		return (*this);
	}

	image& operator *= (T val)
	{
		for (int j = 0; j < _size; j++)
		{
			for (int k = 0; k < _size; k++)
			{
				_a[j][k] *= val;
			}
		}
		return (*this);
	}

	image operator * (const image& a) const
	{
		image tmp(*this);
		tmp *= a;
		return tmp;
	}

	image operator * (T val) const
	{
		image tmp(*this);
		tmp *= val;
		return tmp;
	}

	friend image operator * (T val, const image& a)
	{
		image tmp(a);
		tmp *= val;
		return tmp;
	}

	friend ostream& operator << (ostream& out, const image<T>& a);

	double filling() const
	{
		int count_0 = 0, count_1 = 0;
		for (int j = 0; j < _size; j++)
		{
			for (int k = 0; k < _size; k++)
			{
				if (_a[j][k] == 0) count_0++;
				else count_1++;
			}
		}
		return (double)count_1 / (count_0 + count_1);
	}

	image& circle(int x, int y, int r)
	{
		if (r <= 0) throw("Invalid radius");
		if (x < r || y < r || x <= 0 || y <= 0) throw("Invalid centre");

		clear();

		_size = 30;

		allocate();

		for (int j = 0; j < _size; j++)
		{
			for (int k = 0; k < _size; k++)
			{
				_a[j][k] = 0;
			}
		}

		for (int j = x - r; j < x + r + 1; j++)
		{
			for (int k = y - r; k < y + r + 1; k++)
			{
				_a[j][k] = ((j - x)*(j - x) + (k - y)*(k - y) <= r*r);
			}
		}
		return (*this);
	}
};


image<short>& image<short>::operator += (const image<short>& a)
{
	if (_size != a._size) throw("Different dimensions");
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			int b = _a[j][k];
			int c = a._a[j][k];
			if (b + c > SHRT_MAX || b + c < SHRT_MIN) throw("Overflow of type");
			_a[j][k] += a._a[j][k];
		}
	}
	return (*this);
}

image<short>& image<short>::operator += (short val)
{
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			int b = _a[j][k];
			int c = val;
			if (b + c > SHRT_MAX || b + c < SHRT_MIN) throw("Overflow of type");
			_a[j][k] += val;
		}
	}
	return (*this);
}

image<short> image<short>::operator + (short val) const
{
	image tmp(*this);
	tmp += val;
	return tmp;
}

image<short> image<short>::operator + (const image<short>& a) const
{
	if (a._size != _size) throw("Different dimensions");
	image tmp(*this);
	tmp += a;
	return tmp;
}

template<>
image<short> operator + (short val, const image<short>& a)
{
	image<short> tmp(a);
	tmp += val;
	return tmp;
}

image<short>& image<short>::operator *= (const image<short>& a)
{
	if (a._size != _size) throw("Different dimensions");
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			int b = _a[j][k];
			int c = a._a[j][k];
			if (b * c > SHRT_MAX || b * c < SHRT_MIN) throw("Overflow of type");
			_a[j][k] *= a._a[j][k];
		}
	}
	return (*this);
}

image<short>& image<short>::operator *= (short val)
{
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			int b = _a[j][k];
			int c = val;
			if (b * c > SHRT_MAX || b * c < SHRT_MIN) throw("Overflow of type");
			_a[j][k] *= val;
		}
	}
	return (*this);
}

image<short> image<short>::operator * (const image<short>& a) const
{
	image tmp(*this);
	tmp *= a;
	return tmp;
}

image<short> image<short>::operator * (short val) const
{
	image tmp(*this);
	tmp *= val;
	return tmp;
}

template<>
image<short> operator * (short val, const image<short>& a)
{
	image<short> tmp(a);
	tmp *= val;
	return tmp;
}

ostream& operator << (ostream& out, const image<short>& a)
{
	for (int j = 0; j < a._size; j++)
	{
		for (int k = 0; k < a._size; k++)
		{
			out.width(6);
			out << right << a._a[j][k] << " ";
		}
		out << endl;
	}
	return out;
}


image<float>& image <float> ::operator ! ()
{
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
				_a[j][k] = -_a[j][k];
		}
	}
	return (*this);
}

image<float>& image<float>::operator += (const image<float>& a)
{
	if (_size != a._size) throw("Different dimensions");
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			double b = _a[j][k];
			double c = a._a[j][k];
			if (b + c < FLT_MIN && b + c > -FLT_MIN) throw("Overflow of type");
			else if (b + c > FLT_MAX || b + c < -FLT_MAX) throw("Overflow of type");
			_a[j][k] += a._a[j][k];
		}
	}
	return (*this);
}

image<float>& image<float>::operator += (float val)
{
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			double b = _a[j][k];
			double c = val;
			if (b + c < FLT_MIN && b + c > -FLT_MIN) throw("Overflow of type");
			else if (b + c > FLT_MAX || b + c < -FLT_MAX) throw("Overflow of type");
			_a[j][k] += val;
		}
	}
	return (*this);
}

image<float> image<float>::operator + (float val) const
{
	image tmp(*this);
	tmp += val;
	return tmp;
}

image<float> image<float>::operator + (const image<float>& a) const
{
	if (a._size != _size) throw("Different dimensions");
	image tmp(*this);
	tmp += a;
	return tmp;
}

template<>
image<float> operator + (float val, const image<float>& a)
{
	image <float> tmp(a);
	tmp += val;
	return tmp;
}

image<float>& image<float>::operator *= (const image<float>& a)
{
	if (_size != a._size) throw("Different dimensions");
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			double b = _a[j][k];
			double c = a._a[j][k];
			if (b + c < FLT_MIN && b + c > -FLT_MIN) throw("Overflow of type");
			else if (b + c > FLT_MAX || b + c < -FLT_MAX) throw("Overflow of type");
			_a[j][k] *= a._a[j][k];
		}
	}
	return (*this);
}

image<float>& image<float>::operator *= (float val)
{
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			double b = _a[j][k];
			double c = val;
			if (b + c < FLT_MIN && b + c > -FLT_MIN) throw("Overflow of type");
			else if (b + c > FLT_MAX || b + c < -FLT_MAX) throw("Overflow of type");
			_a[j][k] *= val;
		}
	}
	return (*this);
}

image <float> image<float>::operator * (const image<float>& a) const
{
	image tmp(*this);
	tmp *= a;
	return tmp;
}

image<float> image<float>::operator * (float val) const
{
	image tmp(*this);
	tmp *= val;
	return tmp;
}

template<>
image<float> operator * (float val, const image<float>& a)
{
	image<float> tmp(a);
	tmp *= val;
	return tmp;
}

ostream& operator << (ostream& out, const image<float>& a)
{
	for (int j = 0; j < a._size; j++)
	{
		for (int k = 0; k < a._size; k++)
		{
			out.width(13);
			out.setf(ios::scientific);
			out << right << a._a[j][k] << " ";
		}
		out << endl;
	}
	return out;
}


image<char>& image <char> ::operator ! ()
{
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			_a[j][k] = UCHAR_MAX - _a[j][k];
		}
	}
	return (*this);
}

image<char>& image<char>::operator += (const image<char>& a)
{
	if (_size != a._size) throw("Different dimensions");
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			int b = _a[j][k];
			int c = a._a[j][k];
			if (b + c > UCHAR_MAX) throw("Overflow of type");
			_a[j][k] += a._a[j][k];
		}
	}
	return (*this);
}

image<char>& image<char>::operator += (char val)
{
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			int b = _a[j][k];
			int c = val;
			if (b + c > UCHAR_MAX) throw("Overflow of type");
			_a[j][k] += val;
		}
	}
	return (*this);
}

image<char> image<char>::operator + (char val) const
{
	image tmp(*this);
	tmp += val;
	return tmp;
}

image<char> image<char>::operator + (const image<char>& a) const
{
	if (a._size != _size) throw("Different dimensions");
	image tmp(*this);
	tmp += a;
	return tmp;
}

template<>
image<char> operator + (char val, const image<char>& a)
{
	image<char> tmp(a);
	tmp += val;
	return tmp;
}

image<char>& image<char>::operator *= (const image<char>& a)
{
	if (a._size != _size) throw("Different dimensions");
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			int b = _a[j][k];
			int c = a._a[j][k];
			if (b * c > UCHAR_MAX) throw("Overflow of type");
			_a[j][k] *= a._a[j][k];
		}
	}
	return (*this);
}

image<char>& image<char>::operator *= (char val)
{
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			int b = _a[j][k];
			int c = val;
			if (b * c > UCHAR_MAX) throw("Overflow of type");
			_a[j][k] *= val;
		}
	}
	return (*this);
}

image<char> image<char>::operator * (const image<char>& a) const
{
	image tmp(*this);
	tmp *= a;
	return tmp;
}

image<char> image<char>::operator * (char val) const
{
	image tmp(*this);
	tmp *= val;
	return tmp;
}

template<>
image<char> operator * (char val, const image<char>& a)
{
	image<char> tmp(a);
	tmp *= val;
	return tmp;
}

ostream& operator << (ostream& out, const image<char>& a)
{
	for (int j = 0; j < a._size; j++)
	{
		for (int k = 0; k < a._size; k++)
		{
			out << a._a[j][k] << " ";
		}
		out << endl;
	}
	return out;
}


ostream& operator << (ostream& out, const image<bool>& a)
{
	for (int j = 0; j < a._size; j++)
	{
		for (int k = 0; k < a._size; k++)
		{
			out << a._a[j][k] << " ";
		}
		out << endl;
	}
	return out;
}

image<bool>& image <bool> ::operator ! ()
{
	for (int j = 0; j < _size; j++)
	{
		for (int k = 0; k < _size; k++)
		{
			if (_a[j][k] == 1) _a[j][k] = 0;
			else _a[j][k] = 1;
		}
	}
	return (*this);
}

int main()
{

	image <char> a(2), b(2); 
	image <short> c(2), d(2);
	image <float> e(2), f(2);
	image <bool> g(2), h(2);

	a(0, 0) = 'a';
	a(0, 1) = 'a';
	a(1, 0) = 'a';
	a(1, 1) = 'a';

	b(0, 0) = 'b';
	b(0, 1) = 'b';
	b(1, 0) = 'b';
	b(1, 1) = 'b';

	c(0, 0) = 0;
	c(0, 1) = 5664;
	c(1, 0) = 3200;
	c(1, 1) = 233;

	d(0, 0) = -345;
	d(0, 1) = -8945;
	d(1, 0) = 700;
	d(1, 1) = 498;

	e(0, 0) = 1.4543;
	e(0, 1) = -34845.345;
	e(1, 0) = 700.9;
	e(1, 1) = 0.0f;

	f(0, 0) = -15.98703;
	f(0, 1) = 2500.56;
	f(1, 0) = 700;
	f(1, 1) = 0.0f;

	g(0, 0) = 0;
	g(0, 1) = 56;
	g(1, 0) = 3200;
	g(1, 1) = 67;

	h(0, 0) = 0;
	h(0, 1) = 1;
	h(1, 0) = 0;
	h(1, 1) = 233;

	cout << "a" << endl << a << endl << "b" << endl << b << endl; 
	cout << "c" << endl << c << endl << "d" << endl << d << endl;
	cout << "e" << endl << e << endl << "f" << endl << f << endl;
	cout << "g" << endl << g << endl << "h" << endl << h << endl;

	try 
	{
		a += b;
		cout << a << endl;
	}
	catch (const char* err)
	{
		cout << err << endl;
	}

	cout << !a << endl;

}