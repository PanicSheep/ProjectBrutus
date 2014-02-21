#pragma once
#include <atomic>

template<class T>
class CRunningStatistic
{
public:
	CRunningStatistic()
		: _N(0),
		_E_of_X(0),
		_E_of_X_sq(0),
		_min(0),
		_max(0)
	{ spinlock.clear(); }

	void Add(T value)
	{
		while (spinlock.test_and_set(std::memory_order_acquire)) ;

		if (_N == 0){
			_min = value;
			_max = value;
		}
		++_N;
		if (value < _min)
			_min = value;
		if (_max < value) 
			_max = value;
		_E_of_X += (static_cast<double>(value) - _E_of_X) / static_cast<double>(_N);
		_E_of_X_sq += (static_cast<double>(value)*static_cast<double>(value) - _E_of_X_sq) / static_cast<double>(_N);

		spinlock.clear(std::memory_order_release);
	}

	inline unsigned long long N() const { return _N; }
	inline unsigned long long size() const { return N(); }

	inline double mu() const { return _E_of_X; }
	inline double Average() const { return mu(); }

	inline double Var() const { return _E_of_X_sq - _E_of_X*_E_of_X; }
	inline double Variance() const { return Var(); }

	inline double sigma() const { return std::sqrt(Var()); }
	inline double StandardDeviation() const { return sigma(); }

	inline double RelativeError() const { return sigma()/mu(); }

	inline double Min() const { return _min; }
	inline double Max() const { return _max; }

	CRunningStatistic& operator+=(const CRunningStatistic & rhs)
	{
		unsigned long long sum = this->_N + rhs._N;

		this->_E_of_X = this->_E_of_X * (static_cast<double>(this->_N) / static_cast<double>(sum)) 
					  + rhs._E_of_X * (static_cast<double>(rhs._N) / static_cast<double>(sum));
		this->_E_of_X_sq = this->_E_of_X_sq * (static_cast<double>(this->_N) / static_cast<double>(sum))
						 + rhs._E_of_X_sq * (static_cast<double>(rhs._N) / static_cast<double>(sum));
		this->_N = sum;
		if (rhs._min < this->_min)
			this->_min = rhs._min;
		if (this->_max < rhs._max) 
			this->_max = rhs._max;

		return *this;
	}
	CRunningStatistic operator+(const CRunningStatistic & rhs) const
	{
		RunningStatistic<T> res = RunningStatistic<T>();
		res._N = this->_N + rhs._N;
		res._E_of_X = this->_E_of_X * (static_cast<double>(this->_N) / static_cast<double>(res._N))
					+ rhs._E_of_X * (static_cast<double>(rhs._N) / static_cast<double>(res._N));
		res._E_of_X_sq = this->_E_of_X_sq * (static_cast<double>(this->_N) / static_cast<double>(res._N)) 
					   + rhs._E_of_X_sq * (static_cast<double>(rhs._N) / static_cast<double>(res._N));
		res._min = (this->_min < rhs._min) ? this->_min : rhs._min;
		res._max = (rhs._max < this->_max) ? this->_max : rhs._max;
		return res;
	}

private:
	std::atomic_flag spinlock;
	unsigned long long _N;
	double _E_of_X;
	double _E_of_X_sq;
	T _min;
	T _max;
};