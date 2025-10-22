#include "stl.h"

#include <algorithm>
#include <any>
#include <chrono>
#include <expected>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <stacktrace>
#include <print>
#include <set>
#include <variant>
using namespace std;

#define FUNCNAME  cout << stacktrace::current().at(0).description() << '\n';
#define REGISTERFUNC(NAME) funcList.emplace_back(move(NAME));

void stl_minmax()
{
	FUNCNAME
	cout << min(2011, 2014) << endl;
	cout << min({3, 1, 2011, 2014, 5}) << endl;
	cout << min(-10, -5, [](int a, int b)
	{
		return abs(a) < abs(b);
	}) << endl;
	auto pairInt = minmax(2011, 2014);
	auto pairSeq = minmax({3, 1, 2011, 2014, -5});
	auto pairAbs = minmax({3, 1, 2011, 2014, -5}, [](int a, int b)
	{
		return abs(a) < abs(b);
	});
	cout << pairInt.first << "," << pairInt.second << endl;
	cout << pairSeq.first << "," << pairSeq.second << endl;
	cout << pairAbs.first << "," << pairAbs.second << endl;
}

void stl_midpoint()
{
	FUNCNAME
	cout << midpoint(10, 20) << "\n";

	for (auto v : {0.0, 0.1, 0.2, 0.3, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0})
	{
		cout << "lerp(10,20, " << v << "): " << lerp(10, 20, v) << endl;
	}
}

void stl_move()
{
	vector<int> myBigVec(100000000, 2011);

	vector<int> myVec = myBigVec;
	myVec = move(myBigVec);
}

struct MyData
{
	MyData(int, double, char)
	{
	};
};

template <typename T, typename... Args>
T createT(Args&&... args)
{
	return T(forward<Args>(args)...);
}

void stl_forward()
{
	auto s = createT<string>("Only for testing");
	MyData myData2 = createT<MyData>(1, 3.19, 'a');
	using IntVec = vector<int>;
	IntVec intVec = createT<IntVec>(initializer_list<int>({1, 2, 3}));
}

template <typename T>
inline void stl_swap(T& a, T& b) noexcept
{
	T tmp(std::move(a));
	a = std::move(b);
	b = std::move(tmp);
}

void stl_swap1()
{
	auto swap1 = 10;
	auto swap2 = 20;
	stl_swap(swap1, swap2);
}


double divMe(double a, double b) { return a / b; }

void stl_bind()
{
	FUNCNAME
	function<double(double, double)> myDiv = bind(divMe, placeholders::_1, placeholders::_2);
	function<double(double)> myDiv2 = bind(divMe, 2000, placeholders::_1);
	function<double(double)> myDiv3 = bind_front(divMe, 2000);
	function<double(double)> myDiv4 = bind_back(divMe, 10);
	cout << myDiv(10, 2) << endl;
	cout << myDiv2(2) << endl;
	cout << myDiv3(2) << endl;
	cout << myDiv4(2) << endl;
	map<const char, function<double(double, double)>> tab;
	tab.insert(make_pair('+', [](double a, double b) { return a + b; }));
	tab.insert(make_pair('-', [](double a, double b) { return a - b; }));
	tab.insert(make_pair('*', [](double a, double b) { return a * b; }));
	tab.insert(make_pair('/', [](double a, double b) { return a / b; }));

	println("+: {}", tab['+'](3.5, 3.5));
	println("-: {}", tab['-'](3.5, 4.5));
	println("*: {}", tab['*'](3.5, 4.5));
	println("/: {}", tab['/'](3.5, 4.5));
}

void stl_pair()
{
	FUNCNAME
	pair<const char*, double> charDoub("str", 3.14);
	pair<const char*, double> charDoub2 = make_pair("str", 3.14);
	auto charDoub3 = make_pair("str", 3.14);
	println("{} , {}", charDoub.first, charDoub.second);
	charDoub.first = "Str";
	get<1>(charDoub) = 4.14;
	println("{},{}", charDoub.first, charDoub2.second);
}

