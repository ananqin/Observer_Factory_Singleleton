
#include <iostream>
#include <list>

using namespace std;

class Subject;


class Observer
{

public:
	Observer(){}

	virtual ~Observer(){}

	//�۲��߸�������,������һ������
	//��ģ��,��ʡ����
	virtual void update(Subject *subject) = 0;
	//��ģ��,����ռ�ô���
	virtual void update(string content) = 0;
};



//g++ -o xxx MyObserver.cpp -std=c++11
class Subject
{
public:
	Subject(){}
	virtual ~Subject(){}

	//��ȡ��������
	virtual string getContent()= 0;
	//��ȡժҪ��ַ
	virtual string getAbstractContent() = 0;

	//���÷�������
	virtual void setContent(string content) = 0;
	
	//��������
	void attach(Observer *observer)
	{
		//�ѹ۲��߶���ŵ�������ȥ
		observers.push_back(observer);
	}

	//ȡ������
	void detach(Observer *observer)
	{
		//ɾ���۲��߶���
		//Observers.erase(observer);
		//ʹ��remove�ȽϺ�
		observers.remove(observer);
	}
	//֪ͨ���еĶ�����
	void notifyObservers(){
		for(Observer *reader:observers)
		{

			//��ģ��,�ѵ�ַ���ͳ�ȥ,����������
			//��ģ�͡�ֻ�����͵����ʣ�ֻ������СһЩ
			reader->update(this);
		}
	}

	void notifyObservers(string content)
	{
		for(Observer *reader:observers)
		{
			//��ģ��
			//�����е����ݶ����ͳ�ȥ
			reader->update(content);
		}
	}
private:
	list<Observer *> observers;
};
	

class Reader:public Observer
{
public:
	Reader(){}
	virtual ~Reader(){}
	//��������
	virtual void update(Subject *subject)
	{	
		//��ģ��
		//���������Ӧ�ķ���ȥ��ȡ���ݣ�subject->getContent()
		cout<<m_name<<"�յ���ֽ���Ķ�,��������"<<subject->getContent()<<endl;
	}

	virtual void update(string content)
	{

		//��ģ��
		//���������Ӧ�ķ���ȥ��ȡ���ݣ�subject->getContent()
		cout<<m_name<<"�յ���ֽ���Ķ�,��������"<<content<<endl;
		
	}
	
	string getName()
	{
		return m_name;
	}

	void setName(string name)
	{
		m_name = name;
	}
	
private:
	string m_name;
};

//����ı�ֽ
class NewsPaper:public Subject
{
public:
	NewsPaper(){}
	virtual ~NewsPaper(){}
	//���������壬��������
	
	void setContent(string content)
	{
		m_content = content;
		//���ø���֪ͨ�֪࣬ͨ�����۲���
		notifyObservers();
	}

	//��ȡ��������
	virtual string getContent()
	{

		return m_content;
	}
	//��ȡժҪ��ַ
	virtual string getAbstractContent()
	{

		return "ժҪ:";

	}
	
private:
	string m_content;
};




int main()
{
	//����һ����ֽ����
	//�������Ҳ��һ������ָ��(����ָ��ָ���������)
	NewsPaper *subject= new NewsPaper();

	//�����۲��ߣ�����
	Reader *reader1 = new Reader();
	reader1->setName("king");

	Reader *reader2 = new Reader();
	reader2->setName("antonio");

	Reader *reader3 = new Reader();
	reader3->setName("qaa");
	 /*Reader *reader1 = new Reader();
    reader1->setName("king��ʦ");

    Reader *reader2 = new Reader();
    reader2->setName("������ʦ");

    Reader *reader3 = new Reader();
    reader3->setName("������ʦ");*/

	//����Щ�۲�����ӵ�������
	subject->attach(reader1);
	subject->setContent("���������ģʽ, antonio������");
	cout<<".........................!"<<endl;
	subject->attach(reader2);
	subject->attach(reader3);

	
	subject->setContent("King��ʦȥ�Ϲ�������, qaa������");
	//cout<<".........................!"<<endl;
	
	
	
	cout<<"Hello World!"<<endl;
	return 0;
}

