#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#pragma warning(disable:4996)

using std::cout;
using std::endl;
using std::swap;
using std::string;

class String{
public:
	String(const char* str = ""){
		_size = strlen(str);
		_str = new char[_size + 1];
		_capacity = _size;
		//拷贝输入字符串到对象的资源空间中
		strcpy(_str, str);
	}
	String(const String& str)
		:_str(nullptr)
		, _capacity(0)
		, _size(0){
		String tmp(str._str);   // 创建临时对象式已经完成了开空间和内容拷贝, 
		Swap(tmp);  // 所以现在只需要进行交换即可
	}
	void Swap(String& str){
		swap(str._str, _str);
		swap(str._capacity, _capacity);
		swap(str._size, _size);
	}
	String& operator=(String str){
		Swap(str);
		return *this;
	}

	const char* c_str() const{
		return _str;
	}

	~String(){
		if (_str){
			delete[] _str;
			_str = nullptr;
			_capacity = _size = 0;
		}
	}

	//尾插一个字符
	void pushBack(const char ch){
		////检查容量
		//if (_size == _capacity){
		//	size_t newC = _capacity == 0 ? 15 : 2 * _capacity;
		//	reserve(newC);  // 借助reserve增容接口先检查容量看是否需要增容
		//}
		////尾插
		//_str[_size++] = ch;
		////字符串以\0结束
		//_str[_size] = '\0';

		insert(_size, ch);

	}

	//增容：只增加容量
	void reserve(size_t n){
		if (n > _capacity){
			//开空间
			char* tmp = new char[n + 1];
			//拷内容
			strcpy(tmp, _str);
			//释放原有空间 (已经开了更大的空间了)  先开一个空间 把原有内容考过去, 才能释放原有空间
			delete[] _str;
			_str = tmp;
			//更新容量
			_capacity = n;
		}
	}

	//尾插： 字符串
	void Append(const char* str){
		////检查容量
		//int len = strlen(str);
		//if (_size + len > _capacity){
		//	//注意，不能简单二倍增容，有可能len远大于容量，有多大,就给多大
		//	reserve(_size + len);
		//}
		////尾插
		//strcpy(_str + _size, str);
		////更新_size
		//_size += len;

		insert(_size, str);
	}

	// += 修改左操作数内容
	String& operator+=(const char& ch){
		pushBack(ch);
		return *this;
	}

	String& operator+=(const char* str){
		Append(str);
		return *this;
	}
	String& operator+=(const String& str){
		// 加一个string对象 也是加一个对象的字符串
		Append(str._str);
		return *this;
	}

	void insert(size_t pos, const char& ch){
		if (pos <= _size){
			//插入的位置：[0, _size]
			//检查容量
			if (_size == _capacity){
				size_t newC = _capacity == 0 ? 15 : 2 * _capacity;
				reserve(newC);
			}
			//从后向前移动元素， 区间： [pos, _size]
			size_t end = _size + 1;
			//有风险的循环：while(end >= pos)-->如果pos为0时，是死循环
			while (end > pos){
				//首先移动的元素为：'\0'
				_str[end] = _str[end - 1];
				--end;
			}
			//在pos位置插入新的字符
			_str[pos] = ch;
			//更新size
			++_size;
		}
	}

	void insert(size_t pos, const char* str){
		if (pos <= _size){
			//检查容量
			int len = strlen(str);
			if (_size + len > _capacity){
				reserve(_size + len);
			}
			//从后向前移动元素
			size_t end = _size + len;
			while (end > pos + len - 1){
				//每个元素向后移动len个位置
				_str[end] = _str[end - len];
				--end;
			}
			//插入字符串
			//不要用strcpy: \0会同时拷贝过去，提前终止字符串内容
			//strcpy(_str + pos, str);
			for (int i = 0; i < len; ++i){
				_str[i + pos] = str[i];
			}

			//更新size
			_size += len;
		}
	}

	void erase(size_t pos, size_t len){
		if (pos + len >= _size){
			//要删除的元素大于区间的元素总数，区间的元素全部删除，更新size以及字符串结束标记\0
			_size = pos;
			_str[_size] = '\0';
		}
		else{
			//元素从前向后移动，每个元素向前移动len个位置
			for (int i = pos + len; i <= _size; ++i){
				_str[pos++] = _str[i];
			}
			_size -= len;
		}
	}

