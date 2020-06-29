

#include <iostream>
#include <list>
using namespace std;


/*������
* (1) �����ౣ֤ȫ��ֻ��һ��Ψһ��ʵ������
* (2) �����ౣֻ֤��Ψһ�Ľӿڻ�ȡ��Ψһʵ����
*/
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

#define SINGLETON_INDEX 6  // ���أ���ͬģʽ�Ŀ���

/*
 * ���䣺
 * =default: ������ʽҪ��������ṩ�ϳɰ汾���Ĵ���(���졢��������������ֵ)
 * =delete: ���ڶ���ɾ���������ھɱ�׼�£��������ϣ����ֹ��������ͨ����ʽ�����������캯���Ϳ�����ֵ����Ϊprivate�����±�׼���������Ƕ���ɾ������
 */

// �������� objdump -S -d 4-singleton-c++11 > 4-singleton-c++11.txt
// ֱ�ӻ�ࣺg++ -S -o main2-2.s main2.cpp -std=c++11

//����ʽ�����ӳټ���
//1��ԭʼ����ʽ����ģʽ ����ʽ����������Ҫʹ��������������ʱ���ȥ���������������
#if SINGLETON_INDEX == 1
class Singleton {
//private��ֹ����ȥ����
private:
    static Singleton *m_singleton;
    Singleton() = default;  // �Զ�����Ĭ�Ϲ��캯��
    Singleton(const Singleton& s) = delete;    // ���ÿ������캯��
    Singleton& operator=(const Singleton& s) = delete; // ���ÿ�����ֵ������

	//���������Ҫ���������������������
    class GarbageCollector {
    public:
        ~GarbageCollector() {
            cout << "~GarbageCollector\n";
            if (Singleton::m_singleton) {
                cout << "free m_singleton\n";
                delete Singleton::m_singleton;
                Singleton::m_singleton = nullptr;
            }
        }
    };
    static GarbageCollector m_gc;
public:
    static Singleton* getInstance(){
        if (Singleton::m_singleton == nullptr){
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); //���ߣ�ģ�ⴴ��ʵ����ʱ��
            m_singleton = new Singleton();
        }
        return m_singleton;
    }
};
// �����������ʼ��
Singleton* Singleton::m_singleton = nullptr;
Singleton::GarbageCollector Singleton::m_gc;
#elif SINGLETON_INDEX == 2
// 2 �̰߳�ȫ������ʽ����ģʽ
//�̰߳�ȫ������ʽ����

