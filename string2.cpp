#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
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
		//���������ַ������������Դ�ռ���
		strcpy(_str, str);
	}
	String(const String& str)
		:_str(nullptr)
		, _capacity(0)
		, _size(0){
		String tmp(str._str);   // ������ʱ����ʽ�Ѿ�����˿��ռ�����ݿ���, 
		Swap(tmp);  // ��������ֻ��Ҫ���н�������
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

	char* c_str(){
		return _str;
	}

	~String(){
		if (_str){
			delete[] _str;
			_str = nullptr;
			_capacity = _size = 0;
		}
	}

	//β��һ���ַ�
	void pushBack(const char ch){
		////�������
		//if (_size == _capacity){
		//	size_t newC = _capacity == 0 ? 15 : 2 * _capacity;
		//	reserve(newC);  // ����reserve���ݽӿ��ȼ���������Ƿ���Ҫ����
		//}
		////β��
		//_str[_size++] = ch;
		////�ַ�����\0����
		//_str[_size] = '\0';

		insert(_size, ch);

	}

	//���ݣ�ֻ��������
	void reserve(size_t n){
		if (n > _capacity){
			//���ռ�
			char* tmp = new char[n + 1];
			//������
			strcpy(tmp, _str);
			//�ͷ�ԭ�пռ� (�Ѿ����˸���Ŀռ���)  �ȿ�һ���ռ� ��ԭ�����ݿ���ȥ, �����ͷ�ԭ�пռ�
			delete[] _str;
			_str = tmp;
			//��������
			_capacity = n;
		}
	}

	//β�壺 �ַ���
	void Append(const char* str){
		////�������
		//int len = strlen(str);
		//if (_size + len > _capacity){
		//	//ע�⣬���ܼ򵥶������ݣ��п���lenԶ�����������ж��,�͸����
		//	reserve(_size + len);
		//}
		////β��
		//strcpy(_str + _size, str);
		////����_size
		//_size += len;

		insert(_size, str);
	}

	String& operator+=(const char& ch){
		pushBack(ch);
		return *this;
	}

	String& operator+=(const char* str){
		Append(str);
		return *this;
	}
	String& operator+=(const String& str){
		// ��һ��string���� Ҳ�Ǽ�һ��������ַ���
		Append(str._str);
		return *this;
	}

	void insert(size_t pos, const char& ch){
		if (pos <= _size){
			//�����λ�ã�[0, _size]
			//�������
			if (_size == _capacity){
				size_t newC = _capacity == 0 ? 15 : 2 * _capacity;
				reserve(newC);
			}
			//�Ӻ���ǰ�ƶ�Ԫ�أ� ���䣺 [pos, _size]
			size_t end = _size + 1;
			//�з��յ�ѭ����while(end >= pos)-->���posΪ0ʱ������ѭ��
			while (end > pos){
				//�����ƶ���Ԫ��Ϊ��'\0'
				_str[end] = _str[end - 1];
				--end;
			}
			//��posλ�ò����µ��ַ�
			_str[pos] = ch;
			//����size
			++_size;
		}
	}

	void insert(size_t pos, const char* str){
		if (pos <= _size){
			//�������
			int len = strlen(str);
			if (_size + len > _capacity){
				reserve(_size + len);
			}
			//�Ӻ���ǰ�ƶ�Ԫ��
			size_t end = _size + len;
			while (end > pos + len - 1){
				//ÿ��Ԫ������ƶ�len��λ��
				_str[end] = _str[end - len];
				--end;
			}
			//�����ַ���
			//��Ҫ��strcpy: \0��ͬʱ������ȥ����ǰ��ֹ�ַ�������
			//strcpy(_str + pos, str);
			for (int i = 0; i < len; ++i){
				_str[i + pos] = str[i];
			}

			//����size
			_size += len;
		}
	}

	void erase(size_t pos, size_t len){
		if (pos + len >= _size){
			//Ҫɾ����Ԫ�ش��������Ԫ�������������Ԫ��ȫ��ɾ��������size�Լ��ַ����������\0
			_size = pos;
			_str[_size] = '\0';
		}
		else{
			//Ԫ�ش�ǰ����ƶ���ÿ��Ԫ����ǰ�ƶ�len��λ��
			for (int i = pos + len; i <= _size; ++i){
				_str[pos++] = _str[i];
			}
			_size -= len;
		}
	}

	//n < size : �޸�size
	// size < n < capacity:  �޸�size + ��ֵ
	// n > capacity�� ���� +  �޸�size + ��ֵ
	void resize(size_t n, const char& ch = '\0'){
		//����
		if (n > _capacity){
			reserve(n);
		}
		//��ֵ
		if (n > _size){
			memset(_str + _size, ch, n - _size);
		}
		//�޸�size
		_size = n;
		_str[_size] = '\0';
	}

private:
	char* _str;
	//_size: ��Ч�ַ��ĸ�����������\0
	size_t _size;
	//_capacity: �ܹ���������Ч�ַ��ĸ���
	size_t _capacity;
};

void test7()
{
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

void test8()
{
	String s;
	s.insert(0, '0'); // 0
	s.insert(0, 'f'); // f0
	s.insert(0, 'e'); //ef0
	//β��
	s.insert(3, '1');  //ef01
	s.insert(1, '9');  // e9f01
	s.insert(3, 'g');  //e9fg01
}

void test9()
{
	String s("5678");
	s.insert(0, "1234");
	cout << s.c_str() << endl;
	s.insert(8, "910");
	cout << s.c_str() << endl;
	s.insert(4, "abcd");
	cout << s.c_str() << endl;
}

void test10()
{
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

void test11()
{
	String s("1111");
	cout << s.c_str() << endl;
	//���� + ��ֵ + �޸�size
	s.resize(10, 'a');  //1111aaaaaa
	cout << s.c_str() << endl;
	//�޸�size
	s.resize(5, 'b');  //1111a
	cout << s.c_str() << endl;
	s += 'b';  //1111ab
	//��ֵ + �޸�
	s.resize(8, 'c');  //1111abcc

}