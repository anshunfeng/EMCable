#ifndef _TOOLS_H_
#define _TOOLS_H_
#include "Custom.h"
#include <chrono>//计时

namespace tool {
	// 静态变量，记录时间点
	static  std::chrono::time_point<std::chrono::system_clock>  start = std::chrono::system_clock::now();
	static  std::chrono::time_point<std::chrono::system_clock>  prev = start;

	//定义函数，运行时间
	long long   elapsedTime(Qostream& strm, const Qstring& _description = "previous time cost");
	void        totalTime(Qostream& strm);

	// 定义函数，创建目录
	Qstring     creatFolder(const Qstring& _dir, const Qstring& foldername);


	class Percent {
	public:
		Percent();
		~Percent();
		void operator()(size_t cur, size_t total);
	private:
		size_t  old_;
	};

	class ProcessBar {
	public:
		ProcessBar();
		~ProcessBar();
		void operator()(size_t cur, size_t total);
	private:
		size_t  old_;
		Qstring str_;
	};

	class BarAndPercent {
	public:
		BarAndPercent();
		~BarAndPercent();
		void operator()(size_t cur, size_t total);
	private:
		size_t  old_per_;
		size_t  old_bar_;
		Qstring str_;
	};


} // namespace tool

#endif