class Singleton {
private:
    static Singleton *m_singleton;
    static mutex m_mutex;
    Singleton() = default;
    Singleton(const Singleton& s) = delete; // ���ÿ������캯��
    Singleton& operator=(const Singleton& s) = delete;  // ���ÿ�����ֵ������
    class GarbageCollector {
    public:
        ~GarbageCollector() {
            cout << "~GarbageCollector\n";
            if (Singleton::m_singleton) {
                cout << "free m_singleton\n";
                delete Singleton::m_singleton;
                Singleton::m_singleton = nullptr;
            }
        }
    };
    static GarbageCollector m_gc;
public:
    static Singleton* getInstance() { // ���������ȴ�Ч�ʽϵͣ� �Ը߲����ķ���
        m_mutex.lock(); // ��������ֻ֤��һ���߳��ڷ�����������
        if (Singleton::m_singleton == nullptr){
			//��ʹ�Ӵ����ӳ�Ҳû������
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //���ߣ�ģ�ⴴ��ʵ����ʱ��
            m_singleton = new Singleton();
        }
        m_mutex.unlock();//����
        return m_singleton;
    }
};
Singleton* Singleton::m_singleton = nullptr;
mutex Singleton::m_mutex;
Singleton::GarbageCollector Singleton::m_gc;
#elif SINGLETON_INDEX == 3
// 3����ס��ʼ��ʵ�����֮���ٴμ��ʵ���Ƿ񱻴���
/* ˫��������������ڴ��дreorder����ȫ ��ΪC++��������ʱ����ִ��1�������ڴ棬2 ���ù��죬3 ��ֵ��������������
Ȼ���ִ�CPU�ͱ������߲����¿��ܻ�����������Ų����������������new CSingleton�ĵ�2�����ܻ����ڵ�3������ָ����ã�
������³���δ����ĵ���Ϊ��*/
class Singleton {
private:
    static Singleton *m_singleton;
    static mutex m_mutex;
    Singleton() = default;
    Singleton(const Singleton& s) = default;
    Singleton& operator=(const Singleton& s) = default;
    class GarbageCollector {
    public:
        ~GarbageCollector() {
            cout << "~GarbageCollector\n";
            if (Singleton::m_singleton) {
                cout << "free m_singleton\n";
                delete Singleton::m_singleton;
                Singleton::m_singleton = nullptr;
            }
        }
    };
    static GarbageCollector m_gc;
public:
    void *getSingletonAddress() {
        return m_singleton;
    }
    static Singleton* getInstance() {
        if (Singleton::m_singleton == nullptr){
            m_mutex.lock();  // ��������ֻ֤��һ���߳��ڷ����߳��ڵĴ���
            if (Singleton::m_singleton == nullptr) { //�ٴμ��
            //���˳�����˱仯�����������߳̾ͻ�Ӱ��
            //�����2����ʱ�����m_singleton�ǿգ�����ĳЩ�߳���˵�������ˣ��Ǿͻ᷵�أ�ʵ����������ǿյģ��ȵ���3�����ù��캯�����ͻ��쳣
                m_singleton = new Singleton();  // �����new����ԭ�Ӳ��� 1�������ڴ棬2 ���ù��죬3 ��ֵ����������3����ʱ�����m_singleton�ǿ�
                                                //  1�������ڴ棬2 ��ֵ���� 3 ���ù��죬����2����ʱ�����m_singleton�ǿ�
            }
            m_mutex.unlock();//����
        }
        return m_singleton;
    }
};
Singleton* Singleton::m_singleton = nullptr;
mutex Singleton::m_mutex;
Singleton::GarbageCollector Singleton::m_gc;
#elif SINGLETON_INDEX == 4
// 4��C++ 11�汾֮��Ŀ�ƽ̨ʵ��
class Singleton {
private:
    static std::atomic<Singleton*> m_instance;
    static std::mutex m_mutex;
    Singleton() = default;
    Singleton(const Singleton& s) = default;
    Singleton& operator=(const Singleton& s) = default;
    class GarbageCollector {
    public:
        ~GarbageCollector() {
            cout << "~GarbageCollector\n";
            Singleton* tmp = m_instance.load(std::memory_order_relaxed);
            if (tmp) {
                cout << "free m_singleton: " << tmp << endl;
                delete tmp;
            }
        }
    };
    static GarbageCollector m_gc;
public:
    void *getSingletonAddress() {
        return m_instance;
    }
    static Singleton* getInstance() {
        Singleton* tmp = m_instance.load(std::memory_order_relaxed);
        std::atomic_thread_fence(std::memory_order_acquire);//��ȡ�ڴ�fence
        if (tmp == nullptr) {
            std::lock_guard<std::mutex> lock(m_mutex);
            tmp = m_instance.load(std::memory_order_relaxed);
            if (tmp == nullptr) {
                tmp = new Singleton();  // 1�������ڴ棬2 ���ù��죬3 ��ֵ����
                std::atomic_thread_fence(std::memory_order_release);//�ͷ��ڴ�fence
                m_instance.store(tmp, std::memory_order_relaxed);
            }
        }
        return tmp;
    }
};
std::atomic<Singleton*>  Singleton::m_instance;
std::mutex Singleton::m_mutex;
Singleton::GarbageCollector Singleton::m_gc;
#elif  SINGLETON_INDEX == 5
// ����ʽ
class Singleton {
private:
//    Singleton() = default;  // �Զ�����Ĭ�Ϲ��캯��
    Singleton() {                           // ���캯�� ��Ӱ��ֲ���̬������ ��������ʽ�Ĺ��캯��
        cout << "Singleton construct\n";
    }
    Singleton(const Singleton& s) = delete;    // ���ÿ������캯��
    Singleton& operator=(const Singleton& s) = delete; // ���ÿ�����ֵ������
public:
    static Singleton* getInstance(){
        static Singleton s_singleton;  // C++11�̰߳�ȫ�� C++11֮ǰ�����̰߳�ȫ  __cxa_guard_acquire �� __cxa_guard_release
        return &s_singleton;
    }
};
#elif  SINGLETON_INDEX == 6
// ����ʽ����main��������ǰ��ʼ�������԰�ȫ
class Singleton {
private:

    //    Singleton() = default;   // �Զ�����Ĭ�Ϲ��캯��
    Singleton() {
        cout << "Singleton construct\n";
    }
    Singleton(const Singleton& s) = delete;    // ���ÿ������캯��
    Singleton& operator=(const Singleton& s) = delete; // ���ÿ�����ֵ������
    static Singleton m_singleton;
public:
    static Singleton* getInstance(){
        return &m_singleton;
    }
};
Singleton Singleton::m_singleton;
#endif


//// ����һ��������
static mutex s_m;

void print_address(){
    // ��ȡʵ��
    Singleton* singleton = Singleton::getInstance();
    // ��ӡsingleton��ַ
    s_m.lock(); // ��ס����ֻ֤��һ���߳��ڴ�ӡ��ַ
    cout<<singleton<<endl;
    //    cout<<"Address:" << singleton->getSingletonAddress() << endl; // ֻ��˫�����
    s_m.unlock();// ����
}

void test_Singleton(){
    thread threads[10];
    // ����10���߳�
    for (auto&t : threads)
        t = thread(print_address);
    // ��ÿ���̵߳���join�����̵߳ȴ����߳��������
    for (auto&t : threads)
        t.join();
}


int main()
{
//    Singleton *s = new Singleton();
    cout << "main\n";

    test_Singleton();

    cout << "test finish! " << Singleton::getInstance() << endl;
    return 0;
}



