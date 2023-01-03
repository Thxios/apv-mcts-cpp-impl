
#include <iostream>
#include <future>
#include <memory>

using namespace std;


class Base {
public:
	virtual ~Base() {}
	virtual void DoSomething() {}
	virtual int ReturnSomething() {return 0;}
	virtual Base* GetCopy() {return new Base(*this);}
};


class A : public Base {
public:
	virtual ~A() {}
	virtual void DoSomething() {
		cout << "A::DoSomething cnt:" << count << endl;
	}
	virtual int ReturnSomething() {
		count++;
		cout << "A::ReturnSomething cnt:" << count << endl;
		return count;
	}
	virtual Base* GetCopy() {
		return new A(*this);
	}
private:
	int count = 0;
};


class B : public Base {
public:
	virtual ~B() {}
	virtual void DoSomething() {
		cout << "B::DoSomething cnt:" << count << endl;
	}
	virtual int ReturnSomething() {
		count--;
		cout << "B::ReturnSomething cnt:" << count << endl;
		return count;
	}
	virtual Base* GetCopy() {
		return new B(*this);
	}
private:
	int count = -1;
};


class Holder {
public:
	Holder(Base& obj_) : obj(obj_) {}
	void CallDo() {
		cout << "CallDo Start" << endl;
		obj.DoSomething();
		cout << "CallDo Done" << endl;
	}
	void CallReturn() {
		cout << "CallReturn Start" << endl;
		int ret = obj.ReturnSomething();
		cout << "CallReturn Done - " << ret << endl;
	}
	void CopyTest() {
		Base* copied = obj.GetCopy();
		cout << "original" << endl;
		obj.DoSomething();
		cout << "copied" << endl;
		copied->DoSomething();

		cout << endl;
		copied->ReturnSomething();
		copied->ReturnSomething();
		cout << endl;

		cout << "original" << endl;
		obj.DoSomething();
		cout << "copied" << endl;
		copied->DoSomething();

		delete copied;
	}

private:
	Base& obj;
};



int main() {
	A a;
	B b;
	Holder ah(a), bh(b);

	cout << "A Holder" << endl;
	ah.CallDo();
	ah.CallReturn();

	cout << endl;

	cout << "B Holder" << endl;
	bh.CallDo();
	bh.CallReturn();

	cout << endl << endl;

	ah.CopyTest();
	cout << "====" << endl;
	ah.CallReturn();
	cout << "====" << endl;
	ah.CopyTest();
}