	//n < size : 修改size
	// size < n < capacity:  修改size + 赋值
	// n > capacity： 增容 +  修改size + 赋值
	void resize(size_t n, const char& ch = '\0'){
		//增容
		if (n > _capacity){
			reserve(n);
		}
		//赋值
		if (n > _size){
			memset(_str + _size, ch, n - _size);
		}
		//修改size
		_size = n;
		_str[_size] = '\0';
	}
	// 查找字符串 返回一个位置
	size_t find(const char* str){
		char* start = strstr(_str, str); // 这个函数返回一个指针
		// 找不到 返回NULL
		if (start){
			return start - str; // 返回的是第一次出现的首地址
		}
		return npos;
	}

	// 输出运算符重载函数



private:
	char* _str;
	//_size: 有效字符的个数，不包含\0
	size_t _size;
	//_capacity: 能够存放最大有效字符的个数
	size_t _capacity;
	static const size_t npos; //静态成员在类外初始化
};
const size_t String::npos = -1;

//需要输出size个字符，不能遇到\0就结束输出
ostream& operator<<(ostream& _cout, const String& str){
	for (const auto& ch : str){
		cout << ch;
	}
	return _cout;
}

//+运算符重载函数： 是非成员函数， 效率低（返回值为值传递），不要频繁使用此接口
String operator+(const String& str, const char& ch){
	String tmp(str);
	tmp += ch;
	return tmp;
}

String operator+(const String& str, const char* str2){
	String tmp(str);
	tmp += str2;
	return tmp;
}

String operator+(const String& str, const String& str2){
	String tmp(str);
	tmp += str2;
	return tmp;
}

void test7(){
	String s("1234");
	s.pushBack('a');
	cout << s.c_str() << endl;
	s.Append("bcdefghijklmnopqr");
	cout << s.c_str() << endl;
	s += 'M';
	/*char c = 'a';
	char& pc = c;
	const char& pc2 = 'a';*/
	cout << s.c_str() << endl;

	String s2;
	s2 += s;
	cout << s2.c_str() << endl;
	s2 += "12345";
	cout << s2.c_str() << endl;
}

void test8(){
	String s;
	s.insert(0, '0'); // 0
	s.insert(0, 'f'); // f0
	s.insert(0, 'e'); //ef0
	//尾插
	s.insert(3, '1');  //ef01
	s.insert(1, '9');  // e9f01
	s.insert(3, 'g');  //e9fg01
}

void test9(){
	String s("5678");
	s.insert(0, "1234");
	cout << s.c_str() << endl;
	s.insert(8, "910");
	cout << s.c_str() << endl;
	s.insert(4, "abcd");
	cout << s.c_str() << endl;
}

void test10(){
	String s("0123456789abcdefgh");
	cout << s.c_str() << endl;
	s.erase(5, 5);  //01234abcedfgh
	cout << s.c_str() << endl;
	s.erase(0, 3);  //34abcdefgh
	cout << s.c_str() << endl;
	s.erase(7, 15);//34abcde
	cout << s.c_str() << endl;
	s.erase(4, 3); //34ab
	cout << s.c_str() << endl;
}

void test11(){
	String s("1111");
	cout << s.c_str() << endl;
	//增容 + 赋值 + 修改size
	s.resize(10, 'a');  //1111aaaaaa
	cout << s.c_str() << endl;
	//修改size
	s.resize(5, 'b');  //1111a
	cout << s.c_str() << endl;
	s += 'b';  //1111ab
	//赋值 + 修改
	s.resize(8, 'c');  //1111abcc

}
void test12(){
	String str("0123456789");
	size_t pos = str.find("456");// 返回4
	pos = str.find("abc");// 找不到
}
void test13(){
	String s("123");
	cout << s << endl;
	string s2("123");
	cout << s2 << endl;
	s2.resize(10);
	s.resize(10);
	cout << s2 << "结束" << endl; // 123与结束之间有空格
	cout << s << "结束" << endl; // 没有空格, '123结束'
}

void test14()
{
	// 拼接
	String s1("123");
	String s2("456");
	String ret = s1 + 'a';
	cout << s1 << endl;
	cout << ret << endl;
	ret = s1 + "abc";
	cout << s1 << endl;
	cout << ret << endl;
	ret = s1 + s2;
	cout << s1 << endl;
	cout << s2 << endl;
	cout << ret << endl;
}