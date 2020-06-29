

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

//������ļ�
class ExportTextProduct:public ExportFileProduct
{
public:
	ExportTextProduct(){}
	virtual ~ExportTextProduct(){}
	virtual bool Export(string data)
	{
		cout<<"��������:["<<data<<"]������ı��ķ�ʽ"<<endl;
		return true;
	}
};


//��������ݿ�
class ExportDBProduct:public ExportFileProduct
{
public:
	ExportDBProduct(){}
	virtual ~ExportDBProduct(){}
	virtual bool Export(string data)
	{
		cout<<"��������:["<<data<<"]�������ݿ�ķ�ʽ"<<endl;
		return true;
	}
};

//���µĵ�����ʽ
//(1)�޸�ԭ���Ĺ�������
//(2)�̳й���������չ

//�����XML
class ExportXMLProduct:public ExportFileProduct
{
public:
	ExportXMLProduct(){}
	virtual ~ExportXMLProduct(){}
	virtual bool Export(string data)
	{
		cout<<"��������:["<<data<<"]����XML�ķ�ʽ"<<endl;
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
		cout<<"��������:["<<data<<"]����Protobuffer�ķ�ʽ"<<endl;
		return true;
	}
};



class ExportFactory
{
public:
	ExportFactory(){}
	virtual ~ExportFactory(){}

	/*
		type:����������
		data:���������
	*/	
	virtual bool Export(int type, string data)
	{

		//����type������ͬ�Ķ���
		ExportFileProduct *product =  nullptr;
		//��ȡ����Ĺ���,��������ͬ�ķ���
		product = factoryMethod(type);
		bool ret = false;
		if(product)
		{
			//ȥ�������������
			ret = product->Export(data);
			delete product;
		}else{
			cout<<"û�ж�Ӧ�ĵ�����ʽ";
		}

		return ret;
		
	}
protected:
	virtual ExportFileProduct *factoryMethod(int type)
	{

		ExportFileProduct *product = nullptr;
		if(1==type)
		{
		//������Ҫnewʵ����
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
		//������Ҫnewʵ����
			product = new ExportXMLProduct();
		}else if(4 == type)
		{
			product = new ExportProtobufferProduct();
		}else
		{
		//������������ø���ķ���
			product = ExportFactory::factoryMethod(type);
		}

		return product;
	}
};

int main()
{

	cout<<"ExportFactory"<<endl;
	ExportFactory *factory = new ExportFactory();

	factory->Export(1,"�Ͽ�����");
	factory->Export(2,"�Ͽ�����");
	factory->Export(3,"�Ͽ�����");


	cout<<"\nExportFactory2"<<endl;
	ExportFactory2 *factory2 = new ExportFactory2();

	factory2->Export(1,"�Ͽ�����");
	factory2->Export(2,"�Ͽ�����");
	factory2->Export(3,"�Ͽ�����");
	factory2->Export(4,"�Ͽ�����");

	delete factory;
	delete factory2;
	cout<<"hello world"<<endl;
	return 0;
}