void stl_tuple()
{
	FUNCNAME
	tuple<string, int, float> tup1("first", 3, 4.17f);
	auto tup2 = make_tuple("second", 4, 1.1);
	println("{},{},{}", get<0>(tup1), get<1>(tup1), get<2>(tup1));
	println("{},{},{}", get<0>(tup2), get<1>(tup2), get<2>(tup2));
	println("{}", tup1 < tup2);
	get<0>(tup2) = "Second";
	println("{},{},{}", get<0>(tup2), get<1>(tup2), get<2>(tup2));
	println("{}", tup1 < tup2);
	auto pair = make_pair(1, true);
	tuple<int, bool> tup = pair;
}

void stl_tie()
{
	FUNCNAME
	int first = 1;
	int second = 2;
	int third = 3;
	int forth = 4;
	println("{} {} {} {}", first, second, third, forth);
	auto tup = tie(first, second, third, forth) = make_tuple(101, 102, 103, 104);
	println("{} {} {} {}", get<0>(tup), get<1>(tup), get<1>(tup), get<2>(tup));
	println("{} {} {} {}", first, second, third, forth);
	first = 201;
	get<1>(tup) = 202;
	println("{} {} {} {}", get<0>(tup), get<1>(tup), get<1>(tup), get<2>(tup));
	println("{} {} {} {}", first, second, third, forth);
	int a, b;
	tie(ignore, a, ignore, b) = tup;
	println("{} {}", a, b);
}

void foo()
{
	println("Invoked");
}


typedef void callableUnit();

void invokeMe(const std::string& s)
{
	println("{}: const", s);
}

template <typename T>
void doubleMe(T t)
{
	t *= 2;
}


void stl_wrapper()
{
	reference_wrapper<callableUnit> refWrap(foo);

	refWrap();

	string s{"string"};
	invokeMe(cref(s));
	int i = 1;
	println("{}", i);
	doubleMe(i);
	println("{}", i);

	doubleMe(ref(i));
	println("{}", i);
}

struct MyInt
{
	MyInt(int i) : i_(i)
	{
		println("Construct {} at {:p}", i_, (void*)this);
	}

	~MyInt()
	{
		println("Good Bye from {} at {:p}", i_, (void*)this);
	}

	int i_;
};


class MyStruct
{
public:
	MyStruct() : val(count)
	{
		println("{}, Hello: {}", static_cast<const void*>(this), val);
		MyStruct::count++;
	}

	~MyStruct()
	{
		println("{} Good Bye: {}", static_cast<const void*>(this), val);
		MyStruct::count--;
	}

private:
	int val;
	static int count;
};

int MyStruct::count = 0;

void stl_uniquePointer()
{
	unique_ptr<MyInt> uniquePtr1(new MyInt(1998));
	println("{}", static_cast<const void*>(uniquePtr1.get()));
	unique_ptr<MyInt> uniquePtr2(move(uniquePtr1));
	println("{}", static_cast<const void*>(uniquePtr1.get()));
	println("{}", static_cast<const void*>(uniquePtr2.get()));
	{
		unique_ptr<MyInt> localPtr(new MyInt(2003));
	}
	uniquePtr2.reset(new MyInt(2001));
	MyInt* my_int = uniquePtr2.release();
	delete my_int;

	unique_ptr<MyInt> uniquePtr3(new MyInt(2017));
	unique_ptr<MyInt> uniquePtr4(new MyInt(2022));
	println("{}", static_cast<const void*>(uniquePtr3.get()));
	println("{}", static_cast<const void*>(uniquePtr4.get()));
	swap(uniquePtr3, uniquePtr4);
	println("{}", static_cast<const void*>(uniquePtr3.get()));
	println("{}", static_cast<const void*>(uniquePtr4.get()));

	{
		unique_ptr<MyStruct[]> myUniqueArray(new MyStruct[3]);
	}
}

