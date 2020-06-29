

#include <iostream>
#include <list>
using namespace std;


class ExportFileProduct
{

public:
	ExportFileProduct(){}
	virtual ~ExportFileProduct(){}

	virtual bool Export(string data) = 0;

};

//保存成文件
class ExportTextProduct:public ExportFileProduct
{
public:
	ExportTextProduct(){}
	virtual ~ExportTextProduct(){}
	virtual bool Export(string data)
	{
		cout<<"导出数据:["<<data<<"]保存成文本的方式"<<endl;
		return true;
	}
};


//保存成数据库
class ExportDBProduct:public ExportFileProduct
{
public:
	ExportDBProduct(){}
	virtual ~ExportDBProduct(){}
	virtual bool Export(string data)
	{
		cout<<"导出数据:["<<data<<"]保存数据库的方式"<<endl;
		return true;
	}
};

//加新的导出方式
//(1)修改原来的工厂方法
//(2)继承工厂方法扩展

//保存成XML
class ExportXMLProduct:public ExportFileProduct
{
public:
	ExportXMLProduct(){}
	virtual ~ExportXMLProduct(){}
	virtual bool Export(string data)
	{
		cout<<"导出数据:["<<data<<"]保存XML的方式"<<endl;
		return true;
	}
};

class ExportProtobufferProduct:public ExportFileProduct
{
public:
	ExportProtobufferProduct(){}
	virtual ~ExportProtobufferProduct(){}
	virtual bool Export(string data)
	{
		cout<<"导出数据:["<<data<<"]保存Protobuffer的方式"<<endl;
		return true;
	}
};



class ExportFactory
{
public:
	ExportFactory(){}
	virtual ~ExportFactory(){}

	/*
		type:导出的类型
		data:具体的数据
	*/	
	virtual bool Export(int type, string data)
	{

		//根据type产生不同的对象
		ExportFileProduct *product =  nullptr;
		//获取正真的工厂,生产出不同的方法
		product = factoryMethod(type);
		bool ret = false;
		if(product)
		{
			//去生产正真的数据
			ret = product->Export(data);
			delete product;
		}else{
			cout<<"没有对应的导出方式";
		}

		return ret;
		
	}
protected:
	virtual ExportFileProduct *factoryMethod(int type)
	{

		ExportFileProduct *product = nullptr;
		if(1==type)
		{
		//这里需要new实现类
			product = new ExportTextProduct();
		}else if(2 == type)
		{
			product = new ExportDBProduct();
		}

		
		
		return product;
	}
};


class ExportFactory2:public ExportFactory
{
public:
	ExportFactory2(){}
	virtual ~ExportFactory2(){}
protected:
	virtual ExportFileProduct *factoryMethod(int type)
	{

		ExportFileProduct *product = nullptr;
		if(3 == type)
		{
		//这里需要new实现类
			product = new ExportXMLProduct();
		}else if(4 == type)
		{
			product = new ExportProtobufferProduct();
		}else
		{
		//其它情况，就用父类的方法
			product = ExportFactory::factoryMethod(type);
		}

		return product;
	}
};

int main()
{

	cout<<"ExportFactory"<<endl;
	ExportFactory *factory = new ExportFactory();

	factory->Export(1,"上课人数");
	factory->Export(2,"上课人数");
	factory->Export(3,"上课人数");


	cout<<"\nExportFactory2"<<endl;
	ExportFactory2 *factory2 = new ExportFactory2();

	factory2->Export(1,"上课人数");
	factory2->Export(2,"上课人数");
	factory2->Export(3,"上课人数");
	factory2->Export(4,"上课人数");

	delete factory;
	delete factory2;
	cout<<"hello world"<<endl;
	return 0;
}




