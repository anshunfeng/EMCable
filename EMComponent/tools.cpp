#include "tools.h"
#include <filesystem> // 文件系统操作
#include <iomanip> // 控制输出格式

namespace fs = std::experimental::filesystem;
namespace ct = std::chrono;

//  time cost from previous process end to current process end
//  description : cost time(ms)
//  return time cost，计算程序运行时间
long long tool::elapsedTime(Qostream& strm, const Qstring& _description)
{
	auto oldState = strm.flags(); // 读取/设置流的格式
	strm << std::left; // 左对齐
	
	auto current = ct::system_clock::now();// 现在时间点
	auto period = ct::duration_cast<ct::milliseconds>(current - prev).count();
	if (period < 1000)
	{
		strm << '[' << std::setw(29) << _description + ':' <<  period << " ms]\n";
	}
	else
	{
		//period = (float)((int)(period * 0.01)) / 10;
		period = period / 1000.0;
		strm << '[' << std::setw(29) << _description + ':' << period << " s]\n";
	}
	
	strm.flags(oldState);
	prev = current;
	return period;
}

//  total runtime cost，计算总的时间
void tool::totalTime(Qostream & strm)
{
	auto oldState = strm.flags();
	strm << std::left;
	auto end = ct::system_clock::now();
	auto period = ct::duration_cast<ct::seconds>(end - start).count();
	strm << '[' << std::setw(29) << "total time:" << period << " s]\n";
	strm.flags(oldState);
}

//  creat folder(absolute path)
//  Path: m_dir/foldername
//  return new path，创建目录
Qstring tool::creatFolder(const Qstring & _dir, const Qstring & foldername)
{
	//auto dir = _dir + '/' + foldername;
	auto dir = _dir  + foldername;
	//fs::create_directory(fs::v1::path(dir));
	return dir;
}

tool::Percent::Percent()
	: old_(0)
{
	Qcout << " 0%" << std::flush;
}

tool::Percent::~Percent()
{
}

void tool::Percent::operator()(size_t cur, size_t total)
{
	size_t num = 100 * cur / total;
	if (num != old_)
	{
		old_ = num;
		Qcout << "\b\b\b" << (num < 10 ? " " : "") << num << '%' << std::flush;
	}
}

tool::ProcessBar::ProcessBar()
	: old_(0), str_("[....................]")
{
	Qcout << str_ << std::flush;
}

tool::ProcessBar::~ProcessBar()
{
}

void tool::ProcessBar::operator()(size_t cur, size_t total)
{
	size_t num = 20 * cur / total;
	if (num != old_)
	{
		if (old_ + 1 != num)
		{
			for (size_t i = 1; i < num; ++i)
				str_[i] = '#';
		}
		old_ = num;
		str_[num] = '#';
		Qcout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << str_ << std::flush;
	}
}

tool::BarAndPercent::BarAndPercent()
	: old_per_(0), old_bar_(0), str_("[....................]")
{
	Qcout << str_ << "  0%" << std::flush;
}

tool::BarAndPercent::~BarAndPercent()
{
}

void tool::BarAndPercent::operator()(size_t cur, size_t total)
{
	size_t num_per = 100 * cur / total;
	size_t num_bar = 20 * cur / total;
	if (num_per != old_per_ || num_bar != old_bar_)
	{
		old_per_ = num_per;
		Qcout << "\b\b\b\b";
		if (num_bar != old_bar_)
		{
			if (old_bar_ + 1 != num_bar)
				for (size_t i = 1; i < num_bar; ++i)
					str_[i] = '#';
			old_bar_ = num_bar;
			str_[num_bar] = '#';
			Qcout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << str_;
		}
		Qcout << (num_per < 10 ? "  " : " ") << num_per << '%' << std::flush;
	}
}