class ShareMe : public enable_shared_from_this<ShareMe>
{
public:
	shared_ptr<ShareMe> getShared()
	{
		return shared_from_this();
	}
};

void stl_sharePointer()
{
	auto sharePtr = std::make_shared<MyInt>(1998);
	println("{}", sharePtr.use_count());
	{
		shared_ptr<MyInt> local_shard_ptr(sharePtr);
		println("{}", local_shard_ptr.use_count());
	}
	println("{}", sharePtr.use_count());
	auto globalSharPtr = sharePtr;
	println("{}", sharePtr.use_count());
	globalSharPtr.reset();
	println("{}", sharePtr.use_count());
	sharePtr = std::make_shared<MyInt>(2011);

	shared_ptr<ShareMe> shareMe(new ShareMe);
	auto shareMe1 = shareMe->getShared();
	println("{}", static_cast<const void*>(shareMe.get()));
	println("{}", static_cast<const void*>(shareMe1.get()));
	println("{}", shareMe.use_count());
}

void stl_weakPointer()
{
	auto sharedPtr = make_shared<int>(2011);
	std::weak_ptr<int> weak_ptr(sharedPtr);
	println("{}", weak_ptr.use_count());
	println("{}", sharedPtr.use_count());
	println("{}", weak_ptr.expired());
	if (shared_ptr<int> shared_ptr1 = weak_ptr.lock())
	{
		println("{}", *sharedPtr);
	}
	else
	{
		println("Don't get it.");
	}

	weak_ptr.reset();

	if (shared_ptr<int> shared_ptr1 = weak_ptr.lock())
	{
		println("{}", *sharedPtr);
	}
	else
	{
		println("Don't get it.");
	}
}

struct Son;
struct Daughter;

struct Mother
{
	~Mother() { println("Mother gone"); }
	void setSon(const shared_ptr<Son> s) { mySon = s; }
	void setDaughter(const shared_ptr<Daughter> d) { myDaughter = d; }
	weak_ptr<const Son> mySon;
	weak_ptr<const Daughter> myDaughter;
};

struct Son
{
	Son(shared_ptr<Mother> m) : myMother(m)
	{
	}

	~Son() { println("Son gone"); }
	shared_ptr<const Mother> myMother;
};

struct Daughter
{
	Daughter(shared_ptr<Mother> m) : myMother(m)
	{
	}

	~Daughter() { println("Daughter gone"); }
	shared_ptr<const Mother> myMother;
};

void stl_cyclicRef()
{
	shared_ptr<Mother> mother = shared_ptr<Mother>(new Mother);
	shared_ptr<Son> son = shared_ptr<Son>(new Son(mother));
	shared_ptr<Daughter> daughter = shared_ptr<Daughter>(new Daughter(mother));
	mother->setSon(son);
	mother->setDaughter(daughter);
}

struct A
{
	int a;
	int f(int) { return 2011; }
};

enum E { e = 1 };

union U
{
	int u;
};

void stl_type_traits()
{
	println("{}", is_void<void>::value);
	println("{}", is_integral<short>::value);
	println("{}", is_floating_point<double>::value);
	println("{}", is_array<int[]>::value);
	println("{}", is_pointer<int*>::value);
	println("{}", is_reference<int&>::value);
	println("{}", is_member_object_pointer<int A::*>::value);
	println("{}", is_member_function_pointer<int (A::*)(int)>::value);
	println("{}", is_enum<E>::value);
	println("{}", is_union<U>::value);
	println("{}", std::is_class<std::string>::value);
	println("{}", std::is_function<int*(double)>::value);
	println("{}", is_lvalue_reference<int&>::value);
	println("{}", is_rvalue_reference<int&&>::value);
}

