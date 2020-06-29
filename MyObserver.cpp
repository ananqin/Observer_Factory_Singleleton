
#include <iostream>
#include <list>

using namespace std;

class Subject;


class Observer
{

public:
	Observer(){}

	virtual ~Observer(){}

	//观察者更新数据,并关联一个主体
	//拉模型,节省带宽
	virtual void update(Subject *subject) = 0;
	//推模型,更加占用带宽
	virtual void update(string content) = 0;
};



//g++ -o xxx MyObserver.cpp -std=c++11
class Subject
{
public:
	Subject(){}
	virtual ~Subject(){}

	//获取完整内容
	virtual string getContent()= 0;
	//获取摘要地址
	virtual string getAbstractContent() = 0;

	//设置发送内容
	virtual void setContent(string content) = 0;
	
	//订阅主题
	void attach(Observer *observer)
	{
		//把观察者对象放到链表里去
		observers.push_back(observer);
	}

	//取消订阅
	void detach(Observer *observer)
	{
		//删除观察者对象
		//Observers.erase(observer);
		//使用remove比较好
		observers.remove(observer);
	}
	//通知所有的订阅者
	void notifyObservers(){
		for(Observer *reader:observers)
		{

			//拉模型,把地址推送出去,而不是内容
			//拉模型。只有推送的性质，只是粒度小一些
			reader->update(this);
		}
	}

	void notifyObservers(string content)
	{
		for(Observer *reader:observers)
		{
			//推模型
			//把所有的内容都推送出去
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
	//关联主体
	virtual void update(Subject *subject)
	{	
		//拉模型
		//调用主体对应的方法去拉取内容，subject->getContent()
		cout<<m_name<<"收到报纸和阅读,具体内容"<<subject->getContent()<<endl;
	}

	virtual void update(string content)
	{

		//推模型
		//调用主体对应的方法去拉取内容，subject->getContent()
		cout<<m_name<<"收到报纸和阅读,具体内容"<<content<<endl;
		
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

//具体的报纸
class NewsPaper:public Subject
{
public:
	NewsPaper(){}
	virtual ~NewsPaper(){}
	//真正的主体，更新内容
	
	void setContent(string content)
	{
		m_content = content;
		//调用父类通知类，通知各个观察者
		notifyObservers();
	}

	//获取完整内容
	virtual string getContent()
	{

		return m_content;
	}
	//获取摘要地址
	virtual string getAbstractContent()
	{

		return "摘要:";

	}
	
private:
	string m_content;
};




int main()
{
	//创建一个报纸主题
	//子类对象也是一个父类指针(父类指针指向子类对象)
	NewsPaper *subject= new NewsPaper();

	//创建观察者，读者
	Reader *reader1 = new Reader();
	reader1->setName("king");

	Reader *reader2 = new Reader();
	reader2->setName("antonio");

	Reader *reader3 = new Reader();
	reader3->setName("qaa");
	 /*Reader *reader1 = new Reader();
    reader1->setName("king老师");

    Reader *reader2 = new Reader();
    reader2->setName("柚子老师");

    Reader *reader3 = new Reader();
    reader3->setName("秋香老师");*/

	//把这些观察者添加到链表里
	subject->attach(reader1);
	subject->setContent("今天上设计模式, antonio主持了");
	cout<<".........................!"<<endl;
	subject->attach(reader2);
	subject->attach(reader3);

	
	subject->setContent("King老师去上公开课了, qaa主持了");
	//cout<<".........................!"<<endl;
	
	
	
	cout<<"Hello World!"<<endl;
	return 0;
}

