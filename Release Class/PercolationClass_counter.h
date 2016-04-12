#pragma once

class counter
{
	typedef std::vector<int> counters;
public:
	counter(int $d, int $m) :
		data($d, 0),
		d($d),
		m($m)
	{

	}
	void assignCounter(int, int);	// reset counter
	void pushIn(int, long);			// put data into counter
	counters getCounter(void);		// get data from counter
private:
	std::vector<int> data;
	int d, m;
};

inline void counter::assignCounter(int $d, int $m)
{
	d = $d;
	m = $m;
	data.assign($d, 0);
}

inline void counter::pushIn(int bond, long move)
{
	int r = (move + 1) / m;
	if (r <= data[bond])
	{
		return;
	}
	data[bond] = r;
}

inline counter::counters counter::getCounter(void)
{
	/*返回已得的数据，并清除旧数据*/
	counters temp_c(d, 0);

	std::swap(temp_c, data);
	assignCounter(d, m);

	return temp_c;
}