void stl_epoch()
{
	auto timeNow = chrono::system_clock::now();
	auto duration = timeNow.time_since_epoch();
	println("{} ns", duration.count());

	typedef chrono::duration<double> MySecondTick;
	MySecondTick my_second(duration);
	println("{} s", my_second.count());

	constexpr int minute = 60;
	typedef chrono::duration<double, ratio<minute>> MyMinuteTick;
	MyMinuteTick my_minute(duration);
	println("{} min", my_minute.count());
}

struct MyClass
{
};

class AnyClass
{
public:
	AnyClass()
	{
	}

	~AnyClass()
	{
	}
};

void stl_any()
{
	vector<any> anyVec{true, 2017, string("test"), 3.14, MyClass(), AnyClass()};
	println("{}", any_cast<bool>(anyVec[0]));
	int myInt = any_cast<int>(anyVec[1]);
	println("{}", myInt);
	println("{}", anyVec[0].type().name());
	println("{}", anyVec[1].type().name());
	println("{}", anyVec[5].type().name());
}

auto getFirst(const vector<int>& vec) -> optional<int>
{
	if (!vec.empty()) return optional<int>(vec[0]);
	return {};
}

auto getInt(string arg) -> optional<int>
{
	try
	{
		return {std::stoi(arg)};
	}
	catch (...)
	{
		return {};
	}
}

void stl_optional()
{
	vector<int> myVec{1, 2, 3};
	vector<int> myEmptyVec;

	auto myInt = getFirst(myVec);
	if (myInt)
	{
		println("{}", *myInt);
		println("{}", myInt.value());
		println("{}", myInt.value_or(2017));
	}
	auto myEmptyInt = getFirst(myEmptyVec);
	if (!myEmptyInt)
	{
		println("{}", myEmptyInt.value_or(2017));
	}

	vector<optional<string>> strings = {"66", "foo", "-5"};

	for (const auto& s : strings)
	{
		auto res = s.and_then(getInt).transform([](int n) { return n + 100; }).transform([](int n)
		{
			return to_string(n);
		}).or_else([] { return optional<string>("Error"); });
		println("{}", *res);
	}
}

void stl_variant()
{
	variant<int, float> v, w;
	v = 12;
	int i = get<int>(v);
	w = get<int>(v);
	w = get<0>(v);
	w = v;

	try
	{
		get<float>(w);
	}
	catch (bad_variant_access&)
	{
	}

	variant<string> v2("abc");
	v2 = "def";

	vector<variant<char, long, float, int, double, long long>> vecVariant = {5, '2', 5.4, 10011, 2011l, 3.5f, 2017};
	for (auto& v : vecVariant)
	{
		visit([](auto&& arg) { println("{} ", arg); }, v);
	}
	common_type<char, long, float, int, double, long long>::type res{};
	println("{}", typeid(res).name());

	for (auto& v : vecVariant)
	{
		visit([&res](auto&& arg) { res += arg; }, v);
	}
	println("res: {}", res);

	for (auto& v : vecVariant)
	{
		visit([&res](auto&& arg) { arg *= 2; }, v);
		visit([](auto&& arg) { println("{}", arg); }, v);
	}
}

auto getInt1(string arg) -> expected<int, string>
{
	try
	{
		return stoi(arg);
	}
	catch (...)
	{
		return unexpected{string(arg + ":ERROR")};
	}
}

void stl_expected()
{
	vector<string> strings = {"66", "foo", "-5"};

	for (auto s : strings)
	{
		auto res = getInt1(s);
		if (res)
		{
			print("{} ", res.value());
		}
		else
		{
			print("{} ", res.error());
		}
	}
	println("");
	for (auto s : strings)
	{
		auto res = getInt1(s);
		print("{} ", res.value_or(2023));
	}
	println("");
	for (auto s : strings)
	{
		auto res = getInt1(s).transform([](int n) { return n + 100; }).transform([](int n) { return to_string(n); }).
		                      or_else([](const string& str) { return expected<string, string>{str}; });
		print("{} ", *res);
	}
	println("");
}

void stl_constructor()
{
	vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	map<string, int> m = {{"bart", 12345}, {"jenne", 34929}, {"huber", 840284}};
	unordered_map<string, int> um{m.begin(), m.end()};
	for (auto value : vec)
	{
		print("{} ", value);
	}
	println("");
	for (auto value : m)
	{
		print("{},{} ", value.first, value.second);
	}
	println("");
	for (auto value : um)
	{
		print("{},{} ", value.first, value.second);
	}
	println("");
	vector<int> vec2 = vec;
	println("{}", vec.size());
	println("{}", vec2.size());

	auto vec3 = move(vec);
	println("{}", vec.size());
	println("{}", vec3.size());
	vec3.clear();
	println("{}", vec3.size());
}

void stl_size()
{
	vector<int> intVec{1, 2, 3, 4, 5, 6, 7, 8, 9};
	map<string, int> str2Int = {{"bart", 12345}, {"jenne", 34929}, {"huber", 840284}};
	set<double> douSet{3.14, 2.5};
	println("{}", intVec.empty());
	println("{}", str2Int.empty());
	println("{}", douSet.empty());

	println("{}", intVec.max_size());
	println("{}", str2Int.max_size());
	println("{}", douSet.max_size());
}


void stl_access()
{
	vector<MyInt> myIntVec;
	myIntVec.reserve(10);
	myIntVec.emplace_back(5);
	myIntVec.emplace_back(1);
	myIntVec.emplace_back(3);
	println("{}", myIntVec.size());
	vector<int> intVec;
	intVec.assign({1, 2, 3});
	for (const auto& int_vec : intVec)
	{
		print("{} ", int_vec);
	}
	println("");
	intVec.insert(intVec.begin(), 0);
	for (auto v : intVec)
	{
		print("{} ", v);
	}
	println("");
	intVec.insert(intVec.begin() + 4, 4);
	for (auto my_int_vec : intVec)
	{
		print(" {} ", my_int_vec);
	}
	println("");
	intVec.insert(intVec.end(), {5, 6, 7, 8, 9, 10, 11});
	for (int int_vec : intVec)
	{
		print("{} ", int_vec);
	}
	println("");
	for (auto v = intVec.rbegin(); v != intVec.rend(); ++v)
	{
		print("{} ", *v);
	}
	println("");
	intVec.pop_back();
	for (auto int_vec : intVec)
	{
		print("{} ", int_vec);
	}
	println("");
}

void stl_swap2()
{
	set<int> set1{0, 1, 2, 3, 4, 5};
	set<int> set2{6, 7, 8, 9};
	for (auto value : set1)
	{
		print("{} ", value);
	}
	println("");
}

void initFuncList()
{
	// REGISTERFUNC(stl_minmax)
	// REGISTERFUNC(stl_midpoint)
	// REGISTERFUNC(stl_move)
	// REGISTERFUNC(stl_forward)
	// REGISTERFUNC(stl_swap1)
	// REGISTERFUNC(stl_bind)
	// REGISTERFUNC(stl_pair)
	// REGISTERFUNC(stl_tuple)
	// REGISTERFUNC(stl_tie)
	// REGISTERFUNC(stl_wrapper)
	// REGISTERFUNC(stl_uniquePointer)
	// REGISTERFUNC(stl_sharePointer)
	// REGISTERFUNC(stl_cyclicRef)
	// REGISTERFUNC(stl_type_traits)
	// REGISTERFUNC(stl_epoch)
	// REGISTERFUNC(stl_any)
	// REGISTERFUNC(stl_optional)
	// REGISTERFUNC(stl_variant)
	// REGISTERFUNC(stl_expected)
	// REGISTERFUNC(stl_constructor)
	// REGISTERFUNC(stl_size)
	// REGISTERFUNC(stl_access)
	REGISTERFUNC(stl_swap2)
}
